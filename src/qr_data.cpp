#include "qr_data.h"

uint16_t qr_available_modules(uint8_t version) {
    version++;
    if (version == 1) {
        return 208;
    }
    uint8_t alignmentCount = (version / 7) + 2;
    uint8_t size = (version << 2) + 17;
    return (size * size)
        - 192 // 3 finder patterns
        - (alignmentCount*alignmentCount - 3) * 25 // alignment patterns
        - (((version << 2) + 1) << 1) // timing patterns
        + (alignmentCount - 2) * 10 // timing patterns already covered by alignment patterns
        - 31 // version strip + black module
        - (version > 6 ? 36 : 0); // version polynomial
}

QrBlockStruct qr_block_struct(uint8_t version, QrEcc ecLevel){
    uint16_t totalWords = qr_available_modules(version) >> 3;
    QrBlockStruct s = {0,0,0,0};
    s.ecWordsPerBlock = qr_block_struct_data[version][ecLevel][0];
    uint16_t totalBlocks = qr_block_struct_data[version][ecLevel][1],
        shortBlockLength = totalWords / totalBlocks;
    s.longBlocks = totalWords - (shortBlockLength * totalBlocks);
    s.dataWordsPerShortBlock = shortBlockLength - s.ecWordsPerBlock;
    s.shortBlocks = totalBlocks - s.longBlocks;
    return s;
}

uint8_t qr_lengthBits(uint8_t v, uint8_t mode){
    
    switch(mode) {
        case 0:
            return v<10 ? 10 : v < 27 ? 12 :  14;
        case 1:
            return v<10 ? 9 : v < 27 ? 11 :  13;
        case 2:
            return v<10 ? 8 : 16;
    }
    return 0;
}


uint8_t *qr_alignment_factors(uint8_t v, uint8_t &n) {
    v++; // switch to 1-based
    if (v == 1) {
        n = 1;
        return new uint8_t[1]{6};
    }

    n = (v/7) + 2; 
    uint8_t *r = new uint8_t[n];
    r[0] = 6;
    r[n-1]= 10 + v * 4;

    if (n == 2) return r;

    uint8_t d = r[n-1] - r[0]; 
    uint8_t s = d/(n-1);
    uint8_t rem = d%(n-1);
    if (rem) s++;
    if (s&1) s++;

    uint8_t x = r[n-1];
    for(uint8_t i =  n-2; i > 0; i--){
        x-= s;
        r[i]=x;
    }
    return r;
}