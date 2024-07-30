#ifndef SCENE_H
#define SCENE_H

#include "component-gameobject.h"
#include "ishader.h"

class Scene {
public:
    explicit Scene(
        const std::shared_ptr<CameraObject> &camera = nullptr,
        const std::shared_ptr<IShader> &shader = nullptr,
        const std::shared_ptr<FrameBuffer> &frame_buffer = nullptr)
        : camera_(camera), shader_(shader), frame_buffer_(frame_buffer) { }

    void Render() const;

    void SetCamera(const std::shared_ptr<CameraObject> &camera) { camera_ = camera; }
    void SetShader(const std::shared_ptr<IShader> &shader) { shader_ = shader; }
    void SetFrameBuffer(const std::shared_ptr<FrameBuffer> &frame_buffer) { frame_buffer_ = frame_buffer; }
    void AddMeshObject(const std::shared_ptr<MeshObject> &gameobject) { mesh_objs_.push_back(gameobject); }
    void ClearMeshObjects() { mesh_objs_.clear(); }

    [[nodiscard]] std::shared_ptr<CameraObject> GetCamera() const { return camera_; }
    [[nodiscard]] std::shared_ptr<IShader> GetShader() const { return shader_; }
    [[nodiscard]] std::shared_ptr<FrameBuffer> GetFrameBuffer() const { return frame_buffer_; }
    [[nodiscard]] std::vector<std::shared_ptr<MeshObject>> GetMeshObjects() const { return mesh_objs_; }

private:
    std::shared_ptr<CameraObject> camera_;
    std::vector<std::shared_ptr<MeshObject>> mesh_objs_;
    std::shared_ptr<IShader> shader_;
    std::shared_ptr<FrameBuffer> frame_buffer_;

    [[nodiscard]] bool CanRender() const { return camera_ != nullptr && !mesh_objs_.empty() && shader_ != nullptr && frame_buffer_ != nullptr; }
};

#endif //SCENE_H
