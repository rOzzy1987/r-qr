#ifndef __GF256_H
#define __GF256_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GF_256_PRECOMPUTED


class CGf256 {
    public:
#ifndef GF_256_PRECOMPUTED
        uint8_t _log[256];
        uint8_t _exp[256];
#else 
        uint8_t _log[256] = {0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175};
        uint8_t _exp[256] = {
              1,   2,   4,   8,  16,  32,  64, 128,  29,  58, 116, 232, 205, 135,  19,  38, 
             76, 152,  45,  90, 180, 117, 234, 201, 143,   3,   6,  12,  24,  48,  96, 192, 
            157,  39,  78, 156,  37,  74, 148,  53, 106, 212, 181, 119, 238, 193, 159,  35, 
             70, 140,   5,  10,  20,  40,  80, 160,  93, 186, 105, 210, 185, 111, 222, 161, 
             95, 190,  97, 194, 153,  47,  94, 188, 101, 202, 137,  15,  30,  60, 120, 240, 
            253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163,  91, 182, 113, 226, 
            217, 175,  67, 134,  17,  34,  68, 136,  13,  26,  52, 104, 208, 189, 103, 206, 
            129,  31,  62, 124, 248, 237, 199, 147,  59, 118, 236, 197, 151,  51, 102, 204, 
            133,  23,  46,  92, 184, 109, 218, 169,  79, 158,  33,  66, 132,  21,  42,  84, 
            168,  77, 154,  41,  82, 164,  85, 170,  73, 146,  57, 114, 228, 213, 183, 115, 
            230, 209, 191,  99, 198, 145,  63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 
            227, 219, 171,  75, 150,  49,  98, 196, 149,  55, 110, 220, 165,  87, 174,  65, 
            130,  25,  50, 100, 200, 141,   7,  14,  28,  56, 112, 224, 221, 167,  83, 166, 
             81, 162,  89, 178, 121, 242, 249, 239, 195, 155,  43,  86, 172,  69, 138,   9, 
             18,  36,  72, 144,  61, 122, 244, 245, 247, 243, 251, 235, 203, 139,  11,  22, 
             44,  88, 176, 125, 250, 233, 207, 131,  27,  54, 108, 216, 173,  71, 142,   0
        };
#endif
        CGf256() {

#ifndef GF_256_PRECOMPUTED
            memset(this->_exp, 0, 256);
            memset(this->_log, 0, 256);
            for(uint16_t e = 1, v = 1; e < 256; e++){
                v = v > 127 
                    ? ((v << 1) ^ 285) 
                    : v << 1;
                this->_log[v] = e % 255;
                this->_exp[e % 255] = v;
            }
#endif
        }


        /**
         * Add 2 numbers in the GF(256) field
         * (XOR)
         * Note: subtraction is the same here as addition
         * @param a number 1
         * @param b number 2
         * @return result of the addition
         */
        uint8_t add(uint8_t a, uint8_t b) {
            // return a + b; // for arithmetic debugging only
            return a^b;
        }


        /**
         * Subtract 2 numbers in the GF(256) field
         * (XOR)
         * Note: subtraction is the same here as addition
         * @param a number 1
         * @param b number 2
         * @return result of the addition
         */
        uint8_t sub(uint8_t a, uint8_t b) {
            // return a-b; // for arithmetic debugging only
            return add(a,b);
        }

        /**
         * Multiply 2 numbers in the GF(256) field
         * @param a number 1
         * @param b number 2
         * @return result of the multiplication
         */
        uint8_t mul(uint8_t a, uint8_t b) {
            // return a* b; // for arithmetic debugging only
            return a && b 
                ? this->_exp[(this->_log[a] + this->_log[b]) % 255] 
                : 0;
        }
        
        /**
         * Divide 2 numbers in the GF(256) field
         * @param a number 1
         * @param b number 2
         * @return result of the division
         */
        uint8_t div(uint8_t a, uint8_t b) {
            // return a/b; // for arithmetic debugging only
            return this->_exp[(this->_log[a] + this->_log[b] * 254) % 255];
        }


        /**
         * Raise a polynomial to a specified degree
         */
        void polyRaise(const uint8_t *poly, uint16_t polyLen, uint16_t newLen, uint8_t *result){
            memset(result, 0, newLen);
            memcpy(result, poly, polyLen);
        }

        void polyExpand(const uint8_t *poly, uint16_t polyLen, uint16_t newLen, uint8_t *result){
            memset(result, 0, newLen);
            uint16_t 
                d = newLen > polyLen
                    ? newLen - polyLen
                    : 0,
                l = newLen > polyLen + d
                    ? polyLen
                    : newLen - d; 

            memcpy(result + d, poly, l);
        }

