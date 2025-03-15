#include "../common.h"

void ranker_rank1() {
    QrCode *code = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank1(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(244, r, __LINE__, "");
}
void ranker_rank2() {
    QrCode *code = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank2(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(162, r, __LINE__, "");
}
void ranker_rank3() {
    QrCode *code = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::M);
    uint16_t r = QrRanker.rank3(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(80, r, __LINE__, "");
}
void ranker_rank4() {
    QrCode *code = subject->create("https://www.qrcode.com/", 23, QrMode::Byte, QrEcc::H);

    // printf("\n");
    // print_qr(code);
    // printf("\n");

    uint16_t r = QrRanker.rank4(code);
    
    UNITY_TEST_ASSERT_EQUAL_UINT16(0, r, __LINE__, "");
}