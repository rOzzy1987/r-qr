#include "merge-common.h"

struct OptimizeTestCase{
    const char* data;
    QrDataSegment* expected;
    uint8_t expLen;
};

static const char* strs[] = {
    "óóóó",
    "4A?",
    "a sdfQWER T Z1234524762",
    "4444423523457263452342436236AADSFHSDFHSNVCXCBIZUBVIUGVFZZZKKTHGHJHGERHYDHTH??????????????????????....",
    "13462,.371vniu34 44NIU!L%5IJhLh53_lk33ádfsd",
    "9Sz6j4kk5M35zQjS+aQOIE56+y/1fjKnnkU5Yel0Cy4FCFeRMs8tC2CtYOtFyE7ww4XhDGhOZk1iAX5j+5WcB/s606VXHLd5rmfKOly+21yCoR72gcQwCeh6NpWcshNIbIUvv5FVumaODGnUYHTSP32a6lIWJFyltYAe6tcHFRuKZ7GRyNwes5pKWRd41ayLMnrwWqnnZ6i/Jkms1hCNyRTnyzvb0m+FC9V7CiTnyuCJnob33X77o9xk0GljZGz4tTWz7kKy4uWf+S4w7PJPFvINqMhhPEVDO27PLKzttMsXT2/7low5zLI30WmDb6cFnqpetY5uEPhSe2v0VAnr",
    "9Sz6j4kk5M35zQjS+aQOIE56+y/1fjKnnkU5Yel0Cy4FCFeRMs8tC2CtYOtFyE7ww4XhDGhOZk1iAX5j+5WcB/s606VXHLd5rmfKOly+21yCoR72gcQwCeh6NpWcshNIbIUvv5FVumaODGnUYHTSP32a6lIWJFyltYAe6tcHFRuKZ7GRyNwes5pKWRd41ayLMnrwWqnnZ6i/Jkms1hCNyRTnyzvb0m+FC9V7CiTnyuCJnob33X77o9xk0GljZGz4tTWz7kKy4uWf+S4w7PJPFvINqMhhPEVDO27PLKzttMsXT2/7low5zLI30WmDb6cFnqpetY5uEPhSe2v0VAnr9Sz6j4kk5M35zQjS+aQOIE56+y/1fjKnnkU5Yel0Cy4FCFeRMs8tC2CtYOtFyE7ww4XhDGhOZk1iAX5j+5WcB/s606VXHLd5rmfKOly+21yCoR72gcQwCeh6NpWcshNIbIUvv5FVumaODGnUYHTSP32a6lIWJFyltYAe6tcHFRuKZ7GRyNwes5pKWRd41ayLMnrwWqnnZ6i/Jkms1hCNyRTnyzvb0m+FC9V7CiTnyuCJnob33X77o9xk0GljZGz4tTWz7kKy4uWf+S4w7PJPFvINqMhhPEVDO27PLKzttMsXT2/7low5zLI30WmDb6cFnqpetY5uEPhSe2v0VAnr9Sz6j4kk5M35zQjS+aQOIE56+y/1fjKnnkU5Yel0Cy4FCFeRMs8tC2CtYOtFyE7ww4XhDGhOZk1iAX5j+5WcB/s606VXHLd5rmfKOly+21yCoR72gcQwCeh6NpWcshNIbIUvv5FVumaODGnUYHTSP32a6lIWJFyltYAe6tcHFRuKZ7GRyNwes5pKWRd41ayLMnrwWqnnZ6i/Jkms1hCNyRTnyzvb0m+FC9V7CiTnyuCJnob33X77o9xk0GljZGz4tTWz7kKy4uWf+S4w7PJPFvINqMhhPEVDO27PLKzttMsXT2/7low5zLI30WmDb6cFnqpetY5uEPhSe2v0VAnr12345"
};

static QrDataSegment optimize_case1_exp[] {
    QrDataSegment(0,8,2) // UTF8!
};
static QrDataSegment optimize_case2_exp[] {
    QrDataSegment(0,3,2)
};
static QrDataSegment optimize_case3_exp[] {
    QrDataSegment(0,13,2),
    QrDataSegment(13,23,0)
};
static QrDataSegment optimize_case4_exp[] {
    QrDataSegment(0,28,0),
    QrDataSegment(28,75,1),
    QrDataSegment(75,101,2)
};
static QrDataSegment optimize_case5_exp[] {
    QrDataSegment(0,44,2)
};
static QrDataSegment optimize_case6_exp[] {
    QrDataSegment(0,340,2)
};
static QrDataSegment optimize_case7_exp[] {
    QrDataSegment(0,1025,2)
};

static const OptimizeTestCase optimize_testCases[] = {
    {strs[0], optimize_case1_exp, 1},
    {strs[1], optimize_case2_exp, 1},
    {strs[2], optimize_case3_exp, 2},
    {strs[3], optimize_case4_exp, 3},
    {strs[4], optimize_case5_exp, 1},
    {strs[5], optimize_case6_exp, 1},
    {strs[6], optimize_case7_exp, 1},
};

static void test_optimizeSegments(int n) {
    OptimizeTestCase c = optimize_testCases[n];

    printf("\n%s", c.data);
    uint8_t sl, v=20;
    QrDataSegment *segments = QrEncoder.optimizeSegments(c.data, strlen(c.data), v, sl);
    printSegments(segments, sl, v);
    // sanityCheckSegments(segments, sl);
    compareSegments(c.expected, c.expLen, segments, sl);
}
void encoder_optimizeSegments_simpleUtf8() {
    test_optimizeSegments(0);
}
void encoder_optimizeSegments_3byte3modes() {
    test_optimizeSegments(1);
}
void encoder_optimizeSegments_easilyOptimized() {
    test_optimizeSegments(2);
}
void encoder_optimizeSegments_longerButEasy() {
    test_optimizeSegments(3);
}
void encoder_optimizeSegments_aBitMoreRandom() {
    test_optimizeSegments(4);
}
void encoder_optimizeSegments_base64() {
    test_optimizeSegments(5);
}
void encoder_optimizeSegments_moreThan1Kb() {
    test_optimizeSegments(6);
}

