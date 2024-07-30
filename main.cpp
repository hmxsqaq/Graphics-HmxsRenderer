#include "buffer.h"
#include "win32_wnd.h"
#include "maths/maths.h"
#include <iostream>
#include "component-gameobject.h"
#include "log.h"
#include "model.h"
#include "scene.h"

constexpr int kWidth = 500;
constexpr int kHeigh = 500;
Light light1 = {
    .direction = {1, 1, 1},
    .intensity = {1, 1, 1}
};
Light light2 = {
    .direction = {-1, -1, -1},
    .intensity = {1, 1, 1}
};

int main() {
    Log::Instance().SetLogLevel(Log::Level::LOG_DEBUG);
    Log::Instance().SetLogFile("output.log", std::ios::out);

    const std::vector<std::string> model_list = {
        "../assets/african_head/african_head.obj",
        "../assets/african_head/african_head_eye_inner.obj",
    };

    const auto frame_buffer = std::make_shared<FrameBuffer>(kWidth, kHeigh, RGBA);

    const auto camera = std::make_shared<CameraObject>();
    camera->camera = Camera(60.0f, 1.0f, 0.1f, 1000.0f);
    camera->SetPosition({0, 0, 5});

    const auto shader = std::make_shared<TestShader>();
    shader->AddLights({light1, light2});

    Scene scene(camera, shader, frame_buffer);

    for (const auto &model_path : model_list) {
        const auto mesh_obj = std::make_shared<MeshObject>();
        mesh_obj->mesh = std::make_shared<Mesh>(model_path);
        scene.AddMeshObject(mesh_obj);
    }

    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.SetTextFont("SF Pro Display", 20);
    window.OpenWnd(kWidth, kHeigh);
    while (window.is_running()) {
        scene.Render();
        window.PushBuffer(frame_buffer->color_buffer);
        window.PushText("Hello!");
        window.UpdateWnd();
        frame_buffer->Clear();
        Win32Wnd::HandleMsg();
    }
    return 0;
}
