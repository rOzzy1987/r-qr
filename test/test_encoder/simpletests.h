#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"

void encoder_getMode_Byte() {
    const char* data = "https://www.qrcode.com/";
    QrMode result = QrEncoder.getMode(data, strlen(data));
    ASSERT_EQ_UI8(QrMode::Byte, result);
}

void encoder_getMode_Alpha() {
    const char* data = "HTTPS://WWW.QRCODE.COM/";
    QrMode result = QrEncoder.getMode(data, strlen(data));
    ASSERT_EQ_UI8(QrMode::AlphaNumeric, result);
}

void encoder_getMode_Numeric() {
    const char* data = "12348761234";
    QrMode result = QrEncoder.getMode(data, strlen(data));
    ASSERT_EQ_UI8(QrMode::Numeric, result);
}

void encoder_getVersion_Numeric() {
    uint8_t a = QrEncoder.getMinVersion(123, QrMode::Numeric, QrEcc::M);
    ASSERT_EQ_UI8(3, a);
    uint8_t b = QrEncoder.getMinVersion(1223, QrMode::Numeric, QrEcc::Q);
    ASSERT_EQ_UI8(20, b);
}

void encoder_getVersion_Alpha() {
    uint8_t a = QrEncoder.getMinVersion(1223, QrMode::AlphaNumeric, QrEcc::Q);
    ASSERT_EQ_UI8(27, a);
    uint8_t b = QrEncoder.getMinVersion(657, QrMode::AlphaNumeric, QrEcc::H);
    ASSERT_EQ_UI8(22, b);
}

void encoder_getVersion_Byte() {
    uint8_t a = QrEncoder.getMinVersion(222, QrMode::Byte, QrEcc::L);
    ASSERT_EQ_UI8(8, a);
    uint8_t b = QrEncoder.getMinVersion(2222, QrMode::Byte, QrEcc::M);
    ASSERT_EQ_UI8(39, b);
}
