#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// nayuki qr-code-generator
#include "qrcodegen.hpp"
using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
// I would prefer to use fukuchi's libqrencode but its LGPL-2.1 License does not allow me to sublicense neroshop, which forces me to make my project open-source (LGPL) and requires me to include their code in my project
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
    qrcodegen::QrCode::Ecc ecc; // to-do: change this to a unique_ptr
};
}
