#ifndef KEYBOARD_MODULE
#define KEYBOARD_MODULE

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

#ifdef LINUX_PLATFORM
#include <termios.h>
#include <unistd.h>
#elif defined(MAC_PLATFORM)
#elif defined(WINDOWS_PLATFORM)
#include <conio.h>
#include <windows.h>
#else
#error "Unsupported platform!"
#endif

#include <iostream>

using namespace std;

class Keyboard {
   public:
#ifdef LINUX_PLATFORM
    static int getPressedKeyCode() {
        int pressedKeyCode = getPressedKeyCodeRaw();

        // Handle all escape sequences for Linux and return
        // the true pressed key's code
        if (pressedKeyCode == KEY_ESC &&
            getPressedKeyCodeRaw() == KEY_OPENING_BRACKET) {
            // Since we only care about arrow keys now, we just
            // we just do this:
            return mapESQToPressedArrowKey();
        } else {
            return pressedKeyCode;
        }
    }
#elif defined(MAC_PLATFORM)
    static int getPressedKeyCode() { throw logic_error("unimplemented"); }
#elif defined(WINDOWS_PLATFORM)
    static int getPressedKeyCode() {
        if (_kbhit()) {
            int key = _getch();

            if (key == KEY_SPECIAL_PREFIX) {
                return _getch();
            } else {
                return key;
            }
        }

        return 0;
    }
#else
#error "Unsupported platform!"
#endif
   private:
#ifdef LINUX_PLATFORM
    /**
     * Maps Unix' escape sequence's values to arrow key codes
     *
     * Returns 0 if no arrow key was pressed
     */
    static int mapESQToPressedArrowKey() {
        switch (getPressedKeyCodeRaw()) {
            case KEY_A:
                return KEY_UP;
            case KEY_B:
                return KEY_DOWN;
            case KEY_C:
                return KEY_RIGHT;
            case KEY_D:
                return KEY_LEFT;
            default:
                return 0;
        }

        return 0;
    }
    /** From ChatGPT */
    static int getPressedKeyCodeRaw() {
        struct termios oldt, newt;

        int ch;

        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;

        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        return ch;
    }
#endif
};

#endif
