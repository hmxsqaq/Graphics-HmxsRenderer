#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cstdint>
#include <memory>

#include "color.h"

struct ColorBuffer {
    enum { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

    ColorBuffer();
    ColorBuffer(int width, int height, uint8_t bpp);
    ColorBuffer(ColorBuffer&& other) noexcept;

    std::uint8_t& operator[](int index);
    std::uint8_t  operator[](int index) const;
    ColorBuffer& operator=(ColorBuffer&& other) noexcept;

    void SetPixel(int x, int y, const Color &color) const;
    [[nodiscard]] Color GetPixel(int x, int y) const;

    [[nodiscard]] int Width()  const { return width_; }
    [[nodiscard]] int Height() const { return height_; }
    [[nodiscard]] int BPP()    const { return bpp_; }
    [[nodiscard]] int Size()   const { return width_ * height_ * bpp_; }
    [[nodiscard]] const std::uint8_t* BufferPtr() const { return buffer_.get(); }
    [[nodiscard]]       std::uint8_t* BufferPtr()       { return buffer_.get(); }
private:
    int width_;
    int height_;
    std::uint8_t bpp_;
    std::unique_ptr<std::uint8_t[]> buffer_;
};

#endif //IMAGE_BUFFER_H
