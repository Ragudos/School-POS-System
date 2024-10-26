#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <sys/ioctl.h>
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

#include <algorithm>
#include <cassert>
#include <csignal>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <screen.hpp>

using namespace std;

static unique_ptr<Screen> screen;

Screen& getScreen() { return *screen; }

void initializeScreen() {
    assert(!screen || !"Screen must only be initialized once.");

    screen = make_unique<Screen>();
}

Screen::Screen() { updateScreenDimensions(); }
Screen::~Screen() { subscribers.clear(); }

void Screen::notify() {
    for (const auto& subscriber : subscribers) {
        subscriber();
    }
}

void Screen::subscribe(Screen::SubscriberCallback cb) {
    subscribers.push_back(cb);
}

void Screen::unsubscribe(Screen::SubscriberCallback cb) {
    subscribers.erase(remove_if(subscribers.begin(), subscribers.end(),
                                [&cb](const Screen::SubscriberCallback currCb) {
                                    return currCb.target_type() ==
                                           cb.target_type();
                                }),
                      subscribers.end());
}

unsigned int Screen::getWidth() const noexcept { return width; }
void Screen::setWidth(unsigned int w) {
    if (w != width) {
        width = w;
        notify();
    }
};

unsigned int Screen::getHeight() const noexcept { return height; }
void Screen::setHeight(unsigned int h) {
    if (h != height) {
        height = h;
        notify();
    }
}

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
void Screen::updateScreenDimensions() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    setWidth(w.ws_col);
    setHeight(w.ws_row);
}

#elif defined(WINDOWS_PLATFORM)
void Screen::updateScreenDimensions() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        setWidth(csbi.srWindow.Right - csbi.srWindow.Left + 1);
        setHeight(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
    }
}

#else
#error "Unsupported platform!"

#endif
