#include <conio.h>
#include <windows.h>

#include <iostream>
#include <string>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_S 83
#define KEY_s 115
#define KEY_A 65
#define KEY_a 97
#define KEY_C 67
#define KEY_c 99
#define KEY_Q 81
#define KEY_q 113
#define KEY_PLUS 43
#define KEY_HYPHEN_MINUS 45
#define KEY_ESC 27
#define KEY_BACKSPACE 8

// Playing around with controlling the terminal using ANSI codes
int main() {
    cout << "\033[" << 10 << ";" << 5 << "H";

    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            int value = ch;

            if (value == KEY_BACKSPACE) {
                cout << "\033[" << 1 << "D";
                cout << ' ' << flush;
                cout << "\033[" << 1 << "D";
            } else {
                cout << value << " ";
            }
        }
    }

    system("pause");

    return 0;
}
