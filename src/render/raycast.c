#include "../rnpch.h"
#include "raycast.h"

static const Color24 WHITE = {255, 255, 255};

void render_raycast(const Scene *scene, Buffer* buffer)
{
    for(size_t y = 0; y < buffer->height; ++y)
    {
        for(size_t x = 0; x < buffer->widht; ++x)
        {
            buffer->pixels[y * buffer->widht + x] = WHITE;
        }
    }
}
