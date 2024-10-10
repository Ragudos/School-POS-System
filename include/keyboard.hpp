#ifndef KEYBOARD

#define KEYBOARD

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_S 83
#define KEY_s 115
#define KEY_A 65
#define KEY_a 97
#define KEY_C 67
#define KEY_c 99
#define KEY_Q 81
#define KEY_q 113
#define KEY_PLUS 43
#define KEY_HYPHEN_MINUS 45
#define KEY_ESC 27
#define KEY_BACKSPACE 8

#ifdef LINUX_PLATFORM

#include <iostream>

using namespace std;

int getPressedKeyCode() { throw logic_error("unimplemented"); }

#elif defined(MAC_PLATFORM)

#include <iostream>

using namespace std;

int getPressedKeyCode() { throw logic_error("unimplemented"); }

#elif defined(WINDOWS_PLATFORM)

#include <conio.h>
#include <windows.h>

#include <iostream>

int getPressedKeyCode() {
    if (_kbhit()) {
        return _getch();
    }

    return -1;
}

#else

#error "Unsupported Platform!"

#endif

#endif
