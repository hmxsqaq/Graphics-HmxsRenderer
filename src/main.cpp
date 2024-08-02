#include "utility/frame_timer.h"
#include "utility/log.h"
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

    const std::vector<std::string> model_name_list = {
        "/african_head/african_head.obj",
        "/african_head/african_head_eye_inner.obj",
    };

    const auto frame_buffer = std::make_shared<FrameBuffer>(kWidth, kHeigh, RGBA);

    const auto camera = std::make_shared<CameraObject>();
    camera->camera = Camera(60.0f, 1.0f, 0.1f, 1000.0f);
    camera->SetPosition({0, 0, 5});

    const auto shader = std::make_shared<PhongShader>();
    shader->AddLights({light1, light2});

    const auto scene = std::make_shared<Scene>(camera, shader, frame_buffer);

    for (const auto &model_name : model_name_list) {
        const auto mesh_obj = std::make_shared<MeshObject>();
        const std::string model_path = std::string(ASSETS_PATH) + model_name;
        mesh_obj->mesh = std::make_shared<Mesh>(model_path);
        scene->AddMeshObject(mesh_obj);
    }

    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.SetTextFont("SF Pro Display", 20);
    window.SetUserData(scene);
    window.RegisterKeyCallback(Callbacks::OnKeyPressed);

    FrameTimer frame_timer;

    window.OpenWnd(kWidth, kHeigh);
    while (window.is_running()) {
        scene->Render();
        window.PushBuffer(frame_buffer->color_buffer);
        window.PushText(std::to_string(frame_timer.fps()));
        window.UpdateWnd();
        frame_buffer->Clear();
        Win32Wnd::HandleMsg();
        frame_timer.Tick();
    }
    return 0;
}
