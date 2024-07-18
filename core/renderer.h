#ifndef RENDERER_H
#define RENDERER_H

#include "color.h"
#include "buffer.h"
#include "../maths/maths.h"

class Renderer {
public:
    static void DrawLine(const ColorBuffer &buffer, Vector2f p0, Vector2f p1, const Color &color);
};


#endif //RENDERER_H
