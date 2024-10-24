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
    /**
     * A tuple of [lowercase,uppercase] ASCII codes of the corresponding key
     * that will activate this tab.
     */
    tuple<int, int> keyCodes;
    /**
     *
     * A UTF-8 code for the icon of this tab
     */
    string icon;
    string text;

    Tab(tuple<int, int> keyCodes, string icon, string text)
        : keyCodes(keyCodes), icon(icon), text(text) {}
};

enum ToolbarTabCategory { ANY_TAB, SHOP_TAB, ADMIN_TAB, CHECKOUT_TAB };

struct ToolbarTab : public Tab {
    ToolbarTabCategory category;

    ToolbarTab(Tab tab, ToolbarTabCategory category)
        : Tab(tab), category(category) {}
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
    string description;

    MenuChoice(string value, int qty, double price, string description)
        : value(value), qty(qty), price(price), description(description) {}
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
   public:
    RendererState currentState;
    const int TOOLBAR_ROW_POSITION = 4;
    const int NAV_ROW_POSITION = 8;
    const int CONTENTS_ROW_POSITION = 12;

   private:
    vector<ToolbarTab> toolTips;
    vector<InteractableTab> navigationTabs;
    Select menuItems;
    vector<MenuChoice> menuChoices;
    ostringstream buf;

   private:
    Renderer()
        : currentState(SHOP),
          toolTips({{{{KEY_q, KEY_Q}, "q", "quit"}, ANY_TAB},
                    {{{KEY_r, KEY_R}, "r", "reset"}, SHOP_TAB},
                    {{{KEY_PLUS, KEY_PLUS}, "+", "increment"}, SHOP_TAB},
                    {{{KEY_HYPHEN_MINUS, KEY_HYPHEN_MINUS}, "-", "decrement"},
                     SHOP_TAB}}),
          navigationTabs({
              {{{KEY_s, KEY_S}, "s", "shop"}, true},
              {{{KEY_a, KEY_A}, "a", "admin"}, false},
              {{{KEY_c, KEY_C}, "c", "checkout"}, false},
          }),
          menuItems({{"Caramel Macchiato", "caramel-macchiato"},
                     {"Java Chip", "java-chip"},
                     {"Decaf", "decaf"},
                     {"Cold Brew", "cold-brew"},
                     {"Caffe Americano", "caffe-americano"},
                     {"Blonde Roast", "blonde-roast"}}),
          menuChoices(
              {{"caramel-macchiato", 0, 120.00, "A refreshing drink"},
               {"java-chip", 0, 130.00, "Sweet and chippy"},
               {"decaf", 0, 50.00, "A coffee taste without its kick"},
               {"cold-brew", 0, 60.00, "Cold and strong"},
               {"caffe-americano", 0, 90.00, "A good coffee; The american way"},
               {"blonde-roast", 0, 70.00, "Hot and steamy for the day"}}){};
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void bufferHeroSection() {
        moveCursorTo(1, 1);
        buf << "\033[33m"
            << "Comffee POS System"
            << "\033[37m\033[0m";
    }

    void bufferNav() {
        int prevTabWidth = 1;

        for (size_t i = 0; i < navigationTabs.size(); ++i) {
            InteractableTab* tab = &navigationTabs.at(i);
            moveCursorTo(NAV_ROW_POSITION, prevTabWidth);
            buf << "\033[1m" << tab->icon << "\033[0m";
            moveCursorTo(NAV_ROW_POSITION,
                         prevTabWidth += tab->icon.size() + 1);

            if (!tab->isActive) {
                buf << "\033[2m";
            } else {
                buf << "\033[1m";
            }

            buf << tab->text;

            if (!tab->isActive) {
                buf << "\033[0m";
            }

            prevTabWidth += tab->text.size() + 2;
        }
    }

