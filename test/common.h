#ifndef __TEST_COMMON
#define __TEST_COMMON

#include <unity.h>
#include <stdint.h>
#include <stdio.h>

#define QR_DEBUG
#define QR_TESTING
#include "../include/qr.h"

#define ASSERT_EQ_UI8(exp, act) UNITY_TEST_ASSERT_EQUAL_UINT8(exp, act, __LINE__, "")
#define ASSERT_EQ_UI16(exp, act) UNITY_TEST_ASSERT_EQUAL_UINT16(exp, act, __LINE__, "")
#define ASSERT_EQ_UI32(exp, act) UNITY_TEST_ASSERT_EQUAL_UINT32(exp, act, __LINE__, "")
#define ASSERT_NEQ_UI16(exp, act) UNITY_TEST_ASSERT_NOT_EQUAL_UINT16(exp, act, __LINE__, "")
#define ASSERT_EQ_UI8A(exp, act, l) UNITY_TEST_ASSERT_EQUAL_UINT8_ARRAY(exp, act, l, __LINE__, "")

void setUp(void) {
}

void tearDown(void) {
}

#endif