#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "change-mask.h"
#include "create.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(qrgen_create_onlineExample);
    RUN_TEST(qrgen_create_forcedMultiBlock);
    RUN_TEST(qrgen_create_forcedMultiGroup);

    // RUN_TEST(qrgen_changeMask_smallText);
    // RUN_TEST(qrgen_changeMask_smallNumber);
    // RUN_TEST(qrgen_changeMask_largerText);
    // RUN_TEST(qrgen_changeMask_largeText);

    UNITY_END();
}