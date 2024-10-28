#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <iostream>
#include <string_view>

using namespace std;

constexpr string_view TITLE = "Comffee Point-Of-Sale System";
