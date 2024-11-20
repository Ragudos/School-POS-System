#pragma once
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)

#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

#include <algorithm>
#include <contrib/menu.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

class State {
   private:
    string selectedMenuItemDataName;
    vector<MenuItemData> menuItemsData;

    string selectedMenuItemInCartUid;
    vector<MenuItem> cart;

   public:
    void appendMenuItemData(const MenuItemData&);
    void removeMenuItemDataWithName(const string&);

    void appendMenuItemToCart(const MenuItem&);
    void removeMenuItemFromCartWithUid(const string&);

    optional<MenuItemData> getMenuItemDataWithName(const string&);
    optional<MenuItem*> getMenuItemWithUid(const string&);

    string getSelectedMenuItemDataName() const noexcept;
    void setSelectedMenuItemDataName(const string&);
    string getSelectedMenuItemInCartUid() const noexcept;
    void setSelectedMenuItemInCartUid(const string&);
    void resetSelectedMenuItemInCardUid();

    const vector<MenuItemData>& getMenuItemsData() const noexcept;
    const vector<MenuItem>& getMenuItemsInCart() const noexcept;
};

State& getState() noexcept;
void initializeState();
