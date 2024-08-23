#include "ishader.h"

#include <utility/log.h>

void IShader::NormalizeLights() {
    for (auto& [direction, intensity] : lights) {
        direction = direction.Normalize();
        intensity = intensity.Normalize();
    }
}

void StandardVertexShader::VertexShader(const VertexShaderInput &in, Vertex &out) const {
    out.uv = in.uv;
    out.normal = ((view_matrix * model_matrix).InverseTranspose() * in.normal.Embed<4>(0)).Project<3>();
    out.vertex_model_space = in.vertex_model_space;
    out.vertex_view_space = (view_matrix * model_matrix * in.vertex_model_space.Embed<4>(1)).Project<3>();
    out.vertex_clip_space = projection_matrix * out.vertex_view_space.Embed<4>(1);
    out.vertex_ndc_space = out.vertex_clip_space / out.vertex_clip_space[3];
    out.vertex_screen_space = (viewport_matrix * out.vertex_clip_space / out.vertex_clip_space[3]).Project<2>();
}

bool FixedShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip).Normalize();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        lightness += std::max(0.0f, interpolated_normal * direction);
    }
    if (lightness > 0.85)       out = Color{255, 255, 255, 255} * 1;
    else if (lightness > 0.6)   out = Color{255, 255, 255, 255} * 0.8;
    else if (lightness > 0.45)  out = Color{255, 255, 255, 255} * 0.6;
    else if (lightness > 0.3)   out = Color{255, 255, 255, 255} * 0.45;
    else if (lightness > 0.15)  out = Color{255, 255, 255, 255} * 0.3;
    else                        out = Color{255, 255, 255, 255} * 0;
    return true;
}

bool GrayShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip);

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        lightness += std::max(0.0f, interpolated_normal * direction);
    }
    out = Color{255, 255, 255, 255} * lightness;
    return true;
}

bool PhongShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip).Normalize();
    const Vector2f interpolated_uv = Interpolate(in.triangle[0].uv, in.triangle[1].uv, in.triangle[2].uv, in.bc_clip);
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(interpolated_uv) : Color::White();
    const Color specular_color = model->specular_map() != nullptr ? model->specular_map()->GetPixel(interpolated_uv) : Color::White();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        const float diffuse = std::max(0.0f, interpolated_normal * direction);
        Vector3f reflection = (interpolated_normal * (interpolated_normal * direction) * 2 - direction).Normalize();
        const float specular = static_cast<float>(std::pow(std::max(0.0f, reflection * view_direction), specular_color[0] + 5));
        lightness += diffuse + specular;
    }
    lightness += ambient_light;
    out = texture_color * lightness;
    return true;
}

bool BlinnPhongShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip).Normalize();
    const Vector2f interpolated_uv = Interpolate(in.triangle[0].uv, in.triangle[1].uv, in.triangle[2].uv, in.bc_clip);
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(interpolated_uv) : Color::White();
    const Color specular_color = model->specular_map() != nullptr ? model->specular_map()->GetPixel(interpolated_uv) : Color::White();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        const float diffuse = std::max(0.0f, interpolated_normal * direction);
        const Vector3f half = (direction + view_direction).Normalize() * -1;
        const float specular = static_cast<float>(std::pow(std::max(0.0f, interpolated_normal * half), specular_color[0] + 100));
        lightness += diffuse + specular;
    }
    lightness += ambient_light;
    out = texture_color * lightness;
    return true;
}

bool NormalShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector2f interpolated_uv = Interpolate(in.triangle[0].uv, in.triangle[1].uv, in.triangle[2].uv, in.bc_clip);
    const Vector3f normal = model->normal(interpolated_uv).Normalize() ;
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(interpolated_uv) : Color::White();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        lightness += std::max(0.0f, normal * direction);
    }
    out = texture_color * lightness;
    return true;
}

