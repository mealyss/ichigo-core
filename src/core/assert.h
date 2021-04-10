#ifndef RN_ASSERT_H
#define RN_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define RN_CORE_ASSERT(cnd, mes)\
    if(!(cnd)) {\
        fprintf(stderr,"\033[1;31m");\
        fprintf(stderr, "RAINY_CORE: "mes"\n");\
        fprintf(stderr,"\033[0m"); exit(-1);\
    }

#endif //RN_ASSERT_H
