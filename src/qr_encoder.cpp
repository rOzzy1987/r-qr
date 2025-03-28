
#include "qr_encoder.h"
#include "qr_data.h"
#include "qr_gf256.h"
   
uint8_t* CQrEncoder::encode(const char* data, uint16_t length, uint16_t& resultLen, uint8_t& version, QrMode& mode, QrEcc ecLevel) {
    bool versionSet = version < 40;
    uint8_t segLength;

    if (!versionSet) {
        // get a version that definitely fits
        version = getMinVersion(length, mode, ecLevel);
    }

    QrDataSegment* seg = getSegments(data, length, version, mode, segLength);

    if (!versionSet) {
        version = optimizeVersion(seg, segLength, version, ecLevel);
    }

    QrBlockStruct s = qr_block_struct(version,ecLevel);
    uint16_t buffSize = s.totalWords();
    uint8_t *buff = new uint8_t[buffSize];
    memset(buff, 0, buffSize);
    QrBufferCursor cur;


    for(uint8_t i = 0; i < segLength; i++){
        writeSegmentData(data + seg[i].from, seg[i].to - seg[i].from, buff, cur, version, seg[i].mode);
    }
    delete[] seg;
    addStopBitsAndPad(buff, cur);
    // Add padding to specified length
    addPaddingBytes(buff, cur, s.dataWords());

    // printf("\nVersion: %d", version);
    splitBlob(buff, s);
    // print_codewords_vert(buff, s);
    writeEdc(buff, s);
    // print_codewords_vert(buff, s);
    mixBlocks(buff, s);
    // print_arr_f("%02X ", buff, buffSize, s.shortBlocks + s.longBlocks);

    resultLen = buffSize;
    return buff;
}
void CQrEncoder::splitBlob(uint8_t *buff, QrBlockStruct s) {
    uint16_t 
        iSrc = s.dataWords(),
        iDst = s.totalWords();
    for(uint8_t i = 0; i < s.longBlocks; i++){
        iSrc -= s.dataWordsPerShortBlock + 1;
        iDst -= s.ecWordsPerBlock + s.dataWordsPerShortBlock + 1;
        for(int16_t j = s.dataWordsPerShortBlock + 1; j >= 0; j--){
            buff[iDst + j] = buff[iSrc + j];
            #ifdef QR_TESTING
            buff[iSrc + j] = 0;
            #endif
        }
    }
    // the first data block is already in place
    for(uint8_t i = 0; i < s.shortBlocks - 1; i++){
        iSrc -= s.dataWordsPerShortBlock;
        iDst -= s.ecWordsPerBlock + s.dataWordsPerShortBlock;
        for(int16_t j = s.dataWordsPerShortBlock; j >= 0; j--){
            buff[iDst + j] = buff[iSrc + j];
            #ifdef QR_TESTING
            buff[iSrc + j] = 0;
            #endif
        }
    }
}

