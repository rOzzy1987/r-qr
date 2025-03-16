#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "mask.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(renderer_mask);

    UNITY_END();
}