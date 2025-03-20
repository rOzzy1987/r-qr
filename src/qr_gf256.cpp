
#include <string.h>
#include "qr_gf256.h"

CGf256::CGf256() {
#ifndef GF_256_PRECOMPUTED
    memset(this->exp, 0, 256);
    memset(this->log, 0, 256);
    for(uint16_t e = 1, v = 1; e < 256; e++){
        v = v > 127 
            ? ((v << 1) ^ 285) 
            : v << 1;
        this->log[v] = e % 255;
        this->exp[e % 255] = v;
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
uint8_t CGf256::add(uint8_t a, uint8_t b) {
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
uint8_t CGf256::sub(uint8_t a, uint8_t b) {
    // return a-b; // for arithmetic debugging only
    return add(a,b);
}

/**
 * Multiply 2 numbers in the GF(256) field
 * @param a number 1
 * @param b number 2
 * @return result of the multiplication
 */
uint8_t CGf256::mul(uint8_t a, uint8_t b) {
    // return a* b; // for arithmetic debugging only
    return a && b 
        ? exp[(log[a] + log[b]) % 255] 
        : 0;
}

/**
 * Divide 2 numbers in the GF(256) field
 * @param a number 1
 * @param b number 2
 * @return result of the division
 */
uint8_t CGf256::div(uint8_t a, uint8_t b) {
    // return a/b; // for arithmetic debugging only
    return exp[(log[a] + log[b] * 254) % 255];
}


/**
 * Raise a polynomial to a specified degree
 */
void CGf256::polyRaise(const uint8_t *poly, uint16_t polyLen, uint16_t newLen, uint8_t *result){
    memset(result, 0, newLen);
    memcpy(result, poly, polyLen);
}

void CGf256::polyExpand(const uint8_t *poly, uint16_t polyLen, uint16_t newLen, uint8_t *result){
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
void CGf256::polyMul(const uint8_t *poly1, uint16_t poly1Len, const uint8_t *poly2, uint16_t poly2Len, uint8_t *result, uint16_t &resultLen) {
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
void CGf256::polySub(const uint8_t* poly1, uint16_t poly1Len, const uint8_t* poly2, uint16_t poly2Len, uint8_t* result, uint16_t &resultLen) {
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
void CGf256::polySub2(uint8_t* poly1, uint16_t poly1Len, const uint8_t* poly2, uint16_t poly2Len) {
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
 * @param resultLen Reference to store the length of the remainder. (should be of length divisorLen - 1)
 */

void CGf256::polyRest(const uint8_t *dividend, uint16_t dividendLen, const uint8_t *divisor, uint16_t divisorLen, uint8_t *result, uint16_t &resultLen) {
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

/**
 * Computes the generator polynomial of degree n for QR codes.
 *
 * The generator polynomial is used to generate the error correction
 * codes for QR codes. The polynomial is computed using the formula
 * x^n + x^(n-1) + ... + x^2 + x + 1.
 *
 * @param degree The degree of the generator polynomial.
 * @param genLen The length of the generated polynomial.
 * @return The generated polynomial as an array of coefficients.
 */
uint8_t* CGf256::getGeneratorPoly(uint16_t degree, uint16_t &genLen) {
    genLen = degree+1;
    uint8_t *result = new uint8_t[genLen];
    memset(result, 0, genLen);
    result[0] = 1;
    uint8_t m[2];
    m[0] = 1;
    uint16_t tempLen;
    uint8_t *temp = new uint8_t[degree + 1];

    for (uint8_t i = 0; i < degree; i++) {
        m[1] = exp[i];
        polyMul(result, i+1, m, 2, temp, tempLen);
        memcpy(result, temp, tempLen);
    }
    delete[] temp;
    return result;
}

uint8_t* CGf256::getEdc(const uint8_t *data, uint16_t dataLen, uint16_t codewords, uint16_t &edcLen) {
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


CGf256 Gf256 = CGf256();

