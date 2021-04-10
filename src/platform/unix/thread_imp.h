#ifndef RN_UNIX_THREAD_IMPL_H
#define RN_UNIX_THREAD_IMPL_H


#include "../../rnpch.h"
#include <pthread.h>
#include <signal.h>

typedef struct ThreadImpl 
{
    pthread_t thread;
    bool is_active;
    pthread_mutex_t suspend_mutex;
    pthread_cond_t suspend_cond;
    bool is_paused;
} ThreadImpl;

extern ThreadImpl*  thread_impl_create(void* owner); 

extern void         thread_impl_wait(ThreadImpl* thread);
extern void         thread_impl_terminate(ThreadImpl* thread);
extern void         thread_impl_pause(ThreadImpl* thread);
extern void         thread_impl_resume(ThreadImpl* thread);
extern void         thread_impl_wait_for_resume(ThreadImpl* thread);

#endif //RN_UNIX_THREAD_IMPL_H
