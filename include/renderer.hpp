#ifndef RENDERER_MODULE
#define RENDERER_MODULE

#include <iostream>
#include <sstream>
#include <tuple>

#include "./components/select.hpp"

using namespace std;

/**
 *
 * This will be mainly used for the navigation bar
 */
struct Tab {
    /** A tuple of [x,y] dimensions */
    tuple<int, int> pos;
    /** A tuple of [x,y] dimensions */
    tuple<int, int> dimensions;
    /**
     * A tuple of [lowercase,uppercase] ASCII codes of the corresponding key
     * that will activate this tab.
     */
    tuple<int, int> keyCodes;
    string text;

    Tab(tuple<int, int> pos, tuple<int, int> dimensions,
        tuple<int, int> keyCodes, string text)
        : pos(pos), dimensions(dimensions), keyCodes(keyCodes), text(text) {}
};

struct InteractableTab : public Tab {
    bool isActive;

    InteractableTab(Tab tab, bool isActive) : Tab(tab), isActive(isActive) {}
};

struct MenuChoice {
    /** The id/value of the choice */
    string value;
    /** The quantity */
    int qty;
    /** The price of this menu choice in Php */
    double price;
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

// TODO:
// Read a JSON file that will contain everything (
// menu item, its category, price, etc., and just have that be
// here)

class Renderer {
   private:
    ostringstream buf;
    Renderer()
        : currentState(SHOP),
          toolTips({{{1, 4}, {4, 1}, {KEY_q, KEY_Q}, "quit"},
                    {{5, 4}, {9, 1}, {KEY_PLUS, KEY_PLUS}, "increment"},
                    {{14, 4},
                     {9, 1},
                     {KEY_HYPHEN_MINUS, KEY_HYPHEN_MINUS},
                     "decrement"}}),
          navigationTabs({
              {{{1, 8}, {4, 1}, {KEY_s, KEY_S}, "shop"}, true},
              {{{5, 8}, {5, 1}, {KEY_a, KEY_A}, "admin"}, false},
              {{{10, 8}, {8, 1}, {KEY_c, KEY_C}, "checkout"}, false},
          }),
          menuItems({{"Caramel Macchiato", "caramel-macchiato"},
                     {"Java Chip", "java-chip"},
                     {"Decaf", "decaf"},
                     {"Cold Brew", "cold-brew"},
                     {"Caffe Americano", "caffe-americano"},
                     {"Blonde Roast", "blonde-roast"}}){};
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void bufferHeroSection() {
        moveCursorTo(1, 1);
        buf << "\033[33m"
            << "Comffee POS System"
            << "\033[37m";
    }

    void bufferNav() {
        for (size_t i = 0; i < navigationTabs.size(); ++i) {
            InteractableTab& tab = navigationTabs.at(i);
            int prevTabWidth = 1;

            if (i != 0) {
                InteractableTab prevTab = navigationTabs.at(i - 1);
                prevTabWidth =
                    get<0>(prevTab.pos) + get<0>(prevTab.dimensions) + (4 * i);
            }

            moveCursorTo(get<1>(tab.pos), prevTabWidth);
            buf << "\033[1m" << (char)get<0>(tab.keyCodes) << "\033[0m";
            moveCursorTo(get<1>(tab.pos), prevTabWidth + 2);

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
            for (size_t i = 0; i < menuItems.choices.size(); ++i) {
                SelectChoice item = menuItems.choices.at(i);

                if (i == menuItems.getCurrentChoice()) {
                    buf << "\033[30m\033[47m";
                }

                buf << item.name << "\033[0m";
                moveCursorTo(DYNAMIC_STARTING_ROW + i + 1, 1);
            }
        }
    }

    void bufferToolBar() {
        moveCursorTo(3, 1);

        for (int i = 0; i < 50; ++i) {
            buf << "-";
        }

        moveCursorTo(4, 1);

        buf << "\033[K";

        for (size_t i = 0; i < toolTips.size(); ++i) {
            Tab* toolTip = &toolTips.at(i);

            if (currentState != SHOP) {
                if (toolTip->text == "increment" ||
                    toolTip->text == "decrement") {
                    continue;
                }
            }

            int prevToolTipWidth = 1;

            if (i != 0) {
                Tab* prevToolTip = &toolTips.at(i - 1);
                prevToolTipWidth = get<0>(prevToolTip->pos) +
                                   get<0>(prevToolTip->dimensions) + (4 * i);
            }

            moveCursorTo(get<1>(toolTip->pos), prevToolTipWidth);
            buf << "\033[1m" << (char)get<0>(toolTip->keyCodes) << "\033[0m";
            moveCursorTo(get<1>(toolTip->pos), prevToolTipWidth + 2);
            buf << "\033[2m" << toolTip->text << "\033[0m";
        }

        moveCursorTo(5, 1);

        for (int i = 0; i < 50; ++i) {
            buf << "-";
        }
    }

   public:
    const int DYNAMIC_STARTING_ROW = 12;
    RendererState currentState;
    vector<Tab> toolTips;
    vector<InteractableTab> navigationTabs;
    Select menuItems;

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
        bufferToolBar();
        bufferNav();
        bufferContents();

        cout << buf.str();
        buf.str("");
        buf.clear();
    }

    void changeNavTab(int key) {
        for (size_t i = 0; i < 3; ++i) {
            InteractableTab* tab = &navigationTabs.at(i);

            if (tab->isActive) {
                if (get<0>(tab->keyCodes) == key ||
                    get<1>(tab->keyCodes) == key) {
                    return;
                }

                tab->isActive = false;

                if (tab->text == "shop") {
                    menuItems.reset();
                }
            } else if (get<0>(tab->keyCodes) == key ||
                       get<1>(tab->keyCodes) == key) {
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
#endif