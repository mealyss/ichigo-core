#include "thread_imp.h"
#include "../thread.h"
#include "../../core/assert.h"

static void* entry_point_wrap(void* user_data);

ThreadImpl* thread_impl_create(void* owner)
{
    ThreadImpl* out = malloc(sizeof(ThreadImpl));
    out->is_active = pthread_create(&out->thread, NULL, &entry_point_wrap, owner) == 0;
    out->is_paused = false;
    RN_CORE_ASSERT(out->is_active, "Failed to create thread");
    RN_CORE_ASSERT(!pthread_cond_init(&out->suspend_cond, NULL), "Initializing thread conditional variable failed!");
    RN_CORE_ASSERT(!pthread_mutex_init(&out->suspend_mutex, NULL), "Initializing thread mutex failed!");
    return out;
}

void thread_impl_wait(ThreadImpl* self)
{
    if(self->is_active)
    {
        RN_CORE_ASSERT(pthread_equal(pthread_self(), self->thread) == 0, "A thread cannot wait for itself!");
        pthread_join(self->thread, NULL);
    }

}

void thread_impl_terminate(ThreadImpl* self)
{
    if(self->is_active)
    {
        #if defined(RN_SYSTEM_ANDROID)
            pthread_cancel(self->thread);
        #else
            pthread_kill(self->thread, SIGUSR1);
        #endif
    }
}


void thread_impl_pause(ThreadImpl* thread)
{
    pthread_mutex_lock(&thread->suspend_mutex);
    thread->is_paused = true;
    pthread_mutex_unlock(&thread->suspend_mutex);
}

void thread_impl_resume(ThreadImpl* thread)
{
    pthread_mutex_lock(&thread->suspend_mutex);
    thread->is_paused = false;
    pthread_cond_signal(&thread->suspend_cond);
    pthread_mutex_unlock(&thread->suspend_mutex);
}

void thread_impl_wait_for_resume(ThreadImpl *thread)
{
    pthread_mutex_lock(&thread->suspend_mutex);
    if(thread->is_paused)
        pthread_cond_wait(&thread->suspend_cond, &thread->suspend_mutex);
    thread->is_paused = false;
    pthread_mutex_unlock(&thread->suspend_mutex);
}

static void* entry_point_wrap(void* user_data)
{
    Thread* owner = (Thread*)user_data;

    #ifndef RN_SYSTEM_ANDROID
        // Tell the thread to handle cancel requests immediately
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    #endif

    owner->entry_point(owner->user_data);

    return NULL;
}

