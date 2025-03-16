#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "alignment.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(qrData_alignmentFactors);

    UNITY_END();
}