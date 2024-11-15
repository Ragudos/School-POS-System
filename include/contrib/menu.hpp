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

enum MenuItemSizes { TALL, GRANDE, VENTI, TRENTA };

struct MenuItemAddonData {
   private:
    string id;
    double additionalPrice;
    string description;

   public:
    MenuItemAddonData(const string&, const double&);
    MenuItemAddonData(const string&, const double&, const string&);

    string getId() const noexcept;

    double getAdditionalPrice() const noexcept;

    string getDescription() const noexcept;
    void setDescription(const string&);
};

struct MenuItemAddon {
   private:
    string id;
    double additionalPrice;
    /**
     * Must not exceed the qty
     * of MenuItem this addon is referencing.
     **/
    uint8_t qtyOfMenuItem;

   public:
    MenuItemAddon(const MenuItemAddonData&);
};

struct MenuItemSizeData {
   private:
    MenuItemSizes size;
    double additionalPrice;
    string description;

   public:
    MenuItemSizeData(const MenuItemSizes&, const double&);
    MenuItemSizeData(const MenuItemSizes&, const double&, const string&);

    MenuItemSizes getSize() const noexcept;

    double getPrice() const noexcept;

    string getDescription() const noexcept;
    void setDescription(const string&);
};

struct MenuItemSize {
   private:
    MenuItemSizes size;
    double additionalPrice;
    /**
     * Must not exceed the qty
     * of MenuItem this size is referencing that
     * DOES NOT YET have
     * a specified size.
     *
     * (i.e. a different size, VENTI, has qtyOfMenuItem of 6
     * out of 10 qty of MenuItem, so only 4 can be occupied by this
     * MenuItemSize)
     */
    uint8_t qtyOfMenuItem;

   public:
    MenuItemSize(const MenuItemSizeData&);
};

class MenuItem {
   private:
    string id;
    double price;
    string description;
    uint8_t qty;
    vector<MenuItemAddon> addons;
    vector<MenuItemSize> sizes;

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
