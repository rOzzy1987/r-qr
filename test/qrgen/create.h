#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"

//     actual | expected
// 0        41 | 41
// 1        76 | 76
// 2        87 | 87
// 3        47 | 47
// 4        47 | 47
// 5        07 | 07
// 6        33 | 33
// 7        A2 | A2
// 8        F2 | F2
// 9        F7 | F7
// 10       77 | 77
// 11       77 | 77
// 12       72 | 72
// 13       E7 | E7
// 14       17 | 17
// 15       26 | 26
// 16       36 | 36
// 17       F6 | F6
// 18       46 | 46
// 19       52 | 52
// 20       E6 | E6
// 21       36 | 36
// 22       F6 | F6
// 23       D2 | D2
// 24       F0 | F0
// 25       EC | EC
// 26       11 | 11
// 27       EC | EC
// 28       34 | 34
// 29       3D | 3D
// 30       F2 | F2
// 31       BB | BB
// 32       1D | 1D
// 33       07 | 07
// 34       D8 | D8
// 35       F9 | F9
// 36       67 | 67
// 37       57 | 57
// 38       5F | 5F
// 39       45 | 45
// 40       BC | BC
// 41       86 | 86
// 42       39 | 39
// 43       14 | 14


void qrgen_create() {

    const uint8_t *exp = new uint8_t[44]{65, 118, 135, 71, 71, 7, 51, 162, 242, 247, 119, 119, 114, 231, 23, 38, 54, 246, 70, 82, 230, 54, 246, 210, 240, 236, 17, 236, 
        /* EDC beginning here */ 52, 61, 242, 187, 29, 7, 216, 249, 103, 87, 95, 69, 188, 134, 57, 20};
    QrCode *result = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::L);

    //      123456789012345
    // exp: 100111110010111
    // act: 100110011010010

    // act: 011000110000000
    // exp: 011001000111101
    print_arr_f("%d", result->formatPoly, 15);

    printf("\n");
    print_qr(result);
    printf("\n");
    ASSERT_EQ_UI8A(exp, result->raw, result->rawSize);
    
    delete result;
}