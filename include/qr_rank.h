#ifndef __QR_RANK
#define __QR_RANK

#include "qr_types.h"

class CQrRanker {
    public: 
    
    
        uint16_t rank(QrCode *code);
        /**
         * @brief Rank a QR code based on the number of consecutive pixels in the same
         * color. The score is the sum of the lengths of the sequences of more than 4
         * pixels in the same color minus two. The sequences are searched horizontally
         * and vertically.
         * @param[in] code the QR code to rank
         * @return the rank of the QR code
         */
        uint16_t rank1(QrCode *code);
        /**
         * @brief Rank 2 of a QR code. Penalties are given if 2x2 blocks are
         *        identical. The score is the number of such blocks times 3.
         *
         * @param code The QR code to rank.
         *
         * @return The rank 2 of the QR code.
         */
        uint16_t rank2(QrCode *code);
        
        /**
         * @brief Rank 3 of a QR code. Penalties are given if a finder pattern
         *        (00101111101) is found in the data area of the QR code. The
         *        score is 40 times the number of occurrences of the pattern.
         *
         * @param[in] code The QR code to rank.
         *
         * @return The rank 3 of the QR code.
         */
        uint16_t rank3(QrCode *code);
        /**
         * @brief Rank 4 of a QR code. The score is related to the density of the
         *        QR code. The score is 2 times the absolute difference between
         *        the number of black pixels and half the total number of pixels.
         *
         * @param[in] code the QR code to rank.
         *
         * @return the rank 4 of the QR code.
         */
        uint16_t rank4(QrCode *code);
    private:
        uint8_t getPixel(QrCode *code, uint8_t x, uint8_t y);
};

extern CQrRanker QrRanker;

#endif