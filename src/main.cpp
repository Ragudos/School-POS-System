#include <signal.h>

#include <iostream>
#include <keyboard.hpp>
#include <string>
#include <thread>
#include <utils.hpp>

using namespace std;
using namespace miscellaneous;

bool endProgram = false;

enum RendererState { SHOP, ABOUT, CART, CHECKOUT };

class Renderer {
   private:
    Renderer() : currentState(SHOP){};

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   public:
    RendererState currentState;

    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

    bool render() { throw logic_error("unimplemented"); }
};

int main() {
    while (!endProgram) {
        int pressedKeyCode = getPressedKeyCode();

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
            case KEY_q: {
                // TODO: Cleanup and stuff should be done on exit
                endProgram = true;
            }; break;
            case KEY_PLUS:
                break;
            case KEY_HYPHEN_MINUS:
                break;
            case KEY_ESC:
                break;
            case KEY_BACKSPACE:
                break;
        }

        wait(100);
    }

    return 0;
}
