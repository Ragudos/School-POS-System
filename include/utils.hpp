#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cstdlib>
#include <sstream>
#include <cstdint>
#include <tuple>

using namespace std;

namespace error_handling {
/**
 *
 * Throws `logic_error` if `bool` is false
 */
void assert(bool);
/**
 *
 * Throws `logic_error` if `bool` is false
 */
void assert(bool, string);
}  // namespace error_handling

namespace miscellaneous {
    void wait(unsigned int);
}  // namespace miscellaneous

namespace string_utils {
    string kebabToPascal(const string&, const bool) noexcept;
}

namespace terminal {
void moveCursorTo(const unsigned int) noexcept;
void moveCursorTo(const unsigned int, const unsigned int) noexcept;
void moveCursorTo(ostringstream *, const unsigned int) noexcept;
void moveCursorTo(ostringstream *, const unsigned int, const unsigned int) noexcept;
void moveCursor(const int) noexcept;
void moveCursor(const int, const int) noexcept;
void moveCursor(ostringstream *, const int) noexcept;
void moveCursor(ostringstream *, const int, const int) noexcept;
void moveCursorUp(const unsigned int) noexcept;
void moveCursorUp(ostringstream *, const unsigned int) noexcept;
void moveCursorDown(const unsigned int) noexcept;
void moveCursorDown(ostringstream *, const unsigned int) noexcept;
void moveCursorRight(const unsigned int) noexcept;
void moveCursorRight(ostringstream *, const unsigned int) noexcept;
void moveCursorLeft(const unsigned int) noexcept;
void moveCursorLeft(ostringstream *, const unsigned int) noexcept;
void moveCursorToStartOfCurrLine() noexcept;
void moveCursorToStartOfCurrLine(ostringstream *) noexcept;
void moveCursorToStartOfNextLine(const unsigned int) noexcept;
void moveCursorToStartOfNextLine(ostringstream *, unsigned int) noexcept;
void moveCursorToStartOfPrevLine(const unsigned int) noexcept;
void moveCursorToStartOfPrevLine(ostringstream *, unsigned int) noexcept;
void saveCursorPosition() noexcept;
void saveCursorPosition(ostringstream *) noexcept;
void restoreSavedCursorPosition() noexcept;
void restoreSavedCursorPosition(ostringstream *) noexcept;
void hideCursor() noexcept;
void hideCursor(ostringstream *) noexcept;
void showCursor() noexcept;
void showCursor(ostringstream *) noexcept;
void clearFromCursorToEndOfLine() noexcept;
void clearFromCursorToEndOfLine(ostringstream *) noexcept;
void clearFromCursorToStartOfLine() noexcept;
void clearFromCursorToStartOfLine(ostringstream *) noexcept;
void clearLine() noexcept;
void clearLine(ostringstream *) noexcept;
void clearLinesFromCursorToEndOfLine(const unsigned int);
void clearLinesFromCursorToEndOfLine(ostringstream *, unsigned int);
void clearLinesFromCursorToStartOfLine(const unsigned int);
void clearLinesFromCursorToStartOfLine(ostringstream *, unsigned int);
void clearLines(const unsigned int);
void clearLines(ostringstream *, unsigned int);
void clearFromCursorToEndOfScreen() noexcept;
void clearFromCursorToEndOfScreen(ostringstream *) noexcept;
void clearFromCursorToStartOfScreen() noexcept;
void clearFromCursorToStartOfScreen(ostringstream *) noexcept;
void clearScreen() noexcept;
void clearScreen(ostringstream *) noexcept;
void enterAltScreen() noexcept;
void exitAltScreen() noexcept;
void disableTextWrapping() noexcept;
tuple<unsigned int, unsigned int> getCursorPosition();
void textReset() noexcept;
void textReset(ostringstream *) noexcept;
void textBold() noexcept;
void textBold(ostringstream *) noexcept;
void textRemoveBold() noexcept;
void textRemoveBold(ostringstream *) noexcept;
void textDim() noexcept;
void textDim(ostringstream *) noexcept;
void textNormal() noexcept;
void textNormal(ostringstream *) noexcept;
void textStrikethrough() noexcept;
void textStrikethrough(ostringstream *) noexcept;
void textRemoveStrikethrough() noexcept;
void textRemoveStrikethrough(ostringstream *) noexcept;
void textItalic() noexcept;
void textItalic(ostringstream *) noexcept;
void textRemoveItalic() noexcept;
void textRemoveItalic(ostringstream *) noexcept;
void textUnderline() noexcept;
void textUnderline(ostringstream *) noexcept;
void textRemoveUnderline() noexcept;
void textRemoveUnderline(ostringstream *) noexcept;
void textBackground(const uint8_t, const uint8_t, const uint8_t) noexcept;
void textBackground(ostringstream *, const uint8_t, const uint8_t, const uint8_t) noexcept;
void textForeground(const uint8_t, const uint8_t, const uint8_t) noexcept;
void textForeground(ostringstream *, const uint8_t, const uint8_t, const uint8_t) noexcept;
}  // namespace terminal
