#ifndef RN_RENDER_POOL_H
#define RN_RENDER_POOL_H

#include "../rnpch.h"
#include "render_worker.h"
#include "../platform/mutex.h"

typedef enum TilingMode
{
    NONE,
    LINEAR
} TilingMode;

typedef struct RenderPool
{
    uint16_t workers_count;
    uint16_t workers_working_count;
    Mutex wc_mutex;

    void (*finished_callback_func)(void);

    RenderTask*  workers_tasks;
    RenderWorker workers[];
} RenderPool;

extern RenderPool*  render_pool_create(unsigned short rcount);
extern void         render_pool_terminate(RenderPool* rpool);

extern void         render_pool_begin(RenderPool* self, unsigned thread_count, void (*callback)(void));
extern void         render_pool_stop(RenderPool* self);


#endif
