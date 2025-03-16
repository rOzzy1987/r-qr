#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "create.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(qrgen_create_onlineExample);
    RUN_TEST(qrgen_create_forcedMultiBlock);
    RUN_TEST(qrgen_create_forcedMultiGroup);

    UNITY_END();
}