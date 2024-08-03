#ifndef SCENE_H
#define SCENE_H

#include "platform/win32/win32_wnd.h"
#include "component-gameobject.h"
#include "ishader.h"

struct Scene {
    explicit Scene(
        const std::shared_ptr<CameraObject> &camera = nullptr,
        const std::shared_ptr<IShader> &shader = nullptr,
        const std::shared_ptr<FrameBuffer> &frame_buffer = nullptr)
        : camera(camera), shader(shader), frame_buffer(frame_buffer) { }

    void Render() const;

    void AddMeshObject(const std::shared_ptr<MeshObject> &gameobject) { mesh_objs.push_back(gameobject); }
    void ClearMeshObjects() { mesh_objs.clear(); }

    std::shared_ptr<CameraObject> camera;
    std::vector<std::shared_ptr<MeshObject>> mesh_objs;
    std::shared_ptr<IShader> shader;
    std::shared_ptr<FrameBuffer> frame_buffer;

    [[nodiscard]] bool CanRender() const { return camera != nullptr && !mesh_objs.empty() && shader != nullptr && frame_buffer != nullptr; }
};

struct Callbacks {
    static void OnKeyPressed(Win32Wnd* windows, KeyCode keycode);
    static void OnMousePressed(const Win32Wnd* windows, MouseCode mousecode);
};

#endif //SCENE_H
