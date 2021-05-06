#include "render_worker.h"
#include "render_task.h"

//TODO Remove
#include "../rnpch.h"

static void* render_worker_func(RenderWorker* self);

void render_worker_create(RenderTask* task, RenderWorker* out)
{
    out->render_thread = thread_create(NULL, NULL);

    out->is_working = false;

    out->render_task = task;
    
    thread_init(out->render_thread, (void* (*)(void* data)) render_worker_func, out);

    thread_launch(out->render_thread);
}


void render_worker_terminate(RenderWorker* rworker)
{
    thread_destroy(rworker->render_thread);
    rworker->is_working = false;
}


void render_worker_task_begin(RenderWorker* rworker)
{
    thread_resume(rworker->render_thread);
}

void render_worker_task_stop(RenderWorker* rworker)
{
    thread_terminate(rworker->render_thread);
    thread_init(rworker->render_thread, (void* (*)(void* data)) render_worker_func, rworker);
    thread_launch(rworker->render_thread);
}


// Actually only parralel tilling worker
static void* render_worker_func(RenderWorker* self)
{
    // Indicate thread launched
    self->render_task->callback_func(self->render_pool);

    static unsigned cols;
    static unsigned rows;
    static unsigned tsize;
    static uint8_t* tiles;
    static Buffer* buf;
    static void (*compute)(uint32_t x, uint32_t y);
    static void (*callback)(void*);

    uint16_t ncur;
    uint16_t x0, y0, x1, y1;

_sleep:
    // Thread launching at sleeped state, untill get real task
    self->is_working = false;
    thread_pause(self->render_thread);
    thread_sleep_untill_resumed(self->render_thread);

    // Worker has awaked
    // current tile is a begin tile position + 1. if equals 0 -> there is no work
    if(self->render_task->render_data.tiled.tile_current == 0)
        goto _sleep;

    self->is_working = true;
    buf = self->render_task->buffer;
    tsize = self->render_task->render_data.tiled.tile_size;
    tiles = self->render_task->render_data.tiled.tile_matrix;
    cols = buf->widht  / tsize  + buf->widht % tsize;
    rows = buf->height  / tsize  + buf->height % tsize;
    compute = self->render_task->render_func;
    callback = self->render_task->callback_func;

    ncur = self->render_task->render_data.tiled.tile_current - 1;

_next_tile:
    // Calculate tile
    x0 = tsize * (ncur % cols);
    y0 = tsize * (ncur / cols);
    x1 = x0 + tsize > buf->widht ? buf->widht : x0 + tsize;
    y1 = y0 + tsize > buf->height ? buf->height : y0 + tsize;

    // Beging drawing tile
    for(size_t y = y0; y < y1; ++y)
    {
        for(size_t x = x0; x < x1; ++x)
        {
            compute(x, y);
        }
    }
            
    // Getting new tile
    while(ncur < cols * rows)
    {
        if(!tiles[ncur])
        {
            tiles[ncur] = true;
            goto _next_tile;
        }
        ncur++;
    }


    // Bringing callback to mark work finished
    self->is_working = false;
    callback(self->render_pool);

    goto _sleep;

    return NULL;
}



