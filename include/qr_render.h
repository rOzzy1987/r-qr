#ifndef __QR_RENDER
#define __QR_RENDER

#include "qr_types.h"
#include "qr_debug.h"

class CQrRenderer {
    public:
        void render(QrCode *code);
        void renderVersion(QrCode *code);
        void renderFormat(QrCode *code);
        void renderData(QrCode *code);
        void renderStatics(QrCode *code);
        void applyMask(QrCode *code, uint8_t m);

#ifndef QR_TESTING
    private:
#endif
        uint8_t isProtected(const QrCode *code, uint8_t x, uint8_t y, const uint8_t *af, uint8_t al);
        uint8_t isVersionInfo(const QrCode *code, uint8_t x, uint8_t y);

        void renderQuietZones(QrCode *code);
        void renderTiming(QrCode *code) ;
        void renderLocators(QrCode *code);
        void renderAlignments(QrCode *code);
        void renderLocator(QrCode *code, uint8_t x, uint8_t y);
        void renderAlignment(QrCode *code, uint8_t x, uint8_t y);
        void renderRect(QrCode *code, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t v);
        void setPixel(QrCode *code, uint8_t x, uint8_t y, uint8_t value);

        inline uint8_t mask(uint8_t mask, uint8_t x, uint8_t y);
        uint8_t mask0(uint8_t x, uint8_t y);
        uint8_t mask1(uint8_t x, uint8_t y);
        uint8_t mask2(uint8_t x, uint8_t y);
        uint8_t mask3(uint8_t x, uint8_t y);
        uint8_t mask4(uint8_t x, uint8_t y);
        uint8_t mask5(uint8_t x, uint8_t y);
        uint8_t mask6(uint8_t x, uint8_t y);
        uint8_t mask7(uint8_t x, uint8_t y);

};

extern CQrRenderer QrRenderer;

#endif