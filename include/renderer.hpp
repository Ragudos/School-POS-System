#include <iostream>
#include <sstream>

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