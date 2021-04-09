#include "rnpch.h"
#include "rainy.h"

#include "core/pixel_buffer.h"
#include "format/bitmap.h"

#include "render/raycast.h"

RN_Buffer* rn_buffer_create(uint32_t widht, uint32_t height)
{
    Buffer* out = (Buffer*) malloc(sizeof(uint32_t) + sizeof(uint32_t) + widht * height * sizeof(Color24));
    out->widht = widht;
    out->height = height;
    return out;
}


void rn_buffer_destroy(RN_Buffer* buffer)
{
    free(buffer);
}


RN_Scene* rn_scene_create()
{
    return (RN_Scene*) malloc(sizeof(Scene));
}


void rn_scene_destroy(RN_Scene* scene)
{
    free(scene);
}

bool rn_save_bmp(const RN_Buffer* buffer, const char* file_name)
{
    return bmp_save_buffer(buffer, file_name);
}

void rn_render(const RN_Scene *scene, RN_Buffer *render_buffer)
{
    render_raycast(scene, render_buffer);
}
