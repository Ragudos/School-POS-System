#ifndef UTILITIES

#define UTILITIES

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cstdlib>

using namespace std;

namespace miscellaneous {
void wait(int durationInMilliseconds) {
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
    sleep(durationInMilliseconds / 1000);
#elif defined(WINDOWS_PLATFORM)
    Sleep(durationInMilliseconds);
#else
#error "Unsupported Platform!"

#endif
}

/**
 *
 * Literally clears the terminal's contents (All of it)
 */
void clearTerminal() {
#ifdef WINDOWS_PLATFORM
    system("cls");
#else
    system("clear");
#endif
}
}  // namespace miscellaneous

#endif
