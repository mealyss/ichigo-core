#ifndef RN_MUTEX_H
#define RN_MUTEX_H

#include "../util/platform_detect.h"

#if defined(RN_SYSTEM_WINDOWS)
    #include "windows/mutex_imp.h"
#else
    #include "unix/mutex_imp.h"
#endif


typedef struct Mutex
{
    MutexImpl* native_mutex;
} Mutex;

extern Mutex    mutex_create();
extern void     mutex_destroy(Mutex mutex);

extern void     mutex_lock(Mutex mutex);
extern void     mutex_unlock(Mutex mutex);


#endif
