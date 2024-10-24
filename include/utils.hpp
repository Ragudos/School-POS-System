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
void moveCursorTo(size_t row, size_t col) {
    cout << "\033[" << row << ';' << col << "H";
}
void moveCursorTo(ostringstream *buf, size_t row, size_t col) {
    *buf << "\033[" << row << ';' << col << "H";
}
void moveCursorUpBy(size_t amount) { cout << "\033[" << amount << "A"; }
void moveCursorUpBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "A";
}
void moveCursorDownBy(size_t amount) { cout << "\033[" << amount << "B"; }
void moveCursorDownBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "B";
}
void moveCursorRightBy(size_t amount) { cout << "\033[" << amount << "C"; }
void moveCursorRightBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "C";
}
void moveCursorLeftBy(size_t amount) { cout << "\033[" << amount << "D"; }
void moveCursorLeftBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "D";
}
void moveCursorNextLineBy(size_t amount) { cout << "\033[" << amount << "E"; }
void moveCursorNextLineBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "E";
}
void moveCursorPrevLineBy(size_t amount) { cout << "\033[" << amount << "F"; }
void moveCursorPrevLineBy(ostringstream *buf, size_t amount) {
    *buf << "\033[" << amount << "F";
}
void moveCursorAtCol(size_t col) { cout << "\033[" << col << "G"; }
void moveCursorAtCol(ostringstream *buf, size_t col) {
    *buf << "\033[" << col << "G";
}
void moveCursorUp() { cout << "\033[A"; }
void moveCursorUp(ostringstream *buf) { *buf << "\033[A"; }
void moveCursorDown() { cout << "\033[B"; }
void moveCursorDown(ostringstream *buf) { *buf << "\033[B"; }
void moveCursorRight() { cout << "\033[C"; }
void moveCursorRight(ostringstream *buf) { *buf << "\033[C"; }
void moveCursorLeft() { cout << "\033[D"; }
void moveCursorLeft(ostringstream *buf) { *buf << "\033[D"; }
void moveCursorNextLine() { cout << "\033[E"; }
void moveCursorNextLine(ostringstream *buf) { *buf << "\033[E"; }
void moveCursorPrevLine() { cout << "\033[F"; }
void moveCursorPrevLine(ostringstream *buf) { *buf << "\033[F"; }
void saveCursorPosition() { cout << "\033[s"; }
void saveCursorPosition(ostringstream *buf) { *buf << "\033[s"; }
void restoreSavedCursorPosition() { cout << "\033[u"; }
void restoreSavedCursorPosition(ostringstream *buf) { *buf << "\033[u"; }
void hideCursor() { cout << "\033[?25l"; }
void hideCursor(ostringstream *buf) { *buf << "\033[?25l"; }
void showCursor() { cout << "\033[?25h"; }
void showCursor(ostringstream *buf) { *buf << "\033[?25h"; }
void clearFromCursorToEndOfLine() { cout << "\033[K"; }
void clearFromCursorToEndOfLine(ostringstream *buf) { *buf << "\033[K"; }
void clearFromCursorToStartOfLine() { cout << "\033[1K"; }
void clearFromCursorToStartOfLine(ostringstream *buf) { *buf << "\033[1K"; }
void clearLine() { cout << "\033[2K"; }
void clearLine(ostringstream *buf) { *buf << "\033[2K"; }
void clearFromCursorToEndOfScreen() { cout << "\033[J"; }
void clearFromCursorToEndOfScreen(ostringstream *buf) { *buf << "\033[J"; }
void clearFromCursorToStartOfScreen() { cout << "\033[1J"; }
void clearFromCursorToStartOfScreen(ostringstream *buf) { *buf << "\033[1J"; }
void clearScreen() { cout << "\033[2J"; }
void clearScreen(ostringstream *buf) { *buf << "\033[2J"; }
tuple<int, int> getCursorPosition() {
    cout << "\033[6n";

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
}  // namespace terminal

#endif
