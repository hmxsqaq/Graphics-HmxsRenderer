#include "scene.h"
#include "log.h"
#include "renderer.h"

void Scene::Tick() {
    if (!CanRender()) {
        LOG_DEBUG("Scene - scene are not ready to render");
        return;
    }

    shader_->SetViewMatrix(camera_->GetViewMatrix());
    shader_->SetProjectionMatrix(camera_->GetProjectionMatrix());
    shader_->SetViewportMatrix(frame_buffer_->GetViewportMatrix());
    LOG_DEBUG("ViewMatrix: \n" + shader_->GetViewMatrix().ToString());
    LOG_DEBUG("ProjectionMatrix: \n" + shader_->GetProjectionMatrix().ToString());
    LOG_DEBUG("ViewportMatrix: \n" + shader_->GetViewportMatrix().ToString());
    for (const auto& mesh_obj : mesh_objs_) {
        if (mesh_obj->mesh == nullptr) {
            LOG_DEBUG("Scene - mesh object has no mesh");
            continue;
        }
        shader_->SetModelMatrix(mesh_obj->GetModelMatrix());
        LOG_DEBUG("ModelMatrix: \n" + shader_->GetModelMatrix().ToString());
        Renderer::DrawModel(mesh_obj->mesh->model(), shader_, frame_buffer_);
    }
}
