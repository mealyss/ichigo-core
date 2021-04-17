#ifndef RN_RENDER_WORKER_H
#define RN_RENDER_WORKER_H

#include "../platform/thread.h"
#include "render_task.h"


typedef struct RenderWorker 
{
    bool is_working;
    Thread* render_thread;
    RenderTask* render_task;
    void* render_pool;
} RenderWorker;


extern void            render_worker_create(RenderTask* rtask, RenderWorker* out);
extern void            render_worker_terminate(RenderWorker* rworker); 

extern void            render_worker_task_begin(RenderWorker* rworker);
extern void            render_worker_task_stop(RenderWorker* rworker);


#endif
