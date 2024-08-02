#ifndef ISHADER_H
#define ISHADER_H

#include <component-gameobject.h>
#include <memory>
#include "color.h"
#include "maths/maths.h"
#include <vector>

struct VertexShaderInput {
    Vector3f vertex_model_space;
    Vector3f normal;
    Vector2f uv;
};

struct Triangle {
    Vector4f vertex_clip_space;
    Vector3f vertex_view_space;
    Vector2f vertex_screen_space;
    Vector3f normal;
    Vector2f uv;
};

struct FragmentShaderInput {
    Vector3f interpolated_vertex_view_space;
    Vector3f interpolated_normal;
    Vector2f interpolated_uv;
};

struct Light {
    Vector3f direction;
    Vector3f intensity;
};

struct IShader {
    virtual ~IShader() = default;

    void AddLight(const Light &light) { lights.push_back(light); }
    void AddLights(const std::vector<Light>& new_lights) { lights.insert(lights.end(), new_lights.begin(), new_lights.end()); }
    void ClearLights() { lights.clear(); }
    void NormalizeLights();

    virtual void Vertex(const VertexShaderInput& in, Triangle& out) = 0;
    virtual bool Fragment(const FragmentShaderInput& in, Color& out) = 0;

    Matrix4x4 model_matrix;
    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
    Matrix4x4 viewport_matrix;
    std::vector<Light> lights{};
    std::shared_ptr<Model> model = nullptr;
    Vector3f view_direction;
    float ambient_light = 0.1f;
};

struct StandardVertexShader : IShader {
    void Vertex(const VertexShaderInput& in, Triangle& out) override;
};

struct GrayShader final : StandardVertexShader {
    bool Fragment(const FragmentShaderInput& in, Color& out) override;
};

struct PhongShader final : StandardVertexShader {
    bool Fragment(const FragmentShaderInput& in, Color& out) override;
};

#endif //ISHADER_H
