#ifndef UTILITIES

#define UTILITIES

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

namespace miscellaneous {
void wait(int durationInMilliseconds) {
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
    sleep(durationInMilliseconds / 1000);
#elif defined(WINDOWS_PLATFORM)
    Sleep(durationInMilliseconds);
#else
#error "Unsupported Platform!"

#endif
}

/**
 *
 * Literally clears the terminal's contents (All of it)
 */
void clearTerminal() {
#ifdef WINDOWS_PLATFORM
    system("cls");
#else
    system("clear");
#endif
}
}  // namespace miscellaneous

namespace terminal {
#include <tuple>

const string ESC = "\033[";
const string SEP = ";";

void moveCursorTo(int col) { cout << ESC << col + 1 << "G"; }
void moveCursorTo(int col, int row) {
    cout << ESC << row + 1 << SEP << col + 1 << "H";
}
void moveCursorTo(ostringstream *buf, int col) {
    *buf << ESC << col + 1 << "G";
}
void moveCursorTo(ostringstream *buf, int col, int row) {
    *buf << ESC << row + 1 << SEP << col + 1 << "H";
}
void moveCursor(int cols) {
    if (cols < 0) {
        cout << ESC << (cols * -1) << "D";
    } else if (cols > 0) {
        cout << ESC << cols << "C";
    }
}
void moveCursor(int cols, int rows) {
    moveCursor(cols);

    if (rows < 0) {
        cout << ESC << (rows * -1) << "A";
    } else if (rows > 0) {
        cout << ESC << rows << "B";
    }
}
void moveCursor(ostringstream *buf, int cols) {
    if (cols < 0) {
        *buf << ESC << (cols * -1) << "D";
    } else if (cols > 0) {
        *buf << ESC << cols << "C";
    }
}
void moveCursor(ostringstream *buf, int cols, int rows) {
    moveCursor(buf, cols);

    if (rows < 0) {
        *buf << ESC << (rows * -1) << "A";
    } else if (rows > 0) {
        *buf << ESC << rows << "B";
    }
}
void moveCursorUp(int amount = 1) { cout << ESC << amount << "A"; }
void moveCursorUp(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "A";
}
void moveCursorDown(int amount = 1) { cout << ESC << amount << "B"; }
void moveCursorDown(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "B";
}
void moveCursorRight(int amount = 1) { cout << ESC << amount << "C"; }
void moveCursorRight(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "C";
}
void moveCursorLeft(int amount = 1) { cout << ESC << amount << "D"; }
void moveCursorLeft(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "D";
}
void moveCursorToStartOfCurrLine() { cout << ESC << "G"; }
void moveCursorToStartOfCurrLine(ostringstream *buf) { *buf << ESC << "G"; }
void moveCursorToStartOfNextLine(int amount = 1) {
    cout << ESC << amount << "E";
}
void moveCursorToStartOfNextLine(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "E";
}
void moveCursorToStartOfPrevLine(int amount = 1) {
    cout << ESC << amount << "F";
}
void moveCursorToStartOfPrevLine(ostringstream *buf, int amount = 1) {
    *buf << ESC << amount << "F";
}
void saveCursorPosition() { cout << ESC << "s"; }
void saveCursorPosition(ostringstream *buf) { *buf << ESC << "s"; }
void restoreSavedCursorPosition() { cout << ESC << "u"; }
void restoreSavedCursorPosition(ostringstream *buf) { *buf << ESC << "u"; }
void hideCursor() { cout << ESC << "?25l"; }
void hideCursor(ostringstream *buf) { *buf << ESC << "?25l"; }
void showCursor() { cout << ESC << "?25h"; }
void showCursor(ostringstream *buf) { *buf << ESC << "?25h"; }
void clearFromCursorToEndOfLine() { cout << ESC << "K"; }
void clearFromCursorToEndOfLine(ostringstream *buf) { *buf << ESC << "K"; }
void clearFromCursorToStartOfLine() { cout << ESC << "1K"; }
void clearFromCursorToStartOfLine(ostringstream *buf) { *buf << ESC << "1K"; }
void clearLine() { cout << ESC << "2K"; }
void clearLine(ostringstream *buf) { *buf << ESC << "2K"; }
void clearLines(int amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (int i = 0, l = amount - 1; i < l; ++i) {
        cout << ESC << "2K";
        moveCursorUp();
    }

    cout << ESC << "2K";
    moveCursorToStartOfCurrLine();
}
void clearLines(ostringstream *buf, int amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (int i = 0, l = amount - 1; i < l; ++i) {
        *buf << ESC << "2K";
        moveCursorUp(buf);
    }

    *buf << ESC << "2K";
    moveCursorToStartOfCurrLine(buf);
}
void clearFromCursorToEndOfScreen() { cout << ESC << "J"; }
void clearFromCursorToEndOfScreen(ostringstream *buf) { *buf << ESC << "J"; }
void clearFromCursorToStartOfScreen() { cout << ESC << "1J"; }
void clearFromCursorToStartOfScreen(ostringstream *buf) { *buf << ESC << "1J"; }
void clearScreen() { cout << ESC << "2J" << ESC << "3J" << ESC << "H"; }
void clearScreen(ostringstream *buf) {
    *buf << ESC << "2J" << ESC << "3J" << ESC << "H";
}
void enterAlternativeScreen() { cout << ESC << "?1049h"; }
void exitAlternativeScreen() { cout << ESC << "?1049l"; }
void disableTextWrapping() { cout << ESC << "?7l"; }
tuple<int, int> getCursorPosition() {
    cout << ESC << "6n";

    string res;
    int ch = Keyboard::getPressedKeyCode();

    while (ch) {
        char castCh = ch;
        if (castCh == 'R') {
            break;
        }

        res += castCh;
        ch = Keyboard::getPressedKeyCode();
    }

    int row = 0, col = 0;

    if (sscanf_s(res.c_str(), "\033[%d;%d", &row, &col) == 2) {
        return {row, col};
    } else {
        throw logic_error("Failed to get cursor position");
    }
}
}  // namespace terminal

#endif
