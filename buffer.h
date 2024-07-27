#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cstdint>
#include <memory>
#include "color.h"
#include "maths/matrix.h"

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

    void FilpVertically();
    void FilpHorizontally();
    void Clear(uint8_t value = 0) const;

    [[nodiscard]] size_t width() const { return width_; }
    [[nodiscard]] size_t height() const { return height_; }
    [[nodiscard]] std::uint8_t bpp() const { return bpp_; }
    [[nodiscard]] size_t size() const { return width_ * height_ * bpp_; }
    [[nodiscard]] const std::uint8_t* data() const { return data_.get(); }
    [[nodiscard]]       std::uint8_t* data()       { return data_.get(); }

private:
    size_t width_;
    size_t height_;
    std::uint8_t bpp_;
    std::unique_ptr<std::uint8_t[]> data_;
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

    void Clear(float value = std::numeric_limits<float>::max()) const;

    [[nodiscard]] size_t width() const { return width_; }
    [[nodiscard]] size_t height() const { return height_; }
    [[nodiscard]] size_t size() const { return width_ * height_; }
    [[nodiscard]] const float* data() const { return data_.get(); }
    [[nodiscard]]       float* data()       { return data_.get(); }

private:
    size_t width_;
    size_t height_;
    std::unique_ptr<float[]> data_;
};

struct FrameBuffer {
    FrameBuffer(const size_t width, const size_t height, const uint8_t bpp = RGBA);;

    void Clear() const;

    [[nodiscard]] Matrix4x4 GetViewportMatrix() const;
    [[nodiscard]] static Matrix4x4 GetViewportMatrix(size_t x, size_t y, size_t w, size_t h);

    [[nodiscard]] size_t width() const { return color_buffer.width(); }
    [[nodiscard]] size_t height() const { return color_buffer.height(); }

    ColorBuffer color_buffer;
    DepthBuffer depth_buffer;
};

#endif //IMAGE_BUFFER_H
