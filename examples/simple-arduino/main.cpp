#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"
#include <Arduino.h>
#include <qr.h>


// This is an Adafruit GFX example, you can use any display driver library that is compatible
// Of course the QR code generation is independent, you can send it over in ASCII over UART if you want
#define TFT_DC  4
#define TFT_CS 15
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, A0);

#define QR_BUF_SIZE 165
// 165*165px monochrome pixel buffer
uint8_t qr_buffer[QR_BUF_SIZE * QR_BUF_SIZE >> 3]; 

/**
 * @brief Draws a QR code on the screen, centered
 * @param[in] data the data to encode in the QR code
 * @details This function creates a QR code from the given data string, scales it to fit the screen, and draws it centered on the screen.
 *          The QR code is drawn using the black and white colors of the TFT display.
 *          The function deletes the QR code object after use, so it does not need to be deleted manually.
 */
void drawQr(const char* data) {
  QrCode *code = QrGenerator.create(data, strlen(data));

  uint8_t zoom = QR_BUF_SIZE / code->size;
  uint8_t size = zoom * code->size;
  uint8_t stride = (size + 7) / 8;

  memset(qr_buffer, 0, QR_BUF_SIZE * QR_BUF_SIZE >> 3);
  for (int y = 0; y < code->size; y++) {
    for (int x = 0; x < code->size; x++) {

      uint8_t pixel = (code->bitmap[(y * code->bitmapStride) + (x >> 3)] >> (7 - (x & 7))) & 1;

      for (int dy = 0; dy < zoom; dy++) {
        for (int dx = 0; dx < zoom; dx++) {
          int dstX = x * zoom + dx;
          int dstY = y * zoom + dy;

          qr_buffer[(dstY * stride) + (dstX >> 3)] |= (pixel << (7 - (dstX & 7)));
        }
      }
    }
  }

  tft.drawBitmap(120-(size>>1),120-(size>>1),qr_buffer, size, size, GC9A01A_BLACK, GC9A01A_WHITE);
  delete code;
}

void setup() {
#ifdef ESP8266
  // ESP8266 Bootloader baud rate
  Serial.begin(74880); 
#else 
  #ifdef ESP32
  // ESP32 Bootloader baud rate
  Serial.begin(460800);
  #else 
  // Other boards
  Serial.begin(115200);
  #endif
#endif
  tft.begin();
  // tft.fillScreen(GC9A01A_WHITE);

  unsigned long long start = micros();
  const char* data = "Sample QR code created on a microcontroller! Awesome!";
  // const char* data = "WIFI:S:<YOUR_SSID>;T:WPA;P:<YOUR_PASSWORD>;H:;;";
  drawQr(data);

  Serial.printf("\nQR code generated in: %dus\n", micros() - start);
}




void loop(void) {
}