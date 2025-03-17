#include "common.h" 

struct EncodeTestCase {
    const char* data;
    const uint8_t version;
    const QrMode mode;
    const QrEcc ecLevel;
    const uint8_t expectedVersion;
    const uint8_t* expectedData;
    const uint16_t expectedDataLen;
    const QrMode expectedMode;
};

const char* encode_data_case1 = "https://www.qrcode.com/";
const uint8_t encode_expected_case1[] ={65, 118, 135, 71, 71, 7, 51, 162, 242, 247, 119, 119, 114, 231, 23, 38, 54, 246, 70, 82, 230, 54, 246, 210, 240, 236, 17, 236, /* EDC */ 52, 61, 242, 187, 29, 7, 216, 249, 103, 87, 95, 69, 188, 134, 57, 20};

const char* encode_data_case2 = "HTTPS://WWW.QRCODE.COM";
const uint8_t encode_expected_case2[] ={32, 179, 26, 166, 84, 99, 221, 92, 11, 149, 43, 81, 162, 87, 237, 209, 56, 0, 236, 65, 168, 37, 53, 153, 197, 197};

const char* encode_data_case3 = "12345678901234567890123";
const uint8_t encode_expected_case3[] ={16, 92, 123, 114, 49, 80, 49, 89, 169, 184, 83, 152, 0, 236, 17, 236, 211, 82, 35, 228, 100, 21, 124, 216, 221, 201};

const char* encode_data_case4 = "https://www.qrcode.com/";
const uint8_t encode_expected_case4[] ={65, 118, 135, 71, 71, 7, 51, 162, 242, 247, 119, 119, 114, 84, 95, 201, 88, 163, 191, 40, 225, 129, 110, 34, 252, 135, 125, 122, 129, 24, 23, 163, 148, 197, 211, 231, 23, 38, 54, 246, 70, 82, 230, 54, 246, 210, 240, 236, 25, 196, 87, 10, 128, 247, 2, 242, 118, 87, 30, 246, 223, 168, 155, 124, 107, 143, 165, 148, 141, 236};


const EncodeTestCase encode_testCases[] = {
    {encode_data_case1, 40, QrMode::Unspecified, QrEcc::M, 1, encode_expected_case1, 44, QrMode::Byte},
    {encode_data_case2, 40, QrMode::Unspecified, QrEcc::L, 0, encode_expected_case2, 26, QrMode::AlphaNumeric},
    {encode_data_case3, 40, QrMode::Unspecified, QrEcc::M, 0, encode_expected_case3, 26, QrMode::Numeric},
    {encode_data_case4, 40, QrMode::Unspecified, QrEcc::H, 2, encode_expected_case4, 70, QrMode::Byte},
};

void encode_runTestCase(uint8_t n){
    EncodeTestCase tc = encode_testCases[n];
    QrMode mode = tc.mode;
    uint16_t l;
    uint8_t v = tc.version;
    uint8_t *r = QrEncoder.encode(tc.data, strlen(tc.data), l, v, mode, tc.ecLevel);
    // print_arr(r, l);
    ASSERT_EQ_UI8(tc.expectedMode, mode);
    ASSERT_EQ_UI8(tc.expectedVersion, v);
    ASSERT_EQ_UI16(tc.expectedDataLen, l);
    if (tc.expectedData != nullptr)
        ASSERT_EQ_UI8A(tc.expectedData, r, l);
}

void encoder_encode_sameAsOnlineExample(){
    encode_runTestCase(0);
}

void encoder_encode_uppercaseUrlAsAlpha(){
    encode_runTestCase(1);
}

void encoder_encode_pureNumeric(){
    encode_runTestCase(2);
}
void encoder_encode_highEcLevel(){
    encode_runTestCase(3);
}