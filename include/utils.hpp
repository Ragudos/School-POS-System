#ifndef UTILITIES

#define UTILITIES

namespace miscellaneous {
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>
void wait(int durationInMilliseconds) { sleep(durationInMilliseconds / 1000); }
#elif defined(WINDOWS_PLATFORM)
#include <windows.h>
void wait(int durationInMilliseconds) { Sleep(durationInMilliseconds); }
#else
#error "Unsupported Platform!"
#endif

}  // namespace miscellaneous

#endif
