#include "qr.hpp"

neroshop::QR::QR() {}
////////////////////
neroshop::QR::QR(std::string filename, int image_size, int min_module_pixel_size, std::string text,
            bool overwrite, qrcodegen::QrCode::Ecc ecc) {
    this->filename = filename;
    this->size = image_size;
    this->min_module_pixel_size = min_module_pixel_size;
    this->text = text;
    this->overwrite_existing_file = overwrite;
    this->ecc = ecc;
}
////////////////////
neroshop::QR::~QR() {}
////////////////////    
////////////////////    
bool neroshop::QR::to_png() const {
    if(text.empty()) return false; // text is required
    std::ifstream file(filename.c_str());
    bool exists = file.good();
    file.close();
    if(!overwrite_existing_file && exists) return false; // if overwrite is off
    
    auto qr_code = qrcodegen::QrCode::encodeText(text.c_str(), ecc);
    auto result = write_to_png(qr_code);
    
    return result;
}
////////////////////
bool neroshop::QR::write_to_png(const qrcodegen::QrCode& qr_data) const {
    std::ofstream out(filename.c_str(), std::ios::binary);
    int pngWH = image_size_with_border(qr_data);
    //TinyPngOut pngout(pngWH, pngWH, out);

    auto qrSize = qr_data.getSize();
    auto qrSizeWithBorder = qr_data.getSize() + 2;
    if (qrSizeWithBorder > size)
        return false; // qrcode doesn't fit

    int qrSizeFitsInMaxImgSizeTimes = size / qrSizeWithBorder;
    int pixelsWHPerModule = qrSizeFitsInMaxImgSizeTimes;

    if (qrSizeFitsInMaxImgSizeTimes < min_module_pixel_size)
        return false; // image would be to small to scan

    std::vector<uint8_t> tmpData;
    const uint8_t blackPixel = 0x00;
    const uint8_t whitePixel = 0xFF;

    /* The below loop converts the qrData to RGB8.8.8 pixels and writes it with
     * the tinyPNGoutput library. since we probably have requested a larger
     * qr module pixel size we must transform the qrData modules to be larger
     * pixels (than just 1x1). */

    // border above
    for (int i = 0; i < pngWH; i++) // row
        for (int j = 0; j < pixelsWHPerModule; j++) // module pixel (height)
            tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

    //pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
    tmpData.clear();

    for (int qrModuleAtY = 0; qrModuleAtY < qrSize; qrModuleAtY++) {
        for (int col = 0; col < pixelsWHPerModule; col++) {
            // border left
            for (int i = 0; i < qrSizeFitsInMaxImgSizeTimes; ++i)
                tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

            // qr module to pixel
            for (int qrModuleAtX = 0; qrModuleAtX < (qrSize); qrModuleAtX++) {
            for (int row = 0; row < qrSizeFitsInMaxImgSizeTimes; ++row) {
                    if (qr_data.getModule(qrModuleAtX, qrModuleAtY)) {
                        // insert saves us a for loop or 3 times the same line.
                        tmpData.insert(tmpData.end(), {blackPixel, blackPixel, blackPixel});
                    } else {
                        tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});
                    }
                }
            }
            // border right
            for (int i = 0; i < qrSizeFitsInMaxImgSizeTimes; ++i)
                tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

            // write the entire  row
            //pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
            tmpData.clear();
        }
    }

    // border below
    for (int i = 0; i < pngWH; i++) // row
        for (int j = 0; j < pixelsWHPerModule; j++) // module pixel (height)
            tmpData.insert(tmpData.end(), {whitePixel, whitePixel, whitePixel});

    //pngout.write(tmpData.data(), static_cast<size_t>(tmpData.size() / 3));
    tmpData.clear();

    std::ifstream file(filename.c_str());
    bool exists = file.good();
    return exists;
}
////////////////////
unsigned int neroshop::QR::image_size(const qrcodegen::QrCode& qr_data) const {
    return (size / qr_data.getSize()) * qr_data.getSize();
}
////////////////////
unsigned int neroshop::QR::image_size_with_border(const qrcodegen::QrCode& qr_data) const {
    return (size / (qr_data.getSize() + 2));
}
////////////////////
////////////////////    
