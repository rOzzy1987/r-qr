#include "../common.h"

static void test_mul(const uint8_t *a, const uint16_t al, const uint8_t *b, const uint16_t bl, const uint8_t *ref) {
    uint8_t r[al+bl-1];
    uint16_t rl;
    Gf256.polyMul(a,al,b,bl,r,rl);

    ASSERT_EQ_UI16(al+bl-1, rl);
    ASSERT_EQ_UI8A(ref, r, rl);
}

void gf256_polymul() {

    struct TestCase {  
        uint8_t poly1[8];  
        int poly1_len;  
        uint8_t poly2[8];  
        int poly2_len;  
        uint8_t expected_product[15]; // Max length is sum of lengths - 1, but allocate enough  
        int expected_product_len;  
    };  

    TestCase testCases[] = {  
        // AI suggested cases
        {{1, 2}, 2, {3, 4}, 2, {3, 2, 8}},  // Basic Test
        {{5, 0, 1}, 3, {2, 1}, 2, {10, 5, 2, 1}}, // With a zero coefficient
        {{1}, 1, {1, 2, 3}, 3, {1, 2, 3}}, // Multiply by a constant
        {{1, 1}, 2, {1, 1, 1}, 3, {1, 0, 0, 1}}, // Simple case
        {{2, 3, 4}, 3, {5}, 1, {10, 15, 20}}, // Constant multiplier
        {{1, 0, 0, 1}, 4, {1, 0, 0, 1}, 4, {1, 0, 0, 0, 0, 0, 1}}, // Sparse Polynomials
        {{255}, 1, {2}, 1, {227}}, // Max Value
        {{1, 2, 3, 4, 5}, 5, {6, 7}, 2, {6, 11, 4, 17, 2, 27}}, // Larger Coefficients
        {{1, 1, 1}, 3, {1, 1, 1}, 3, {1, 0, 1, 0, 1}}, // All 1s
        {{2, 0, 3, 0}, 4, {4, 0, 5}, 3, {8, 0, 6, 0, 15, 0}}, // Zero coefficients interspersed
        {{1, 2, 3, 4}, 4, {5, 6, 7, 8}, 4, {5, 12, 4, 24, 1, 4, 32}}, // More complex
        {{2, 3}, 2, {0}, 1, {0, 0}}, // Multiply by Zero
        {{0}, 1, {1, 2}, 2, {0, 0}}, // Zero multiplier
        {{0}, 1, {0}, 1, {0}},// Zero by Zero 
        {{1, 2, 3, 4, 5, 6, 7, 8}, 8, {1}, 1, {1, 2, 3, 4, 5, 6, 7, 8}},// Identity
        
        // calculated and verified by hand
        {{1, 1}, 2, {2, 1}, 2, {2, 3, 1}},
        {{3, 45}, 2, {73, 59}, 2, {219, 154, 33}},
        {{1, 1}, 2, {1, 1}, 2, {1, 0, 1}},
        {{5}, 1, {83}, 1, {2}}
    }; 

    for(uint8_t i = 0; i < 19; i++) {
        TestCase t = testCases[i];
        test_mul(t.poly1, t.poly1_len, t.poly2, t.poly2_len, t.expected_product);
    }
}
