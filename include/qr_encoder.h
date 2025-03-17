#ifndef __QR_ENCODER
#define __QR_ENCODER

#include <stdint.h>
#include "qr_types.h"
#include "qr_data.h"

#if defined ARDUINO || defined ESP32 
    #include <arduino.h>
#else 
    #define min(a,b) ((a) > (b) ? (b) : (a))
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

struct QrDataSegment {
    uint16_t from = 0;
    uint16_t to = 0;
    QrMode mode = QrMode::Numeric;
    QrDataSegment() {}
    QrDataSegment(uint16_t from, uint16_t to, uint8_t mode){
        QrDataSegment::from = from;
        QrDataSegment::to = to;
        QrDataSegment::mode = (QrMode)mode;
    }
    uint16_t getWordCount(uint8_t version){
        return QrDataSegment::getWordCount(mode, version, to-from);
    }
    static uint16_t getWordCount(QrMode mode, uint8_t version, uint16_t length) {
        uint16_t bits = 8, // mode bits + stop bits 
            charsInUnit = qr_mode_charsPerUnit[mode],
            unitLength = qr_mode_unitBitLength[mode];
        bits += qr_lengthBits(version, mode);
        bits += ((length * unitLength) + charsInUnit - 1) 
            / charsInUnit; 
        return (bits + 7) >> 3; // poor man's Math.ceil(x/8)
    }
};

#ifdef QR_DEBUG

    void print_segments(const QrDataSegment *cs, uint8_t sl, uint8_t v){
        printf("\n%d segments", sl);
        QrDataSegment *s = (QrDataSegment*)cs;
        uint16_t total = 0;
        for (uint8_t i = 0; i < sl; i++){
            QrMode m = s[i].mode;
            total += s[i].getWordCount( v);
            printf("\n(%3d - %3d): %5s (%3d)", s[i].from, s[i].to, m == 0 ? "Num" : m == 1 ? "Alpha" : "Byte", s[i].getWordCount(v));
        }
        printf("\n(total: %4d)\n", total);
    }

#endif

class CQrEncoder{
    public:    
    uint8_t* encode(const char* data, uint16_t length, uint16_t& resultLen, uint8_t& version, QrMode& mode, QrEcc ecLevel = QrEcc::L) {
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

        QrBlockStruct s = qr_blocks[version][ecLevel];
        uint16_t buffSize = s.totalWords(),
            dataWordsWritten = 0;
        uint8_t *buff = new uint8_t[buffSize];
        memset(buff, 0, buffSize);
        uint8_t *currBuff = buff;
        QrBufferCursor cur;


        for(uint8_t i = 0; i < segLength; i++){
            cur.bit = 0;
            cur.byte = 0;
            writeSegmentData(data + seg[i].from, seg[i].to - seg[i].from, currBuff, &cur, version, mode);
            currBuff += cur.byte;
            dataWordsWritten += cur.byte;
        }
        // Add padding to specified length
        cur.bit = 0;
        cur.byte = 0;
        addPaddingBytes(currBuff, &cur, s.dataWords() - dataWordsWritten);

        splitBlob(buff, dataWordsWritten, s);
        writeEdc(buff, s);
        mixBlocks(buff, buffSize, s);

        resultLen = buffSize;
        return buff;
    }

#ifndef QR_TESTING
    private:
#endif
    void splitBlob(uint8_t *buff, uint16_t buffSize, QrBlockStruct s) {
        uint16_t 
            iSrc = s.dataWords(),
            iDst = s.totalWords();
        for(uint8_t i = 0; i < s.longBlocks.blockCount; i++){
            iSrc -= s.longBlocks.dataWordsPerBlock;
            iDst -= s.ecWordsPerBlock + s.longBlocks.dataWordsPerBlock;
            for(int16_t j = s.longBlocks.dataWordsPerBlock; j >= 0; j--){
                buff[iDst + j] = buff[iSrc + j];
            }
        }
        // the first data block is already in place
        for(uint8_t i = 0; i < s.shortBlocks.blockCount; i++){
            iSrc -= s.shortBlocks.dataWordsPerBlock;
            iDst -= s.ecWordsPerBlock + s.shortBlocks.dataWordsPerBlock;
            for(int16_t j = s.shortBlocks.dataWordsPerBlock; j >= 0; j--){
                buff[iDst + j] = buff[iSrc + j];
            }
        }
    }

