#include "merge-common.h"

static QrDataSegment leastPainful_case1_input[] = {
    QrDataSegment(0, 58, 2), 
    QrDataSegment(58, 68, 0), 
    QrDataSegment(68, 218, 2),
    QrDataSegment(218, 228, 1), 
    QrDataSegment(228, 324, 2)
};
static QrDataSegment leastPainful_case1_expected [] = {
    QrDataSegment(0, 58, 2), 
    QrDataSegment(58, 68, 0), 
    QrDataSegment(68, 324, 2)
};

static QrDataSegment leastPainful_case2_input[] = {
    QrDataSegment(0, 50, 1), 
    QrDataSegment(50, 100, 2), 
    QrDataSegment(100, 150, 0),
    QrDataSegment(150, 200, 1), 
    QrDataSegment(200, 250, 2)
};
static QrDataSegment leastPainful_case2_expected [] = {
    QrDataSegment(0, 50, 1), 
    QrDataSegment(50, 100, 2), 
    QrDataSegment(100, 150, 0),
    QrDataSegment(150, 250, 2)
};

static QrDataSegment leastPainful_case3_input[] = {
    QrDataSegment(0, 50, 1)
};
static QrDataSegment leastPainful_case3_expected [] = {
    QrDataSegment(0, 50, 1)
};

static MergeTestCase leastPainful_testCases[] = {
    {leastPainful_case1_input, leastPainful_case1_expected, 5, 3},
    {leastPainful_case2_input, leastPainful_case2_expected, 5, 4},
    {leastPainful_case3_input, leastPainful_case3_expected, 1, 1},
};


static void leastPainful_runTestCase(uint8_t i) {
    
    QrDataSegment *s = leastPainful_testCases[i].inputs;
    uint8_t sl = leastPainful_testCases[i].inputLength,
        v = 20;

    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    QrEncoder.mergeLeastPainfulSegment(s, sl, v);
    // printSegments(s, sl, v);
    sanityCheckSegments(s, sl);
    compareSegments(leastPainful_testCases[i].expected, leastPainful_testCases[i].expectedLength, s, sl);
    // printf("\n");
}

void encoder_mergeLeastPainfulSegment_alphaPickedOverNumeric() {
    leastPainful_runTestCase(0);
}
void encoder_mergeLeastPainfulSegment_sameLengthOverall() {
    leastPainful_runTestCase(1);
}
void encoder_mergeLeastPainfulSegment_singleElement() {
    leastPainful_runTestCase(2);
}

