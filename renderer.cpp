#include "renderer.h"
#include <cmath>
#include "log.h"

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

void Renderer::DrawModel(const Model &model,
                         const std::shared_ptr<IShader> &shader,
                         const std::shared_ptr<FrameBuffer> &frame_buffer) {
    for (int face_index = 0; face_index < model.faces_size(); face_index++) {
        std::array<VertexShaderOutput, 3> vertex_shader_output{};
        for (const int vertex_index : {0, 1, 2}) {
            VertexShaderInput vertex_shader_input {
                .vertex_model_space = model.vertex(face_index, vertex_index),
                .normal = model.normal(face_index, vertex_index),
                .uv = model.uv(face_index, vertex_index)
            };
            shader->Vertex(vertex_shader_input, vertex_shader_output[vertex_index]);
        }
        RasterizeTriangle(vertex_shader_output, shader, frame_buffer);
    }
}

void Renderer::RasterizeTriangle(const std::array<VertexShaderOutput, 3> &triangle,
                                 const std::shared_ptr<IShader> &shared,
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
    LOG_DEBUG("Bounding Box: " + box_min.ToString() + " " + box_max.ToString());
}

