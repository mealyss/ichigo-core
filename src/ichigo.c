#include "rnpch.h"
#include "ichigo.h"

#include "core/pixel_buffer.h"
#include "format/bitmap.h"
#include "core/assert.h"

#include "render/pathtrace.h"
#include "parallel/render_pool.h"
#include "platform/sleep.h"

ICH_Buffer* ichigo_buffer_create(uint32_t widht, uint32_t height)
{
    Buffer* out = (Buffer*) malloc(sizeof(Buffer) + widht * height * sizeof(Color24));
//    Buffer* out = (Buffer*) calloc(1, sizeof(Buffer) + widht * height * sizeof(Color24));

    RN_CORE_ASSERT(out, "Allocation failed");
    out->widht = widht;
    out->height = height;
    return out;
}


void ichigo_buffer_destroy(ICH_Buffer* buffer)
{
    free(buffer);
}


ICH_Scene* ichigo_scene_create()
{
    return (ICH_Scene*) malloc(sizeof(Scene));
}


void ichigo_scene_destroy(ICH_Scene* scene)
{
    free(scene);
}

bool ichigo_save_bmp(const ICH_Buffer* buffer, const char* file_name)
{
    return bmp_save_buffer(buffer, file_name);
}

void ichigo_render(const ICH_Scene *scene, ICH_Buffer *buffer)
{
    RN_CORE_ASSERT(scene, "Null scene passed");
    RN_CORE_ASSERT(buffer, "Null buffer passed");

    Buffer* buf = (Buffer*) buffer;
    Color24* pixel = buf->pixels;

    pathtrace_setup(scene, buffer);

    for(size_t y = 0; y < buf->height; ++y)
    {
        for(size_t x = 0; x < buf->widht; ++x)
        {
            pathtrace_compute(x, y, pixel++);
        }
    }

    RN_LOG("Rendering done\n");
}

ICH_RenderWorkers* ichigo_workers_create(unsigned short thread_count)
{
    return render_pool_create(thread_count);
}


void ichigo_workers_terminate(ICH_RenderWorkers* workers)
{
    render_pool_terminate(workers);
}


void ichigo_parallel_render_tiled_async(const ICH_Scene* scene, ICH_Buffer* render_buffer,
                                             ICH_RenderWorkers* workers, 
                                             ICH_TilingMode mode,
                                             unsigned tile_size,
                                             void (*callback)(void))
{
    RN_CORE_ASSERT(scene && render_buffer && workers && mode && tile_size, "NULL argument was passed");

    pathtrace_setup(scene, render_buffer);

    RenderPool* rpool = (RenderPool*) workers;
    Buffer*     buf = (Buffer*) render_buffer;
    uint8_t*   tile_matrix;

    unsigned rows = buf->height / tile_size + buf->height % tile_size;
    unsigned cols = buf->widht / tile_size  + buf->widht % tile_size;
    unsigned thread_count = 0;

    tile_matrix = (uint8_t*) calloc(rows * cols, sizeof(uint8_t));
    RN_CORE_ASSERT(tile_matrix, "Allocation failed");

    switch(mode)
    {
        case ICH_LINEAR:
        
            for(size_t i = 0; i < rpool->workers_count && i < cols * rows; ++i)
            {
                rpool->workers_tasks[i].buffer = buf;
                rpool->workers_tasks[i].render_func = pathtrace_compute;
                rpool->workers_tasks[i].render_data.tiled.tile_size = tile_size;
                rpool->workers_tasks[i].render_data.tiled.tile_matrix = tile_matrix;
                rpool->workers_tasks[i].render_data.tiled.tile_current = i + 1;
                tile_matrix[i] = true;
                ++thread_count;
            }

            break;
        default:
            RN_CORE_ASSERT(false, "Invalid Tiling mode");
            break;
    }
    render_pool_begin(rpool, thread_count, callback);
}

static bool is_rendered = false;
static void rendered_callback()
{
    is_rendered = true;
}


void ichigo_parallel_render_tiled(const ICH_Scene* scene, ICH_Buffer* render_buffer,
                                             ICH_RenderWorkers* workers, 
                                             ICH_TilingMode mode,
                                             unsigned tile_size)
{
    is_rendered = false;
    ichigo_parallel_render_tiled_async(scene, render_buffer, workers, mode, tile_size, rendered_callback);

    while(!is_rendered)
        thread_sleep(100);

    RN_LOG("Rendering done\n");
}










