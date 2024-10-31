#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

constexpr static size_t MIN_MENU_ITEM_LEN = 10;

class MenuItem {
   private:
    string id;
    string description;
    uint8_t qty;
    double price;

   public:
    MenuItem(string, double);
    MenuItem(string, double, string);

   public:
    string getId() const noexcept;
    void setDescription(string);
    string getDescription() const noexcept;
    uint8_t getQty() const noexcept;
    void increaseQty(uint8_t);
    void decreaseQty(uint8_t);
    void resetQty() noexcept;
    double getPrice() const noexcept;

   public:
    double calculateSubTotal() const noexcept;
};

double calculateChange(double, double);
double calculateTotalOfChosenMenuItems(const vector<MenuItem>&);
string formatNumber(double);

/**
 *
 * TODO:
 *
 * THIS IS JUST FOR CONVENIENCE RIGHT NOW.
 * IF YOU WANT TO IMPLEMENT THE LOGIC OF
 * TURNING A NUMBER INTO A FORMATTED VERSION,
 * THEN PLEASE DO SO.
 *
 * (e.g. 1234.00 to 1,234.00)
 */
template <typename T>
string formatNumber(T number) {
    static_assert(is_arithmetic<T>::value,
                  "formatNumber() requires a numeric type of int, float, "
                  "double, long, etc.");

    return formatNumber(static_cast<double>(number));
};
