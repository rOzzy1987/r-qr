#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"

void qrgen_create() {

    const uint8_t *exp = new uint8_t[44]{65, 118, 135, 71, 71, 7, 51, 162, 242, 247, 119, 119, 114, 231, 23, 38, 54, 246, 70, 82, 230, 54, 246, 210, 240, 236, 17, 236, 
        /* EDC beginning here */ 52, 61, 242, 187, 29, 7, 216, 249, 103, 87, 95, 69, 188, 134, 57, 20};
    QrCode *result = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);

    print_qr(result);
    print_bin(result->formatPoly);
    printf("\n");

    ASSERT_EQ_UI8(2, result->mask);
    ASSERT_EQ_UI8A(exp, result->raw, result->rawSize);
    ASSERT_EQ_UI16(0b101111001111100, result->formatPoly);
    delete result;
}