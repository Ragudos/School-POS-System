#include <windows.h>

#include <iostream>
#include <keyboard.hpp>
#include <thread>

using namespace std;

enum GUI_STATE { SHOP, ABOUT, CART, CHECKOUT };

class GUI {
   private:
    GUI() : currentState(SHOP){};

    GUI(const GUI&) = delete;
    GUI& operator=(const GUI&) = delete;

   public:
    GUI_STATE currentState;

    static GUI& getInstance() {
        static GUI instance;
        return instance;
    }

    bool render() { throw logic_error("unimplemented"); }
};

int main() {
    while (true) {
        int pressedKeyCode = getPressedKeyCode();

        cout << pressedKeyCode << " ";

        Sleep(100);
    }

    return 0;
}
