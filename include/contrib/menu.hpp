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

constexpr static size_t MAX_MENU_ITEM_NAME_LENGTH = 10;
constexpr static size_t MAX_MENU_ITEM_DESCRIPTION_LENGTH = 50;

class MenuItem {
   private:
    string id;
    double price;
    string description;
    uint8_t qty;

   public:
    MenuItem(const string&, const double&);
    MenuItem(const string&, const double&, const string&);

    string getId() const noexcept;

    void setDescription(const string&);
    string getDescription() const noexcept;

    uint8_t getQty() const noexcept;
    void increaseQty(uint8_t);
    void decreaseQty(uint8_t);
    void resetQty() noexcept;

    double getPrice() const noexcept;

    double calculateSubTotal() const noexcept;
};
