#ifndef RN_RENDER_TASK_H
#define RN_RENDER_TASK_H

#include "../core/color.h"
#include "../core/pixel_buffer.h"


typedef struct RenderTask 
{
    Buffer* buffer;
    void (*render_func)(uint32_t pixel_x, uint32_t pixel_y);
    void (*callback_func)(void* render_pool);
    union 
    {
        struct 
        {
            uint32_t tile_size;
            uint32_t tile_current;
            uint8_t* tile_matrix;
        } tiled;

    } render_data;
} RenderTask;



#endif
