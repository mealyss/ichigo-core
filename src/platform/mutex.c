#include "mutex.h"

Mutex mutex_create()
{
    Mutex out;

    out.native_mutex = mutex_impl_create();

    return out;
}

void mutex_destroy(Mutex mutex)
{
    mutex_impl_destroy(mutex.native_mutex);
}

void mutex_lock(Mutex mutex)
{
    mutex_impl_lock(mutex.native_mutex);
}

void mutex_unlock(Mutex mutex)
{
    mutex_impl_unlock(mutex.native_mutex);
}


