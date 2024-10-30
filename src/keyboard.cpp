#include <keyboard.hpp>

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
                return KEY_DOWN;
            case KEY_C:
                return KEY_RIGHT;
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
    // termios structure
    // contain terminal input output settings that control
    // the terminal
    struct termios oldt, newt;

    int ch;

    // tcgetattr saves the terminal's current settings
    // STDIN_FILENO means standard input (usually the keyboard)
    // and stores it in the `oldt` variable.
    tcgetattr(STDIN_FILENO, &oldt);

    // we say that newt is oldt so we
    // base ont the terminal's settings
    // before changes are made
    // and avoid changing the oldt's values.
    newt = oldt;
    // we clear the terminal's ICANON and ECHO flags
    // using bitwise operations
    // which means:
    // ICANON: non-canonical, meaning the each character outputted to the
    // terminal will be read/processed immediately instead of reading it
    // line-by-line (need to press Enter).
    // ECHO: echo or outputs the input character to the terminal, and
    // clearing this will disable this behavior.
    newt.c_lflag &= ~(ICANON | ECHO);

    // set the applied settings
    // TCSANOW means it will be applied immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // now, we wait for a key to be pressed (input character)
    ch = getchar();

    // Restore the old settings
    // TCSANOW means it will be applied immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

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
