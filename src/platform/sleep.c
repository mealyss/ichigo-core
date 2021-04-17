#include "sleep.h"

#include "../util/platform_detect.h"

#if defined(RN_SYSTEM_WINDOWS)
    #include "windows/sleep_imp.h"
#else
    #include "unix/sleep_imp.h"
#endif


void thread_sleep(uint64_t milisec)
{
    thread_sleep_impl(milisec);
}




