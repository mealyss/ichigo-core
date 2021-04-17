#ifndef RN_ASSERT_H
#define RN_ASSERT_H

#include "../rnpch.h"

#define RN_CORE_ASSERT(cnd, mes)\
    if(!(cnd)) {\
        fprintf(stderr,"\033[1;31m");\
        fprintf(stderr, "ICHIGO CORE: "mes"\n");\
        fprintf(stderr,"\033[0m");\
        exit(-1);\
    }

#define RN_LOG(mes)\
    printf("ICHIGO CORE: "mes);

#define RN_LOGV(mes, var)\
    printf("ICHIGO CORE: "mes,(var));
#define RN_LOGVVV(mes, var1, var2, var3)\
    printf("ICHIGO CORE: "mes,(var1), (var2), (var3));

#endif //RN_ASSERT_H
