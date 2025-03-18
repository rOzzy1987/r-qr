
#include "../common.h"

const char* changeMask_data[] = {
    "asdf",
    "123", // this will be encoded to a single 10-bit codeword!
    "Sample text long enough to become version 3 or maybe even 4",
    "Sample text long enough to become version 7. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vel risus ac felis bibendum semper.",
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean vel risus ac felis bibendum semper. Duis ornare, felis eget consectetur venenatis, ante turpis facilisis erat, quis laoreet sem sem quis quam. Pellentesque id pellentesque nisl, venenatis cursus augue. Nulla sed velit neque. Maecenas varius, nisl egestas condimentum pulvinar, leo libero molestie purus, vitae placerat lectus felis vitae mauris. Donec vitae commodo lacus, sit amet scelerisque erat. Aenean lacinia in nisi id vehicula. Donec eget accumsan."
};

void changeMask_runTestCase(uint8_t n) {
    QrCode *code = QrGenerator.create(changeMask_data[n], strlen(changeMask_data[n]));

    for(uint8_t i = 0; i < 8; i++) {
        QrGenerator.changeMask(code, i);
        if (code->version > 8) 
            continue;
        // printf("\nMask #%d", i);
        // printf("\nVersion #%d", code->version+1);
        // print_qr(code);
        // printf("\n");
        // print_arr(code->bitmap, code->bitmapSize);
    }
}

void qrgen_changeMask_smallText(){
    changeMask_runTestCase(0);
}
void qrgen_changeMask_smallNumber(){
    changeMask_runTestCase(1);
}
void qrgen_changeMask_largerText(){
    changeMask_runTestCase(2);
}
void qrgen_changeMask_v7Text(){
    changeMask_runTestCase(3);
}
void qrgen_changeMask_largeText(){
    changeMask_runTestCase(4);
}
