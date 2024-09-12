#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

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
    [[nodiscard]] Color GetPixel(Vector2f uv) const;

    void FlipVertically();
    void FlipHorizontally();
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

    void Set(size_t x, size_t y, float depth) const;
    [[nodiscard]] float Get(size_t x, size_t y) const;

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

template <size_t N>
class VectorBuffer {
public:
    VectorBuffer(const size_t width, const size_t height) : width_(width), height_(height), data_(std::make_unique<float[]>(width * height * N)) {
        Clear();
    }

    void Set(const size_t x, const size_t y, const Vector<float, N> &vector) const {
        assert(x < width_ && y < height_ && data_ != nullptr);
        const int offset = (y * width_ + x) * N;
        std::copy_n(vector.data.begin(), N, data_.get() + offset);
    }

    void Set(const Vector2f &uv, const Vector<float, N> &vector) const {
        const auto x = static_cast<size_t>(uv[0] * static_cast<float>(width_));
        const auto y = static_cast<size_t>(uv[1] * static_cast<float>(height_));
        return Set(x, y, vector);
    }

    [[nodiscard]] Vector<float, N> Get(const size_t x, const size_t y) const {
        assert(x < width_ && y < height_ && data_ != nullptr);
        Vector<float, N> vector;
        const int offset = (y * width_ + x) * N;
        std::copy_n(data_.get() + offset, N, vector.data.begin());
        return vector;
    }

    void Clear(const float value = 0) const {
        std::fill_n(data_.get(), width_ * height_ * N, value);
    }

    [[nodiscard]] size_t width() const { return width_; }
    [[nodiscard]] size_t height() const { return height_; }
    [[nodiscard]] size_t size() const { return width_ * height_ * N; }
    [[nodiscard]] const float* data() const { return data_.get(); }
    [[nodiscard]]       float* data()       { return data_.get(); }
private:
    size_t width_;
    size_t height_;
    std::unique_ptr<float[]> data_;
};

struct FrameBuffer {
    FrameBuffer(size_t width, size_t height, uint8_t bpp = RGBA);

    void Clear(uint8_t default_color = 0, float default_depth = std::numeric_limits<float>::max()) const;

    [[nodiscard]] Matrix4x4 GetViewportMatrix() const;
    [[nodiscard]] static Matrix4x4 GetViewportMatrix(size_t x, size_t y, size_t w, size_t h);

    [[nodiscard]] size_t width() const { return color_buffer.width(); }
    [[nodiscard]] size_t height() const { return color_buffer.height(); }

    ColorBuffer color_buffer;
    DepthBuffer depth_buffer;
};

struct GBuffer {
    GBuffer(const size_t width, const size_t height) : normal(width, height) { };

    void Clear() const {
        normal.Clear();
    }

    VectorBuffer<3> normal;
};

#endif //IMAGE_BUFFER_H
