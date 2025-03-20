#include "qr.h"
#include "qr_data.h"
#include "qr_gf256.h"
#include "qr_render.h"
#include "qr_rank.h"
#include "qr_encoder.h"

QrCode* CQrGen::create(const char* data, uint16_t length, QrMode mode, QrEcc ecLevel, uint8_t version){
    QrCode *code = createRaw(data, length, mode, ecLevel, version);
    setFormatPoly(code);
    setVersionPoly(code);
    QrRenderer.render(code);
    QrRenderer.renderFormat(code);
    QrRenderer.renderVersion(code);
    applyBestMask(code);

    return code;
}

QrCode* CQrGen::createRaw(const char* data, uint16_t length, QrMode mode, QrEcc ecLevel, uint8_t version) {
    uint8_t *buff;
    uint16_t dataLen;

    buff = QrEncoder.encode(data, length, dataLen, version, mode, ecLevel);
    QrCode *code = new QrCode(version, ecLevel);
    code->mode = mode;
    code->raw = buff;
    code->rawSize = dataLen;

    return code;
}

void CQrGen::setFormatPoly(QrCode *code){
    // Need to map ecLevel according to the table below
    // Note that only the last bit is flipped
    //                L  M  Q  H
    // Logical value  00 01 10 11
    // Mapped value   01 00 11 10
    int8_t 
        ec = code->ecLevel ^ 0x01,
        m = code->mask;
    uint16_t 
        f = ((ec << 3) | m) << 10;

    f = (uint16_t)setEccBits(f, qr_format_divisor, 15, 11);
    f ^= qr_format_mask;
    code->formatPoly = f;
}

void CQrGen::setVersionPoly(QrCode *code){
    code->versionPoly = setEccBits((code->version+1) << 12, qr_version_divisor, 18, 13);
}

uint32_t CQrGen::setEccBits(uint32_t f, uint32_t d, uint8_t totalLength, uint8_t divisorLength){
    uint32_t t = f,
    bit = 1 << (totalLength - 1);

    for(int8_t i = totalLength - divisorLength; i >= 0; i--){
        if (t & bit){
            t ^= d << i;
        }
        bit >>= 1;
    }
    return f | t;
}

void CQrGen::applyBestMask(QrCode *code){
    uint16_t scores[8] = {QrRanker.rank(code)};
    for(uint8_t i = 1; i < 8; i++){
        changeMask(code, i);
        scores[i] = QrRanker.rank(code);
    }

    uint8_t mi = 0;
    for (uint8_t i = 0; i < 8; i++){
        if (scores[i] < scores[mi]) mi = i;
    }
    changeMask(code, mi);
}

void CQrGen::changeMask(QrCode *code, uint8_t mask){
    QrRenderer.applyMask(code, code->mask);
    QrRenderer.applyMask(code, mask);
    code->mask = mask;
    setFormatPoly(code);
    QrRenderer.renderFormat(code);
}

CQrGen QrGenerator;