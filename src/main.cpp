#include <iostream>
#include <keyboard.hpp>
#include <looplambda.hpp>
#include <renderer.hpp>
#include <screen.hpp>
#include <utils.hpp>


using namespace std;
using namespace miscellaneous;
using namespace terminal;
using namespace keyboard;

void programEntryPoint(LoopLambda*);
void onScreenSizeChange();

int main() {
    initializeRenderer();
    initializeScreen();

    Screen& screen = getScreen();

    enterAltScreen();
    hideCursor();

    screen.subscribe(onScreenSizeChange);

    LoopLambda loop(50, programEntryPoint);

    loop.start();

    screen.unsubscribe(onScreenSizeChange);

    showCursor();
    exitAltScreen();

    return 0;
};

void onScreenSizeChange() {
    cout << "Width: " << getScreen().getWidth()
         << ", Height: " << getScreen().getHeight() << "; ";
}

void programEntryPoint(LoopLambda* loop) {
    Screen& screen = getScreen();
    Renderer& renderer = getRenderer();

    screen.updateScreenDimensions();

    try {
        int pressedKeyCode = getPressedKeyCode();

        switch (pressedKeyCode) {
            case KEY_UP:
                break;
            case KEY_DOWN:
                // renderer.onKeyPressed(pressedKeyCode);
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case KEY_A:
            case KEY_a:
            case KEY_S:
            case KEY_s:
            case KEY_C:
            case KEY_c:
                break;
            case KEY_R:
            case KEY_r:
                break;
            case KEY_Q:
            case KEY_q:
                // TODO: Cleanup operations
                loop->stop();
                break;
            case KEY_PLUS:
                break;
            case KEY_HYPHEN_MINUS:
                break;
            case KEY_ESC:
                break;
            case KEY_BACKSPACE:
                break;
        }

        // renderer.render();
    } catch (const exception& err) {
        clearScreen();
        textForeground(255, 0, 0);
        cout << "ERROR!" << endl;
        textReset();
        cout << err.what() << endl << endl;
        cout << "Press q to exit...";

        while (true) {
            int pressedKeyCode = getPressedKeyCode();

            if (pressedKeyCode == KEY_Q || pressedKeyCode == KEY_q) {
                break;
            }
        }

        loop->stop();
    }
}
