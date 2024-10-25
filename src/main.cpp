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
        case KEY_DOWN:
            renderer.onKeyPressed(pressedKeyCode);
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

    renderer.render();

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
    enterAlternativeScreen();

    Renderer& renderer = Renderer::getInstance();

    disableTextWrapping();
    hideCursor();

    renderer.initializeComponents();
    renderer.rootNode->render(&renderer.buf);
    renderer.render();

    LoopLambda loop(10, programEntryPoint);

    try {
        loop.start();
    } catch (const runtime_error& err) {
        cout << err.what();
    }

    showCursor();
    exitAlternativeScreen();

    return 0;
}
