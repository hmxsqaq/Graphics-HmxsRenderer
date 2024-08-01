#ifndef SHADER_H
#define SHADER_H

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

    void SetModelMatrix(const Matrix4x4& model_matrix) { model_matrix_ = model_matrix; }
    void SetViewMatrix(const Matrix4x4& view_matrix) { view_matrix_ = view_matrix; }
    void SetProjectionMatrix(const Matrix4x4& projection_matrix) { projection_matrix_ = projection_matrix; }
    void SetViewportMatrix(const Matrix4x4& viewport_matrix) { viewport_matrix_ = viewport_matrix; }
    [[nodiscard]] Matrix4x4 model_matrix() const { return model_matrix_; }
    [[nodiscard]] Matrix4x4 view_matrix() const { return view_matrix_; }
    [[nodiscard]] Matrix4x4 projection_matrix() const { return projection_matrix_; }
    [[nodiscard]] Matrix4x4 viewport_matrix() const { return viewport_matrix_; }

    void AddLight(const Light &light) { lights_.push_back(light); }
    void AddLights(const std::vector<Light>& lights) { lights_.insert(lights_.end(), lights.begin(), lights.end()); }
    void ClearLights() { lights_.clear(); }
    void NormalizeLights();

    virtual void Vertex(const VertexShaderInput& in, Triangle& out) = 0;
    virtual bool Fragment(const FragmentShaderInput& in, Color& out) = 0;

protected:
    Matrix4x4 model_matrix_;
    Matrix4x4 view_matrix_;
    Matrix4x4 projection_matrix_;
    Matrix4x4 viewport_matrix_;
    std::vector<Light> lights_{};
};

struct TestShader final : IShader {
    void Vertex(const VertexShaderInput& in, Triangle& out) override;
    bool Fragment(const FragmentShaderInput& in, Color& out) override;
};

#endif //SHADER_H