bool NormalTangentShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip).Normalize();
    const Vector2f interpolated_uv = Interpolate(in.triangle[0].uv, in.triangle[1].uv, in.triangle[2].uv, in.bc_clip);
    const Vector3f normal_tangent = model->normal_map_tangent() != nullptr ? model->normal_tangent(interpolated_uv) : interpolated_normal;
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(interpolated_uv) : Color::White();
    const Color specular_color = model->specular_map() != nullptr ? model->specular_map()->GetPixel(interpolated_uv) : Color::White();

    const Matrix3x3 A_inverse = Matrix3x3 {
        (in.triangle[1].vertex_ndc_space - in.triangle[0].vertex_ndc_space).Project<3>(),
        (in.triangle[2].vertex_ndc_space - in.triangle[0].vertex_ndc_space).Project<3>(),
        interpolated_normal
    }.Inverse();
    const Vector3f tangent = A_inverse * Vector3f {
        in.triangle[1].uv[0] - in.triangle[0].uv[0],
        in.triangle[2].uv[0] - in.triangle[0].uv[0],
        0
    };
    const Vector3f binoormal = A_inverse * Vector3f {
        in.triangle[1].uv[1] - in.triangle[0].uv[1],
        in.triangle[2].uv[1] - in.triangle[0].uv[1],
        0
    };
    const Matrix3x3 TBN = Matrix3x3 {
        tangent.Normalize(),
        binoormal.Normalize(),
        interpolated_normal
    }.Transpose();
    const Vector3f normal_mapping = (TBN * normal_tangent).Normalize();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        const float diffuse = std::max(0.0f, normal_mapping * direction);
        const Vector3f half = (direction + view_direction).Normalize() * -1;
        const float specular = static_cast<float>(std::pow(std::max(0.0f, normal_mapping * half), specular_color[0] + 100));
        lightness += diffuse + specular;
    }
    lightness += ambient_light;
    out = texture_color * lightness;
    return true;
}

bool TestShader::FragmentShader(const FragmentShaderInput &in, Color &out) const {
    const Vector3f interpolated_normal = Interpolate(in.triangle[0].normal, in.triangle[1].normal, in.triangle[2].normal, in.bc_clip).Normalize();
    const Vector2f interpolated_uv = Interpolate(in.triangle[0].uv, in.triangle[1].uv, in.triangle[2].uv, in.bc_clip);
    const Vector3f normal_tangent = model->normal_map_tangent() != nullptr ? model->normal_tangent(interpolated_uv) : interpolated_normal;
    const Color texture_color = model->diffuse_map() != nullptr ? model->diffuse_map()->GetPixel(interpolated_uv) : Color::White();
    const Color specular_color = model->specular_map() != nullptr ? model->specular_map()->GetPixel(interpolated_uv) : Color::White();

    const Matrix3x3 A_inverse = Matrix3x3 {
        in.triangle[1].vertex_view_space - in.triangle[0].vertex_view_space,
        in.triangle[2].vertex_view_space - in.triangle[0].vertex_view_space,
        interpolated_normal
    }.Inverse();
    const Vector3f tangent = A_inverse * Vector3f {
        in.triangle[1].uv[0] - in.triangle[0].uv[0],
        in.triangle[2].uv[0] - in.triangle[0].uv[0],
        0
    };
    const Vector3f binoormal = A_inverse * Vector3f {
        in.triangle[1].uv[1] - in.triangle[0].uv[1],
        in.triangle[2].uv[1] - in.triangle[0].uv[1],
        0
    };
    const Matrix3x3 TBN = Matrix3x3 {
        tangent.Normalize(),
        binoormal.Normalize(),
        interpolated_normal
    }.Transpose();
    const Vector3f normal_mapping = (TBN * normal_tangent).Normalize();

    float lightness = 0.0;
    for (const auto& [direction, intensity] : lights) {
        const float diffuse = std::max(0.0f, normal_mapping * direction);
        const Vector3f half = (direction + view_direction).Normalize() * -1;
        const float specular = static_cast<float>(std::pow(std::max(0.0f, normal_mapping * half), specular_color[0] + 100));
        lightness += diffuse + specular;
    }
    lightness += ambient_light;
    out = texture_color * lightness;
    return true;
}
