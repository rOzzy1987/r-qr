#include "../common.h"

void gf256_getGeneratorPoly() {
    uint16_t genLen;
    uint8_t * gen;
    const uint8_t gen1[] = {1,1};
    const uint8_t gen2[] = {1,3,2};
    const uint8_t gen4[] = {1,15,54,120,64};
    const uint8_t gen8[] = {1,255,11,81,54,239,173,200,24};
    const uint8_t gen16[] = {1,59,13,104,189,68,209,30,8,163,65,41,229,98,50,36,59};

    gen = Gf256.getGeneratorPoly(1, genLen);    
    ASSERT_EQ_UI8A(gen1, gen, genLen);
    delete[] gen;

    gen = Gf256.getGeneratorPoly(2, genLen);
    ASSERT_EQ_UI8A(gen2, gen, genLen);
    delete[] gen;

    gen = Gf256.getGeneratorPoly(4, genLen);
    ASSERT_EQ_UI8A(gen4, gen, genLen);
    delete[] gen;

    gen = Gf256.getGeneratorPoly(8, genLen);
    ASSERT_EQ_UI8A(gen8, gen, genLen);
    delete[] gen;

    gen = Gf256.getGeneratorPoly(16, genLen);
    ASSERT_EQ_UI8A(gen16, gen, genLen);
    delete[] gen;
}