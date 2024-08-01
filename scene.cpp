#include "scene.h"
#include "utility/log.h"
#include "renderer.h"

void Scene::Render() const {
    if (!CanRender()) {
        LOG_ERROR("Scene - scene are not ready to render");
        return;
    }

    shader_->SetViewMatrix(camera_->GetViewMatrix());
    shader_->SetProjectionMatrix(camera_->GetProjectionMatrix());
    shader_->SetViewportMatrix(frame_buffer_->GetViewportMatrix());
    for (const auto& mesh_obj : mesh_objs_) {
        if (mesh_obj->mesh == nullptr) {
            LOG_ERROR("Scene - mesh object has no mesh");
            continue;
        }
        shader_->SetModelMatrix(mesh_obj->GetModelMatrix());
        shader_->NormalizeLights();
        Renderer::DrawModel(mesh_obj->mesh->model(), shader_, frame_buffer_);
    }
}
