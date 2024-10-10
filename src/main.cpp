#include <signal.h>

#include <functional>
#include <iostream>
#include <keyboard.hpp>
#include <string>
#include <thread>
#include <utils.hpp>

using namespace std;
using namespace miscellaneous;

enum RendererState { SHOP, ABOUT, CART, CHECKOUT };

class Renderer {
   private:
    Renderer() : currentState(SHOP){};

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

   public:
    RendererState currentState;

    /**
     *
     * This is a singleton pattern.
     * We use this to allow us to access this from anywhere without having to
     * pass this as an argument in functions.
     */
    static Renderer& getInstance() {
        static Renderer instance;
        return instance;
    }

    bool render() { throw logic_error("unimplemented"); }
};

class ProgramLoop {
   private:
    function<void(ProgramLoop*)> lambda;
    /**
     *
     * Time before the next iteration is ran. This is to conserve CPU.
     * This ProgramLoop class will use the same strategy as polling.
     */
    int waitTime;
    bool shouldLoop;

   public:
    ProgramLoop(const function<void(ProgramLoop*)> lambda, const int waitTime)
        : lambda(lambda), waitTime(waitTime), shouldLoop(false){};

    void start() {
        if (shouldLoop) {
            return;
        }

        shouldLoop = true;

        while (shouldLoop) {
            lambda(this);
            wait(waitTime);
        }
    }

    void stop() { shouldLoop = false; }
};

void programEntryPoint(ProgramLoop* programLoop) {
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
        case KEY_q:
            // TODO: Cleanup here
            programLoop->stop();
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
}

int main() {
    ProgramLoop programLoop(programEntryPoint, 100);

    programLoop.start();

    return 0;
}
