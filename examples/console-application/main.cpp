
#include <stdio.h>
#include <string.h>
// #define QR_DEBUG
#include "qr.h"

int main( int argc, char **argv) {
    uint8_t options;
    QrMode mode = QrMode::Unspecified;
    QrEcc ecc = QrEcc::L;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--stats") == 0){
            printf("\nStats enabled");
            options |= 0x80;
        } else if (strcmp(argv[i], "--details") == 0){
            printf("\nDetails enabled");
            options |= 0x40;
        } else if (strncmp(argv[i], "--ecc=", 6) == 0){
            printf("\n ECC level: ");
            switch(argv[i][6]) {
                case 'L':
                case 'l':
                    printf("L");
                    ecc = QrEcc::L;
                    break;
                case 'm':
                case 'M':
                    printf("M");
                    ecc = QrEcc::M;
                    break;
                case 'q':
                case 'Q':
                    printf("Q");
                    ecc = QrEcc::Q;
                    break;
                case 'h':
                case 'H':
                    printf("H");
                    ecc = QrEcc::H;
                    break;

            }
        } else if (strncmp(argv[i], "--mode=", 7) == 0){
            printf("\n Mode: ");
            switch(argv[i][7]) {
                case 'n':
                case 'N':
                    printf("Numeric");
                    mode = QrMode::Numeric;
                    break;
                case 'a':
                case 'A':
                    printf("Alphanumeric");
                    mode = QrMode::AlphaNumeric;
                    break;
                case 'b':
                case 'B':
                    printf("Byte");
                    mode = QrMode::Byte;
                    break;

            }
        } else if (strcmp(argv[i], "--stress") == 0){
            printf("\nStress test, generating 1,000,000 codes");
            for (int i = 0; i < 1000000; i++) {
                QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Unspecified, QrEcc::M );
                delete code;
            }
        } else {
            printf("\nQR:");
            QrCode *code = QrGenerator.create(argv[i], strlen(argv[i]), mode, ecc);
            print_qr(code);
            printf("\nCode: %s", argv[i]);
            if (options & 0x40) {
                printf("\n\nDetails:");
                printf("\n-----------------------------");
                printf("\n    version: %2d", code->version + 1);
                printf("\n       mask:  %d", code->mask);
                printf("\n  ECC level:  %d", code->ecLevel);
                printf("\n       size: %3d", code->size);
            }
            if (options & 0x80) {
                printf("\n\nStats:");
                printf("\n-----------------------------");
                printf("\n  memory size: %6d bytes", sizeof(code) + code->rawSize + code->bitmapSize);
                printf("\n-----------------------------");
                printf("\n  struct size: %6d bytes", sizeof(code));
                printf("\n     raw size: %6d bytes", code->rawSize);
                printf("\n  bitmap size: %6d bytes", code->bitmapSize);
            }
        }
    }

}