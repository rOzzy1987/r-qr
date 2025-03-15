#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"

void qrgen_getMode_Byte() {
    const char* data = "https://www.qrcode.com/";
    QrMode result = subject->getMode(data, strlen(data));
    UNITY_TEST_ASSERT_EQUAL_INT8(QrMode::Byte, result, __LINE__, "result should be Byte");
}

void qrgen_getMode_Alpha() {
    const char* data = "HTTPS://WWW.QRCODE.COM/";
    QrMode result = subject->getMode(data, strlen(data));
    UNITY_TEST_ASSERT_EQUAL_INT8(QrMode::AlphaNumeric, result, __LINE__, "result should be Alphanumeric");
}

void qrgen_getMode_Numeric() {
    const char* data = "12348761234";
    QrMode result = subject->getMode(data, strlen(data));
    UNITY_TEST_ASSERT_EQUAL_INT8(QrMode::Numeric, result, __LINE__, "result should be Numeric");
}

void qrgen_getVersion_Numeric() {
    uint8_t a = subject->getVersion(123, QrMode::Numeric, QrEcc::M);
    UNITY_TEST_ASSERT_EQUAL_UINT8(3, a, __LINE__, "version should be v4 (0-based idx) = 3");
    uint8_t b = subject->getVersion(1223, QrMode::Numeric, QrEcc::Q);
    UNITY_TEST_ASSERT_EQUAL_UINT8(20, b, __LINE__, "version should be v21 (0-based idx) = 20");
}

void qrgen_getVersion_Alpha() {
    uint8_t a = subject->getVersion(1223, QrMode::AlphaNumeric, QrEcc::Q);
    UNITY_TEST_ASSERT_EQUAL_UINT8(27, a, __LINE__, "version should be v28 (0-based idx) = 27");
    uint8_t b = subject->getVersion(657, QrMode::AlphaNumeric, QrEcc::H);
    UNITY_TEST_ASSERT_EQUAL_UINT8(22, b, __LINE__, "version should be v23 (0-based idx) = 22");
}

void qrgen_getVersion_Byte() {
    uint8_t a = subject->getVersion(222, QrMode::Byte, QrEcc::L);
    UNITY_TEST_ASSERT_EQUAL_UINT8(8, a, __LINE__, "version should be v9 (0-based idx) = 8");
    uint8_t b = subject->getVersion(2222, QrMode::Byte, QrEcc::M);
    UNITY_TEST_ASSERT_EQUAL_UINT8(39, b, __LINE__, "version should be v40 (0-based idx) = 39");
}
