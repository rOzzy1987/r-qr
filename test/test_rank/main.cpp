#include <unity.h>
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "rank.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(ranker_rank1);
    RUN_TEST(ranker_rank2);
    RUN_TEST(ranker_rank3);
    RUN_TEST(ranker_rank4);
    
    UNITY_END();
}