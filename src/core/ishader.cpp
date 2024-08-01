#include "ishader.h"

void IShader::NormalizeLights() {
    for (auto& [direction, intensity] : lights_) {
        direction = (view_matrix_ * model_matrix_ * direction.Embed<4>(0)).Project<3>().Normalize();
        intensity = (view_matrix_ * model_matrix_ * intensity.Embed<4>(0)).Project<3>();
    }
}

void TestShader::Vertex(const VertexShaderInput &in, Triangle &out) {
    out.uv = in.uv;
    out.normal = ((view_matrix_ * model_matrix_).InverseTranspose() * in.normal.Embed<4>(0)).Project<3>();
    out.vertex_view_space = (view_matrix_ * model_matrix_ * in.vertex_model_space.Embed<4>(1)).Project<3>();
    out.vertex_clip_space = projection_matrix_ * out.vertex_view_space.Embed<4>(1);
    out.vertex_screen_space = (viewport_matrix_ * out.vertex_clip_space / out.vertex_clip_space[3]).Project<2>();
}

bool TestShader::Fragment(const FragmentShaderInput &in, Color &out) {
    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights_) {
        lightness += std::max(0.0f, in.interpolated_normal * direction);
    }
    out = Color{255, 255, 255, 255} * lightness;
    return true;
}
