#include "sleep_imp.h"

#include <time.h>
#include <errno.h>

void thread_sleep_impl(uint64_t milisec)
{
    uint64_t usecs = milisec * 1000;

    struct timespec ti;
    ti.tv_nsec = (usecs % 1000000) * 1000;
    ti.tv_sec  = usecs / 1000000;

     while ((nanosleep(&ti, &ti) == -1) && (errno == EINTR))
    {
    }

}


