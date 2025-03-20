#ifndef __QR_ENCODER
#define __QR_ENCODER

#include <stdint.h>
#include "qr_types.h"
#include "qr_segment.h"

#if defined ARDUINO || defined ESP32 
    #include <arduino.h>
#else 
    #define min(a,b) ((a) > (b) ? (b) : (a))
    #define max(a,b) ((a) > (b) ? (a) : (b))
#endif

class CQrEncoder{
    public:    
    uint8_t* encode(const char* data, uint16_t length, uint16_t& resultLen, uint8_t& version, QrMode& mode, QrEcc ecLevel = QrEcc::L);

#ifndef QR_TESTING
    private:
#endif
    void splitBlob(uint8_t *buff, QrBlockStruct s);
    void mixBlocks(uint8_t *buff, QrBlockStruct str);
    void writeSegmentData(const char* data, uint16_t dataLen, uint8_t *buff, QrBufferCursor& cur, uint8_t version, QrMode mode);
    void writeEdc(uint8_t *buff, QrBlockStruct s);

    uint8_t optimizeVersion(QrDataSegment *segments, uint8_t segLength, uint8_t version, QrEcc ecLevel);
    uint16_t getTotalWords(QrDataSegment *segments, uint8_t length, uint8_t version);
    uint8_t getMinVersion(uint16_t length, QrMode mode, QrEcc ecLevel);
    QrDataSegment* getSegments(const char* data, uint16_t length, uint8_t version, QrMode& mode, uint8_t &segLength);

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
    QrDataSegment* optimizeSegments(const char* data, uint16_t length, uint8_t version, uint8_t& resultLength);
    
    /**
     * Merges the segment that will have the least negative impact on overall size with its adjacent one
     * @param segments pointer to the segments array
     * @param segmentCount the length of the segments array
     * @param version the QR code version
     */
    void mergeLeastPainfulSegment(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version);
    void mergeAdjacentSegments(QrDataSegment *segments, uint8_t &segmentCount);
    void mergeSegmentsBySize(QrDataSegment *segments, uint8_t& segmentCount, uint8_t version);
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
    QrMode getMode(const char* data, uint16_t length);

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

    QrMode getMode(char c);

    void addData(uint8_t *buff, QrBufferCursor& cursor, const char* data, uint16_t length, QrMode mode);
    void addDataNum(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor);
    void addDataAlpha(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor);
    void addDataByte(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor& cursor);
    void addValue(uint8_t *buff, QrBufferCursor& cursor, uint16_t value, uint8_t bitLength);
    void addStopBitsAndPad(uint8_t *buff, QrBufferCursor& cursor);
    void addPaddingBytes(uint8_t *buff, QrBufferCursor& cursor, uint16_t length);
};

extern CQrEncoder QrEncoder;

#endif //__QR_ENCODER