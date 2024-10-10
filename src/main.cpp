#include <signal.h>

#include <iostream>
#include <keyboard.hpp>
#include <thread>

using namespace std;

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
    while (true) {
        int pressedKeyCode = getPressedKeyCode();

        cout << pressedKeyCode << " ";
    }

    return 0;
}
