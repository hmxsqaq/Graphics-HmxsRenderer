#include "core/color_buffer.h"
#include "core/renderer.h"
#include "display/win32_wnd.h"
#include "maths/maths.h"
#include <iostream>

constexpr int WIDTH = 500;
constexpr int HEIGHT = 500;

int main() {
    ColorBuffer frame_buffer(WIDTH, HEIGHT, ColorType::GRAYSCALE);
    Vector2I p0{0, 0}, p1{WIDTH - 1, HEIGHT - 1};
    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.openWnd(WIDTH, HEIGHT);
    while (window.isRunning()) {
        Renderer::DrawLine(frame_buffer, p0[0], p0[1], p1[0], p1[1], {255, 0, 0, 255});
        window.drawBuffer(frame_buffer);
        window.drawText("Hello, Hmxs!");
        window.updateWnd();
        frame_buffer.clear(0);
        Win32Wnd::HandleMsg();
    }
    return 0;
}
