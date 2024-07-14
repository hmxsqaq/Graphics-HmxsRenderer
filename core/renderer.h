#ifndef RENDERER_H
#define RENDERER_H
#include "color.h"
#include "color_buffer.h"

class Renderer {
public:
    static void DrawLine(const ColorBuffer &buffer, int x0, int y0, int x1, int y1, const Color &color);
};


#endif //RENDERER_H
