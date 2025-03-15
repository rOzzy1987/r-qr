#include "../common.h"

#define min(a,b) (a>b) ? (b) : (a)

static void test_rest(const uint8_t *a, const uint16_t al, const uint8_t *b, const uint16_t bl, const uint8_t *ref) {
    uint8_t r[bl-1];
    uint16_t rl;
    Gf256.polyRest(a,al,b,bl,r,rl);

    ASSERT_EQ_UI16(min(bl-1, al), rl);
    if (rl > 0) ASSERT_EQ_UI8A(ref, r, rl);
}

void gf256_polyrest() {
    struct TestCase {  
        uint8_t dividend[8];  
        int dividend_len;  
        uint8_t divisor[8];  
        int divisor_len;  
        uint8_t expected_remainder[8];  
    };  

    TestCase testCases[] = { 
        {{1, 2, 3}, 3, {1, 1}, 2, {0}},
        {{5, 4, 3, 2, 1}, 5, {2, 1}, 2, {216}},
        {{1, 0, 1, 0, 1}, 5, {1, 2, 3}, 3, {10, 11}},
        {{10, 20, 30, 40}, 4, {5, 5}, 2, {41}},
        {{1, 2, 3, 4, 5, 6, 7, 8}, 8, {1, 0, 1}, 3, {0, 9}},
        {{1, 2}, 2, {1, 2, 3, 4, 5, 6, 7, 8}, 8, {1, 2}},
        {{5, 4, 3, 2}, 4, {3, 2, 1}, 3, {247, 247}},
        {{20, 15, 10, 5, 1}, 5, {7, 3}, 2, {4}},
        {{1, 1, 1, 1, 1, 1}, 6, {1, 1, 1}, 3, {1, 0}},
        {{255, 200, 150, 100, 50}, 5, {20, 10}, 2, {1}},
        {{1, 2, 3, 4, 5, 6}, 6, {1}, 1, {}},
        {{1, 2, 3, 4, 5, 6}, 6, {0, 1}, 2, {7}},
        {{1, 2, 3, 4, 5, 6, 7}, 7, {1, 0, 0, 1}, 4, {7, 5, 2}},
        {{5, 0, 0, 0, 0, 0, 1}, 7, {1, 0, 0, 0, 1}, 5, {0, 5, 1, 0}},
        {{1, 2, 3, 4}, 4, {4, 3, 2, 1}, 4, {203, 141, 67}}
    };    

    //TODO: find test data
    for(uint8_t i = 0; i < 15; i++){
        TestCase t  = testCases[i];
        test_rest(t.dividend, t.dividend_len, t.divisor, t.divisor_len, t.expected_remainder);
    }
}
