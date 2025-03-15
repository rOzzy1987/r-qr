#ifndef __TEST_COMMON
#define __TEST_COMMON

#include <unity.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/qr.h"

#define ASSERT_EQ_UI8(exp, act) UNITY_TEST_ASSERT_EQUAL_UINT8(exp, act, __LINE__, "")
#define ASSERT_EQ_UI16(exp, act) UNITY_TEST_ASSERT_EQUAL_UINT16(exp, act, __LINE__, "")
#define ASSERT_EQ_UI8A(exp, act, l) UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(exp, act, l, __LINE__, "")

CQrGen *subject; 

void setUp(void) {
    // set stuff up here
    subject = new CQrGen();
}

void tearDown(void) {
    // clean stuff up here
    delete(subject);
}

#endif