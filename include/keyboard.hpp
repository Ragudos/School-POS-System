#pragma once

// ASCII Key Code Definitions
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
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
#define KEY_O 79
#define KEY_o 111
#define KEY_PLUS 43
#define KEY_HYPHEN_MINUS 45
#define KEY_ESC 27
#define KEY_BACKSPACE 8
#define KEY_DEL 127
#define KEY_OPENING_BRACKET 91
/** Carriage return */
#define KEY_ENTER 13
#define KEY_ENTER_LINUX 10

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
