#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "simpletests.h"
#include "bitman.h"
#include "encode.h"
#include "mix.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(encoder_getMode_Byte);
    RUN_TEST(encoder_getMode_Alpha);
    RUN_TEST(encoder_getMode_Numeric);
    RUN_TEST(encoder_getVersion_Numeric);
    RUN_TEST(encoder_getVersion_Alpha);
    RUN_TEST(encoder_getVersion_Byte);

    RUN_TEST(encoder_addValue);
    RUN_TEST(encoder_addDataNum);
    RUN_TEST(encoder_addDataAlpha);
    RUN_TEST(encoder_addDataByte);

    RUN_TEST(encoder_encode_sameAsOnlineExample);
    RUN_TEST(encoder_encode_uppercaseUrlAsAlpha);
    RUN_TEST(encoder_encode_pureNumeric);
    RUN_TEST(encoder_encode_highEcLevel);

    RUN_TEST(encoder_mix_simpleMultiBlock);
    RUN_TEST(encoder_mix_simpleMultiGroup);
    RUN_TEST(encoder_mix_unevenMultiGroup);
    RUN_TEST(encoder_mix_singleBlock);

    UNITY_END();
}