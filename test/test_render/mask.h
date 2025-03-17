#include "../common.h"



static void test_mask(uint8_t m) {
    QrCode *code = 
        new QrCode(0, 0);
    QrRenderer.applyMask(code, m);
    uint32_t acc = 0;
    for (uint16_t i = 0;i < code->bitmapSize; i++){
        acc+= code->bitmap[i];
    }
    // print_qr(code);
    // printf("\n");
    ASSERT_EQ_UI8(1, acc > 0);
}

void renderer_applyMask_mask0(){
    test_mask(0);
}
void renderer_applyMask_mask1(){
    test_mask(1);
}
void renderer_applyMask_mask2(){
    test_mask(2);
}
void renderer_applyMask_mask3(){
    test_mask(3);
}
void renderer_applyMask_mask4(){
    test_mask(4);
}
void renderer_applyMask_mask5(){
    test_mask(5);
}
void renderer_applyMask_mask6(){
    test_mask(6);
}
void renderer_applyMask_mask7(){
    test_mask(7);
}