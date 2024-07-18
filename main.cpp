#include "core/buffer.h"
#include "core/renderer.h"
#include "display/win32_wnd.h"
#include "maths/maths.h"
#include <iostream>

constexpr int kWidth = 500;
constexpr int kHeigh = 500;

int main() {
    ColorBuffer frame_buffer(kWidth, kHeigh, GRAYSCALE);
    Vector2f p0{0, 0}, p1{kWidth - 1, kHeigh - 1};
    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.OpenWnd(kWidth, kHeigh);
    while (window.is_running()) {
        Renderer::DrawLine(frame_buffer, p0, p1, {255, 0, 0, 255});
        window.PushBuffer(frame_buffer);
        window.PushText("Hello, Hmxs!");
        window.UpdateWnd();
        frame_buffer.Clear(0);
        Win32Wnd::HandleMsg();
    }
    return 0;
}
