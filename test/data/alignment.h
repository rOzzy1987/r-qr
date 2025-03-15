#include "../common.h"

void qrData_alignmentFactors() {
    for(uint8_t i = 0; i < 40; i++){
        uint8_t n;
        uint8_t *r = qr_alignment_factors(i, n);
        // print_arr(r, n);
        // printf(" << v%d\n", i+1);
        UNITY_TEST_ASSERT_GREATER_OR_EQUAL_UINT8(1, n, __LINE__, "");
    }
}