void CQrEncoder::mixBlocks(uint8_t *buff, QrBlockStruct str){
    uint16_t 
        totalBlocks = str.shortBlocks + str.longBlocks,
        blockLength = str.dataWordsPerShortBlock + str.ecWordsPerBlock,
        n = blockLength;


    if (totalBlocks < 2) return;
        
    uint8_t temp[totalBlocks],
        *movingBuff = buff;
    uint16_t moved[totalBlocks];
    memset(moved, 0, totalBlocks);
    #ifdef QR_TESTING
    memset(temp, 0, totalBlocks);
    #endif
    
    for (uint16_t i0 = 1; i0 <= n; i0++) {
        // get a batch of values
        uint16_t p = blockLength; // start with the second block, the first byte is already in place
        for (uint16_t i1 = 1; i1 < totalBlocks; i1++)
        {
            temp[i1] = movingBuff[p];
            #ifdef QR_TESTING
            movingBuff[p] = 0;
            #endif
            moved[i1] = p;

            p += i1 < str.shortBlocks ? blockLength : blockLength + 1;      
        }

        // shift to eliminate 'empty' spaces
        uint8_t shift = 0;
        p = totalBlocks - 1;
        for (uint16_t i1 = moved[p]; i1 >= totalBlocks; i1--)
        {
            if (p > 0 && i1 - shift == moved[p]) {
                shift++;
                p--;
            }
            movingBuff[i1] = movingBuff[i1 - shift];
            #ifdef QR_TESTING
            movingBuff[i1-shift] = 0;
            #endif
        }
        // write back temp to buff
        memcpy(movingBuff + 1, temp + 1, totalBlocks -1);

        blockLength--;
        movingBuff += totalBlocks;
    }

    // correct shifting for different block sizes 
    if(str.longBlocks == 0) return;

    uint16_t p = totalBlocks * str.dataWordsPerShortBlock;
    movingBuff = buff + p;
    for (uint8_t i0 = 0; i0 < str.ecWordsPerBlock; i0++ ){
        for (uint8_t i1 = 0; i1 < str.longBlocks; i1++) {
            temp[i1] = movingBuff[str.shortBlocks + i1];
        }
        for (uint8_t i1 = totalBlocks - 1; i1 >= str.longBlocks; i1--) {
            movingBuff[i1] = movingBuff[i1 - str.longBlocks];
        }
        for (uint8_t i1 = 0; i1 < str.longBlocks; i1++) {
            movingBuff[i1] = temp[i1];
        }
        movingBuff += totalBlocks;
    }
}

void CQrEncoder::writeSegmentData(const char* data, uint16_t dataLen, uint8_t *buff, QrBufferCursor& cur, uint8_t version, QrMode mode){
    addValue(buff, cur, qr_mode_indicator[mode], 4);
    addValue(buff, cur, dataLen, qr_lengthBits(version, mode));
    addData(buff, cur, data, dataLen, mode);
}

void CQrEncoder::writeEdc(uint8_t *buff, QrBlockStruct s){
    uint16_t dataWords = s.dataWordsPerShortBlock,
    blockWords = dataWords + s.ecWordsPerBlock;
    for(uint8_t i = 0; i < s.shortBlocks; i++){
        uint16_t edcLen = 0;

        uint8_t *edc = Gf256.getEdc(buff, dataWords, blockWords, edcLen);
        memcpy(buff + dataWords, edc, edcLen);
        delete[] edc;
        buff += blockWords;
    }
    dataWords = s.dataWordsPerShortBlock + 1;
    blockWords = dataWords + s.ecWordsPerBlock;
    for(uint8_t i = 0; i < s.longBlocks; i++){
        uint16_t edcLen = 0;

        uint8_t *edc = Gf256.getEdc(buff, dataWords, blockWords, edcLen);
        memcpy(buff + dataWords, edc, edcLen);
        delete[] edc;
        buff += blockWords;
    }
}

uint8_t CQrEncoder::optimizeVersion(QrDataSegment *segments, uint8_t segLength, uint8_t version, QrEcc ecLevel) {
    QrBlockStruct s = qr_block_struct(version,ecLevel);
    uint16_t actualWords = getTotalWords(segments, segLength, version);
    if (actualWords < s.dataWords()){
        while(true&& version > 0) {
            s = qr_block_struct(version-1,ecLevel);
            actualWords = getTotalWords(segments, segLength, version - 1);
            if (actualWords > s.dataWords()) {
                return version;
            }
            version--;
        }
        return 0;
    } 

    while( version < 39) {
        version++;
        s = qr_block_struct(version,ecLevel);
        if (getTotalWords(segments, segLength, version) <= s.dataWords())
            return version;
    }
    return 39;
}

uint16_t CQrEncoder::getTotalWords(QrDataSegment *segments, uint8_t length, uint8_t version){
    uint16_t total = 0;
    for(uint8_t i = 0; i < length; i++){
        total += segments[i].getWordCount(version);
    }
    return total;
}

