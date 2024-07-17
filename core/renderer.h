#ifndef RENDERER_H
#define RENDERER_H

#include "color.h"
#include "buffer.h"
#include "../maths/maths.h"

class Renderer {
public:
    static void DrawLine(const ColorBuffer &buffer, Vector2 p0, Vector2 p1, const Color &color);
};


#endif //RENDERER_H
