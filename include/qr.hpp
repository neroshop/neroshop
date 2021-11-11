// filename: .hpp
//#pragma once // use #ifndef _HPP, #define _HPP, and #endif instead for portability

#ifndef QR_HPP_NEROSHOP // recommended to add unique identifier like _NEROSHOP to avoid naming collision with other libraries
#define QR_HPP_NEROSHOP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// nayuki qr-code-generator
#include <qrcodegen.hpp>
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
// nayuki tinypngout-c (libpng is statically built into neroshop so TinyPngOut will be used for now until I find help from other programmers)
//#include <TinyPngOut.hpp>
// dokun-ui
//#include <texture.hpp>

namespace neroshop {
class QR {
public:
    QR();
    QR(std::string fileName, int imgSize, int minModulePixelSize, std::string text,
            bool overwriteExistingFile, qrcodegen::QrCode::Ecc ecc);
    ~QR();
    bool to_png() const;
    bool write_to_png(const qrcodegen::QrCode& qr_data) const;
    unsigned int image_size(const qrcodegen::QrCode& qr_data) const;
    unsigned int image_size_with_border(const qrcodegen::QrCode& qr_data) const;
private:
    std::string filename;
    int size;
    int min_module_pixel_size;
    std::string text;
    bool overwrite_existing_file;
    qrcodegen::QrCode::Ecc ecc;
};
}
#endif