uint8_t CQrEncoder::getMinVersion(uint16_t length, QrMode mode, QrEcc ecLevel) {
    QrMode m = mode == QrMode::Unspecified ? QrMode::Byte : mode;
    for(uint8_t v = 0; v < 40; v++){
        if (qr_capacities[m][v][ecLevel] > length)
            return v;
    } 
    return 39;
}

QrDataSegment* CQrEncoder::getSegments(const char* data, uint16_t length, uint8_t version, QrMode& mode, uint8_t &segLength){
    QrDataSegment* seg = nullptr;;

    if (mode == QrMode::Unspecified) {
#ifdef QR_OPTIMIZE_SEGMENTS
            seg = optimizeSegments(data, length, version, segLength);
            mode = segLength == 1 ? seg[0].mode : QrMode::Mixed;
#else
            mode = getMode(data,length);
#endif
    }
    if(seg == nullptr){
        seg = new QrDataSegment[1];
        seg->from = 0;
        seg->to = length;
        seg->mode = mode;
        segLength = 1;
    }
    return seg;
}

#ifdef QR_OPTIMIZE_SEGMENTS
/**
 * Optimizes the segments used to encode the given data.
 * The algorithm works by starting with one segment for each character and then merging adjacent segments
 * that can be represented by a more compact mode. At the end, the segments are merged again to reduce the 
 * number of segments.
 * @param data the data to encode
 * @param length the length of the data
 * @param version the version of the QR code (may be modified)
 * @param resultLength the number of segments returned
 * @return an array of segments
 */
QrDataSegment* CQrEncoder::optimizeSegments(const char* data, uint16_t length, uint8_t version, uint8_t& resultLength) {
    const uint8_t maxSegments = 128;
    QrDataSegment* segments = new QrDataSegment[maxSegments];

    // Initialize the first segment
    segments[0].from = 0;
    segments[0].to = 0;
    segments[0].mode =  length == 0 ? QrMode::Numeric : getMode(data[0]);
    resultLength = 1;
    uint8_t segmentIndex = 0;

    // Iterate over the data
    for (uint16_t i = 1; i < length; i++) {
        segments[segmentIndex].to = i;
        // Get the mode of the current character
        QrMode mode = getMode(data[i]);

        // If the mode is different from the previous segment, create a new segment
        if (segments[segmentIndex].mode != mode) {
            // Check if we have enough space for another segment
            if (segmentIndex == maxSegments - 1) {
                // If not, merge the segments to free up space
                mergeSegmentsBySize(segments, resultLength, version);
                segmentIndex = resultLength - 1;
                // If we still don't have enough space, merge the smallest segment
                if (segmentIndex == maxSegments - 1) {
                    mergeLeastPainfulSegment(segments, resultLength, version);
                    segmentIndex = resultLength - 1;
                    // WTF? 128 segments and nothing to merge? Get out of here....
                    // If we still don't have enough space, we can't do anything else
                    // except for return a failure code. But we don't do that yet.
                }    
                // Retry the loop with the new segment
                i--; 
            } else {
                // If we have enough space, just create a new segment
                segmentIndex++;
                resultLength = segmentIndex + 1;
                segments[segmentIndex].from = i;
                segments[segmentIndex].mode = mode;
            }
        }
    }
    segments[resultLength-1].to = length;

    // Merge the segments one last time
    mergeSegmentsBySize(segments, resultLength, version);

    // Return the segments
    return segments;
}

/**
 * Merges the segment that will have the least negative impact on overall size with its adjacent one
 * @param segments pointer to the segments array
 * @param segmentCount the length of the segments array
 * @param version the QR code version
 */
