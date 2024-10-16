#include <keyboard.hpp>
#include <looplambda.hpp>
#include <renderer.hpp>
#include <utils.hpp>

using namespace std;
using namespace miscellaneous;

void programEntryPoint(LoopLambda* loop) {
    Renderer& renderer = Renderer::getInstance();
    int pressedKeyCode = Keyboard::getPressedKeyCode();

    switch (pressedKeyCode) {
        case KEY_UP:
            break;
        case KEY_DOWN:
            break;
        case KEY_LEFT:
            break;
        case KEY_RIGHT:
            break;
        case KEY_A:
        case KEY_a:
            break;
        case KEY_S:
        case KEY_s:
            break;
        case KEY_C:
        case KEY_c:
            break;
        case KEY_Q:
        case KEY_q:
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

    renderer.render();
}

int main() {
    // Reset the terminal so cursor gets reset
    clearTerminal();

    LoopLambda loop(100, programEntryPoint);

    loop.start();

    return 0;
}
