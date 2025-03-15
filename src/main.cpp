
#include <stdio.h>
#include <string.h>

#define QR_DEBUG

#include "qr.h"


int main( int argc, char **argv) {
    QrCode *code = QrGenerator.create("https://www.qrcode.com/", 23, QrMode::Unspecified, QrEcc::M );


}