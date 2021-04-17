#include "../util/platform_detect.h"
#include "thread.h"

#if defined(RN_SYSTEM_WINDOWS)
    #include "windows/thread_wait.h"
#else 
    #include "unix/thread_imp.h"
#endif


Thread* thread_create(void* (*task)(void* arg), void* data)
{
    Thread* out = (Thread*) malloc(sizeof(Thread));
    out->thread_native = NULL;
    out->entry_point = task;
    out->user_data = data;
    return out;
}


void thread_init(Thread* self, void* (*task)(void* arg), void* data)
{
    self->thread_native = NULL;
    self->entry_point = task;
    self->user_data = data;
}

void thread_destroy(Thread* thread)
{
    thread_terminate(thread);
    free(thread);
}


void thread_launch(Thread* thread)
{
    thread_wait(thread);
    thread->thread_native = thread_impl_create(thread);
}


void thread_wait(Thread* thread)
{
    if(thread->thread_native)
    {
        thread_impl_wait(thread->thread_native);
        free(thread->thread_native);
        thread->thread_native = NULL;
    }
}

void thread_pause(Thread* thread)
{
    thread_impl_pause(thread->thread_native);
}

void thread_resume(Thread* thread)
{
    thread_impl_resume(thread->thread_native);
}

void thread_sleep_untill_resumed(Thread* thread)
{
    thread_impl_wait_for_resume(thread->thread_native);
}

void thread_terminate(Thread* thread)
{
    if(thread->thread_native)
    {
        thread_impl_terminate(thread->thread_native);
        free(thread->thread_native);
        thread->thread_native = NULL;
    }
}





