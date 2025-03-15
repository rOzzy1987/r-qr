#include "../common.h"

static const char* strs[] = {
    "óóóó",
    "4A?",
    "asdfQWERTZ1234524762",
    "13462371vniu34 44NIU!L%5IJhLh53_lk33ádfsd",
    "ASDFHSDretSDFADSFHrt3345sfdSDG",
    "9Sz6j4kk5M35zQjS+aQOIE56+y/1fjKnnkU5Yel0Cy4FCFeRMs8tC2CtYOtFyE7ww4XhDGhOZk1iAX5j+5WcB/s606VXHLd5rmfKOly+21yCoR72gcQwCeh6NpWcshNIbIUvv5FVumaODGnUYHTSP32a6lIWJFyltYAe6tcHFRuKZ7GRyNwes5pKWRd41ayLMnrwWqnnZ6i/Jkms1hCNyRTnyzvb0m+FC9V7CiTnyuCJnob33X77o9xk0GljZGz4tTWz7kKy4uWf+S4w7PJPFvINqMhhPEVDO27PLKzttMsXT2/7low5zLI30WmDb6cFnqpetY5uEPhSe2v0VAnr"
};

static void test_optimizeSegments(const char* data, uint16_t len, uint8_t expectedLen) {
    printf("\n%s", data);
    uint8_t sl;
    QrDataSegment *segments = QrEncoder.optimizeSegments(data,len,20, sl);
    printf("\n%d", sl);
    for (uint8_t i = 0; i < sl; i++){
        QrMode m = segments[i].mode;
        printf("\n(%3d-%3d): %s", segments[i].from, segments[i].to, m == 0 ? "Num" : m == 1 ? "Alpha" : "Byte");
    }
    printf("\n");
}
void encoder_optimizeSegments() {
    for(uint8_t i = 0; i <4; i++){
        test_optimizeSegments(strs[i], strlen(strs[i]), 2);
    }

    printf("\n\n  l   N   A   B");
    for (uint8_t i = 0; i < 32; i++){
        printf("\n%3d %3d %3d %3d", i, 
            QrEncoder.getSegmentWordCount(QrMode::Numeric, 5, i),
            QrEncoder.getSegmentWordCount(QrMode::AlphaNumeric, 5, i),
            QrEncoder.getSegmentWordCount(QrMode::Byte, 5, i)
        );
    }
    printf("\n");
}

