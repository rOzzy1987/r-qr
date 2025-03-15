#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "./qrgen/simpletests.h"
#include "./qrgen/bitman.h"
#include "./qrgen/create.h"

#include "./gf256/init.h"
#include "./gf256/mul_div.h"
#include "./gf256/genpoly.h"
#include "./gf256/polymul.h"
#include "./gf256/polyrest.h"

#include "./render/mask.h"

#include "./data/alignment.h"

#include "./rank/rank.h"

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
    
    RUN_TEST(gf256_log);
    RUN_TEST(gf256_exp);
    RUN_TEST(gf256_mul);
    RUN_TEST(gf256_div);
    RUN_TEST(gf256_polymul);
    RUN_TEST(gf256_polyrest);
    RUN_TEST(gf256_getGeneratorPoly);

    RUN_TEST(qrData_alignmentFactors);

    RUN_TEST(qrgen_create);

    RUN_TEST(renderer_mask);

    RUN_TEST(ranker_rank1);
    RUN_TEST(ranker_rank2);
    RUN_TEST(ranker_rank3);
    RUN_TEST(ranker_rank4);

    UNITY_END();
}