    void mixBlocks(uint8_t *buff, uint16_t buffSize, QrBlockStruct str){
        uint16_t 
            sbc = str.shortBlocks.blockCount,
            lbc = str.longBlocks.blockCount,
            tbc = sbc+lbc,
            sbl = str.shortBlocks.dataWordsPerBlock + str.ecWordsPerBlock,
            lbl = str.longBlocks.dataWordsPerBlock + str.ecWordsPerBlock,
            obl;

        if (tbc < 2) return;

        if (sbc > 0 && lbc == 0){
            lbc = sbc;
            sbc = 0;
            lbl = sbl;
            sbl = 0;
        }
        obl = lbl;
            
        uint8_t temp[tbc];
        
        for (uint16_t i0 = 0; i0 < obl - 1; i0++) {
            // get a batch of values
            for(uint16_t i1 = 0; i1 < tbc; i1++) {
                uint16_t i2 = i1 * lbl;
                if (i1 >= lbc)
                    i2 -= i1 - lbc;
                temp[i1] = buff[i2]; 
                buff[i2] = 0;
            }

            // shift to eliminate 'empty' spaces
            sbl = sbl > 0 ? sbl - 1 : 0;
            lbl = lbl > 0 ? lbl - 1 : 0;
            int16_t p = buffSize - 1;
            uint8_t sh = 0;

            for (uint8_t i1 = 0; i1 < sbc; i1++){
                for (uint8_t i2 = 0; i2 < sbl; i2++){
                    if (sh)
                        buff[p] = buff[p-sh];
                    p--;
                }
                sh++;
            }
            for (uint8_t i1 = 0; i1 < lbc; i1++){
                for (uint8_t i2 = 0; i2 < lbl; i2++){
                    if (sh)
                        buff[p] = buff[p-sh];
                    p--;
                }
                sh++;
            }

            // write back temp to buff
            memcpy(buff, temp, tbc);

            buff += tbc;
            buffSize -= tbc;
        }
    }

    void writeSegmentData(const char* data, uint16_t dataLen, uint8_t *buff, QrBufferCursor *cur, uint8_t version, QrMode mode){
        addValue(buff, cur, qr_mode_indicator[mode], 4);
        addValue(buff, cur, dataLen, qr_lengthBits(version, mode));
        addData(buff, cur, data, dataLen, mode);
        addStopBitsAndPad(buff, cur);
    }

    void writeEdc(uint8_t *buff, QrBlockStruct s){
        uint16_t dataWords = s.shortBlocks.dataWordsPerBlock,
        blockWords = dataWords + s.ecWordsPerBlock;
        for(uint8_t i = 0; i < s.shortBlocks.blockCount; i++){
            uint16_t edcLen = 0;

            uint8_t *edc = Gf256.getEdc(buff, dataWords, blockWords, edcLen);
            memcpy(buff + dataWords, edc, edcLen);
            delete[] edc;
            buff += blockWords;
        }
        dataWords = s.longBlocks.dataWordsPerBlock;
        blockWords = dataWords + s.ecWordsPerBlock;
        for(uint8_t i = 0; i < s.longBlocks.blockCount; i++){
            uint16_t edcLen = 0;

            uint8_t *edc = Gf256.getEdc(buff, dataWords, blockWords, edcLen);
            memcpy(buff + dataWords, edc, edcLen);
            delete[] edc;
            buff += blockWords;
        }
    }

