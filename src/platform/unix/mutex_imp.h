#ifndef RN_UNIX_MUTEX_IMPL
#define RN_UNIX_MUTEX_IMPL

#include <pthread.h>

typedef struct MutexImpl
{
    pthread_mutex_t mutex;
} MutexImpl;


extern MutexImpl*   mutex_impl_create();
extern void     mutex_impl_destroy(MutexImpl* self);

extern void     mutex_impl_lock(MutexImpl* self);
extern void     mutex_impl_unlock(MutexImpl* self);


#endif
