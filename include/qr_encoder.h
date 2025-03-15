#ifndef __QR_ENCODER
#define __QR_ENCODER

#include <stdint.h>
#include "qr_types.h"
#include "qr_data.h"

#define min(a,b) (a) > (b) ? (b) : (a)
#define max(a,b) (a) > (b) ? (a) : (b)

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
};

class CQrEncoder{
    public:
    bool optimize = true;

    uint8_t* encode(const char* data, uint16_t length, QrEcc ecLevel = QrEcc::L, QrMode mode = QrMode::Unspecified, uint8_t version = 0xFF) {
        QrBlockStruct s = qr_blocks[version][ecLevel];

        uint8_t *buff = new uint8_t[s.totalWords()];
        QrBufferCursor cur;
        QrDataSegment* seg = nullptr;
        uint8_t segL;
    
        if (mode == QrMode::Unspecified) {
            if(optimize) {
                seg = optimizeSegments(data, length, 12, segL);
            } else {
                mode = getMode(data,length);
            }
        }
        if(seg == nullptr){
            seg = new QrDataSegment[1];
            seg->from = 0;
            seg->to = length;
            seg->mode = mode;
        }

        for(uint8_t i = 0; i < s.shortBlocks.blockCount; i++){

        }
        return nullptr;
    }

    QrDataSegment* optimizeSegments(const char* data, uint16_t length, uint8_t version, uint8_t& resultLength) {
        const uint8_t maxSegments = 128;
        QrDataSegment* segments = new QrDataSegment[maxSegments];

        segments[0].from = 0;
        segments[0].mode = getMode(data[0]);

        uint8_t segmentIndex = 0;
        for (uint16_t i = 1; i < length; i++) {
            QrMode mode = getMode(data[i]);
            if (segments[segmentIndex].mode != mode) {
                if (segmentIndex == maxSegments - 1) {
                    mergeSegments(segments, maxSegments, version, segmentIndex);
                    if (segmentIndex == maxSegments - 1) {
                        mergeSmallestSegment(segments, maxSegments, segmentIndex);
                        // WTF? 128 segments and nothing to merge? Get out of here....
                    }    
                    i--; // Retry after merging;
                } else {
                    segmentIndex++;
                    segments[segmentIndex].from = i;
                    segments[segmentIndex].mode = mode;
                }
            }
            segments[segmentIndex].to = i + 1;
        }

        mergeSegments(segments, maxSegments, version, segmentIndex);
        resultLength = segmentIndex + 1;
        return segments;
    }

    /**
 * Merges the smallest segment that has a less compact mode with its adjacent segments.
 * After merging, adjacent segments with the same mode are also merged. 
 * This operation reduces the size of segments by at most 2.
 * @param segments array of segments to merge
 * @param len the length of the segments array
 * @param lastIdx the index of the last active element after the merge.
 */
void mergeSmallestSegment(QrDataSegment *segments, uint8_t len, uint8_t &lastIdx) {
    uint8_t minIndex = 0, smallestSize = segments[0].to - segments[0].from;
    QrMode minMode = segments[0].mode;

    // Find the smallest segment with a neighbor of less compact mode
    for (uint8_t i = 0; i < len; ++i) {
        bool canMerge = (i > 0 && segments[i - 1].mode > segments[i].mode) ||
                        (i < len - 1 && segments[i + 1].mode > segments[i].mode);

        if (!canMerge) continue;

        uint8_t currentSize = segments[i].to - segments[i].from;
        QrMode currentMode = segments[i].mode;

        if (currentMode < minMode || (currentMode == minMode && currentSize < smallestSize)) {
            minIndex = i;
            smallestSize = currentSize;
            minMode = currentMode;
        }
    }

    // Merge the smallest segment with its adjacent one
    if (minIndex > 0) {
        segments[minIndex - 1].to = segments[minIndex].to;
        for (uint8_t i = minIndex; i < len - 1; ++i) {
            segments[i] = segments[i + 1];
        }
        len--;
    } else if (minIndex < len - 1) {
        segments[minIndex + 1].from = segments[minIndex].from;
        for (uint8_t i = minIndex; i < len - 1; ++i) {
            segments[i] = segments[i + 1];
        }
        len--;
    }

    // Merge adjacent segments with the same mode
    for (uint8_t i = 0; i < len - 1; ++i) {
        if (segments[i].mode == segments[i + 1].mode) {
            segments[i + 1].from = segments[i].from;
            for (uint8_t j = i; j < len - 1; ++j) {
                segments[j] = segments[j + 1];
            }
            len--;
        }
    }

    lastIdx = len - 1;
}

    
/**
 * Merges adjacent QR data segments if combining them results in a lower
 * word count than encoding them separately. This function iterates 
 * through the given segments and checks if merging two consecutive 
 * segments results in fewer words for the QR code. If so, it merges 
 * them into a single segment, adjusts the array, and updates the new 
 * length of the segments array.
 *
 * @param segments The array of QR data segments to be merged.
 * @param len The initial length of the segments array.
 * @param version The QR version number (0-based).
 * @param lastIdx the index of thelast active element after the merge.
 */

