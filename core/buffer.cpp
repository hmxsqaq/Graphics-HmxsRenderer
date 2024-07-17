#include "buffer.h"
#include <algorithm>

ColorBuffer::ColorBuffer()
    : width_(0), height_(0), bpp_(0), buffer_(nullptr) {
}

ColorBuffer::ColorBuffer(const int width, const int height, const uint8_t bpp)
    : width_(width), height_(height), bpp_(bpp), buffer_(nullptr) {
    assert(bpp == GRAYSCALE || bpp == RGB || bpp == RGBA);
    buffer_ = std::make_unique<std::uint8_t[]>(width * height * bpp);
}

std::uint8_t& ColorBuffer::operator[](const int index) {
    assert(index >= 0 && index < width_ * height_ * bpp_);
    return buffer_[index];
}

std::uint8_t ColorBuffer::operator[](const int index) const {
    assert(index >= 0 && index < width_ * height_ * bpp_);
    return buffer_[index];
}

void ColorBuffer::Clear(const uint8_t value) const {
    std::fill_n(buffer_.get(), width_ * height_ * bpp_, value);
}

void ColorBuffer::SetPixel(const int x, const int y, const Color &color) const {
    assert(x >= 0 && x < width_ && y >= 0 && y < height_);
    std::copy_n(color.bgra.begin(), bpp_, buffer_.get() + (x + y * width_) * bpp_);
}

Color ColorBuffer::GetPixel(const int x, const int y) const {
    assert(x >= 0 && x < width_ && y >= 0 && y < height_);
    Color ret = {0, 0, 0, 0};
    std::copy_n(buffer_.get() + (x + y * width_) * bpp_, bpp_, ret.bgra.begin());
    return ret;
}
