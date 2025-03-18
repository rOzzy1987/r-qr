#include <unity.h>
#include <stdio.h>
#include <string.h>
#include "alignment.h"
#include "struct.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(qrData_alignmentFactors);
    RUN_TEST(qrData_structs);

    UNITY_END();
}