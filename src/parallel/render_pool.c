#include "render_pool.h"

#include "../core/assert.h"

#include "../platform/sleep.h"

static void worker_callback_func(void* self);
static void worker_launched_callback_func(void);

static bool is_threads_lauched = false;

RenderPool* render_pool_create(unsigned short thread_count)
{

    RN_CORE_ASSERT(thread_count > 0, "Failed to create render workers, thread count must > 0");

    RenderPool* out = (RenderPool*) malloc(sizeof(RenderPool) + thread_count * sizeof(RenderWorker) + thread_count * sizeof(RenderTask));

    RN_CORE_ASSERT(out, "Allocation Failed");
    
    out->workers_count = thread_count;
    out->finished_callback_func = worker_launched_callback_func;
    is_threads_lauched = false;
    // Working count -> workers to launch
    out->workers_working_count = thread_count;
    out->wc_mutex = mutex_create();
    out->workers_tasks = (RenderTask*) (out->workers + thread_count);

    for(size_t i = 0; i < thread_count; ++i)
    {
        out->workers_tasks[i].callback_func = worker_callback_func;
        out->workers[i].render_pool = out;
        render_worker_create(out->workers_tasks + i, out->workers + i);
    }

    //Wait for thread launched
    while(!is_threads_lauched)
        thread_sleep(100);

    out->workers_working_count = 0;
    out->finished_callback_func = NULL;
    return out;
}

void render_pool_terminate(RenderPool* rpool)
{
    RN_CORE_ASSERT(rpool, "Null Render Pool passed");

    for(size_t i = 0; i < rpool->workers_count; ++i)
    {
        render_worker_terminate(rpool->workers + i);
    }

    mutex_destroy(rpool->wc_mutex);
    free(rpool);
}


void render_pool_begin(RenderPool* self, unsigned thread_count, void (*callback)(void))
{
    RN_CORE_ASSERT(self->workers_working_count == 0, "Render workers already works");
    RN_CORE_ASSERT(thread_count <= self->workers_count, "Thread count > then workers");

    self->finished_callback_func = callback;
    self->workers_working_count = thread_count;

    for(size_t i = 0; i < self->workers_count; ++i)
        render_worker_task_begin(self->workers + i);
}


void render_pool_stop(RenderPool* self)
{
    for(size_t i = 0; i < self->workers_count; ++i)
        render_worker_task_stop(self->workers + i);

    self->finished_callback_func();
    self->workers_working_count = 0;
}

static void worker_callback_func(void* self)
{
    RenderPool* pool = (RenderPool*) self;

    mutex_lock(pool->wc_mutex);
    
    pool->workers_working_count--;

    if(pool->finished_callback_func != worker_launched_callback_func)
        RN_LOGV("Workers working: %d\n", pool->workers_working_count);

    if(pool->workers_working_count == 0)
        pool->finished_callback_func();

    mutex_unlock(pool->wc_mutex);
}



static void worker_launched_callback_func(void)
{
    RN_LOG("Workers launched\n");
    is_threads_lauched = true;
}

