#ifndef RENDERER_H
#define RENDERER_H

#include <scene.h>

#include "color.h"
#include "buffer.h"
#include "component-gameobject.h"
#include "ishader.h"
#include "maths/maths.h"

class Renderer {
public:
    static void DrawLine(Vector2f p0, Vector2f p1, const Color &color, const ColorBuffer &buffer);
    static void DrawModel(const Model &model, const IShader &shader, const FrameBuffer &frame_buffer, const GBuffer &g_buffer, const RenderPath &
                          render_path);
private:
    static void RasterizeTriangle(const std::array<Vertex, 3> &triangle, const IShader &shader, const FrameBuffer &frame_buffer, const GBuffer &g_buffer, const
                                  RenderPath &render_path);
    static Vector3f GetBarycentric2d(const std::array<Vertex, 3> &triangle, const Vector2f &p);
};


#endif //RENDERER_H
