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
#include <string>
#include <vector>

using namespace std;

double calculateChange(const double&, const double&);
double calculateTotalOfChosenMenuItems(const vector<MenuItem>&);

string formatNumber(const int&);
string formatNumber(const double&);
string formatNumber(const double&, const int&);
