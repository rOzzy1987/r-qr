#include "../common.h"

// as per specification
uint16_t format_testCases[][3]{
    { 1, 0, 0x5412},
    { 1, 1, 0x5125},
    { 1, 2, 0x5E7C},
    { 1, 3, 0x5B4B},
    { 1, 4, 0x45F9},
    { 1, 5, 0x40CE},
    { 1, 6, 0x4F97},
    { 1, 7, 0x4AA0},
    { 0, 0, 0x77C4},
    { 0, 1, 0x72F3},
    { 0, 2, 0x7DAA},
    { 0, 3, 0x789D},
    { 0, 4, 0x662F},
    { 0, 5, 0x6318},
    { 0, 6, 0x6C41},
    { 0, 7, 0x6976},
    { 3, 0, 0x1689},
    { 3, 1, 0x13BE},
    { 3, 2, 0x1CE7},
    { 3, 3, 0x19D0},
    { 3, 4, 0x0762},
    { 3, 5, 0x0255},
    { 3, 6, 0x0D0C},
    { 3, 7, 0x083B},
    { 2, 0, 0x355F},
    { 2, 1, 0x3068},
    { 2, 2, 0x3F31},
    { 2, 3, 0x3A06},
    { 2, 4, 0x24B4},
    { 2, 5, 0x2183},
    { 2, 6, 0x2EDA},
    { 2, 7, 0x2BED},
};
uint32_t version_testCases[][2] = {
    {  6, 0x07C94},
    {  7, 0x085BC},
    {  8, 0x09A99},
    {  9, 0x0A4D3},
    { 10, 0x0BBF6},
    { 11, 0x0C762},
    { 12, 0x0D847},
    { 13, 0x0E60D},
    { 14, 0x0F928},
    { 15, 0x10B78},
    { 16, 0x1145D},
    { 17, 0x12A17},
    { 18, 0x13532},
    { 19, 0x149A6},
    { 20, 0x15683},
    { 21, 0x168C9},
    { 22, 0x177EC},
    { 23, 0x18EC4},
    { 24, 0x191E1},
    { 25, 0x1AFAB},
    { 26, 0x1B08E},
    { 27, 0x1CC1A},
    { 28, 0x1D33F},
    { 29, 0x1ED75},
    { 30, 0x1F250},
    { 31, 0x209D5},
    { 32, 0x216F0},
    { 33, 0x228BA},
    { 34, 0x2379F},
    { 35, 0x24B0B},
    { 36, 0x2542E},
    { 37, 0x26A64},
    { 38, 0x27541},
    { 39, 0x28C69},
};

void qrgen_setFormatPoly(){
    for(uint8_t i = 0; i < 32; i++){
        QrCode *c = new QrCode(0, format_testCases[i][0]);
        c->mask = format_testCases[i][1];
        
        QrGenerator.setFormatPoly(c);
        // printf("\n---\nec:%d mask:%d", c->ecLevel, c->mask);
        // print_hex(format_testCases[i][2]);
        // print_hex(c->formatPoly);
        // printf("\n");

        ASSERT_EQ_UI16(format_testCases[i][2], c->formatPoly);
    }
}
void qrgen_setVersionPoly(){
    for(uint8_t i = 0; i < 34; i++){
        QrCode *c = new QrCode(version_testCases[i][0], 0);
        
        QrGenerator.setVersionPoly(c);
        // printf("\n---\nv%d", c->version+1);
        // print_hex(version_testCases[i][1]);
        // print_hex(c->versionPoly);
        // printf("\n");

        ASSERT_EQ_UI32(version_testCases[i][1], c->versionPoly);
    }
}