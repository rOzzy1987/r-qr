#include "../common.h"



static void test_mask(uint8_t m) {
    QrCode *code  =  new QrCode(0, 0);
    QrRenderer.applyMask(code, m);
    uint32_t acc = 0;
    for (uint16_t i = 0;i < code->bitmapSize; i++){
        acc+= code->bitmap[i];
    }
    ASSERT_EQ_UI8(1, acc > 0);
    delete(code);
}

void renderer_mask(){
    test_mask(0);
    test_mask(1);
    test_mask(2);
    test_mask(3);
    test_mask(4);
    test_mask(5);
    test_mask(6);
    test_mask(7);
}