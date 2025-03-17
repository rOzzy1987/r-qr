#include "common.h"

struct MixTestCase {
    const uint8_t *in;
    const uint8_t *exp;
    const uint16_t len;
    const QrBlockStruct str;
};

uint8_t mix_case1_inp[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
uint8_t mix_case1_out[] = {0,8,16,24,1,9,17,25,2,10,18,26,3,11,19,27,4,12,20,28,5,13,21,29,6,14,22,30,7,15,23,31};
uint8_t mix_case2_inp[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
uint8_t mix_case2_out[] = {0,8,16,23,1,9,17,24,2,10,18,25,3,11,19,26,4,12,20,27,5,13,21,28,6,14,22,29,7,15};

MixTestCase mix_testCases[]= {
    {mix_case1_inp, mix_case1_out, 32, {3, {4,5}, {0,0}}},
    {mix_case2_inp, mix_case2_out, 30, {3, {2,4}, {2,5}}},
    {mix_case1_inp, mix_case1_inp, 32, {10, {1,22}, {0,0}}}
};

static void mix_runTestCase(uint8_t n){
    MixTestCase tc = mix_testCases[n];

    uint8_t *b = new uint8_t[tc.len];
    memcpy(b, tc.in, tc.len);

    QrEncoder.mixBlocks(b, tc.len, tc.str);

    ASSERT_EQ_UI8A(tc.exp, b, tc.len);
}

void encoder_mix_simpleMultiBlock(){
    mix_runTestCase(0);
}
void encoder_mix_simpleMultiGroup(){
    mix_runTestCase(1);
}
void encoder_mix_singleBlock(){
    mix_runTestCase(2);
}

