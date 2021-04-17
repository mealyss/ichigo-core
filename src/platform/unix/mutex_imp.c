#include "../../rnpch.h"
#include "../../core/assert.h"
#include "mutex_imp.h"

MutexImpl* mutex_impl_create()
{
    MutexImpl* out = malloc(sizeof(MutexImpl));
    RN_CORE_ASSERT(out, "Allocation failed");

    RN_CORE_ASSERT(!pthread_mutex_init(&out->mutex, NULL), "Creating mutex failed");

    return out;
}


void mutex_impl_destroy(MutexImpl* self)
{
    pthread_mutex_destroy(&self->mutex);
    free(self);
}

void mutex_impl_lock(MutexImpl* self)
{
    pthread_mutex_lock(&self->mutex);
}

void mutex_impl_unlock(MutexImpl* self)
{
    pthread_mutex_unlock(&self->mutex);
}
