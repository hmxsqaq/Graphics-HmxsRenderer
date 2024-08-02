#include "buffer.h"
#include <algorithm>

// ColorBuffer
ColorBuffer::ColorBuffer()
    : width_(0), height_(0), bpp_(0), data_(nullptr) {
}

ColorBuffer::ColorBuffer(const size_t width, const size_t height, const uint8_t bpp)
    : width_(width), height_(height), bpp_(bpp), data_(nullptr) {
    assert(bpp == GRAYSCALE || bpp == RGB || bpp == RGBA);
    data_ = std::make_unique<std::uint8_t[]>(width * height * bpp);
    Clear();
}

std::uint8_t& ColorBuffer::operator[](const size_t index) {
    assert(index < width_ * height_ * bpp_);
    return data_[index];
}

std::uint8_t ColorBuffer::operator[](const size_t index) const {
    assert(index < width_ * height_ * bpp_);
    return data_[index];
}

void ColorBuffer::SetPixel(const size_t x, const size_t y, const Color &color) const {
    assert(x < width_ && y < height_ && data_ != nullptr);
    std::copy_n(color.bgra_array.begin(), bpp_, data_.get() + (x + y * width_) * bpp_);
}

Color ColorBuffer::GetPixel(const size_t x, const size_t y) const {
    assert(x < width_ && y < height_ && data_ != nullptr);
    Color ret = {0, 0, 0, 0};
    std::copy_n(data_.get() + (x + y * width_) * bpp_, bpp_, ret.bgra_array.begin());
    return ret;
}

Color ColorBuffer::GetPixel(Vector2f uv) const {
    assert(uv[0] >= 0 && uv[0] <= 1 && uv[1] >= 0 && uv[1] <= 1);
    const auto x = static_cast<size_t>(uv[0] * static_cast<float>(width_));
    const auto y = static_cast<size_t>(uv[1] * static_cast<float>(height_));
    return GetPixel(x, y);
}

void ColorBuffer::FilpVertically() {
    const size_t half = height_ >> 1;
    for (int x = 0; x < width_; ++x)
        for (int y = 0; y < half; ++y)
            for (int b = 0; b < bpp_; ++b)
                std::swap(data_[(x + y * width_) * bpp_ + b],
                          data_[(x + (height_ - 1 - y) * width_) * bpp_ + b]);
}

void ColorBuffer::FilpHorizontally() {
    const size_t half = width_ >> 1;
    for (int x = 0; x < half; ++x)
        for (int y = 0; y < height_; ++y)
            for (int b = 0; b < bpp_; ++b)
                std::swap(data_[(x + y * width_) * bpp_ + b],
                          data_[(width_ - 1 - x + y * width_) * bpp_ + b]);
}

void ColorBuffer::Clear(const uint8_t value) const {
    std::fill_n(data_.get(), width_ * height_ * bpp_, value);
}

// DepthBuffer
DepthBuffer::DepthBuffer()
    : width_(0), height_(0), data_(nullptr) {
}

DepthBuffer::DepthBuffer(const size_t width, const size_t height)
    : width_(width), height_(height), data_(nullptr) {
    data_ = std::make_unique<float[]>(width * height);
    Clear();
}

float& DepthBuffer::operator[](const size_t index) {
    assert(index < width_ * height_);
    return data_[index];
}

float DepthBuffer::operator[](const size_t index) const {
    assert(index < width_ * height_);
    return data_[index];
}

void DepthBuffer::SetDepth(const size_t x, const size_t y, const float depth) const {
    assert(x < width_ && y < height_ && data_ != nullptr);
    data_[x + y * width_] = depth;
}

float DepthBuffer::GetDepth(const size_t x, const size_t y) const {
    assert(x < width_ && y < height_ && data_ != nullptr);
    return data_[x + y * width_];
}

void DepthBuffer::Clear(const float value) const {
    std::fill_n(data_.get(), width_ * height_, value);
}

// FrameBuffer
FrameBuffer::FrameBuffer(const size_t width, const size_t height, const uint8_t bpp)
    : color_buffer(width, height, bpp), depth_buffer(width, height) {}

void FrameBuffer::Clear(const uint8_t default_color, const float default_depth) const {
    color_buffer.Clear(default_color);
    depth_buffer.Clear(default_depth);
}

Matrix4x4 FrameBuffer::GetViewportMatrix() const {
    return GetViewportMatrix(0, 0, width(), height());
}

Matrix4x4 FrameBuffer::GetViewportMatrix(const size_t x, const size_t y, const size_t w, const size_t h) {
    const auto x_f = static_cast<float>(x);
    const auto y_f = static_cast<float>(y);
    const auto w_f = static_cast<float>(w);
    const auto h_f = static_cast<float>(h);
    return {
        {w_f / 2.0f, 0, 0, x_f + w_f / 2.0f},
        {0, h_f / 2.0f, 0, y_f + h_f / 2.0f},
        {0, 0, 0.5f, 0.5f},
        {0, 0, 0, 1.0f}
    };
}
