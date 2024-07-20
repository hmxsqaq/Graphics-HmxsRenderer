#include "renderer.h"
#include <cmath>

void Renderer::DrawLine(const ColorBuffer &buffer, Vector2f p0, Vector2f p1, const Color &color) {
    bool steep = false;
    if (std::abs(p0[0] - p1[0]) < std::abs(p0[1] - p1[1])) {
        std::swap(p0[0], p0[1]);
        std::swap(p1[0], p1[0]);
        steep = true;
    }
    if (p0[0] > p1[0]) {
        std::swap(p0, p1);
    }
    const int x0 = std::floor(p0[0]), y0 = std::floor(p0[1]);
    const int x1 = std::floor(p1[0]), y1 = std::floor(p1[1]);
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    int per_error = std::abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep)  buffer.SetPixel(y, x, color);
        else        buffer.SetPixel(x, y, color);
        error += per_error;
        if (error > dx) {
            y += (y1 > y0 ? 1 : -1);
            error -= 2 * dx;
        }
    }
}
