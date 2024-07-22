#ifndef RENDERER_H
#define RENDERER_H

#include "color.h"
#include "buffer.h"
#include "maths/maths.h"

class Renderer {
public:
    static void DrawLine(Vector2f p0, Vector2f p1, const Color &color, const ColorBuffer &buffer);
    static void DrawTriangle(const FrameBuffer &buffer);
};


#endif //RENDERER_H
