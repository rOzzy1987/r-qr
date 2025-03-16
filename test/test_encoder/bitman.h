#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"


void encoder_addValue() {

    uint8_t *buff = new uint8_t[4]{0,0,0,0};
    QrBufferCursor c;

    QrEncoder.addValue(buff, &c, 127, 7);
    ASSERT_EQ_UI8(254, buff[0]);
    ASSERT_EQ_UI8(0, buff[1]);
    ASSERT_EQ_UI16(0, c.byte);
    ASSERT_EQ_UI8(7, c.bit);

    QrEncoder.addValue(buff, &c, 5, 3);
    ASSERT_EQ_UI8(255, buff[0]);
    ASSERT_EQ_UI8(64, buff[1]);
    ASSERT_EQ_UI16(1, c.byte);
    ASSERT_EQ_UI8(2, c.bit);

    QrEncoder.addValue(buff, &c, 623, 10);
    ASSERT_EQ_UI8(64 + (623 >> 4), buff[1]);
    ASSERT_EQ_UI8((623 & 0x0F) << 4, buff[2]);
    ASSERT_EQ_UI16(2, c.byte);
    ASSERT_EQ_UI8(4, c.bit);
    
    delete[] buff;
}


void encoder_addDataNum() {
    uint8_t *buff = new uint8_t[4]{0,0,0,0};
    QrBufferCursor c;
    QrEncoder.addDataNum("789", 3, buff, &c);
    ASSERT_EQ_UI8(789 >> 2, buff[0]);
    ASSERT_EQ_UI8((789 & 0x03) << 6, buff[1]);
    ASSERT_EQ_UI16(1, c.byte);
    ASSERT_EQ_UI8(2, c.bit);
    QrEncoder.addDataNum("3", 1, buff, &c);
    ASSERT_EQ_UI8(((789 & 0x03) << 6) | 300 >> 4, buff[1]);
    ASSERT_EQ_UI8((300 & 0x0F) << 4, buff[2]);
    ASSERT_EQ_UI16(2, c.byte);
    ASSERT_EQ_UI8(4, c.bit);
    
    delete[] buff;
}

void encoder_addDataAlpha() {
    uint8_t *buff = new uint8_t[5]{0,0,0,0,0};
    QrBufferCursor c;
    QrEncoder.addDataAlpha("7AB", 3, buff, &c);
    ASSERT_EQ_UI16(2, c.byte);
    ASSERT_EQ_UI8(6, c.bit);
    ASSERT_EQ_UI8(40, buff[0]);
    ASSERT_EQ_UI8(167, buff[1]);
    ASSERT_EQ_UI8(188, buff[2]);
    QrEncoder.addDataAlpha("CD", 1, buff, &c);
    ASSERT_EQ_UI16(4, c.byte);
    ASSERT_EQ_UI8(1, c.bit);
    ASSERT_EQ_UI8(189, buff[2]);
    ASSERT_EQ_UI8(14, buff[3]);
    ASSERT_EQ_UI8(0, buff[4]);
    
    delete[] buff;
}

void encoder_addDataByte() {
    uint8_t *buff = new uint8_t[4]{0,0,0,0};
    QrBufferCursor c;
    QrEncoder.addDataByte("asdf", 3, buff, &c);
    ASSERT_EQ_UI16(3, c.byte);
    ASSERT_EQ_UI8(0, c.bit);
    ASSERT_EQ_UI8('a', buff[0]);
    ASSERT_EQ_UI8('s', buff[1]);
    ASSERT_EQ_UI8('d', buff[2]);
    QrEncoder.addDataByte("C", 1, buff, &c);
    ASSERT_EQ_UI16(4, c.byte);
    ASSERT_EQ_UI8(0, c.bit);
    ASSERT_EQ_UI8('C', buff[3]);
    
    delete[] buff;
}
