#include <unity.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(qrData_alignmentFactors);

    UNITY_END();
}