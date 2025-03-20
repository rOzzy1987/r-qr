#include "qr_render.h"
#include "qr_data.h"

void CQrRenderer::render(QrCode *code) {
    renderStatics(code);
    renderData(code);
    applyMask(code, code->mask);
}

void CQrRenderer::renderVersion(QrCode *code) {
    if (code->version < 6) return;
    for(uint8_t i = 0; i < 17; i++){
        uint8_t 
            x = (i % 3) + (code->size - 11),
            y = i / 3,
            v = (uint8_t)((code->versionPoly >> i) & 1);
        setPixel(code, x, y, v);
        setPixel(code, y, x, v);
    }
}

void CQrRenderer::renderFormat(QrCode *code){
    for(uint8_t i = 0; i < 15; i++){
        uint8_t 
            s = code->size,
            x = i < 7 
                ? i == 6 
                    ? 7 
                    : i 
                : i + code->size - 15,
            y = i < 7 
                ? s - 1 - i 
                : i < 9 
                    ? 15 - i
                    : 14 - i,
            v = (uint8_t)(code->formatPoly >> (14 - i)) & 1;
        setPixel(code, x, 8, v);
        setPixel(code, 8, y, v);
    }
}

void CQrRenderer::renderData(QrCode *code){
    uint16_t B = 0;
    uint8_t 
        b = 0,
        s = code->size,
        x = s - 1,
        y = x,
        al = 0,
        *af = qr_alignment_factors(code->version, al);

    while(B < code->rawSize && x < s && y < s) {
        uint8_t p = isProtected(code, x,y, af,al) || isVersionInfo(code, x, y);
        
        if (p == 0){
            setPixel(code, x,y,(code->raw[B] & (1 << (7 - b))));

            b++;
            if (b == 8) {
                b = 0;
                B++;
            }
        }


        uint8_t 
            isEnd = x <= 6,
            isDn = isEnd
                ? (((x >> 1) & 1) == 0) 
                : (((s-x-1)>>1) & 1); 
        if(isDn) {
            // Downward snake
            if (y == s-1) {
                x--;
            } else {
                if((x & 1) == isEnd) {
                    x--;
                } else {
                    x++;
                    y++;
                }
            }
        } else {
            // Upward snake
            if (y > 0) {
                if ((x & 1) == isEnd) {
                    x--;
                } else {
                    x++; 
                    y--;
                }
            } else {
                x--;
                if (x == 6) x--; 
            }
        }
        
    }

    delete[] af;
}
void CQrRenderer::renderStatics(QrCode *code){
    renderQuietZones(code);
    renderTiming(code);
    renderLocators(code);
    renderAlignments(code);
}

void CQrRenderer::applyMask(QrCode *code, uint8_t m){
    uint8_t al,
        *af = qr_alignment_factors(code->version, al);

    for(uint8_t y = 0; y< code->size; y++) {
        for (uint8_t x= 0; x< code->size; x++) {
            if (isProtected(code, x,y,af,al)) continue;
            if (isVersionInfo(code, x, y)) continue;

            uint16_t i = (y * code->bitmapStride) + (x >> 3),
                z = mask(m,x,y);
            code->bitmap[i] ^= z << (uint8_t)(7-(x & 7));
        }
    }
    
    delete[] af;
}

inline uint8_t CQrRenderer::mask(uint8_t mask, uint8_t x, uint8_t y){
    switch(mask) {
        case 0: return mask0(x,y);
        case 1: return mask1(x,y);
        case 2: return mask2(x,y);
        case 3: return mask3(x,y);
        case 4: return mask4(x,y);
        case 5: return mask5(x,y);
        case 6: return mask6(x,y);
        case 7: return mask7(x,y);
        default: return 0;
    }
}


uint8_t CQrRenderer::isProtected(const QrCode *code, uint8_t x, uint8_t y, const uint8_t *af, uint8_t al){
    uint8_t p = 0, s = code->size;

    if (x <= 8 && y <= 8) p = 1; // Top left locator
    else if (x <= 8 && y >= s-8) p=1; // Bottom left locator
    else if (x >= s-8 && y <= 8) p=1; // Top right locator
    else if (x == 6 || y == 6) p=1; // timing pattern
    else { // Alignment patterns
        for(uint8_t a0 = 0; a0 < al; a0++){
            if ( x <= af[a0] + 2 && x >= af[a0] - 2 ){
                for (uint8_t a1 = 0; a1 < al; a1++) {
                    if (a0 == 0 && a1 == 0) continue;
                    if (a0 == al-1 && a1 == 0) continue;
                    if (a0 == 0 && a1 == al-1) continue;
                    if (y <= af[a1] + 2 && y >= af[a1] - 2){
                        p = 1;
                        a1 = 0xFE;
                        a0 = 0xFE;
                    }
                }
            }
        }
    }
    return p;
}

