#include <unity.h>
#include <stdio.h>
#include <string.h>

#include "optimize.h"
#include "merge-adjacent.h"
#include "merge-by-size.h"
#include "merge-least-painful.h"

int main( int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(encoder_mergeAdjacentSegments_singleMerge);
    RUN_TEST(encoder_mergeAdjacentSegments_mergeConsecutive);
    RUN_TEST(encoder_mergeAdjacentSegments_separateMerges);
    RUN_TEST(encoder_mergeAdjacentSegments_nothingToMerge);
    RUN_TEST(encoder_mergeAdjacentSegments_singleElement);

    RUN_TEST(encoder_mergeSegmentsBySize_numericsNotWorthKeeping);
    RUN_TEST(encoder_mergeSegmentsBySize_adjacentSegmentsWithSameMode);
    RUN_TEST(encoder_mergeSegmentsBySize_nothingToMerge);
    RUN_TEST(encoder_mergeSegmentsBySize_singleElement);

    RUN_TEST(encoder_mergeLeastPainfulSegment_alphaPickedOverNumeric);
    RUN_TEST(encoder_mergeLeastPainfulSegment_sameLengthOverall);
    RUN_TEST(encoder_mergeLeastPainfulSegment_singleElement);

    RUN_TEST(encoder_optimizeSegments_simpleUtf8);
    RUN_TEST(encoder_optimizeSegments_3byte3modes);
    RUN_TEST(encoder_optimizeSegments_easilyOptimized);
    RUN_TEST(encoder_optimizeSegments_longerButEasy);
    RUN_TEST(encoder_optimizeSegments_aBitMoreRandom);
    RUN_TEST(encoder_optimizeSegments_base64);
    RUN_TEST(encoder_optimizeSegments_moreThan1Kb);

    UNITY_END();
}