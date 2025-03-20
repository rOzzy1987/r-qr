#ifndef __QR_DEBUG
#define __QR_DEBUG
#ifdef QR_DEBUG

#include <stdio.h>
#include <stdint.h>
#include "qr_types.h"
#include "qr_segment.h"

#define BTBF "%c%c%c%c%c%c%c%c"
#define BTB(b)\
    ((uint8_t)(b) & 0x80 ? '1' : '0'),\
    ((uint8_t)(b) & 0x40 ? '1' : '0'),\
    ((uint8_t)(b) & 0x20 ? '1' : '0'),\
    ((uint8_t)(b) & 0x10 ? '1' : '0'),\
    ((uint8_t)(b) & 0x08 ? '1' : '0'),\
    ((uint8_t)(b) & 0x04 ? '1' : '0'),\
    ((uint8_t)(b) & 0x02 ? '1' : '0'),\
    ((uint8_t)(b) & 0x01 ? '1' : '0')




    void print_bin(uint8_t v){
        printf("\n" BTBF, BTB(v));
        fflush(stdout);
    }
    
    void print_bin(uint16_t v){
        printf("\n" BTBF, BTB(v >> 8));
        printf(" " BTBF, BTB(v & 0xFF));
        fflush(stdout);
    }
    void print_bin(uint32_t v){
        printf("\n" BTBF, BTB(v >> 24));
        printf(" " BTBF, BTB((v >> 16) & 0xFF));
        printf(" " BTBF, BTB((v >> 8) & 0xFF));
        printf(" " BTBF, BTB(v & 0xFF));
        fflush(stdout);
    }

    void print_hex(uint8_t v){
        printf("\n%#06x", v);
        fflush(stdout);
    }
    
    void print_hex(uint16_t v){
        printf("\n%#06x", v);
        fflush(stdout);
    }
    void print_hex(uint32_t v){
        printf("\n%#06x %04x", v>>16, v&0xFFFF);
        fflush(stdout);
    }

void print_arr_f(const char* f, const uint16_t *arr, uint16_t len, uint8_t itemsPerLine = 0){
    printf("\n(%d) [ %s", len, itemsPerLine > 0 ? "\n" : "");
    uint16_t c = 0;
    for(uint16_t i = 0; i < len; i++){
        printf(f, arr[i]);
        c++;
        if(itemsPerLine > 0 && c >= itemsPerLine){
            printf("\n");
            c = 0;
        }
    }

    printf("]");
    fflush(stdout);
}
void print_arr_f(const char* f, const uint8_t *arr, uint16_t len, uint8_t itemsPerLine = 0){
    printf("\n(%d) [ %s", len, itemsPerLine > 0 ? "\n" : "");
    uint16_t c = 0;
    for(uint16_t i = 0; i < len; i++){
        printf(f, arr[i]);
        c++;
        if(itemsPerLine > 0 && c >= itemsPerLine){
            printf("\n");
            c = 0;
        }
    }

    printf("]");
    fflush(stdout);
}

void print_arr(const uint16_t *arr, uint16_t len){
    print_arr_f("%d, ", arr, len);
} 
void print_arr(const uint8_t *arr, uint16_t len){
    print_arr_f("%d, ", arr, len);
}

void print_arr_hex(const uint8_t *arr, uint16_t len){
    print_arr_f("%#02x, ", arr, len);
}

void print_cursor(QrBufferCursor c) {
    printf("\n{%d:%d}", c.byte, c.bit);
}

void print_qr_details(QrCode *code){
    printf("\n----------------\nQR code details:");
    printf("\nversion:       %5d", code->version);
    printf("\nsize:          %5d", code->size);
    printf("\nEcc level:     %5d", code->ecLevel);
    printf("\nMask:          %5d", code->mask);
}



#ifndef ARDUINO

#define CCBOW "\033[30;47m"
#define CCWOB "\033[37;40m"
#define CCRST "\033[0m"
#define BTBMPCC(b)\
    ((uint8_t)(b) & 0x80 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x40 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x20 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x10 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x08 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x04 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x02 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)(b) & 0x01 ? CCWOB "  " : CCBOW "  ")
#define ConsoleColorBitmap(b) printf("%s%s%s%s%s%s%s%s", BTBMPCC(b))

#define APW "##"
#define APB "  "
#define APL 2
#define BTBMP(b)\
    ((uint8_t)(b) & 0x80 ? APB : APW),\
    ((uint8_t)(b) & 0x40 ? APB : APW),\
    ((uint8_t)(b) & 0x20 ? APB : APW),\
    ((uint8_t)(b) & 0x10 ? APB : APW),\
    ((uint8_t)(b) & 0x08 ? APB : APW),\
    ((uint8_t)(b) & 0x04 ? APB : APW),\
    ((uint8_t)(b) & 0x02 ? APB : APW),\
    ((uint8_t)(b) & 0x01 ? APB : APW)
#define ASCIIBitmap(b) printf("%s%s%s%s%s%s%s%s", BTBMP(b))