uint8_t CQrRenderer::isVersionInfo(const QrCode *code, uint8_t x, uint8_t y){
    return
        code->version < 6 ? 0 : 
        (x < 6 && y >= code->size - 11) // we should check for 3 rows only 
        ||
        (y < 6 && x >= code->size - 11) // but those pixels are covered by isProtected() anyway
        ;
}

void CQrRenderer::renderQuietZones(QrCode *code){
    uint8_t 
        s = code->size - 1,
        z = s - 7;
    renderRect(code, 0, 7, 7, 7, 0);
    renderRect(code, 7, 0, 7, 7, 0);

    renderRect(code, 0, z, 7, z, 0);
    renderRect(code, 7, z, 7, s, 0);

    renderRect(code, z, 0, z, 7, 0);
    renderRect(code, z, 7, s, 7, 0);

    setPixel(code, 8, z, 1);
}
void CQrRenderer::renderTiming(QrCode *code) {
    for(uint8_t i = 8; i < code->size - 8; i++) {
        setPixel(code, 6, i, (i & 1) == 0);
        setPixel(code, i, 6, (i & 1) == 0);
    }
}

void CQrRenderer::renderLocators(QrCode *code) {
    renderLocator(code, 3, 3);
    renderLocator(code, code->size - 4, 3);
    renderLocator(code, 3, code->size - 4);
}

void CQrRenderer::renderAlignments(QrCode *code){
    uint8_t al = 0,
        *af = qr_alignment_factors(code->version, al);
    for (uint8_t i = 0; i < al; i++) {
        for (uint8_t j = 0; j < al; j++) {
            if (i == 0 && j == 0) continue;
            if (i == 0 && j == al - 1) continue;
            if (i == al -1 && j == 0) continue;

            renderAlignment(code, af[i], af[j]);
        }
    } 
    delete[] af;
}

void CQrRenderer::renderLocator(QrCode *code, uint8_t x, uint8_t y) {
    renderRect(code, x-3, x+3, y-3, y-3, 1);
    renderRect(code, x-3, x+3, y+3, y+3, 1);
    renderRect(code, x-3, x-3, y-2, y+2, 1);
    renderRect(code, x+3, x+3, y-2, y+2, 1);
    
    renderRect(code, x-2, x+2, y-2, y-2, 0);
    renderRect(code, x-2, x+2, y+2, y+2, 0);
    renderRect(code, x-2, x-2, y-1, y+1, 0);
    renderRect(code, x+2, x+2, y-1, y+1, 0);

    renderRect(code, x-1, x+1, y-1, y+1, 1);
}
void CQrRenderer::renderAlignment(QrCode *code, uint8_t x, uint8_t y) {
    renderRect(code, x-2, x+2, y-2, y-2, 1);
    renderRect(code, x-2, x+2, y+2, y+2, 1);
    renderRect(code, x-2, x-2, y-1, y+1, 1);
    renderRect(code, x+2, x+2, y-1, y+1, 1);
    
    renderRect(code, x-1, x+1, y-1, y-1, 0);
    renderRect(code, x-1, x+1, y+1, y+1, 0);
    setPixel(code, x-1, y, 0);
    setPixel(code, x+1, y, 0);
    setPixel(code, x, y, 1);
}    

void CQrRenderer::renderRect(QrCode *code, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t v){
    for(uint8_t y = y0; y <= y1; y++){
        for(uint8_t x = x0; x <= x1; x++){
            setPixel(code, x,y,v);
        }
    }
}
void CQrRenderer::setPixel(QrCode *code, uint8_t x, uint8_t y, uint8_t value) {
    uint8_t xbyte = x >> 3;
    uint8_t xbit = x & 7;
    uint16_t i = code->bitmapStride * y + xbyte;
    if (value)
        code->bitmap[i] |= 1 << (7-xbit);
    else
        code->bitmap[i] &= ~(1 << (7-xbit));
}


uint8_t CQrRenderer::mask0(uint8_t x, uint8_t y) {
    return ((x + y) & 1) == 0;
}

uint8_t CQrRenderer::mask1(uint8_t x, uint8_t y) {
    return (y & 1) == 0;
}

uint8_t CQrRenderer::mask2(uint8_t x, uint8_t y) {
    return (x % 3) == 0;
}

uint8_t CQrRenderer::mask3(uint8_t x, uint8_t y) {
    return ((x + y) % 3) == 0;
}

uint8_t CQrRenderer::mask4(uint8_t x, uint8_t y) {
    return ((x/3 + y/2) & 1) == 0;
}

uint8_t CQrRenderer::mask5(uint8_t x, uint8_t y) {
    return (((y * x) & 1) + y * x % 3) == 0;
}

uint8_t CQrRenderer::mask6(uint8_t x, uint8_t y) {
    return ((((y * x) & 1) + y * x % 3) % 2) == 0;
}

uint8_t CQrRenderer::mask7(uint8_t x, uint8_t y) {
    return ((((y + x) & 1) + y * x % 3) % 2) == 0;
}

CQrRenderer QrRenderer;