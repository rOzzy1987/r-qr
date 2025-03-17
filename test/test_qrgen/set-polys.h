#include "../common.h"

uint32_t format_testCases[][4]{
    { 1, 0, 0b111011111000100, 0b000010000101101111},
    {24, 2, 0b011010101011111, 0b011001000111100001},
};

void qrgen_setPolys(){
    for(uint8_t i = 0; i < 2; i++){
        QrCode *c = new QrCode(format_testCases[i][0], format_testCases[i][1]);
        printf("\n---");
        QrGenerator.setFormatPoly(c);
        print_bin(c->formatPoly);
        QrGenerator.setVersionPoly(c);
        print_bin(c->versionPoly);
        printf("\n");

        ASSERT_EQ_UI16(format_testCases[i][2], c->formatPoly);
        ASSERT_EQ_UI16(format_testCases[i][3], c->versionPoly);
    }
}