#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "mask.h"
#include "protection.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(renderer_applyMask_mask0);
    RUN_TEST(renderer_applyMask_mask1);
    RUN_TEST(renderer_applyMask_mask2);
    RUN_TEST(renderer_applyMask_mask3);
    RUN_TEST(renderer_applyMask_mask4);
    RUN_TEST(renderer_applyMask_mask5);
    RUN_TEST(renderer_applyMask_mask6);
    RUN_TEST(renderer_applyMask_mask7);

    RUN_TEST(renderer_protection);

    UNITY_END();
}