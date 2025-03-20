#include "qr_segment.h"
#include "qr_data.h"

QrDataSegment::QrDataSegment(uint16_t from, uint16_t to, uint8_t mode){
    QrDataSegment::from = from;
    QrDataSegment::to = to;
    QrDataSegment::mode = (QrMode)mode;
}
uint16_t QrDataSegment::getWordCount(uint8_t version){
    return QrDataSegment::getWordCount(mode, version, to-from);
}
uint16_t QrDataSegment::getWordCount(QrMode mode, uint8_t version, uint16_t length) {
    uint16_t bits = 4, // mode bits 
        charsInUnit = qr_mode_charsPerUnit[mode],
        unitLength = qr_mode_unitBitLength[mode];
    bits += qr_lengthBits(version, mode);
    bits += ((length * unitLength) + charsInUnit - 1) 
        / charsInUnit; 
    return (bits + 7) >> 3; // poor man's Math.ceil(x/8)
}