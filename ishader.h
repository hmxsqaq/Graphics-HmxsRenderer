#ifndef SHADER_H
#define SHADER_H

#include "color.h"
#include "component-gameobject.h"
#include "maths/maths.h"

struct VertexShaderInput {

};

struct VertexShaderOutput {

};

struct FragmentShaderInput {

};

struct IShader {
    virtual ~IShader() = default;

    void SetModelMatrix(const Matrix4x4& model_matrix) { model_matrix_ = model_matrix; }
    void SetViewMatrix(const Matrix4x4& view_matrix) { view_matrix_ = view_matrix; }
    void SetProjectionMatrix(const Matrix4x4& projection_matrix) { projection_matrix_ = projection_matrix; }

    virtual void Vertex(const VertexShaderInput& in, VertexShaderOutput& out) = 0;
    virtual bool Fragment(const FragmentShaderInput& in, Color& out) = 0;

private:
    Matrix4x4 model_matrix_;
    Matrix4x4 view_matrix_;
    Matrix4x4 projection_matrix_;
};

#endif //SHADER_H
