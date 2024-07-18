#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cstdint>
#include <memory>
#include "color.h"

/**
 * @brief enum of bytes per pixel.
 */
enum ColorType {
    GRAYSCALE = 1,
    RGB = 3,
    RGBA = 4
};

/**
 * @brief buffer of a color image
 */
class ColorBuffer {
public:
    ColorBuffer();
    ColorBuffer(size_t width, size_t height, uint8_t bpp = RGBA);

    std::uint8_t& operator[](size_t index);
    std::uint8_t  operator[](size_t index) const;

    void SetPixel(size_t x, size_t y, const Color &color) const;
    [[nodiscard]] Color GetPixel(size_t x, size_t y) const;

    void Clear(uint8_t value = 0) const;

    [[nodiscard]] size_t width() const { return width_; }
    [[nodiscard]] size_t height() const { return height_; }
    [[nodiscard]] std::uint8_t bpp() const { return bpp_; }
    [[nodiscard]] size_t size() const { return width_ * height_ * bpp_; }
    [[nodiscard]] const std::uint8_t* buffer() const { return buffer_.get(); }
    [[nodiscard]]       std::uint8_t* buffer()       { return buffer_.get(); }

private:
    size_t width_;
    size_t height_;
    std::uint8_t bpp_;
    std::unique_ptr<std::uint8_t[]> buffer_;
};

/**
 * @brief buffer for depth test
 */
class DepthBuffer {
public:
    DepthBuffer();
    DepthBuffer(size_t width, size_t height);

    float& operator[](size_t index);
    float  operator[](size_t index) const;

    void SetDepth(size_t x, size_t y, float depth) const;
    [[nodiscard]] float GetDepth(size_t x, size_t y) const;

    void Clear(float value = 1.0f) const;

    [[nodiscard]] size_t width() const { return width_; }
    [[nodiscard]] size_t height() const { return height_; }
    [[nodiscard]] size_t size() const { return width_ * height_; }
    [[nodiscard]] const float* buffer() const { return buffer_.get(); }
    [[nodiscard]]       float* buffer()       { return buffer_.get(); }

private:
    size_t width_;
    size_t height_;
    std::unique_ptr<float[]> buffer_;
};

#endif //IMAGE_BUFFER_H
