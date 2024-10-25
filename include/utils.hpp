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

string kebabToPascal(const string &kebabCaseStr, bool addSpace = true) {
    stringstream res;
    bool uppercaseNext = true;
    bool isBeginning = true;

    for (char ch : kebabCaseStr) {
        if (ch == '-') {
            uppercaseNext = true;
        } else if (uppercaseNext) {
            if (addSpace && !isBeginning) {
                res << " ";
            }

            res << static_cast<char>(toupper(ch));
            uppercaseNext = false;
        } else {
            res << ch;
        }

        if (isBeginning) {
            isBeginning = false;
        }
    }

    return res.str();
}
}  // namespace miscellaneous

namespace terminal {
#include <tuple>

const string ESC = "\033[";
const string SEP = ";";

void moveCursorTo(int col) { cout << ESC << (col + 1) << "G"; }
void moveCursorTo(int col, int row) {
    cout << ESC << (row + 1) << SEP << (col + 1) << "H";
}
void moveCursorTo(ostringstream *buf, int col) {
    *buf << ESC << (col + 1) << "G";
}
void moveCursorTo(ostringstream *buf, int col, int row) {
    *buf << ESC << (row + 1) << SEP << (col + 1) << "H";
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
void clearLinesFromCursorToEndOfLine(size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (int i = 0, l = amount - 1; i < l; ++i) {
        clearFromCursorToEndOfLine();
        moveCursorUp();
    }

    clearFromCursorToEndOfLine();
    moveCursorToStartOfCurrLine();
}
void clearLinesFromCursorToEndOfLine(ostringstream *buf, size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (size_t i = 0, l = amount - 1; i < l; ++i) {
        clearFromCursorToEndOfLine(buf);
        moveCursorUp(buf);
    }

    clearFromCursorToEndOfLine(buf);
    moveCursorToStartOfCurrLine(buf);
}
void clearLinesFromCursorToStartOfLine(size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (size_t i = 0, l = amount - 1; i < l; ++i) {
        clearFromCursorToStartOfLine();
        moveCursorUp();
    }

    clearFromCursorToStartOfLine();
    moveCursorToStartOfCurrLine();
}
void clearLinesFromCursorToStartOfLine(ostringstream *buf, size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (size_t i = 0, l = amount - 1; i < l; ++i) {
        clearFromCursorToStartOfLine(buf);
        moveCursorUp(buf);
    }

    clearFromCursorToStartOfLine(buf);
    moveCursorToStartOfCurrLine(buf);
}
void clearLines(size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (size_t i = 0, l = amount - 1; i < l; ++i) {
        clearLine();
        moveCursorUp();
    }

    clearLine();
    moveCursorToStartOfCurrLine();
}
void clearLines(ostringstream *buf, size_t amount = 1) {
    if (amount <= 0) {
        throw logic_error(
            "Cannot clear a non-positive integer amount of lines");
    }

    for (size_t i = 0, l = amount - 1; i < l; ++i) {
        clearLine(buf);
        moveCursorUp(buf);
    }

    clearLine(buf);
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

    if (sscanf(res.c_str(), "\033[%d;%d", &row, &col) == 2) {
        return {row, col};
    } else {
        throw logic_error("Failed to get cursor position");
    }
}

void textReset() { cout << ESC << "0m"; }
void textReset(ostringstream *buf) { *buf << ESC << "0m"; }
void textBold() { cout << ESC << "1m"; }
void textBold(ostringstream *buf) { *buf << ESC << "1m"; }
void textRemoveBold() { cout << ESC << "21m"; }
void textRemoveBold(ostringstream *buf) { *buf << ESC << "21m"; }
void textDim() { cout << ESC << "2m"; }
void textDim(ostringstream *buf) { *buf << ESC << "2m"; }
void textNormal() { cout << ESC << "22m"; }
void textNormal(ostringstream *buf) { *buf << ESC << "22m"; }
void textStrikethrough() { cout << ESC << "9m"; }
void textStrikethrough(ostringstream *buf) { *buf << ESC << "9m"; }
void textRemoveStrikethrough() { cout << ESC << "29m"; }
void textRemoveStrikethrough(ostringstream *buf) { *buf << ESC << "29m"; }
void textItalic() { cout << ESC << "3m"; }
void textItalic(ostringstream *buf) { *buf << ESC << "3m"; }
void textRemoveItalic() { cout << ESC << "23m"; }
void textRemoveItalic(ostringstream *buf) { *buf << ESC << "23m"; }
void textUnderline() { cout << ESC << "4m"; }
void textUnderline(ostringstream *buf) { *buf << ESC << "4m"; }
void textRemoveUnderline() { cout << ESC << "24m"; }
void textRemoveUnderline(ostringstream *buf) { *buf << ESC << "24m"; }
void textBackground(int r = 0, int g = 0, int b = 0) {
    cout << ESC << "48" << SEP << "2" << SEP << r << SEP << g << SEP << b
         << "m";
}
void textBackground(ostringstream *buf, int r = 0, int g = 0, int b = 0) {
    *buf << ESC << "48" << SEP << "2" << SEP << r << SEP << g << SEP << b
         << "m";
}
void textForeground(int r = 255, int g = 255, int b = 255) {
    cout << ESC << "38" << SEP << "2" << SEP << r << SEP << g << SEP << b
         << "m";
}
void textForeground(ostringstream *buf, int r = 255, int g = 255, int b = 255) {
    *buf << ESC << "38" << SEP << "2" << SEP << r << SEP << g << SEP << b
         << "m";
}
}  // namespace terminal

#endif
