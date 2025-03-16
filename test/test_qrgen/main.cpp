#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "simpletests.h"
#include "bitman.h"
#include "create.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(qrgen_getMode_Byte);
    RUN_TEST(qrgen_getMode_Alpha);
    RUN_TEST(qrgen_getMode_Numeric);
    RUN_TEST(qrgen_getVersion_Numeric);
    RUN_TEST(qrgen_getVersion_Alpha);
    RUN_TEST(qrgen_getVersion_Byte);

    RUN_TEST(qrgen_addValue);
    RUN_TEST(qrgen_addDataNum);
    RUN_TEST(qrgen_addDataAlpha);
    RUN_TEST(qrgen_addDataByte);

    UNITY_END();
}