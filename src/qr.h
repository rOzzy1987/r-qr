#ifndef __QR_H
#define __QR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "qr_types.h"
#include "qr_data.h"
#include "qr_gf256.h"
#include "qr_render.h"
#include "qr_rank.h"
#include "qr_debug.h"


static bool contains(const uint8_t* arr, uint16_t length, uint8_t search){
    for(uint16_t i = 0; i < length; i++){
        if (arr[i] == search) return true;
    }
    return false;
}


class CQrGen {
    public:
        CQrGen() {
        }
        QrCode* create(const char* data, uint16_t length, QrMode mode = QrMode::Unspecified, QrEcc ecLevel = QrEcc::L){
            QrCode *result = createRaw(data, length, mode, ecLevel);
            setFormat(result);
            QrRenderer.render(result);
            QrRenderer.renderFormat(result);

            changeMask(result, 4);
            // applyBestMask(result);

            return result;
        }

        QrCode* createRaw(const char* data, uint16_t length, QrMode mode = QrMode::Unspecified, QrEcc ecLevel = QrEcc::L) {
            if (mode == QrMode::Unspecified)
                mode = this->getMode(data, length);
            uint8_t version = this->getVersion(length, mode, ecLevel);
            QrCode *result = new QrCode(version, ecLevel);

            QrBlockStruct s = qr_blocks[version][ecLevel];
            uint8_t *buff = (uint8_t*)malloc(s.totalWords());
            memset(buff, 0, s.totalWords());
            QrBufferCursor buffCur;
            
            addValue(buff, &buffCur, qr_mode_indicator[mode], 4);
            addValue(buff, &buffCur, length, qr_lengthBits(version, mode));
            addData(buff, &buffCur, data, length, mode);

            addPadding(buff, &buffCur, s.dataWords());

            uint16_t edcLen = 0;
            uint8_t *edc = Gf256.getEdc(buff, buffCur.byte, s.totalWords(), edcLen);

            memcpy(buff+buffCur.byte, edc, edcLen);
            result->raw = buff;
            result->rawSize = buffCur.byte + edcLen;

            return result;
        }

        /**
         * Sets the format of the QR code, setting the 15 bits into the formatPoly array of the code.
         * The format is as follows:
         *  bits 0-1: Error correction level
         *  bits 2-4: Mask
         *  bits 5-14: remainder of the polynomial division
         * The format is then xored with the format mask
         * @param code The QR code to set the format of
         */
        void setFormat(QrCode *code){
            // 00, 01, 10, 11
            // 01, 00, 11, 10
            uint8_t 
                ec = code->ecLevel ^ 0x01,
                m = code->mask,
                *f = code->formatPoly;
            memset(f,0, 15);
            f[0] = ec >> 1;
            f[1] = ec & 1;
            f[2] = m >> 2;
            f[3] = (m >> 1) & 1;
            f[4] = m & 1;

            printf("\nec:%d m:%d", ec,m);
            print_arr_f("%d", f, 15);

            uint8_t *rest = new uint8_t[10];
            uint16_t restLen;
            Gf256.polyRest(f, 5, qr_format_divisor, 11, rest, restLen);
            memcpy(f + 5, rest, restLen);

            print_arr_f("%d", qr_format_divisor, 11);
            print_arr_f("%d", rest, restLen);
            print_arr_f("%d", f, 15);

            delete[] rest;
            for(uint8_t i = 0; i < 15; i++){
                f[i] ^= qr_format_mask[i]; 
            }
        }
        
        /**
         * Applies the best mask to the QR code. It does this by computing the rank of the QR code with each mask and then
         * selecting the mask with the lowest rank.
         * @param code The QR code to apply the best mask to
         */
        void applyBestMask(QrCode *code){
            uint16_t scores[8] = {QrRanker.rank(code)};
            for(uint8_t i = 1; i < 8; i++){
                changeMask(code, i);
                scores[i] = QrRanker.rank(code);
            }

            uint8_t mi = 0;
            for (uint8_t i = 0; i < 8; i++){
                if (scores[i] < scores[mi]) mi = i;
            }
            printf("\n Bast mask: %d", mi);
            changeMask(code, mi);
        }

        void changeMask(QrCode *code, uint8_t mask){
            QrRenderer.applyMask(code, code->mask);
            QrRenderer.applyMask(code, mask);
            code->mask = mask;
            setFormat(code);
            QrRenderer.renderFormat(code);
        }



