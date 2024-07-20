#include "buffer.h"
#include <algorithm>

ColorBuffer::ColorBuffer()
    : width_(0), height_(0), bpp_(0), buffer_(nullptr) {
}

ColorBuffer::ColorBuffer(const size_t width, const size_t height, const uint8_t bpp)
    : width_(width), height_(height), bpp_(bpp), buffer_(nullptr) {
    assert(bpp == GRAYSCALE || bpp == RGB || bpp == RGBA);
    buffer_ = std::make_unique<std::uint8_t[]>(width * height * bpp);
}

std::uint8_t& ColorBuffer::operator[](const size_t index) {
    assert(index < width_ * height_ * bpp_);
    return buffer_[index];
}

std::uint8_t ColorBuffer::operator[](const size_t index) const {
    assert(index < width_ * height_ * bpp_);
    return buffer_[index];
}

void ColorBuffer::SetPixel(const size_t x, const size_t y, const Color &color) const {
    assert(x < width_ && y < height_ && buffer_ != nullptr);
    std::copy_n(color.bgra.begin(), bpp_, buffer_.get() + (x + y * width_) * bpp_);
}

Color ColorBuffer::GetPixel(const size_t x, const size_t y) const {
    assert(x < width_ && y < height_ && buffer_ != nullptr);
    Color ret = {0, 0, 0, 0};
    std::copy_n(buffer_.get() + (x + y * width_) * bpp_, bpp_, ret.bgra.begin());
    return ret;
}

void ColorBuffer::Clear(const uint8_t value) const {
    std::fill_n(buffer_.get(), width_ * height_ * bpp_, value);
}

DepthBuffer::DepthBuffer()
    : width_(0), height_(0), buffer_(nullptr) {
}

DepthBuffer::DepthBuffer(const size_t width, const size_t height)
    : width_(width), height_(height), buffer_(nullptr) {
    buffer_ = std::make_unique<float[]>(width * height);
}

float& DepthBuffer::operator[](const size_t index) {
    assert(index < width_ * height_);
    return buffer_[index];
}

float DepthBuffer::operator[](const size_t index) const {
    assert(index < width_ * height_);
    return buffer_[index];
}

void DepthBuffer::SetDepth(const size_t x, const size_t y, const float depth) const {
    assert(x < width_ && y < height_ && buffer_ != nullptr);
    buffer_[x + y * width_] = depth;
}

float DepthBuffer::GetDepth(const size_t x, const size_t y) const {
    assert(x < width_ && y < height_ && buffer_ != nullptr);
    return buffer_[x + y * width_];
}

void DepthBuffer::Clear(const float value) const {
    std::fill_n(buffer_.get(), width_ * height_, value);
}