    void bufferContents() {
        int currentCursorRowPosition = CONTENTS_ROW_POSITION;

        moveCursorTo(CONTENTS_ROW_POSITION, 1, false);
        cout << "\033[J";
        moveCursorTo(CONTENTS_ROW_POSITION, 1);

        if (currentState == SHOP) {
            string chosenItemId;

            for (size_t i = 0; i < menuItems.choices.size(); ++i) {
                SelectChoice item = menuItems.choices.at(i);

                if (i == menuItems.getCurrentChoice()) {
                    buf << "\033[30m\033[47m";
                    chosenItemId = item.val;
                }

                buf << item.name << "\033[0m";
                currentCursorRowPosition = CONTENTS_ROW_POSITION + i + 1;
                moveCursorTo(currentCursorRowPosition, 1);
            }

            if (!chosenItemId.empty()) {
                MenuChoice* chosenItem = nullptr;

                for (size_t i = 0; i < menuChoices.size(); ++i) {
                    MenuChoice* item = &menuChoices.at(i);

                    if (item->value == chosenItemId) {
                        chosenItem = item;
                        break;
                    }
                }

                if (chosenItem != nullptr) {
                    currentCursorRowPosition += 1;

                    moveCursorTo(currentCursorRowPosition, 1);

                    for (size_t i = 0; i < chosenItem->description.size();
                         ++i) {
                        buf << '-';
                    }

                    currentCursorRowPosition += 1;
                    moveCursorTo(currentCursorRowPosition, 1);
                    buf << chosenItem->description;

                    currentCursorRowPosition += 2;
                    moveCursorTo(currentCursorRowPosition, 1);
                    buf << "price: Php " << chosenItem->price;

                    currentCursorRowPosition += 1;
                    moveCursorTo(currentCursorRowPosition, 1);
                    buf << "qty: " << chosenItem->qty;
                }
            }
        }
    }

    void bufferToolBar() {
        int toolBarWidth = 1;

        for (size_t i = 0; i < toolTips.size(); ++i) {
            ToolbarTab* toolTip = &toolTips.at(i);

            if (toolTip->category != ANY_TAB) {
                if (currentState != SHOP && toolTip->category == SHOP_TAB) {
                    continue;
                }
            }

            moveCursorTo(TOOLBAR_ROW_POSITION, toolBarWidth);

            if (i == 0) {
                buf << "\033[K";
            }

            buf << "\033[1m" << toolTip->icon << "\033[0m";
            moveCursorTo(TOOLBAR_ROW_POSITION,
                         toolBarWidth += toolTip->icon.size() + 1);
            buf << "\033[2m" << toolTip->text << "\033[0m";

            toolBarWidth += toolTip->text.size() + 2;
        }

        moveCursorTo(TOOLBAR_ROW_POSITION - 1, 1);
        buf << "\033[K";

        for (int i = 0; i < toolBarWidth; ++i) {
            buf << "-";
        }

        moveCursorTo(TOOLBAR_ROW_POSITION + 1, 1);
        buf << "\033[K";

        for (int i = 0; i < toolBarWidth; ++i) {
            buf << "-";
        }
    }

    MenuChoice* getChosenMenuItem() {
        size_t currentMenuChoiceIdx = menuItems.getCurrentChoice();
        SelectChoice currentMenuChoice =
            menuItems.choices.at(currentMenuChoiceIdx);
        MenuChoice* menuChoice = nullptr;

        for (size_t i = 0; i < menuChoices.size(); ++i) {
            MenuChoice* choice = &menuChoices.at(i);

            if (choice->value == currentMenuChoice.val) {
                menuChoice = choice;
                break;
            }
        }

        return menuChoice;
    }

   public:
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

    void nextMenu() {
        menuItems.next();
        render();
    }

    void prevMenu() {
        menuItems.prev();
        render();
    }

    void increment() {
        MenuChoice* menuChoice = getChosenMenuItem();

        if (menuChoice != nullptr) {
            menuChoice->qty += 1;
            render();
        }
    }

    void decrement() {
        MenuChoice* menuChoice = getChosenMenuItem();

        if (menuChoice != nullptr) {
            if (menuChoice->qty > 0) {
                menuChoice->qty -= 1;
                render();
            }
        }
    }

    void resetMenuChoices() {
        for (size_t i = 0; i < menuChoices.size(); ++i) {
            MenuChoice* choice = &menuChoices.at(i);

            choice->qty = 0;
        }

        render();
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