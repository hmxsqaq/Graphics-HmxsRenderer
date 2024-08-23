#include "utility/frame_timer.h"
#include "utility/log.h"
#include "scene.h"

constexpr int kWidth = 1024;
constexpr int kHeigh = 1024;

Light light1 = {
    .direction = {1, 1, 1},
    .intensity = {1, 1, 1}
};
Light light2 = {
    .direction = {-1, -1, -1},
    .intensity = {1, 1, 1}
};

std::string GetUiText(const Scene &scene, const FrameTimer &timer) {
    std::ostringstream oss;
    oss << "INFO\n";
    oss << "Fps:     " << static_cast<int>(timer.fps()) << "\n";
    oss << "Camera:  " << scene.camera_obj->transform.position << "\n";
    oss << "Models:  ";
    for (const auto &mesh_obj : scene.mesh_objs)
        oss << "\"" << mesh_obj->name << "\"  ";
    oss << "\n";
    oss << "Shader:  " << scene.shader_list[scene.current_shader_index]->name << "  ";
    for (const auto &shader : scene.shader_list) {
        oss << "[" << shader->name << "] ";
    }
    oss << "\n";
    oss << "Lights:  ";
    for (const auto &[direction, intensity] : scene.lights)
        oss << direction << "  ";
    oss << "\n";
    oss << "Rotate:  " << (scene.auto_rotate ? "On" : "Off") << "\n";
    oss << "\n";
    oss << "OPERATION\n";
    oss << "W A S D Q E - Move camera\n";
    oss << "SPACE       - Reset models\n";
    oss << "Left Click  - Turn on/off rotation\n";
    oss << "Right Click - Switch Shader";
    return oss.str();
}

int main() {
    Log::Instance().SetLogLevel(Log::Level::LOG_DEBUG);
    Log::Instance().SetLogFile("output.log", std::ios::out);

    const std::vector<std::string> model_name_list = {
        "/african_head/african_head.obj",
        "/african_head/african_head_eye_inner.obj",
    };

    const auto frame_buffer = std::make_shared<FrameBuffer>(kWidth, kHeigh, RGBA);

    const auto camera_obj = std::make_shared<CameraObject>();
    camera_obj->camera = Camera(60.0f, 1.0f, 0.1f, 1000.0f);
    camera_obj->SetPosition({0, 0, 5});

    const auto gray_shader = std::make_shared<GrayShader>();
    const auto phong_shader = std::make_shared<PhongShader>();

    const auto scene = std::make_shared<Scene>();
    scene->camera_obj = camera_obj;
    scene->frame_buffer = frame_buffer;
    scene->shader_list.push_back(phong_shader);
    scene->shader_list.push_back(gray_shader);
    scene->lights.push_back(light1);
    scene->lights.push_back(light2);
    for (const auto &model_name : model_name_list) {
        const size_t last_slash = model_name.find_last_of('/');
        const size_t last_dot = model_name.find_last_of('.');
        auto mesh_obj = std::make_shared<MeshObject>(model_name.substr(last_slash + 1, last_dot - last_slash - 1));
        const std::string model_path = std::string(ASSETS_PATH) + model_name;
        mesh_obj->mesh = std::make_shared<Mesh>(model_path);
        scene->mesh_objs.push_back(mesh_obj);
    }

    Win32Wnd window("Hmxs", "HmxsRenderer");
    window.SetTextFont("mononoki", 20);
    window.SetUserData(scene);
    window.RegisterKeyCallback(Callbacks::OnKeyPressed);
    window.RegisterMouseCallback(Callbacks::OnMousePressed);

    FrameTimer frame_timer;

    window.OpenWnd(kWidth, kHeigh);
    while (window.is_running()) {
        scene->Render();
        window.PushBuffer(frame_buffer->color_buffer);
        window.PushText(GetUiText(*scene, frame_timer));
        window.UpdateWnd();
        frame_buffer->Clear();
        Win32Wnd::HandleMsg();
        frame_timer.Tick();

        if (scene->auto_rotate)
        {
            for (const auto& mesh_obj: scene->mesh_objs) {
                mesh_obj->transform.rotation[1] += static_cast<float>(5.0 * frame_timer.delta_time());
            }
        }
    }
    return 0;
}
