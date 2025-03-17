#include "merge-common.h"

static QrDataSegment adjacent_case1_input[] = {
    QrDataSegment(0, 8, 1), 
    QrDataSegment(8,12,0), 
    QrDataSegment(12,16,1),
    QrDataSegment (16,18,1), 
    QrDataSegment(18,24,2)
};
static QrDataSegment adjacent_case1_expected [] = {
    QrDataSegment(0, 8, 1), 
    QrDataSegment(8,12,0), 
    QrDataSegment(12,18,1), 
    QrDataSegment(18,24,2)
};

static QrDataSegment adjacent_case2_input[] = {
    QrDataSegment(0, 8, 1), 
    QrDataSegment(8,12,1), 
    QrDataSegment(12,16,1),
    QrDataSegment(16,18,1), 
    QrDataSegment(18,24,2)
};
static QrDataSegment adjacent_case2_expected [] = {
    QrDataSegment(0, 18,1), 
    QrDataSegment(18,24,2)
};

static QrDataSegment adjacent_case3_input[] = {
    QrDataSegment(0, 8, 0), 
    QrDataSegment(8,12,1), 
    QrDataSegment(12,16,1),
    QrDataSegment (16,18,2), 
    QrDataSegment(18,24,2)
};
static QrDataSegment adjacent_case3_expected [] = {
    QrDataSegment(0, 8, 0), 
    QrDataSegment(8,16,1),
    QrDataSegment (16,24,2)
};

static QrDataSegment adjacent_case4_input[] = {
    QrDataSegment(0, 8, 0), 
    QrDataSegment(8,12,1), 
    QrDataSegment(12,16,0),
    QrDataSegment (16,18,2), 
    QrDataSegment(18,24,1)
};

static QrDataSegment adjacent_case4_expected[] = {
    QrDataSegment(0, 8, 0), 
    QrDataSegment(8,12,1), 
    QrDataSegment(12,16,0),
    QrDataSegment (16,18,2), 
    QrDataSegment(18,24,1)
};

static QrDataSegment adjacent_case5_input[] = {
    QrDataSegment(0, 8, 0)
};

static QrDataSegment adjacent_case5_expected[] = {
    QrDataSegment(0, 8, 0)
};

static MergeTestCase adjacent_testCases[] = {
    {adjacent_case1_input, adjacent_case1_expected, 5, 4},
    {adjacent_case2_input, adjacent_case2_expected, 5, 2},
    {adjacent_case3_input, adjacent_case3_expected, 5, 3},
    {adjacent_case4_input, adjacent_case4_expected, 5, 5},
    {adjacent_case5_input, adjacent_case5_expected, 1, 1},
};


static void adjacent_runTestCase(uint8_t i) {
    
    QrDataSegment *s = adjacent_testCases[i].inputs;
    uint8_t sl = adjacent_testCases[i].inputLength,
        v = 20;

    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    QrEncoder.mergeAdjacentSegments(s, sl);
    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    compareSegments(adjacent_testCases[i].expected, adjacent_testCases[i].expectedLength, s, sl);
    // printf("\n");
}

void encoder_mergeAdjacentSegments_singleMerge() {
    adjacent_runTestCase(0);
}
void encoder_mergeAdjacentSegments_mergeConsecutive() {
    adjacent_runTestCase(1);
}
void encoder_mergeAdjacentSegments_separateMerges() {
    adjacent_runTestCase(2);
}
void encoder_mergeAdjacentSegments_nothingToMerge() {
    adjacent_runTestCase(3);
}

void encoder_mergeAdjacentSegments_singleElement() {
    adjacent_runTestCase(4);
}

