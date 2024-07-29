#include "scene.h"
#include "log.h"
#include "renderer.h"

void Scene::Render() const {
    if (!CanRender()) {
        LOG_DEBUG("Scene - scene are not ready to render");
        return;
    }

    shader_->SetViewMatrix(camera_->GetViewMatrix());
    shader_->SetProjectionMatrix(camera_->GetProjectionMatrix());
    shader_->SetViewportMatrix(frame_buffer_->GetViewportMatrix());

    LOG_DEBUG("\nViewMatrix:" + shader_->GetViewMatrix().ToString());
    LOG_DEBUG("\nProjectionMatrix:" + shader_->GetProjectionMatrix().ToString());
    LOG_DEBUG("\nViewportMatrix:" + shader_->GetViewportMatrix().ToString());

    for (const auto& mesh_obj : mesh_objs_) {
        if (mesh_obj->mesh == nullptr) {
            LOG_ERROR("Scene - mesh object has no mesh");
            continue;
        }
        shader_->SetModelMatrix(mesh_obj->GetModelMatrix());
        LOG_DEBUG("ModelMatrix: \n" + shader_->GetModelMatrix().ToString());
        Renderer::DrawModel(mesh_obj->mesh->model(), shader_, frame_buffer_);
    }
}
