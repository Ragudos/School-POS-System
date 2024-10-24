#include <keyboard.hpp>
#include <looplambda.hpp>
#include <renderer.hpp>
#include <utils.hpp>

using namespace miscellaneous;
using namespace terminal;

void programEntryPoint(LoopLambda* loop) {
    Renderer& renderer = Renderer::getInstance();
    int pressedKeyCode = Keyboard::getPressedKeyCode();

    switch (pressedKeyCode) {
        case KEY_UP:
            if (renderer.currentState == SHOP) {
                renderer.prevMenu();
            }
            break;
        case KEY_DOWN:
            if (renderer.currentState == SHOP) {
                renderer.nextMenu();
            }
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
        case KEY_R:
        case KEY_r:
            if (renderer.currentState == SHOP) {
                renderer.resetMenuChoices();
            }
            break;
        case KEY_Q:
        case KEY_q:
            // TODO: Cleanup operations
            loop->stop();
            break;
        case KEY_PLUS:
            if (renderer.currentState == SHOP) {
                renderer.increment();
            }
            break;
        case KEY_HYPHEN_MINUS:
            if (renderer.currentState == SHOP) {
                renderer.decrement();
            }
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

    int i = 0;

    while (true) {
        clearLine();
        restoreSavedCursorPosition();
        saveCursorPosition();
        moveCursorAtCol(i % 5);
        tuple<int, int> cursorPos = getCursorPosition();
        // Reset to beginning to display the text below;
        moveCursorAtCol(1);
        cout << "Row: " << get<0>(cursorPos) << ", Col: " << get<1>(cursorPos)
             << " " << i++ << endl;

        wait(100);
    }

    /*Renderer& renderer = Renderer::getInstance();

    // Disable text wrapping
    cout << "\033[?7l";
    // Hide cursor
    cout << "\033[?25l";

    renderer.render();

    LoopLambda loop(10, programEntryPoint);

    loop.start();

    // Clear everything on terminal when program ends.
    clearTerminal();
    // Show cursor
    cout << "\033[?25h"; */

    return 0;
}
