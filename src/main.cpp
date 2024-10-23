#include <keyboard.hpp>
#include <looplambda.hpp>
#include <renderer.hpp>
#include <utils.hpp>

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
        case KEY_S:
        case KEY_s:
        case KEY_C:
        case KEY_c:
            // TODO: Check if we can go to checkout
            // (condition is that there should be items
            // in the cart).
            renderer.changeNavTab(pressedKeyCode);
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

    /**
     * Sample to get input from user
     *
     * renderer.moveCursorTo(renderer.DYNAMIC_STARTING_ROW, 1);
        renderer.buf << "Enter your name: ";

        renderer.render();

        renderer.moveCursorTo(renderer.DYNAMIC_STARTING_ROW, 18, false);

        string in;
        cin >> in;

        renderer.moveCursorTo(renderer.DYNAMIC_STARTING_ROW, 1, false);
        cout << "\033[K";

        cout << "Hello, " << in;
    */
}

int main() {
    // Reset the terminal so cursor gets reset
    clearTerminal();

    Renderer& renderer = Renderer::getInstance();

    // Disable text wrapping
    renderer.buf << "\e[?7l";
    renderer.render();

    LoopLambda loop(10, programEntryPoint);

    loop.start();

    return 0;
}
