#ifndef RN_THREAD_H
#define RN_THREAD_H

#include "../util/platform_detect.h"

#if defined(RN_SYSTEM_WINDOWS)
    #include "windows/thread_imp.h"
#else
    #include "unix/thread_imp.h"
#endif

typedef struct Thread 
{
    ThreadImpl* thread_native;
    void* (*entry_point)(void*);
    void* user_data;
} Thread;

extern Thread*  thread_create(void* (*task)(void* arg), void* data);
extern void     thread_init(Thread* self, void* (*task)(void* arg), void* data);
extern void     thread_destroy(Thread* thread);
extern void     thread_launch(Thread* thread);
extern void     thread_wait(Thread* thread);
extern void     thread_pause(Thread* thread);
extern void     thread_resume(Thread* thread);
extern void     thread_sleep_untill_resumed(Thread* thread);
extern void     thread_terminate(Thread* thread);

#endif //RN_THREAD_H