void CQrEncoder::mergeLeastPainfulSegment(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version) {
    if (segmentCount == 1) return;
    int16_t minScore = 0x7FFF;
    uint8_t i, minSegmentIndex = 1;
    uint16_t 
        currSegmentLength = segments->to - segments->from, 
        currSegmentSize = segments[0].getWordCount(version);

    // Find the segment with the least negative impact on overall size
    for (i = 1; i < segmentCount; ++i) {
        uint16_t 
            prevSegmentLength = currSegmentLength,
            prevSegmentSize = currSegmentSize;
        currSegmentLength = segments[i].getWordCount(version);
        currSegmentSize = segments[i].to - segments[i].from;

        int score = QrDataSegment::getWordCount(
            max(segments[i-1].mode, segments[i].mode),
            version,
            prevSegmentLength + currSegmentLength
        ) - prevSegmentSize + currSegmentSize;

        if (score < minScore) {
            minSegmentIndex = i;
        }
    }

    // Merge the segment with its adjacent one
    segments[minSegmentIndex].from = segments[minSegmentIndex - 1].from;
    segments[minSegmentIndex].mode = max(
        segments[minSegmentIndex -1].mode,
        segments[minSegmentIndex].mode
    );

    // Shift the other segments
    for (i = minSegmentIndex; i < segmentCount; ++i) {
        segments[minSegmentIndex -1] = segments[minSegmentIndex];
    }
    segmentCount--;
    mergeAdjacentSegments(segments, segmentCount);
}

void CQrEncoder::mergeAdjacentSegments(QrDataSegment *segments, uint8_t &segmentCount) {
    for (uint8_t i = 0; i < segmentCount - 1; ++i) {
        if (segments[i].mode == segments[i + 1].mode) {
            segments[i + 1].from = segments[i].from;
            for (uint8_t j = i; j < segmentCount - 1; ++j) {
                segments[j] = segments[j + 1];
            }
            segmentCount--;
            i--;
        }
    }
}

void CQrEncoder::mergeSegmentsBySize(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version) {
    for (uint8_t index = 1; index < segmentCount; ++index) {
        QrDataSegment *previousSegment = &segments[index - 1];
        QrDataSegment *currentSegment = &segments[index];

        uint16_t combinedLength = (previousSegment->to - previousSegment->from) + (currentSegment->to - currentSegment->from);
        QrMode optimalMode = max(previousSegment->mode, currentSegment->mode);

        uint16_t separateWordCount = previousSegment->getWordCount(version) + currentSegment->getWordCount(version);
        uint16_t mergedWordCount = QrDataSegment::getWordCount(optimalMode, version, combinedLength);

        if (separateWordCount >= mergedWordCount) {
            previousSegment->to = currentSegment->to;
            previousSegment->mode = optimalMode;

            for (uint8_t shiftIndex = index; shiftIndex < segmentCount - 1; ++shiftIndex) {
                segments[shiftIndex] = segments[shiftIndex + 1];
            }
            segmentCount--;
            --index;
        }
    }
}
#endif // QR_OPTIMIZE_SEGMENTS

/**
 * Determines the optimal QR mode for a given string of characters.
 *
 * This function takes a given string and determines the optimal QR mode for
 * encoding it. The function iterates over the string and evaluates each
 * character, determining the optimal mode based on the character's value.
 * The function returns the optimal mode as a QrMode value.
 *
 * @param data The string to evaluate.
 * @param length The length of the string.
 * @return The optimal QrMode for the given string.
 */
QrMode CQrEncoder::getMode(const char* data, uint16_t length) {
    QrMode optimalMode = QrMode::Numeric;
    for (uint16_t i = 0; i < length; ++i) {
        QrMode currentMode = getMode(data[i]);
        if (currentMode == QrMode::Byte) {
            return QrMode::Byte;
        }
        optimalMode = max(optimalMode, currentMode);
    }
    return optimalMode;
}

/**
 * Determines the QR mode for a single character.
 *
 * This function evaluates a given character and determines the appropriate
 * QR mode for encoding it. The possible modes are Numeric, AlphaNumeric,
 * and Byte. Numeric mode is selected for digits ('0'-'9'), AlphaNumeric
 * mode is selected for uppercase letters ('A'-'Z') and special alphanumeric
 * characters, and Byte mode is selected for all other characters.
 *
 * @param c The character to evaluate.
 * @return The corresponding QrMode for the character.
 */

