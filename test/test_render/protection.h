#include "../common.h"

static uint8_t protection_testCases [][4] = {
    {6, 0, 8, 0}, // nothing there
    {6, 1, 7, 1}, // quiet zone
    {6, 22, 6, 1}, // timing pattern
    {6, 8, 4, 0}, // nothing there
};

static void protection_runTestCase(uint8_t n){
    uint8_t *tc = protection_testCases[n];

    QrCode *c = new QrCode(tc[0],0);
    uint8_t al, *af = qr_alignment_factors(tc[0], al);
    ASSERT_EQ_UI8(tc[3], QrRenderer.isProtected(c, tc[1], tc[2], af, al) + QrRenderer.isVersionInfo(c, tc[1], tc[2]));
}

void renderer_protection(){
    for(uint8_t i = 0; i < 4; i++)
        protection_runTestCase(i);
}