#ifndef __QR_DEBUG
#define __QR_DEBUG
#ifdef QR_DEBUG

#define BTBF "%c%c%c%c %c%c%c%c"
#define BTB(b)\
    ((uint8_t)b & 0x80 ? '1' : '0'),\
    ((uint8_t)b & 0x40 ? '1' : '0'),\
    ((uint8_t)b & 0x20 ? '1' : '0'),\
    ((uint8_t)b & 0x10 ? '1' : '0'),\
    ((uint8_t)b & 0x08 ? '1' : '0'),\
    ((uint8_t)b & 0x04 ? '1' : '0'),\
    ((uint8_t)b & 0x02 ? '1' : '0'),\
    ((uint8_t)b & 0x01 ? '1' : '0')
#define ITBF BTBF "  " BTBF
#define ITB(i) BTB(i >> 8), BTB(i & 0xFF)

#define CCBOW "\033[30;47m"
#define CCWOB "\033[37;40m"
#define CCRST "\033[0m"

#define BTBMP(b)\
    ((uint8_t)b & 0x80 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x40 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x20 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x10 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x08 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x04 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x02 ? CCWOB "  " : CCBOW "  "),\
    ((uint8_t)b & 0x01 ? CCWOB "  " : CCBOW "  ")
#define BMP(b) printf("%s%s%s%s%s%s%s%s", BTBMP(b))



void print_arr_f(const char* f, const uint8_t *arr, uint16_t len){
    printf("\n(%d) [ ", len);
    
    for(uint16_t i = 0; i < len; i++){
        printf(f, arr[i]);
    }

    printf("]");
}

void print_arr(const uint8_t *arr, uint16_t len){
    print_arr_f("%d, ", arr, len);
}

void print_arr_hex(const uint8_t *arr, uint16_t len){
    print_arr_f("%#02x, ", arr, len);
}

void print_cursor(QrBufferCursor *c) {
    printf("\n{%d:%d}", c->byte, c->bit);
}

#endif //QR_DEBUG
#endif //__QR_DEBUG