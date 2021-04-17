#ifdef __GNUC__
    #define RN_COMPILER_GCC_CLANG
#elif _MSC_VER
    #define RN_COMPILER_MSC
#endif

#ifdef RN_COMPILER_GCC_CLANG 
//    #define RN_FASTCALL __attribute__((fastcall))
    #define RN_FORCE_INLINE __attribute__((always_inline))
#elif RN_COMPILER_MSC 
//    #define RN_FASTCALL __fastcall
    #define RN_FORCE_INLINE __forceinline
#else 
//    #define RN_FASTCALL
    #define RN_FORCE_INLINE
#endif







