#pragma once

// ASCII Key Code Definitions
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_S 83
#define KEY_s 115
#define KEY_A 65
#define KEY_a 97
#define KEY_B 66
#define KEY_C 67
#define KEY_c 99
#define KEY_D 68
#define KEY_Q 81
#define KEY_q 113
#define KEY_R 82
#define KEY_r 114
#define KEY_PLUS 43
#define KEY_HYPHEN_MINUS 45
#define KEY_ESC 27
#define KEY_BACKSPACE 8
#define KEY_OPENING_BRACKET 91

// Special values for specfic platforms
#ifdef WINDOWS_PLATFORM
#define KEY_SPECIAL_PREFIX 224

#endif

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <termios.h>
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <conio.h>
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

#include <cstdint>
#include <iostream>

using namespace std;

namespace keyboard {
unsigned int getPressedKeyCode();
}
