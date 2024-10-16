#include <signal.h>

#include <functional>
#include <iostream>
#include <keyboard.hpp>
#include <sstream>
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
    ostringstream buf;
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

    void render() {
        cout << buf.str();
        buf.str("");
        buf.clear();
    }

    void clearArea(int fromRow, int fromCol, int height) {
        buf << "\033[" << fromRow << ";" << fromCol << "H";

        for (int i = 0; i < height; ++i) {
            buf << "\033[K";
            buf << "\033[B";
        }

        buf << "\033[" << fromRow << ";" << fromCol << "H";
    }
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

struct Choice {
    int position;
    string displayName;
    string value;
};

class Menu {
   private:
    int chosenChoice = 1;
    bool isValidChoice(int choice) { return choice < 5 && choice > 0; }

    Menu(){};
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

   public:
    const int MAX_MENU_CHOICES = 4;
    const Choice menuChoices[4] = {
        {1, "Decaf Coffee", "decaf"},
        {2, "Cold Brew", "cold-brew"},
        {3, "Java Chip", "java-chip"},
        {4, "Caramel Macchiato", "caramel-macchiato"}};

    /**
     *
     * This is a singleton pattern.
     * We use this to allow us to access this from anywhere without having to
     * pass this as an argument in functions.
     */
    static Menu& getInstance() {
        static Menu instance;
        return instance;
    }

    bool setChosenChoice(int choice) {
        if (!isValidChoice(choice)) {
            return false;
        }

        chosenChoice = choice;

        return true;
    }

    int getChosenChoice() { return chosenChoice; }

    void displayChoices() {
        Renderer& renderer = Renderer::getInstance();

        for (int i = 0; i < MAX_MENU_CHOICES; ++i) {
            Choice choice = menuChoices[i];

            renderer.buf << choice.displayName;

            if (choice.position == getChosenChoice()) {
                renderer.buf << " <";
            }

            renderer.buf << endl;
        }
    }
};

void programEntryPoint(ProgramLoop* programLoop) {
    Menu& menu = Menu::getInstance();
    Renderer& renderer = Renderer::getInstance();
    int pressedKeyCode = getPressedKeyCode();

    switch (pressedKeyCode) {
        case KEY_UP: {
            if (menu.getChosenChoice() == 1) {
                menu.setChosenChoice(menu.MAX_MENU_CHOICES);
            } else {
                menu.setChosenChoice(menu.getChosenChoice() - 1);
            }
            renderer.clearArea(0, 0, 5);
            menu.displayChoices();
        }; break;
        case KEY_DOWN: {
            if (menu.getChosenChoice() == menu.MAX_MENU_CHOICES) {
                menu.setChosenChoice(1);
            } else {
                menu.setChosenChoice(menu.getChosenChoice() + 1);
            }
            renderer.clearArea(0, 0, 5);
            menu.displayChoices();
        }; break;
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

    renderer.render();
}

int main() {
    Menu& menu = Menu::getInstance();
    Renderer& renderer = Renderer::getInstance();
    ProgramLoop programLoop(programEntryPoint, 100);

    // Hide cursor
    menu.displayChoices();
    programLoop.start();

    return 0;
}
