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
#include <optional>
#include <utils.hpp>
#include <vector>

using namespace std;
using namespace string_utils;

enum MenuItemSizes { TALL, GRANDE, VENTI, TRENTA };

class MenuItemSizeData {
   private:
    MenuItemSizes size;
    string description;
    double additionalPrice;

   public:
    MenuItemSizeData(const MenuItemSizes&, const string&, const double&);

    MenuItemSizes getSize() const noexcept;

    string getDescription() const noexcept;
    double getAdditionalPrice() const noexcept;
};

string toString(const MenuItemSizes&) noexcept;
MenuItemSizes fromString(const string&);
double getAdditionalPriceForMenuItemSize(const MenuItemSizes&) noexcept;

class MenuItemAddonData {
   private:
    string name;
    double price;
    string description;

   public:
    MenuItemAddonData(const string&, const double&);
    MenuItemAddonData(const string&, const double&, const string&);

    string getName() const noexcept;

    double getPrice() const noexcept;

    string getDescription() const noexcept;
    void setDescription(const string&);
};

class MenuItemAddon {
   private:
    string menuItemUid;

    string name;
    double price;

    uint8_t qty;

   public:
    MenuItemAddon(const string&, const MenuItemAddonData&);

    string getName() const noexcept;

    double getPrice() const noexcept;

    string getMenuItemUid() const noexcept;
};

class MenuItemData {
   private:
    string name;
    double basePrice;
    string description;

   public:
    MenuItemData(const string&, const double&);
    MenuItemData(const string&, const double&, const string&);

    string getName() const noexcept;

    string getDescription() const noexcept;
    void setDescription(const string&);

    double getBasePrice() const noexcept;
};

class MenuItem {
   private:
    string uid;
    string name;
    double basePrice;

    MenuItemSizes size;

    uint8_t qty;

    optional<string> remarks;

   public:
    MenuItem(const MenuItemData&);
    MenuItem(const MenuItemData&, const MenuItemSizes&);

    string getUid() const noexcept;

    string getName() const noexcept;

    double getBasePrice() const noexcept;

    MenuItemSizes getSize() const noexcept;
    void setSize(MenuItemSizes size) noexcept;

    uint8_t getQty() const noexcept;
    void increaseQty(const uint8_t&);
    void decreaseQty(const uint8_t&);
    void resetQty() noexcept;

    optional<string> getRemarks() const noexcept;
    void setRemarks(const string&);
};
