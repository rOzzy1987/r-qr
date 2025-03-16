#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "init.h"
#include "mul_div.h"
#include "genpoly.h"
#include "polymul.h"
#include "polyrest.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(gf256_log);
    RUN_TEST(gf256_exp);
    RUN_TEST(gf256_mul);
    RUN_TEST(gf256_div);
    RUN_TEST(gf256_polymul);
    RUN_TEST(gf256_polyrest);
    RUN_TEST(gf256_getGeneratorPoly);

    UNITY_END();
}