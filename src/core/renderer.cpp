#include "renderer.h"
#include <cmath>
#include "utility/log.h"

void Renderer::DrawLine(Vector2f p0, Vector2f p1, const Color &color, const ColorBuffer &buffer) {
    bool steep = false;
    if (std::abs(p0[0] - p1[0]) < std::abs(p0[1] - p1[1])) {
        std::swap(p0[0], p0[1]);
        std::swap(p1[0], p1[0]);
        steep = true;
    }
    if (p0[0] > p1[0]) {
        std::swap(p0, p1);
    }
    const int x0 = std::floor(p0[0]), y0 = std::floor(p0[1]);
    const int x1 = std::floor(p1[0]), y1 = std::floor(p1[1]);
    const int dx = x1 - x0;
    const int dy = y1 - y0;
    int per_error = std::abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (steep)  buffer.SetPixel(y, x, color);
        else        buffer.SetPixel(x, y, color);
        error += per_error;
        if (error > dx) {
            y += (y1 > y0 ? 1 : -1);
            error -= 2 * dx;
        }
    }
}

void Renderer::DrawModel(const std::shared_ptr<Model> &model,
                         const std::shared_ptr<IShader> &shader,
                         const std::shared_ptr<FrameBuffer> &frame_buffer) {
    for (int face_index = 0; face_index < model->faces_size(); face_index++) {
        std::array<Triangle, 3> vertex_shader_output{};
        for (const int vertex_index : {0, 1, 2}) {
            VertexShaderInput vertex_shader_input {
                .vertex_model_space = model->vertex(face_index, vertex_index),
                .normal = model->normal(face_index, vertex_index),
                .uv = model->uv(face_index, vertex_index)
            };
            shader->Vertex(vertex_shader_input, vertex_shader_output[vertex_index]);
        }
        RasterizeTriangle(vertex_shader_output, shader, frame_buffer);
    }
}

void Renderer::RasterizeTriangle(const std::array<Triangle, 3> &triangle,
                                 const std::shared_ptr<IShader> &shader,
                                 const std::shared_ptr<FrameBuffer> &frame_buffer) {
    // create bounding box
    Vector2s box_min = {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
    Vector2s box_max = {std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::min()};
    for (const auto &vertex : triangle) {
        box_min[0] = std::min(box_min[0], static_cast<size_t>(vertex.vertex_screen_space[0]));
        box_min[1] = std::min(box_min[1], static_cast<size_t>(vertex.vertex_screen_space[1]));
        box_max[0] = std::max(box_max[0], static_cast<size_t>(vertex.vertex_screen_space[0]));
        box_max[1] = std::max(box_max[1], static_cast<size_t>(vertex.vertex_screen_space[1]));
    }
    // ensure bounding box is within the frame buffer
    box_min[0] = std::max(box_min[0], static_cast<size_t>(0));
    box_min[1] = std::max(box_min[1], static_cast<size_t>(0));
    box_max[0] = std::min(box_max[0], frame_buffer->width() - 1);
    box_max[1] = std::min(box_max[1], frame_buffer->height() - 1);

#pragma omp parallel for
    for (size_t x = box_min[0]; x <= box_max[0]; x++) {
        for (size_t y = box_min[1]; y <= box_max[1]; y++) {
            const Vector3f bc_screen = GetBarycentric2d(triangle, {static_cast<float>(x), static_cast<float>(y)});
            if (bc_screen[0] < 0 || bc_screen[1] < 0 || bc_screen[2] < 0) continue; // triangle test
            // inside the triangle
            Vector3f bc_clip = {bc_screen[0] / triangle[0].vertex_clip_space[3],
                                bc_screen[1] / triangle[1].vertex_clip_space[3],
                                bc_screen[2] / triangle[2].vertex_clip_space[3]};
            bc_clip = bc_clip / (bc_clip[0] + bc_clip[1] + bc_clip[2]); // perspective correction
            const float depth = triangle[0].vertex_clip_space[2] * bc_clip[0] +
                                triangle[1].vertex_clip_space[2] * bc_clip[1] +
                                triangle[2].vertex_clip_space[2] * bc_clip[2];
            if (depth > frame_buffer->depth_buffer.GetDepth(x, y)) continue; // depth test
            // depth test passed
            Color color;
            FragmentShaderInput fragment_shader_input {
                .interpolated_vertex_view_space = Interpolate(triangle[0].vertex_view_space, triangle[1].vertex_view_space, triangle[2].vertex_view_space, bc_clip),
                .interpolated_normal = Interpolate(triangle[0].normal, triangle[1].normal, triangle[2].normal, bc_clip).Normalize(),
                .interpolated_uv = Interpolate(triangle[0].uv, triangle[1].uv, triangle[2].uv, bc_clip)
            };
            if (!shader->Fragment(fragment_shader_input, color)) continue; // fragment shader test
            // fragment shader passed
            frame_buffer->color_buffer.SetPixel(x, y, color);
            frame_buffer->depth_buffer.SetDepth(x, y, depth);
        }
    }
}

Vector3f Renderer::GetBarycentric2d(const std::array<Triangle, 3> &triangle, const Vector2f &p) {
    const float x0 = triangle[0].vertex_screen_space[0], y0 = triangle[0].vertex_screen_space[1];
    const float x1 = triangle[1].vertex_screen_space[0], y1 = triangle[1].vertex_screen_space[1];
    const float x2 = triangle[2].vertex_screen_space[0], y2 = triangle[2].vertex_screen_space[1];
    const float twice_area = x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1);
    if (std::abs(twice_area) < 1e-2) return {-1, 1, 1};
    const float u = (p[0] * (y1 - y2) + p[1] * (x2 - x1) + x1 * y2 - x2 * y1) / twice_area;
    const float v = (p[0] * (y2 - y0) + p[1] * (x0 - x2) + x2 * y0 - x0 * y2) / twice_area;
    const float w = 1 - u - v;
    return {u, v, w};
}

