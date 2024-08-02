#include "ishader.h"

void IShader::NormalizeLights() {
    for (auto& [direction, intensity] : lights) {
        direction = (view_matrix * model_matrix * direction.Embed<4>(0)).Project<3>().Normalize();
        intensity = (view_matrix * model_matrix * intensity.Embed<4>(0)).Project<3>();
    }
}

void StandardVertexShader::Vertex(const VertexShaderInput &in, Triangle &out) {
    out.uv = in.uv;
    out.normal = ((view_matrix * model_matrix).InverseTranspose() * in.normal.Embed<4>(0)).Project<3>();
    out.vertex_view_space = (view_matrix * model_matrix * in.vertex_model_space.Embed<4>(1)).Project<3>();
    out.vertex_clip_space = projection_matrix * out.vertex_view_space.Embed<4>(1);
    out.vertex_screen_space = (viewport_matrix * out.vertex_clip_space / out.vertex_clip_space[3]).Project<2>();
}

bool GrayShader::Fragment(const FragmentShaderInput &in, Color &out) {
    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        lightness += std::max(0.0f, in.interpolated_normal * direction);
    }
    out = Color{255, 255, 255, 255} * lightness;
    return true;
}

bool PhongShader::Fragment(const FragmentShaderInput &in, Color &out) {
    if (model == nullptr) return false;
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(in.interpolated_uv) : Color::White();
    const Color specular_color = model->specular_map() != nullptr ? model->specular_map()->GetPixel(in.interpolated_uv) : Color::White();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        const float diffuse = std::max(0.0f, in.interpolated_normal * direction);
        Vector3f reflection = (in.interpolated_normal * (in.interpolated_normal * direction) * 2 - direction).Normalize();
        const float specular = static_cast<float>(std::pow(std::max(0.0f, reflection * view_direction), specular_color[0] + 5));
        lightness += diffuse + specular;
    }
    lightness += ambient_light;
    out = texture_color * lightness;
    return true;
}
