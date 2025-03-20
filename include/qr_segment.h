#ifndef __QR_SEGMENT
#define __QR_SEGMENT

#include <stdint.h>
#include "qr_types.h"

struct QrDataSegment {
    uint16_t from = 0;
    uint16_t to = 0;
    QrMode mode = QrMode::Numeric;
    
    QrDataSegment() {}
    QrDataSegment(uint16_t from, uint16_t to, uint8_t mode);
    uint16_t getWordCount(uint8_t version);
    static uint16_t getWordCount(QrMode mode, uint8_t version, uint16_t length);
};

#endif