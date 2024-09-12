#include "scene.h"
#include "utility/log.h"
#include "renderer.h"

void Scene::Render() const {
    if (!CanRender()) {
        LOG_ERROR("Scene - scene are not ready to render");
        return;
    }

    auto& shader = shader_list[current_shader_index];
    shader->view_matrix = camera_obj->GetViewMatrix();
    shader->projection_matrix = camera_obj->GetProjectionMatrix();
    shader->viewport_matrix = frame_buffer->GetViewportMatrix();
    shader->lights = lights;
    shader->NormalizeLights();
    for (const auto& mesh_obj : mesh_objs) {
        if (mesh_obj->mesh == nullptr) {
            LOG_ERROR("Scene - mesh object has no mesh");
            continue;
        }
        shader->model_matrix = mesh_obj->GetModelMatrix();
        shader->view_direction = camera_obj->GetViewDirection();
        shader->model = mesh_obj->mesh->model();
        Renderer::DrawModel(*mesh_obj->mesh->model(), *shader, *frame_buffer, *g_buffer, render_path);
    }
    if (render_path == DEFERRED) { shader->Deferred(*g_buffer, *frame_buffer); }
}

void Callbacks::OnKeyPressed(Win32Wnd *windows, const KeyCode keycode) {
    const auto scene = static_cast<Scene*>(windows->GetUserData().get());
    if (scene == nullptr) {
        LOG_WARNING("Callbacks - cannot get scene object from user data");
        return;
    }
    const auto& camera = scene->camera_obj;
    switch (keycode) {
        case A:
            camera->transform.position[0] += 0.1f;
            break;
        case D:
            camera->transform.position[0] -= 0.1f;
            break;
        case W:
            camera->transform.position[2] -= 0.1f;
            break;
        case S:
            camera->transform.position[2] += 0.1f;
            break;
        case Q:
            camera->transform.position[1] += 0.1f;
            break;
        case E:
            camera->transform.position[1] -= 0.1f;
            break;
        case SPACE:
            camera->transform.position = {0, 0.5, 5};
            camera->transform.rotation = {0, 0, 0};
            for (auto& mesh_obj: scene->mesh_objs) {
                mesh_obj->transform.position = {0, 0, 0};
                mesh_obj->transform.rotation = {0, 0, 0};
            }
            break;
        case ESC:
            windows->CloseWnd();
            break;
        case ENTER:
            scene->auto_rotate = !scene->auto_rotate;
            break;
        default: break;
    }
}

void Callbacks::OnMousePressed(const Win32Wnd *windows, const MouseCode mousecode) {
    const auto scene = static_cast<Scene*>(windows->GetUserData().get());
    if (scene == nullptr) {
        LOG_WARNING("Callbacks - cannot get scene object from user data");
        return;
    }
    switch (mousecode) {
        case L:
            scene->current_shader_index++;
            if (scene->current_shader_index >= scene->shader_list.size()) scene->current_shader_index = 0;
            break;
        case R:
            scene->current_shader_index--;
            if (scene->current_shader_index < 0) scene->current_shader_index = static_cast<int>(scene->shader_list.size()) - 1;
            break;
    }
}
