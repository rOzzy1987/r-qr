#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"

struct CreateTestCase {
    const char* data;
    const uint8_t version;
    const QrMode mode;
    const QrEcc ecLevel;
    const uint16_t expectedFormatPoly;
    const uint8_t expectedMask;
};

CreateTestCase create_testCases[] = {
    {"https://www.qrcode.com/", 1, QrMode::Byte, QrEcc::M, 0b101111001111100, 2},
    {"https://www.qrcode.com/", 6, QrMode::Byte, QrEcc::M, 0b101010000010010, 0},
    {"https://www.qrcode.com/", 7, QrMode::Unspecified, QrEcc::H, 0b000011101100010, 4},
};

static void create_runTestCase(uint8_t n){
    CreateTestCase tc = create_testCases[n];

    QrCode *r = QrGenerator.create(tc.data, strlen(tc.data), tc.mode, tc.ecLevel, tc.version);
    
    // print_qr(r);
    // print_qr_details(r);
    // print_bin(r->formatPoly);
    // printf("\n");
    
    ASSERT_EQ_UI8(tc.expectedMask, r->mask);
    ASSERT_EQ_UI16(tc.expectedFormatPoly, r->formatPoly);
    delete r;
}

void qrgen_create_onlineExample() {
    create_runTestCase(0);
}

void qrgen_create_forcedMultiBlock() {
    create_runTestCase(1);
}

void qrgen_create_forcedMultiGroup() {
    create_runTestCase(2);
}