        /**
         * Gets the most data-dense mode the data can fit
         */
        QrMode getMode(const char* data, uint16_t length) {
            uint8_t modes = 0b111;
            for(uint16_t i = 0; i < length; i++){
                uint8_t r = 0b100;
                if (data[i] >= '0' && data[i] <= '9') r |= 0b011;
                if (data[i] >= 'A' && data[i] <= 'Z') r |= 0b010;
                if (contains(qr_alphanum_chars, 9, data[i])) r |= 0b010;

                modes &= r;

                if(modes == 4)
                return QrMode::Byte;
            }
            if (modes & 1) return QrMode::Numeric;
            if (modes & 2) return QrMode::AlphaNumeric;
            return QrMode::Byte;
        };
        /**
         * Gets the smallest suitable version for a given mode and length
         * @returns 0-based version number (0-39)
         */
        uint8_t getVersion(uint16_t length, QrMode mode, QrEcc ecLevel){
            for(uint8_t i = 0; i < 40; i++) {
                if(qr_capacities[mode][i][ecLevel] > length) return i;
            }
            return 0xFF;
        }

        void addData(uint8_t *buff, QrBufferCursor *cursor, const char* data, uint16_t length, QrMode mode){
            switch(mode) {
                case QrMode::Numeric:
                    return this->addDataNum(data, length, buff, cursor);
                case QrMode::AlphaNumeric:
                    return this->addDataAlpha(data, length, buff, cursor);
                case QrMode::Byte:
                    return this->addDataByte(data, length, buff, cursor);
            }
        }
        void addDataNum(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
            uint16_t i = 0;
            uint16_t acc = 0;
            while (i < length) {
                acc = (acc*10)+(data[i] - '0');
                i++;
                if (i % 3 > 0) continue;

                addValue(buff, cursor, acc, 10);
                acc = 0;
            } 
            if (i%3 >0) {
                uint8_t rem  = 3 - (i % 3);
                for (uint8_t j = 0; j <rem; j++){
                    acc = acc*10;
                }
                addValue(buff, cursor, acc, 10);
            }
        }
        void addDataAlpha(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
            uint16_t i = 0;
            uint16_t acc = 0;
            while (i < length) {
                uint8_t d = 0;
                if (data[i] >= '0' && data[i] <= '9')
                    d = data[i] - '0';
                else if (data[i] >= 'A' && data[i] <= 'Z')
                    d = 10 + data[i] - 'A';
                else {
                    for(uint8_t j = 0;j < 9;j++) {
                        if (qr_alphanum_chars[j] != data[i]) continue;
                        d = 36 + j;
                        j = 9;
                    }
                }
                if (i & 1) 
                    acc *= 45; 
                acc += d;
                i++;  
                
                if (i & 1) continue;
                addValue(buff, cursor, acc, 11);

                acc = 0;
            } 
            
            if (i & 1) {
                acc *= 45;
                addValue(buff, cursor, acc, 11);
            }

        }
        void addDataByte(const char *data, uint16_t length, uint8_t *buff, QrBufferCursor *cursor){
            uint16_t i = 0;
            while (i < length) {
                addValue(buff, cursor, data[i], 8);
                i++;
            } 
        }
        void addValue(uint8_t *buff, QrBufferCursor *cursor, uint16_t value, uint8_t bitLength) {
            if (bitLength > 8){
                addValue(buff, cursor, value >> 8, bitLength - 8);
                addValue(buff, cursor, value & 0xFF, 8);
                return;
            }

            int8_t sh = 8 - cursor->bit - bitLength;
            if (sh >= 0){
                buff[cursor->byte] |= value << sh;
                cursor->bit += bitLength;
            } else {
                buff[cursor->byte] |= value >> (-sh);
                cursor->bit = 0;
                cursor->byte++;
                
                
                buff[cursor->byte] |= value << (8+sh);
                cursor->bit += -sh;
            }

            if (cursor->bit == 8) {
                cursor->bit = 0;
                cursor->byte++;
            }
        }
    
        void addPadding(uint8_t *buff, QrBufferCursor *cursor, uint16_t len){
            addValue(buff,cursor,0,4);
            if(cursor->bit != 0){
                addValue(buff, cursor, 0, 8-cursor->bit);
            }

            uint8_t f = 0;
            while(cursor->byte < len){
                addValue(buff,cursor, f ? 17 : 236, 8);
                f = !f;
            }
        }
};

CQrGen QrGenerator;

#endif //__QR_H