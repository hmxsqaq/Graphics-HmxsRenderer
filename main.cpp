#include "buffer.h"
#include "renderer.h"
#include "win32_wnd.h"
#include "maths/maths.h"
#include <iostream>

#include "component-gameobject.h"
#include "log.h"
#include "model.h"

constexpr int kWidth = 500;
constexpr int kHeigh = 500;

int main() {
    Log::Instance().SetLogLevel(Log::Level::LOG_DEBUG);
    Log::Instance().SetLogFile("output.log", std::ios::out);
    const auto model = std::make_shared<Model>("african_head.obj");
    MeshObject mesh_object(model);
    // ColorBuffer frame_buffer(kWidth, kHeigh, GRAYSCALE);
    // Vector2f p0{0, 0}, p1{kWidth - 1, kHeigh - 1};
    // Win32Wnd window("Hmxs", "HmxsRenderer");
    // window.OpenWnd(kWidth, kHeigh);
    // while (window.is_running()) {
    //     Renderer::DrawLine(p0, p1, {255, 0, 0, 255}, frame_buffer);
    //     window.PushBuffer(frame_buffer);
    //     window.PushText("Hello, Hmxs!");
    //     window.UpdateWnd();
    //     frame_buffer.Clear(0);
    //     Win32Wnd::HandleMsg();
    // }
    return 0;
}