        /**
         * Poynomial multiplication
         * @param poly1 first polynomial
         * @param poly1Len first polynomial lenght
         * @param poly2 second polynomial
         * @param poly2Len second polynomial length
         * @param result Should be at least of length (poly1Len + poly2Len - 1)
         */
        void polyMul(const uint8_t *poly1, uint16_t poly1Len, const uint8_t *poly2, uint16_t poly2Len, uint8_t *result, uint16_t &resultLen) {
            resultLen = poly1Len + poly2Len -1;  
            memset(result, 0, resultLen);

            for(uint16_t p1i = 0; p1i < poly1Len; p1i++){
                for (uint16_t p2i = 0; p2i < poly2Len; p2i++) {
                    uint8_t v = result[p1i + p2i];
                    result[p1i+p2i] = add(v, mul(poly1[p1i], poly2[p2i])); 
                }
            }
        }

        /**
         * Poynomial subtraction
         * @param poly1 first polynomial
         * @param poly1Len first polynomial lenght
         * @param poly2 second polynomial
         * @param poly2Len second polynomial length
         * @param result Should be at least of length (poly1Len)
         */
        void polySub(const uint8_t* poly1, uint16_t poly1Len, const uint8_t* poly2, uint16_t poly2Len, uint8_t* result, uint16_t &resultLen) {
            resultLen = poly1Len;
            uint16_t d = poly1Len - poly2Len;
            memcpy(result, poly1, poly1Len);
            for(uint16_t p1i = d, p2i = 0; p1i < poly1Len; p1i++, p2i++) {
                result[p1i] = sub(poly1[p1i], poly2[p2i]);
            }
        }

        /**
         * Subtract polynomial 2 from polynomial 1 in place
         * @param poly1 first polynomial
         * @param poly1Len first polynomial lenght
         * @param poly2 second polynomial
         * @param poly2Len second polynomial length
         */
        void polySub2(uint8_t* poly1, uint16_t poly1Len, const uint8_t* poly2, uint16_t poly2Len) {
            uint16_t d = poly1Len - poly2Len;
            for(uint16_t p1i = d, p2i = 0; p1i < poly1Len; p1i++, p2i++) {
                poly1[p1i] = sub(poly1[p1i], poly2[p2i]);
            }
        }
        
        /**
         * Computes the remainder of a polynomial division in GF(256).
         * 
         * This function performs polynomial division on the given dividend and divisor,
         * and returns the remainder of the division.
         *
         * @param dividend The dividend polynomial represented as an array of coefficients.
         * @param dividendLen The length of the dividend polynomial.
         * @param divisor The divisor polynomial represented as an array of coefficients.
         * @param divisorLen The length of the divisor polynomial.
         * @param result The array to store the remainder of the division.
         * @param resultLen Reference to store the length of the remainder. (should be )
         */

        void polyRest(const uint8_t *dividend, uint16_t dividendLen, const uint8_t *divisor, uint16_t divisorLen, uint8_t *result, uint16_t &resultLen) {
            uint16_t dummy;
            
            uint8_t *rest = new uint8_t[dividendLen];
            uint8_t *subtr = new uint8_t[dividendLen];
            memcpy(rest, dividend, dividendLen);
            uint16_t restLen = dividendLen;
            uint16_t ri = 0;
        
            uint8_t *temp = new uint8_t[divisorLen]; 
            memset(temp, 0, divisorLen);
        
            for (ri = 0; restLen >= divisorLen; ri++, restLen--) {
                // divide the current term of the dividend with the first term of divisor
                uint8_t q = div(rest[ri], divisor[0]);
                // multiply the divisor by the quotient
                polyMul(divisor, divisorLen, &q, 1, temp, dummy);
                // Raise the result to the same power as rest
                polyRaise(temp, divisorLen, restLen, subtr);
                // subtract the result from the dividend (effectively 0 out the current term) 
                polySub2(rest + ri, restLen, subtr, restLen);
            }
        
            resultLen = restLen;
            memcpy(result, rest + ri, restLen);
        
            delete[] rest;
            delete[] subtr;
            delete[] temp;
        }
    
        uint8_t* getGeneratorPoly(uint16_t degree, uint16_t &genLen) {
            genLen = degree+1;
            uint8_t *result = new uint8_t[genLen];
            memset(result, 0, genLen);
            result[0] = 1;
            uint8_t m[2];
            m[0] = 1;
            uint16_t tempLen;
            uint8_t *temp = new uint8_t[degree + 1];

            for (uint8_t i = 0; i < degree; i++) {
                m[1] = _exp[i];
                polyMul(result, i+1, m, 2, temp, tempLen);
                memcpy(result, temp, tempLen);
            }
            delete[] temp;
            return result;
        }

        uint8_t* getEdc(const uint8_t *data, uint16_t dataLen, uint16_t codewords, uint16_t &edcLen) {
            uint16_t degree = codewords - dataLen;
            uint8_t *messagePoly = new uint8_t[codewords];
            memset(messagePoly, 0, codewords);
            memcpy(messagePoly, data, dataLen);

            uint16_t genLen;
            uint8_t *gen = getGeneratorPoly(degree, genLen);
            uint8_t *result = new uint8_t[degree];
            memset(result, 0, degree);
            

            polyRest(messagePoly, codewords, gen, genLen, result, edcLen);
            delete[] messagePoly;
            delete[] gen;

            return result;
          }

};

CGf256 Gf256 = CGf256();

#endif //__GF256_H

