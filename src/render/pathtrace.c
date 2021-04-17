#include "pathtrace.h"
#include "../core/assert.h"

#include "math.h"


static const Color24 WHITE = {255, 255, 255};

static const Scene*  scene;
static Buffer*       buffer;



void pathtrace_setup(const Scene* sc, Buffer* buf)
{
    scene = sc;
    buffer = buf;
}



void pathtrace_compute(uint32_t x, uint32_t y, Color24* result)
{
    //Just payload
    double c = cos(x * y * 1.0);
    double s = sin(x * y * 1.0);
    c = fabs(c);
    s = fabs(s);
    result->r = (uint8_t)(c * 256);
    result->g = (uint8_t)(s * 256);
    result->b = (uint8_t)((pow(c, 5) + pow(s, 5))* 256);
}


//TODO Remove
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
