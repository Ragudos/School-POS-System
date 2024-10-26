#ifndef SCREEN_MODULE
#define SCREEN_MODULE

#ifdef LINUX_PLATFORM
#include <sys/ioctl.h>
#include <unistd.h>
#elif defined(MAC_PLATFORM)
#elif defined(WINDOWS_PLATFORM)
#include <windows.h>
#else
#error "Unsupported platform!"
#endif

#include <iostream>

using namespace std;

class Screen {
   private:
    int width;
    int height;

   private:
    Screen() { queryTerminalSize(); }
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

   public:
    static Screen& getInstance() {
        static Screen instance;
        return instance;
    }

   public:
    void queryTerminalSize() {
#ifdef LINUX_PLATFORM
        struct winsize w;

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
            throw logic_error("Error getting terminal size.");
        } else {
            width = w.ws_col;
            height = w.ws_row;
        }
#elif defined(WINDOWS_PLATFORM)
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                       &csbi)) {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        } else {
            throw logic_error("Error getting terminal size.");
        }
#elif defined(MAC_PLATFORM)
#else
#error "Unsupported platform!"
#endif
    }

    int getWidth() {
        queryTerminalSize();
        return width;
    }
    int getHeight() {
        queryTerminalSize();
        return height;
    }
};

#endif