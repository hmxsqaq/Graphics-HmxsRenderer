#include "renderer.h"
#include <cmath>

void Renderer::DrawLine(const ColorBuffer &buffer, int x0, int y0, int x1, int y1, const Color &color) {
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    x0 = std::floor(x0);
    y0 = std::floor(y0);
    x1 = std::floor(x1);
    y1 = std::floor(y1);
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    int per_error = std::abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep)
            buffer.SetPixel(y, x, color);
        else
            buffer.SetPixel(x, y, color);
        error += per_error;
        if (error > dx) {
            y += (y1 > y0 ? 1 : -1);
            error -= 2 * dx;
        }
    }
}
