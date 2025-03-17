//////////////////
// Sample main file to build all and get memory/flash statistics 
// from PlatformIO build

#if defined ARDUINO || defined ESP32
#include <arduino.h>
#endif

// Comment out to only use a single data segment in the code
#define QR_OPTIMIZE_SEGMENTS
#include "qr.h"

#if defined ARDUINO || defined ESP32
    void setup() {
        QrCode *code = QrGenerator.create("asdf", 4);
        const uint8_t *bmp = code->bitmap;
        code->bitmap = nullptr;
        delete code;

        Serial.println(bmp[0]);
    }

    void loop() {

    }

#else
    int main(){
        QrCode *code = QrGenerator.create("asdf", 4);
        const uint8_t *bmp = code->bitmap;
        code->bitmap = nullptr;
        delete code;

        return bmp[0];
    }
#endif