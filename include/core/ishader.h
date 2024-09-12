#ifndef ISHADER_H
#define ISHADER_H

#include <component-gameobject.h>
#include <memory>
#include "color.h"
#include <vector>

struct VertexShaderInput {
    Vector3f vertex_model_space;
    Vector3f normal;
    Vector2f uv;
};

struct Vertex {
    Vector3f vertex_model_space;
    Vector3f vertex_view_space;
    Vector4f vertex_clip_space;
    Vector4f vertex_ndc_space;
    Vector2f vertex_screen_space;
    Vector3f normal;
    Vector2f uv;
};

struct FragmentShaderInput {
    const std::array<Vertex, 3> &triangle;
    Vector3f &bc_clip;
};

struct FragmentShaderOutput {
    Color color;
    Vector3f normal;
};

struct Light {
    Vector3f direction;
    Vector3f intensity;
};

struct IShader {
    virtual ~IShader() = default;

    void NormalizeLights();

    virtual void VertexShader(const VertexShaderInput& in, Vertex& out) const = 0;
    virtual bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const = 0;

    void Deferred(const GBuffer &g_buffer, const FrameBuffer &frame_buffer) const;

    std::string name;
    Matrix4x4 model_matrix;
    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
    Matrix4x4 viewport_matrix;
    std::vector<Light> lights{};
    std::shared_ptr<Model> model = nullptr;
    Vector3f view_direction;
    float ambient_light = 0.1f;

protected:
    explicit IShader(std::string name) : name(std::move(name)) { }
};

struct StandardVertexShader : IShader {
    void VertexShader(const VertexShaderInput& in, Vertex& out) const override;

protected:
    explicit StandardVertexShader(std::string name) : IShader(std::move(name)) { }
};

struct FixedShader final :StandardVertexShader {
    FixedShader() : StandardVertexShader("Fixed") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct GrayShader final : StandardVertexShader {
    GrayShader() : StandardVertexShader("Gray") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct PhongShader final : StandardVertexShader {
    PhongShader() : StandardVertexShader("Phong") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct BlinnPhongShader final : StandardVertexShader {
    BlinnPhongShader() : StandardVertexShader("BlinnPhong") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct NormalShader final : StandardVertexShader {
    NormalShader() : StandardVertexShader("Normal") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct NormalTangentShader final : StandardVertexShader {
    NormalTangentShader() : StandardVertexShader("Tangent") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct DeferredShader final : StandardVertexShader {
    DeferredShader() : StandardVertexShader("Deferred") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

struct TestShader final : StandardVertexShader {
    TestShader() : StandardVertexShader("Test") { }

    bool Fragment(const FragmentShaderInput& in, FragmentShaderOutput &out) const override;
};

#endif //ISHADER_H
