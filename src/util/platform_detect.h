#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define RN_SYSTEM_WINDOWS
        #define RN_SYSTEM_WIN64
	#else
		/* Windows x86 */
		#define RN_SYSTEM_WINDOWS
        #define RN_SYSTEM_WIN86
	#endif

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
        #define RN_SYSTEM_IOS
        #define RN_SYSTEM_IOS_SIMULATOR
	#elif TARGET_OS_IPHONE == 1
        #define RN_SYSTEM_IOS
	#elif TARGET_OS_MAC == 1
		#define RN_SYSTEM_MACOS
	#else
		// Unknown Apple platform
	#endif

/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define RN_SYSTEM_ANDROID
#elif defined(__linux__)
	#define RN_SYSTEM_LINUX
#else
	/* Unknown compiler/platform */
#endif // End of platform detection
