#include <keyboard.hpp>

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <termios.h>
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <conio.h>
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

static unsigned int getPressedKeyCodeRaw();

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
unsigned int keyboard::getPressedKeyCode() {
    unsigned int pressedKeyCode = getPressedKeyCodeRaw();

    // On Unix systems, an escape sequence is prefixed
    // for control keys.
    if (pressedKeyCode == KEY_ESC &&
        getPressedKeyCodeRaw() == KEY_OPENING_BRACKET) {
        // 0 for now, since we only care about arrow keys.
        switch (getPressedKeyCodeRaw()) {
            case KEY_A:
                return KEY_UP;
            case KEY_B:
                return KEY_DOWN : case KEY_C : return KEY_RIGHT;
            case KEY_D:
                return KEY_LEFT;
            default:
                return 0;
        }
    }

    return pressedKeyCode;
}

#elif defined(WINDOWS_PLATFORM)
unsigned int keyboard::getPressedKeyCode() { return getPressedKeyCodeRaw(); }

#else
#error "Unsupported platform!"

#endif

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
static unsigned int getPressedKeyCodeRaw() {
    struct termios oldt, newt;

    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &olt);

    return ch;
}

#elif defined(WINDOWS_PLATFORM)
static unsigned int getPressedKeyCodeRaw() {
    if (_kbhit()) {
        unsigned int key = _getch();

        if (key == KEY_SPECIAL_PREFIX) {
            return _getch();
        }

        return key;
    }

    return 0;
}

#else
#error "Unsupported platform!"

#endif
