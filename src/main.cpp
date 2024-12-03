#include <contrib/state.hpp>
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
void gracefulError(const exception&);
void gracefulError(const string&);

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");

    enterAltScreen();
    hideCursor();
    disableTextWrapping();

    try {
        initializeState();
        initializeScreen();
        initializeRenderer();

        Screen& screen = getScreen();

        screen.subscribe(onScreenSizeChange);
        getRenderer().renderBuffer();

        LoopLambda loop(100, programEntryPoint);

        loop.start();
        screen.unsubscribe(onScreenSizeChange);
    } catch (const exception& e) {
        gracefulError(e);
    }

    showCursor();
    exitAltScreen();
    enableTextWrapping();

    return 0;
};

// For now, we just re-initialize all the nodes based
// on current view state since I'm too lazy
// to implement a cascading change of dimensions for all nodes.
void onScreenSizeChange() {
    Renderer& renderer = getRenderer();

    renderer.createView();
    renderer.renderBuffer();
}

void programEntryPoint(LoopLambda* loop) {
    string err;

    try {
        State& state = getState();
        Screen& screen = getScreen();
        Renderer& renderer = getRenderer();

        screen.updateScreenDimensions();
        // getPressedKeyCode() is a blocking operation on Linux.
        unsigned int pressedKeyCode = getPressedKeyCode();

        if (renderer.viewState ==
            RendererState::ADMIN_MENU_REVIEW_ORDER_INPUT_EDIT) {
            switch (pressedKeyCode) {
                case KEY_ESC: {
                    renderer.viewState =
                        RendererState::ADMIN_MENU_REVIEW_ORDER_INPUT_READONLY;
                }; break;
                case KEY_BACKSPACE:
                case KEY_DEL: {
                    string id = state.getOrderIdInput();

                    if (!id.empty()) {
                        id.pop_back();

                        state.setOrderIdInput(id);
                    }
                }; break;
                default: {
                    char key = pressedKeyCode;
                    string id = state.getOrderIdInput();

                    state.setOrderIdInput(id + key);
                }
            }

            renderer.createView();
            renderer.renderBuffer();
        } else {
            switch (pressedKeyCode) {
                case KEY_LEFT:
                    break;
                case KEY_RIGHT:
                    break;
                case KEY_R:
                case KEY_r:
                    break;
                case KEY_Q:
                case KEY_q:
                    // TODO: Cleanup operations
                    loop->stop();
                    break;
                case KEY_BACKSPACE:
                    break;
                default:
                    renderer.onKeyPressed(pressedKeyCode);
            }
        }
    } catch (const exception& e) {
        err = e.what();
    } catch (...) {
        err = "unknown error occured";
    }

    if (!err.empty()) {
        gracefulError(err);
        loop->stop();
    }
}

void gracefulError(const exception& e) { gracefulError(e.what()); }
void gracefulError(const string& e) {
    // clearScreen();
    textForeground(255, 0, 0);
    cout << "ERROR!" << endl;
    textReset();
    cout << e << endl << endl;
    cout << "Press q to exit...";

    while (true) {
        int pressedKeyCode = getPressedKeyCode();

        if (pressedKeyCode == KEY_Q || pressedKeyCode == KEY_q) {
            break;
        }
    }
}
