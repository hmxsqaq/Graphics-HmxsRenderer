#ifndef SCENE_H
#define SCENE_H

#include "platform/win32/win32_wnd.h"
#include "component-gameobject.h"
#include "ishader.h"

enum RenderPath {
    FORWARD = 0,
    DEFERRED = 1
};

struct Scene {
    explicit Scene(
        const std::shared_ptr<CameraObject> &camera = nullptr,
        const std::shared_ptr<FrameBuffer> &frame_buffer = nullptr)
        : camera_obj(camera), frame_buffer(frame_buffer) { }

    std::shared_ptr<CameraObject> camera_obj;
    std::shared_ptr<FrameBuffer> frame_buffer;
    std::vector<std::shared_ptr<MeshObject>> mesh_objs{};
    std::vector<std::shared_ptr<IShader>> shader_list{};
    std::vector<Light> lights{};
    int current_shader_index = 0;
    bool auto_rotate = true;
    RenderPath render_path = FORWARD;
    std::shared_ptr<GBuffer> g_buffer;

    void Render() const;

    [[nodiscard]] bool CanRender() const { return camera_obj != nullptr && frame_buffer != nullptr && !mesh_objs.empty() && shader_list[current_shader_index] != nullptr; }
};

struct Callbacks {
    static void OnKeyPressed(Win32Wnd* windows, KeyCode keycode);
    static void OnMousePressed(const Win32Wnd* windows, MouseCode mousecode);
};

#endif //SCENE_H
