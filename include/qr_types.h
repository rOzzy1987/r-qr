#ifndef __QR_TYPES_H
#define __QR_TYPES_H

#include <stdint.h>
#include <string.h>

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
 * Defines the data structure for a QR code
 */
struct QrBlockStruct {
    uint8_t ecWordsPerBlock;
    uint8_t dataWordsPerShortBlock;
    uint8_t shortBlocks;
    uint8_t longBlocks;

    uint16_t dataWords() {
        return (shortBlocks * dataWordsPerShortBlock) + (longBlocks * (dataWordsPerShortBlock+1));
    }
    uint16_t totalWords() {
        return dataWords() + ecWords();
    }
    uint16_t ecWords() {
        return (shortBlocks + longBlocks) * ecWordsPerBlock;
    }
};

enum QrMode {
    Numeric = QR_MODE_NUM,
    AlphaNumeric = QR_MODE_ALPHA,
    Byte = QR_MODE_BYTE,
    Mixed = 0xFE,
    Unspecified = 0xFF
};

enum QrEcc {
    L = QR_ERR_L,
    M = QR_ERR_M,
    Q = QR_ERR_Q,
    H = QR_ERR_H
};

extern "C" struct QrCode {
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
     * Error correction level
     */
    uint8_t mode;
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

    /**
     * format polynomial
     * since it only contains 1s and 0s, we can just use an int and do operations bitwise 
     */
    uint16_t formatPoly;

    /**
     * version polymonial
     * Only comes into play once version (0 based) is larger than 5
     */
    uint32_t versionPoly;

    QrCode() {
        QrCode(0, QrEcc::L);
    }
    
    QrCode(uint8_t version, uint8_t ecLevel){
        this->version = version;
        this->ecLevel = ecLevel;
        version ++;

        size = (version << 2) + 17;
        bitmapStride = (size + 7) >> 3;
        bitmapSize = size * bitmapStride;
        bitmap = new uint8_t[bitmapSize];
        memset(bitmap, 0, bitmapSize);

        this->mode = 0;
        this->formatPoly = 0;
        this->versionPoly = 0;
        this->raw = nullptr;
        this->rawSize = 0;
    }

    QrCode(const QrCode &c) {
        this->bitmapSize = c.bitmapSize;
        this->bitmapStride = c.bitmapStride;
        this->ecLevel = c.ecLevel;
        this->formatPoly = c.formatPoly;
        this->mask = c.mask;
        this->rawSize = c.rawSize;
        this->size = c.size;
        this->version = c.version;
        this->versionPoly = c.versionPoly;

        this->bitmap = new uint8_t[c.bitmapSize];
        this->raw = this->rawSize == 0 ? nullptr : new uint8_t[this->rawSize];
    }

    ~QrCode(){
        if (this->bitmap != nullptr)
            delete[] this->bitmap;
        if (this->raw != nullptr)
            delete[] this->raw;
    }
};

struct QrBufferCursor {
    uint16_t byte = 0;
    uint8_t bit = 0;
};


#endif