/**
 * Prints the QR code to the console using ASCII characters to represent the
 * QR code's bitmap. The QR code's bitmap is printed row by row, with each
 * bit represented as a pair of spaces with different background colors. The
 * function handles any size of QR code by iterating over the bitmap using
 * the provided size and stride information from the QrCode structure.
 *
 * @param code Pointer to the QrCode structure containing the QR code data
 * to be printed.
 */

 void print_qr_cc(QrCode *code) {
    printf(CCBOW  "\n\n\n\n");
    for(uint16_t y = 0; y< code->size; y++) {
        printf(CCBOW "        ");
        for (uint16_t x = 0; x < code->bitmapStride; x++)
            ConsoleColorBitmap(code->bitmap[y * code->bitmapStride + x]);
        printf("        \n");
    }
    printf("\n\n\n\n" CCRST);
    fflush(stdout);
}
void print_qr(QrCode *code) {
    for(uint16_t y = 0; y< 4; y++) {
        printf("\n");
        for (uint16_t x = 0; x < (code->bitmapStride << 3) + (APL << 2); x++){
            printf(APW);
        }
    }
    for(uint16_t y = 0; y< code->size; y++) {
        printf("\n" APW APW APW APW);
        for (uint16_t x = 0; x < code->bitmapStride; x++) {
            ASCIIBitmap(code->bitmap[y * code->bitmapStride + x]);
        }
        printf(APW APW APW APW);
    }
    for(uint16_t y = 0; y< 4; y++) {
        printf("\n");
        for (uint16_t x = 0; x < (code->bitmapStride << 3) + (APL << 2); x++){
            printf(APW);
        }
    }
    printf("\n");
    fflush(stdout);
}
#endif //ARDUINO

void print_segments(const QrDataSegment *cs, uint8_t sl, uint8_t v){
    printf("\n%d segments", sl);
    QrDataSegment *s = (QrDataSegment*)cs;
    uint16_t total = 0;
    for (uint8_t i = 0; i < sl; i++){
        QrMode m = s[i].mode;
        total += s[i].getWordCount( v);
        printf("\n(%3d - %3d): %5s (%3d)", s[i].from, s[i].to, m == 0 ? "Num" : m == 1 ? "Alpha" : "Byte", s[i].getWordCount(v));
    }
    printf("\n(total: %4d)\n", total);
}

void print_codewords(const uint8_t *buff, QrBlockStruct s){
    printf("\nShort:%d, Long:%d, ShortSize:%d, LongSize:%d\n(%d) [\n", s.shortBlocks, s.longBlocks, s.dataWordsPerShortBlock + s.ecWordsPerBlock, s.dataWordsPerShortBlock +s.ecWordsPerBlock + 1, s.totalWords());
    for(uint16_t i = 0; i < s.shortBlocks; i++){
        for (uint16_t j = 0; j < s.dataWordsPerShortBlock + s.ecWordsPerBlock; j++){
            printf("%02X ", buff[i * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + j]);
        }
        printf("\n");
    }
    for (uint16_t i = 0; i < s.longBlocks; i++){
        for (uint16_t j = 0; j < s.dataWordsPerShortBlock + s.ecWordsPerBlock + 1; j++){
            printf("%02X ", buff[s.shortBlocks * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + i * (s.dataWordsPerShortBlock + s.ecWordsPerBlock + 1) + j]);
        }
        printf("\n");
    }
    printf("]\n");
}
void print_codewords_vert(const uint8_t *buff, QrBlockStruct s){
    printf("\nShort:%d, Long:%d, ShortSize:%d, LongSize:%d\n(%d) [\n", s.shortBlocks, s.longBlocks, s.dataWordsPerShortBlock + s.ecWordsPerBlock, s.dataWordsPerShortBlock +s.ecWordsPerBlock + 1, s.totalWords());
    uint16_t c = 0;
    for(uint16_t i = 0; i < s.dataWordsPerShortBlock; i++){
        for (uint16_t j = 0; j < s.shortBlocks + s.longBlocks; j++){
            c = j < s.shortBlocks 
                ? j * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + i
                : s.shortBlocks * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + 
                  (j - s.shortBlocks) * (s.dataWordsPerShortBlock + s.ecWordsPerBlock + 1) + i; 
            printf("%02X ", buff[c]);
        }
        printf("\n");
    }
    
    for (uint16_t j = 0; j < s.shortBlocks; j++){
        printf("   ");
    }
    for (uint16_t j = 0; j < s.longBlocks; j++){
        c = s.shortBlocks * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) +
         j * (s.dataWordsPerShortBlock + s.ecWordsPerBlock + 1) +
         s.dataWordsPerShortBlock; 
        printf("%02X ", buff[c]);
    }
    printf("\n");
    for(uint16_t i = 0; i < s.ecWordsPerBlock; i++){
        for (uint16_t j = 0; j < s.shortBlocks + s.longBlocks; j++){
            c = j < s.shortBlocks 
                ? j * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + i + s.dataWordsPerShortBlock 
                : s.shortBlocks * (s.dataWordsPerShortBlock + s.ecWordsPerBlock) + 
                  (j - s.shortBlocks) * (s.dataWordsPerShortBlock + s.ecWordsPerBlock + 1) + i + 
                  s.dataWordsPerShortBlock + 1; 
            printf("%02X ", buff[c]);
        }
        printf("\n");
    }
    printf("]\n");
}

#endif //QR_DEBUG

#endif //__QR_DEBUG