    void mergeSegments(QrDataSegment *segments, uint8_t len, uint8_t version, uint8_t &lastIdx) {
        // Merge adjacent segments if combining them results in fewer words
        for (uint8_t i = 1; i < lastIdx; ++i) {
            QrDataSegment *prev = &segments[i - 1];
            QrDataSegment *cur = &segments[i];

            if (getSegmentWordCount(prev, version) + getSegmentWordCount(cur, version) >=
                getSegmentWordCount(max(prev->mode, cur->mode), version, (prev->to - prev->from) + (cur->to - cur->from))) {
                prev->to = cur->to;

                // Remove the current segment and shift the rest
                for (uint8_t j = i; j < lastIdx - 1; ++j) {
                    segments[j] = segments[j + 1];
                }
                lastIdx--;
                --i;
            }
        }
    }


/**
 * Calculates the word count needed for a QR segment.
 *
 * This is a convenience method that uses the segment's mode and length
 * to calculate the word count required for the segment.
 *
 * @param segment The QR segment for which to calculate the word count.
 * @param version The QR version number (0-based).
 * @return The number of words required to encode the segment.
 */
    uint16_t getSegmentWordCount(QrDataSegment *segment, uint8_t version) {
        return getSegmentWordCount(segment->mode, version, segment->to - segment->from);
    }

/**
 * Calculates the word count needed for a QR segment.
 *
 * This function calculates the number of words required to encode
 * a QR data segment based on the given mode, version, and length
 * of the segment. It takes into account the mode bits, length bits,
 * and the number of units required for the specified length.
 *
 * @param mode The QR mode for the segment (e.g., Numeric, Alphanumeric, Byte).
 * @param version The QR version number (0-based).
 * @param length The length of the segment in characters.
 * @return The number of words required to encode the segment.
 */

    uint16_t getSegmentWordCount(QrMode mode, uint8_t version, uint16_t length) {
        uint16_t bits = 8, // mode bits + stop bits 
            charsInUnit = qr_mode_charsPerUnit[mode];
        bits += qr_lengthBits(version, mode);
        bits += (length + charsInUnit - 1) 
            * qr_mode_unitBitLength[mode] 
            / charsInUnit; 
        return (bits + 7) >> 3; // poor man's Math.ceil
    }

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
        QrMode 
            mode =  QrMode::Numeric,
            other;
        uint8_t isAplha = 0;
        for(uint16_t i = 0; i < length; i++){
            other = getMode(data[i]);

            mode = max(mode, other);

            if(mode == QrMode::Byte)
                return QrMode::Byte;
        }
        return mode;
    };

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

    QrMode getMode(char c){
        
        if (c >= '0' && c <= '9') return QrMode::Numeric;
        if (c >= 'A' && c <= 'Z') return QrMode::AlphaNumeric;

        bool isAplha = false;
        for(uint8_t j = 0; j < 9; j++){
            if (qr_alphanum_chars[j] == c) {
                return QrMode::AlphaNumeric;
            }
        }
        
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

    /**
     * Adds padding to the QR code data buffer to ensure it reaches the required length.
     * 
     * This function first adds a 4-bit terminator to the data buffer. If the current bit position
     * within the current byte is not aligned (i.e., not zero), it pads the remaining bits of 
     * the current byte with zeros to align to the next byte boundary. After that, it alternates 
     * between adding the bytes 236 and 17 to the buffer until the buffer length reaches the specified 
     * length 'len'. The padding helps ensure the QR code has a fixed size and is compliant with 
     * specifications.
     *
     * @param buff Pointer to the buffer where the data is stored.
     * @param cursor Pointer to the QrBufferCursor struct that tracks the current byte and bit position 
     * in the buffer.
     * @param len The length in bytes that the buffer should reach after padding.
     */
    void addPadding(uint8_t *buff, QrBufferCursor *cursor, uint16_t len){
        addValue(buff,cursor,0,4);
        if(cursor->bit != 0){
            addValue(buff, cursor, 0, 8-cursor->bit);
        }

        uint8_t f = 0;
        while(cursor->byte < len){
            addValue(buff,cursor, f ? 17 : 236, 8);
            f = !f;
        }
    }
};

CQrEncoder QrEncoder;

#endif //__QR_ENCODER