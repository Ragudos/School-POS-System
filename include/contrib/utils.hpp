#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cassert>
#include <contrib/menu.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class MoneyPunct : public numpunct<char> {
   protected:
    char do_thousands_sep() const override;
    string do_grouping() const override;
};

double calculateChange(const double&, const double&);
double calculateTotalOfChosenMenuItems();

string formatNumber(const int&);
string formatNumber(const double&);
string formatNumber(const double&, const int&);
