// Web browsers don't really have a resource limitation
// so we enable the optimization code
#define QR_OPTIMIZE_SEGMENTS
#include "qr.h"
#include <string.h>
#include <vector>
#include <emscripten/bind.h>


extern "C" {
    // when running on a PC, we can use more sophisticated data structures without any problem
    class WasmQrCode {
        public:
        uint16_t version;
        uint8_t size;
        uint8_t stride;
        std::vector<uint8_t> bitmap;
    };

    WasmQrCode createQr(const std::string &data, uint8_t ecLevel) {
        QrCode *c = QrGenerator.create(data.c_str(), data.length(),QrMode::Unspecified, (QrEcc)ecLevel);

        WasmQrCode c2;
        c2.version = c->version;
        c2.size = c->size;
        c2.stride = c->bitmapStride;
        c2.bitmap = std::vector(c->bitmap, c->bitmap + c->bitmapSize);

        return c2;
    }
}


using namespace emscripten;
EMSCRIPTEN_BINDINGS(qr_module) {
    value_object<WasmQrCode>("WasmQrCode")
        .field("version", &WasmQrCode::version)
        .field("size", &WasmQrCode::size)
        .field("stride", &WasmQrCode::stride)
        .field("bitmap", &WasmQrCode::bitmap);

    function("createQr", &createQr, allow_raw_pointers());
    register_vector<uint8_t>("UInt8Array");
}
