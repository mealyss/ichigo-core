#include "render.h"
#include "../core/assert.h"

#include "../core/vec4f.h"

static const Color24 WHITE = {255, 255, 255};

static const Scene*  scene;
static Buffer*       buffer;



void naive_pathtrace_setup(const Scene* sc, Buffer* buf)
{
    vec4f_init_lib();
    scene = sc;
    buffer = buf;
}

static inline void write_pixel(unsigned x, unsigned y, const vec4f_stored* color)
{
    Color24* res = &buffer->pixels[y * buffer->widht + x];
    res->r = color->x * 256;
    res->g = color->y * 256;
    res->b = color->z * 256;
}

void naive_pathtrace_compute(uint32_t xu, uint32_t yu)
{
    vec4f_stored res_color;
    float x = (float)xu, y = (float)yu;

    vec4f_store(vec4f_set(x / buffer->widht, y / buffer->height, 0), &res_color);


    write_pixel(x, y, &res_color);
}



