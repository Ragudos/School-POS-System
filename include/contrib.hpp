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
    MenuItem(string);

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
