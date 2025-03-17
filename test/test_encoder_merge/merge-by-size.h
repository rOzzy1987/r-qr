#include "merge-common.h"


static QrDataSegment bySize_case1_input[] = {
    QrDataSegment(0, 8, 2), 
    QrDataSegment(8,12,0), 
    QrDataSegment(12,36,1),
    QrDataSegment(36,38,2), 
    QrDataSegment(38,42,0)
};
static QrDataSegment bySize_case1_expected [] = {
    QrDataSegment(0, 12,2), 
    QrDataSegment(12,36,1), 
    QrDataSegment(36,42,2)
};

static QrDataSegment bySize_case2_input[] = {
    QrDataSegment(0, 8, 1), 
    QrDataSegment(8,12,1), 
    QrDataSegment(12,16,1),
    QrDataSegment(16,18,1), 
    QrDataSegment(18,24,2)
};
static QrDataSegment bySize_case2_expected [] = {
    QrDataSegment(0, 18,1), 
    QrDataSegment(18,24,2)
};

static QrDataSegment bySize_case3_input[] = {
    QrDataSegment(0, 58, 2), 
    QrDataSegment(58,104,0), 
    QrDataSegment(104,157,1),
    QrDataSegment(157,238,2), 
    QrDataSegment(238,262,0)
};
static QrDataSegment bySize_case3_expected [] = {
    QrDataSegment(0, 58, 2), 
    QrDataSegment(58,104,0), 
    QrDataSegment(104,157,1),
    QrDataSegment(157,238,2), 
    QrDataSegment(238,262,0)
};

static QrDataSegment bySize_case4_input[] = {
    QrDataSegment(0, 58, 2)
};
static QrDataSegment bySize_case4_expected [] = {
    QrDataSegment(0, 58, 2)
};


static MergeTestCase bySize_testCases[] = {
    {bySize_case1_input, bySize_case1_expected, 5, 3},
    {bySize_case2_input, bySize_case2_expected, 5, 2},
    {bySize_case3_input, bySize_case3_expected, 5, 5},
    {bySize_case4_input, bySize_case4_expected, 1, 1},
};


inline void bySize_runTestCase(uint8_t i) {
    
    QrDataSegment *s = bySize_testCases[i].inputs;
    uint8_t sl = bySize_testCases[i].inputLength,
        v = 20;

    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    QrEncoder.mergeSegmentsBySize(s, sl, v);
    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    compareSegments(bySize_testCases[i].expected, bySize_testCases[i].expectedLength, s, sl);
    // printf("\n");
}

void encoder_mergeSegmentsBySize_numericsNotWorthKeeping() {
    bySize_runTestCase(0);
}
void encoder_mergeSegmentsBySize_adjacentSegmentsWithSameMode() {
    bySize_runTestCase(1);
}
void encoder_mergeSegmentsBySize_nothingToMerge() {
    bySize_runTestCase(2);
}
void encoder_mergeSegmentsBySize_singleElement() {
    bySize_runTestCase(3);
}

