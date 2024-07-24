#include "scene.h"
#include "log.h"

void Scene::Tick() {
    if (!CanRender()) {
        LOG_DEBUG("Scene - scene are not ready to render");
        return;
    }

    shader_->SetViewMatrix(camera().GetViewMatrix());
    shader_->SetProjectionMatrix(camera().GetProjectionMatrix());
    for (const auto& mesh_obj : mesh_objs_) {
        if (mesh_obj->mesh == nullptr) {
            LOG_DEBUG("Scene - mesh object has no mesh");
            continue;
        }

        shader_->SetModelMatrix(mesh_obj->GetModelMatrix());
        shader_->SetMesh(mesh_obj->mesh);
        for (int i = 0; i < mesh_obj->mesh->model().faces_size(); i++) {

        }
    }
}
