#pragma once

#include <iostream>
#include <string_view>

using namespace std;

namespace terminal {
constexpr string_view ESC = "\033[";
constexpr string_view SEP = ";";
}  // namespace terminal
