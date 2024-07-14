#include "core/color_buffer.h"
#include "core/renderer.h"
#include "display/win32_wnd.h"

constexpr int WIDTH = 500;
constexpr int HEIGHT = 500;

int main() {
    ColorBuffer frame_buffer(WIDTH, HEIGHT, ColorBuffer::GRAYSCALE);
    Renderer::DrawLine(frame_buffer, 0, 0, WIDTH - 1, HEIGHT - 1, {255, 0, 0, 255});
    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.Open(WIDTH, HEIGHT);
    while (window.IsRunning()) {
        window.Draw(frame_buffer);
        Win32Wnd::HandleMsg();
    }
    return 0;
}
