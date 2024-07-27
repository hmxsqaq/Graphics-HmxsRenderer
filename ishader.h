#ifndef SHADER_H
#define SHADER_H

#include "color.h"
#include "maths/maths.h"

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

};

struct IShader {
    virtual ~IShader() = default;

    void SetModelMatrix(const Matrix4x4& model_matrix) { model_matrix_ = model_matrix; }
    void SetViewMatrix(const Matrix4x4& view_matrix) { view_matrix_ = view_matrix; }
    void SetProjectionMatrix(const Matrix4x4& projection_matrix) { projection_matrix_ = projection_matrix; }
    void SetViewportMatrix(const Matrix4x4& viewport_matrix) { viewport_matrix_ = viewport_matrix; }
    [[nodiscard]] Matrix4x4 GetModelMatrix() const { return model_matrix_; }
    [[nodiscard]] Matrix4x4 GetViewMatrix() const { return view_matrix_; }
    [[nodiscard]] Matrix4x4 GetProjectionMatrix() const { return projection_matrix_; }
    [[nodiscard]] Matrix4x4 GetViewportMatrix() const { return viewport_matrix_; }

    virtual void Vertex(const VertexShaderInput& in, Triangle& out) = 0;
    virtual bool Fragment(const FragmentShaderInput& in, Color& out) = 0;

private:
    Matrix4x4 model_matrix_;
    Matrix4x4 view_matrix_;
    Matrix4x4 projection_matrix_;
    Matrix4x4 viewport_matrix_;
};

#endif //SHADER_H
