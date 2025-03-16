#ifndef __TEST_MERGE_COMMON
#define __TEST_MERGE_COMMON

#include "common.h"

typedef struct  {
    QrDataSegment *inputs;
    QrDataSegment *expected;
    uint8_t inputLength;
    uint8_t expectedLength;
} MergeTestCase;

void sanityCheckSegments(const QrDataSegment *s, uint8_t sl){
    for (uint8_t i = 0; i < sl; i++){
        ASSERT_NEQ_UI16(0, s[i].to - s[i].from );
    }
    for (uint8_t i = 1; i < sl; i++){
        ASSERT_EQ_UI16(s[i-1].to, s[i].from );
    }
}

void compareSegments(const QrDataSegment *exp, uint8_t expL, const QrDataSegment *act, uint8_t actL){
    ASSERT_EQ_UI8(expL, actL);
    for(uint8_t i = 0; i < expL; i++){
        ASSERT_EQ_UI16(exp[i].from, act[i].from);
        ASSERT_EQ_UI16(exp[i].to, act[i].to);
        ASSERT_EQ_UI8(exp[i].mode, act[i].mode);
    }
}

#endif // __TEST_MERGE_COMMON