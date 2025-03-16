#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "mask.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(renderer_mask);

    UNITY_END();
}