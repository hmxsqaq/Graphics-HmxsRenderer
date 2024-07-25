#ifndef RENDERER_H
#define RENDERER_H

#include "color.h"
#include "buffer.h"
#include "component-gameobject.h"
#include "ishader.h"
#include "maths/maths.h"

class Renderer {
public:
    static void DrawLine(Vector2f p0, Vector2f p1, const Color &color, const ColorBuffer &buffer);


    static void DrawModel(const Model &model, const std::shared_ptr<IShader> &shader, const std::shared_ptr<FrameBuffer> &frame_buffer);

    static void RasterizeTriangle(const std::array<VertexShaderOutput, 3> &array, const std::shared_ptr<IShader> &shared, const std::shared_ptr<FrameBuffer> &frame_buffer);
};


#endif //RENDERER_H
