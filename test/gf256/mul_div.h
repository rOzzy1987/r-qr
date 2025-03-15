#include "../common.h"

void gf256_div() {
    ASSERT_EQ_UI8(91,  Gf256.div(23, 74));
    ASSERT_EQ_UI8(44,  Gf256.div(1, 38));
    ASSERT_EQ_UI8(188, Gf256.div(255, 13));
    ASSERT_EQ_UI8(139, Gf256.div(0, 45));
    ASSERT_EQ_UI8(1,   Gf256.div(24, 24));
}

void gf256_mul() {
    ASSERT_EQ_UI8(63,  Gf256.mul(23, 74));
    ASSERT_EQ_UI8(38,  Gf256.mul(1, 38));
    ASSERT_EQ_UI8(143, Gf256.mul(255, 13));
    ASSERT_EQ_UI8(0,   Gf256.mul(0, 45));
    ASSERT_EQ_UI8(93,  Gf256.mul(24, 24));
}