    uint8_t optimizeVersion(QrDataSegment *segments, uint8_t segLength, uint8_t version, QrEcc ecLevel) {
        QrBlockStruct s = qr_blocks[version][ecLevel];
        uint16_t actualWords = getTotalWords(segments, segLength, version);
        if (actualWords < s.dataWords()){
            while(true&& version > 0) {
                s = qr_blocks[version - 1][ecLevel];
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
            s = qr_blocks[version][ecLevel];
            if (getTotalWords(segments, segLength, version) <= s.dataWords())
                return version;
        }
        return 39;
    }

    uint16_t getTotalWords(QrDataSegment *segments, uint8_t length, uint8_t version){
        uint16_t total = 0;
        for(uint8_t i = 0; i < length; i++){
            total += segments[i].getWordCount(version);
        }
        return total;
    }

    uint8_t getMinVersion(uint16_t length, QrMode mode, QrEcc ecLevel) {
        QrMode m = mode == QrMode::Unspecified ? QrMode::Byte : mode;
        for(uint8_t v = 0; v < 40; v++){
            if (qr_capacities[m][v][ecLevel] > length)
                return v;
        } 
        return 39;
    }

    QrDataSegment* getSegments(const char* data, uint16_t length, uint8_t version, QrMode& mode, uint8_t &segLength){
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
    QrDataSegment* optimizeSegments(const char* data, uint16_t length, uint8_t version, uint8_t& resultLength) {
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
    void mergeLeastPainfulSegment(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version) {
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
    
    void mergeAdjacentSegments(QrDataSegment *segments, uint8_t &segmentCount) {
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

    void mergeSegmentsBySize(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version) {
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
    QrMode getMode(const char* data, uint16_t length) {
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

    QrMode getMode(char c) {
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

    void addData(uint8_t *buff, QrBufferCursor *cursor, const char* data, uint16_t length, QrMode mode){
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
    void addDataNum(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
        uint16_t i = 0;
        uint16_t acc = 0;
        while (i < length) {
            acc = (acc*10)+(data[i] - '0');
            i++;
            if (i % 3 > 0) continue;

            addValue(buff, cursor, acc, 10);
            acc = 0;
        } 
        if (i%3 >0) {
            uint8_t rem  = 3 - (i % 3);
            for (uint8_t j = 0; j <rem; j++){
                acc = acc*10;
            }
            addValue(buff, cursor, acc, 10);
        }
    }
    void addDataAlpha(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
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
            acc *= 45;
            addValue(buff, cursor, acc, 11);
        }

    }
    void addDataByte(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
        uint16_t i = 0;
        while (i < length) {
            addValue(buff, cursor, data[i], 8);
            i++;
        } 
    }
    void addValue(uint8_t *buff, QrBufferCursor *cursor, uint16_t value, uint8_t bitLength) {
        if (bitLength > 8){
            addValue(buff, cursor, value >> 8, bitLength - 8);
            addValue(buff, cursor, value & 0xFF, 8);
            return;
        }

        int8_t sh = 8 - cursor->bit - bitLength;
        if (sh >= 0){
            buff[cursor->byte] |= value << sh;
            cursor->bit += bitLength;
        } else {
            buff[cursor->byte] |= value >> (-sh);
            cursor->bit = 0;
            cursor->byte++;
            
            
            buff[cursor->byte] |= value << (8+sh);
            cursor->bit += -sh;
        }

        if (cursor->bit == 8) {
            cursor->bit = 0;
            cursor->byte++;
        }
    }

    
    void addStopBitsAndPad(uint8_t *buff, QrBufferCursor *cursor){
        addValue(buff,cursor,0,4);
        if(cursor->bit != 0){
            addValue(buff, cursor, 0, 8-cursor->bit);
        }
    }
    void addPaddingBytes(uint8_t *buff, QrBufferCursor *cursor, uint16_t length){
        uint8_t f = 0;
        while(cursor->byte < length){
            addValue(buff,cursor, f ? 17 : 236, 8);
            f = !f;
        }
    }
};

CQrEncoder QrEncoder;

#endif //__QR_ENCODER