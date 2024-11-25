#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cassert>
#include <chrono>
#include <contrib/menu.hpp>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

class MoneyPunct : public numpunct<char> {
   protected:
    char do_thousands_sep() const override;
    string do_grouping() const override;
};

string getCurrentDate();
double calculateChange(const double&, const double&);
double calculateTotalOfChosenMenuItems();

string formatNumber(const int&);
string formatNumber(const double&);
string formatNumber(const double&, const int&);
string formatDoublePrecision(const double&);
string formatDoublePrecision(const double&, const int&);
string parseDate(const tm&);
tm parseDate(const string&);
