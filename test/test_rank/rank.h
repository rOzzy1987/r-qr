#include "../common.h"

void ranker_rank1() {
    QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank1(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(248, r, __LINE__, "");
}
void ranker_rank2() {
    QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank2(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(117, r, __LINE__, "");
}
void ranker_rank3() {
    QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank3(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(40, r, __LINE__, "");
}
void ranker_rank4() {
    QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);

    uint16_t r = QrRanker.rank4(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(10, r, __LINE__, "");
}