QrMode CQrEncoder::getMode(char c) {
    // Check for numeric characters
    if (c >= '0' && c <= '9') {
        return QrMode::Numeric;
    }

    // Check for alphanumeric characters
    if (c >= 'A' && c <= 'Z') {
        return QrMode::AlphaNumeric;
    }

    // Check for special alphanumeric characters
    for (uint8_t i = 0; i < 9; ++i) {
        if (qr_alphanum_chars[i] == c) {
            return QrMode::AlphaNumeric;
        }
    }

    // Default to byte mode
    return QrMode::Byte;
}

void CQrEncoder::addData(uint8_t *buff, QrBufferCursor& cursor, const char* data, uint16_t length, QrMode mode){
    switch(mode) {
        case QrMode::Numeric:
            return this->addDataNum(data, length, buff, cursor);
        case QrMode::AlphaNumeric:
            return this->addDataAlpha(data, length, buff, cursor);
        case QrMode::Byte:
            return this->addDataByte(data, length, buff, cursor);
        default: 
            return;
    }
}
void CQrEncoder::addDataNum(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor){
    uint16_t i = 0;
    uint16_t acc = 0;
    while (i < length) {
        acc = (acc*10)+(data[i] - '0');
        i++;
        if (i % 3 > 0) continue;

        addValue(buff, cursor, acc, 10);
        acc = 0;
    } 
    uint8_t rem = i % 3;
    if (rem == 2) {
        addValue(buff, cursor, acc, 7);
        }
    if (rem == 1) {
        addValue(buff, cursor, acc, 4);
    }
}
void CQrEncoder::addDataAlpha(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor){
    uint16_t i = 0;
    uint16_t acc = 0;
    while (i < length) {
        uint8_t d = 0;
        if (data[i] >= '0' && data[i] <= '9')
            d = data[i] - '0';
        else if (data[i] >= 'A' && data[i] <= 'Z')
            d = 10 + data[i] - 'A';
        else {
            for(uint8_t j = 0;j < 9;j++) {
                if (qr_alphanum_chars[j] != data[i]) continue;
                d = 36 + j;
                j = 9;
            }
        }
        if (i & 1) 
            acc *= 45; 
        acc += d;
        i++;  
        
        if (i & 1) continue;
        addValue(buff, cursor, acc, 11);

        acc = 0;
    } 
    
    if (i & 1) {
        addValue(buff, cursor, acc, 6);
    }

}
void CQrEncoder::addDataByte(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor){
    uint16_t i = 0;
    while (i < length) {
        addValue(buff, cursor, data[i], 8);
        i++;
    } 
}
void CQrEncoder::addValue(uint8_t *buff, QrBufferCursor& cursor, uint16_t value, uint8_t bitLength) {
    if (bitLength > 8){
        addValue(buff, cursor, value >> 8, bitLength - 8);
        addValue(buff, cursor, value & 0xFF, 8);
        return;
    }

    int8_t sh = 8 - cursor.bit - bitLength;
    if (sh >= 0){
        buff[cursor.byte] |= value << sh;
        cursor.bit += bitLength;
    } else {
        buff[cursor.byte] |= value >> (-sh);
        cursor.bit = 0;
        cursor.byte++;
        
        
        buff[cursor.byte] |= value << (8+sh);
        cursor.bit += -sh;
    }

    if (cursor.bit == 8) {
        cursor.bit = 0;
        cursor.byte++;
    }
}


void CQrEncoder::addStopBitsAndPad(uint8_t *buff, QrBufferCursor& cursor){
    addValue(buff,cursor,0,4);
    if(cursor.bit != 0){
        addValue(buff, cursor, 0, 8-cursor.bit);
    }
}
void CQrEncoder::addPaddingBytes(uint8_t *buff, QrBufferCursor& cursor, uint16_t length){
    uint8_t f = 0;
    while(cursor.byte < length){
        addValue(buff,cursor, f ? 17 : 236, 8);
        f = !f;
    }
}

CQrEncoder QrEncoder;