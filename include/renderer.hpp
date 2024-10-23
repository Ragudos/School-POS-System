#include <iostream>
#include <sstream>

#include "./components/select.hpp"

using namespace std;

/**
 *
 * This will be mainly used for the navigation bar
 */
struct Tab {
    bool isActive;
    /** A tuple of [x,y] dimensions */
    int pos[2];
    /** A tuple of [x,y] dimensions */
    int dimensions[2];
    string text;
    /**
     * A tuple of [lowercase,uppercase] ASCII codes of the corresponding key
     * that will activate this tab.
     */
    int keyCodes[2];
};

enum RendererState {
    SHOP,
    SHOP_CONFIRMATION,
    SHOP_RESULTS,
    ADMIN_OPTIONS,
    ADMIN_STATISTICS,
    ADMIN_CANCEL_ORDER,
    ADMIN_CANCEL_ORDER_CONFIRM,
    ADMIN_CANCEL_ORDER_RESULTS,
    ADMIN_FINISH_ORDER,
    ADMIN_FINISH_ORDER_CONFIRM,
    ADMIN_FINISH_ORDER_RESULTS
};

class Shop {
   private:
    Shop()
        : menuItems({{"Caramel Macchiato", "caramel-macchiato"},
                     {"Java Chip", "java-chip"},
                     {"Decaf", "decaf"},
                     {"Cold Brew", "cold-brew"},
                     {"Caffe Americano", "caffe-americano"},
                     {"Blonde Roast", "blonde-roast"}}){};
    Shop(const Shop&) = delete;
    Shop& operator=(const Shop&) = delete;

   public:
    Select menuItems;
    static Shop& getInstance() {
        static Shop instance;
        return instance;
    }

    void buffer(ostringstream* buf) {
        for (size_t i = 0; i < menuItems.choices.size(); ++i) {
            SelectChoice item = menuItems.choices.at(i);

            if (i == menuItems.getCurrentChoice()) {
                *buf << "\033[30m\033[47m";
            }

            *buf << item.name << "\033[0m" << endl;
        }
    }
};

class Renderer {
   private:
    Renderer() : currentState(SHOP){};
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void bufferHeroSection() {
        moveCursorTo(1, 1);
        buf << "\033[33m"
            << "Comffee POS System"
            << "\033[37m";
    }

    void bufferNav() {
        for (int i = 0; i < 3; ++i) {
            Tab& tab = navigationTabs[i];
            int prevTabWidth = 1;

            if (i != 0) {
                Tab prevTab = navigationTabs[i - 1];
                prevTabWidth = prevTab.pos[0] + prevTab.dimensions[0] + (4 * i);
            }

            moveCursorTo(tab.pos[1], prevTabWidth);
            buf << "\033[1m" << (char)tab.keyCodes[0] << "\033[0m";
            moveCursorTo(tab.pos[1], prevTabWidth + 2);

            if (!tab.isActive) {
                buf << "\033[2m";
            } else {
                buf << "\033[1m";
            }

            buf << tab.text;

            if (!tab.isActive) {
                buf << "\033[0m";
            }
        }
    }

    void bufferContents() {
        moveCursorTo(DYNAMIC_STARTING_ROW, 1, false);
        cout << "\033[J";

        moveCursorTo(DYNAMIC_STARTING_ROW, 1);

        if (currentState == SHOP) {
            Shop::getInstance().buffer(&buf);
        }
    }

   public:
    /** The row and below that can isn't controlled by this Renderer */
    const int DYNAMIC_STARTING_ROW = 6;
    Tab navigationTabs[3] = {
        {true, {1, 3}, {4, 1}, "shop", {KEY_s, KEY_S}},
        {false, {5, 3}, {5, 1}, "admin", {KEY_a, KEY_A}},
        {false, {10, 3}, {8, 1}, "checkout", {KEY_c, KEY_C}},
    };
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
        bufferHeroSection();
        bufferNav();
        bufferContents();

        cout << buf.str();
        buf.str("");
        buf.clear();
    }

    void changeNavTab(int key) {
        for (int i = 0; i < 3; ++i) {
            Tab* tab = &navigationTabs[i];

            if (tab->isActive) {
                if (tab->keyCodes[0] == key || tab->keyCodes[1] == key) {
                    return;
                }

                tab->isActive = false;

                if (tab->text == "shop") {
                    Shop::getInstance().menuItems.reset();
                }
            } else if (tab->keyCodes[0] == key || tab->keyCodes[1] == key) {
                if (tab->text == "shop") {
                    currentState = SHOP;
                } else if (tab->text == "admin") {
                    currentState = ADMIN_OPTIONS;
                } else if (tab->text == "checkout") {
                    currentState = SHOP_CONFIRMATION;
                }
                tab->isActive = true;
            }
        }

        render();
    }

    void moveCursorTo(int row, int col, bool buffer = true) {
        if (buffer) {
            buf << "\033[" << row << ";" << col << "H";
        } else {
            cout << "\033[" << row << ";" << col << "H";
        }
    }

    void clearArea(int fromRow, int fromCol, int height) {
        moveCursorTo(fromRow, fromCol);

        for (int i = 0; i < height; ++i) {
            buf << "\033[K";
            buf << "\033[B";
        }

        moveCursorTo(fromRow, fromCol);
    }
};