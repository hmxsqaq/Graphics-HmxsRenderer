#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cstdint>
#include <memory>
#include "color.h"

enum ColorType {
    GRAYSCALE = 1,
    RGB = 3,
    RGBA = 4
};

class ColorBuffer {
public:
    ColorBuffer();
    ColorBuffer(int width, int height, uint8_t bpp);

    std::uint8_t& operator[](int index);
    std::uint8_t  operator[](int index) const;

    void Clear(uint8_t value) const;

    void SetPixel(int x, int y, const Color &color) const;
    [[nodiscard]] Color GetPixel(int x, int y) const;
    [[nodiscard]] int width()  const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] int bpp()    const { return bpp_; }
    [[nodiscard]] int size()   const { return width_ * height_ * bpp_; }
    [[nodiscard]] const std::uint8_t* buffer() const { return buffer_.get(); }
    [[nodiscard]]       std::uint8_t* buffer()       { return buffer_.get(); }

private:
    int width_;
    int height_;
    std::uint8_t bpp_;
    std::unique_ptr<std::uint8_t[]> buffer_;
};

#endif //IMAGE_BUFFER_H
