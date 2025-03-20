
#include "qr_rank.h"

uint16_t CQrRanker::rank(QrCode *code){
    return 
        rank1(code)
        + rank2(code)
        + rank3(code);
}

uint16_t CQrRanker::rank1(QrCode *code){
    uint16_t score = 0;
    uint8_t x, y, s = code->size, li, lv, sl, p;
    for (y = 0; y < s; y++)
        for (x = 1, li = 0, lv = getPixel(code, 0, y); x < s; x++){
            p = getPixel(code, x, y);
            if (p != lv){
                sl = x - li;
                score  += sl >= 5 ? sl - 2 : 0;
                lv = p;
                li = x;
            }
            if (x == s - 1){
                sl = s - li;
                score  += sl >= 5 ? sl - 2 : 0;
            }
        }
    for (x = 0; x < s; x++)
        for (y = 1, li = 0, lv = getPixel(code, x, 0); y < s; y++){
            p = getPixel(code, x, y);
            if (p != lv){
                sl = y - li;
                score  += sl >= 5 ? sl - 2 : 0;
                lv = p;
                li = y;
            }
            if (y == s - 1){
                sl = s - li;
                score  += sl >= 5 ? sl - 2 : 0;
            }
        }
    return score;
}

uint16_t CQrRanker::rank2(QrCode *code){
    uint16_t score = 0;
    uint8_t x, y, s = code->size, p[4];
    for (y = 0; y < s - 1; y++){
        p[0] = getPixel(code, 0, y );
        p[2] = getPixel(code, 0, y + 1);
        for (x = 0; x < s - 1; x++){
            p[1] = getPixel(code, x + 1, y);
            p[3] = getPixel(code, x + 1, y + 1);
            if (p[0] == p[1] && p[1] == p[2] && p[2] == p[3])
                score += 3;
            p[0] = p[1];
            p[2] = p[3];
        }
    }
    return score;
}

uint16_t CQrRanker::rank3(QrCode *code){
    uint16_t score = 0;
    uint8_t x, y, s = code->size, p[11], i;
    const uint8_t 
        pattern[] = {0,0,0,0,1,0,1,1,1,0,1};
    for (y = 0; y < s; y++){
        for (x = 0; x < 10; x++) {
            p[x] = getPixel(code, x, y);
        }
        for (x = 10; x < s; x++){
            p[10] = getPixel(code, x, y);
            for(i = 0; i < 11; i++) {
                if (p[i] != pattern[i]) break;
            }
            if (i == 11){
                score += 40;
            }

            for(i = 0; i < 11; i++) {
                if (p[i] != pattern[10-i]) break;
            }
            if (i == 11){;
                score += 40;
            }
            
            for(i = 0; i < 10; i++) {
                p[i] = p[i+1];
            }
        }
    }
    for (x = 0; x < s; x++){
        for (y = 0; y < 10; y++) {
            p[y] = getPixel(code, x, y);
        }
        for (y = 10; y < s; y++){
            p[10] = getPixel(code, x, y);
            for(i = 0; i < 11; i++) {
                if (p[i] != pattern[i]) break;
            }
            if (i == 11){
                score += 40;
            }

            for(i = 0; i < 11; i++) {
                if (p[i] != pattern[10-i]) break;
            }
            if (i == 11){
                score += 40;
            }
            
            for(i = 0; i < 10; i++) {
                p[i] = p[i+1];
            }
        }
    }
    return score;
}

uint16_t CQrRanker::rank4(QrCode *code){
    uint8_t x, y, s = code->size;
    uint16_t score = 0, ss = s*s;
    for (y = 0; y < s; y++){
        for (x = 0; x < s; x++) {
            if (getPixel(code,x,y)) score++;
        }
    }

    score = (score * 100) / ss; 
    score = score >= 50 || score % 5 == 0
        ? (score / 5) *5
        : ((score + 5) / 5) * 5; 
    score = score > 50 ? score - 50 : 50 - score;
    score <<= 1;

    return score;
}

uint8_t CQrRanker::getPixel(QrCode *code, uint8_t x, uint8_t y){
    return (code->bitmap[(y * code->bitmapStride) + (x >> 3)] & (1 << (7 - (x & 7)))) > 0;
}

CQrRanker QrRanker;