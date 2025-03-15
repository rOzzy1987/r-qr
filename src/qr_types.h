#ifndef __QR_TYPES_H
#define __QR_TYPES_H

#include <stdint.h>
#include <stdlib.h>

#define QR_MODE_NUM 0
#define QR_MODE_ALPHA 1 
#define QR_MODE_BYTE 2

#define QR_ERR_L 0
#define QR_ERR_M 1
#define QR_ERR_Q 2
#define QR_ERR_H 3

/**
 * Defines the data structure for a group in a block
 */
struct QrGroupStruct {
    uint8_t blockCount = 0;
    uint8_t dataWordsPerBlock = 0;

    QrGroupStruct(uint8_t bc, uint8_t dw) {
        dataWordsPerBlock = dw;
        blockCount = bc;
    }
};

/**
 * Defines the data structure for a block
 * 
 * 
    v3-L {15, {1, 55}, {0, 0}},
 */
struct QrBlockStruct {
    uint8_t ecWordsPerBlock;
    QrGroupStruct group1;
    QrGroupStruct group2;

    uint16_t dataWords() {
        return group1.blockCount * group1.dataWordsPerBlock + group2.blockCount * group2.dataWordsPerBlock;
    }
    uint16_t totalWords() {
        return dataWords() + ecWords();
    }
    uint16_t ecWords() {
        return (group1.blockCount + group2.blockCount) * ecWordsPerBlock;
    }
};

enum QrMode {
    Numeric = QR_MODE_NUM,
    AlphaNumeric = QR_MODE_ALPHA,
    Byte = QR_MODE_BYTE,
    Unspecified = 0xFF
};

enum QrEcc {
    L = QR_ERR_L,
    M = QR_ERR_M,
    Q = QR_ERR_Q,
    H = QR_ERR_H
};

struct QrCode {
    /**
     * QR code version (0 based!) 
     */
    uint8_t version;
    /**
     * pixel size of the edge of the square
     */
    uint8_t size;
    /**
     * Mask used;
     */
    uint8_t mask = 0;
    /**
     * Error correction level
     */
    uint8_t ecLevel;
    /**
     * 1 bit bitmap of the code
     */
    uint8_t *bitmap;
    /**
     * bitmap sie in bytes
     */
    uint16_t bitmapSize;
    /**
     * bitmap line length in bits
     */
    uint8_t bitmapStride;
    /**
     * raw encoded data & edc
     */
    uint8_t *raw;
    /**
     * raw data size in bytes
     */
    uint16_t rawSize;

    uint8_t *formatPoly;
    
    QrCode(uint8_t version, uint8_t ecLevel){
        this->version = version;
        this->ecLevel = ecLevel;
        version ++;

        size = (version << 2) + 17;
        bitmapStride = (size >> 3) + 1;
        bitmapSize = size * bitmapStride;
        bitmap = new uint8_t[bitmapSize];
        memset(bitmap, 0, bitmapSize);

        formatPoly =  new uint8_t[15];
        memset(formatPoly, 0, 15 );
    }
};

struct QrBufferCursor {
    uint16_t byte = 0;
    uint8_t bit = 0;
};


#endif