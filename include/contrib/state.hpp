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

void initializeMenuItemSelectData();
void initializeMenuItemSizesSelectData();

class State {
   private:
    string selectedMenuItemDataName;
    vector<MenuItemData> menuItemsData;

    string selectedMenuItemInCartUid;
    vector<MenuItem> cart;

    string selectedMenuItemSizeName;
    vector<MenuItemSizeData> menuItemSizesData;

    string selectedMenuItemAddonData;
    vector<MenuItemAddonData> menuItemAddonData;

   public:
    void appendMenuItemData(const MenuItemData&);
    void removeMenuItemDataWithName(const string&);

    void appendMenuItemToCart(const MenuItem&);
    void removeMenuItemFromCartWithUid(const string&);

    void appendMenuItemSizeData(const MenuItemSizeData&);
    void removeMenuItemSizeData(const MenuItemSizes&);

    void appendMenuItemAddonData(const MenuItemAddonData&);
    void removeMenuItemAddonData(const string&);

    optional<MenuItemData> getMenuItemDataWithName(const string&);
    optional<MenuItem*> getMenuItemWithUid(const string&);
    optional<MenuItemSizeData> getSelectedMenuItemSizeName(const string&);

    string getSelectedMenuItemDataName() const noexcept;
    void setSelectedMenuItemDataName(const string&);
    string getSelectedMenuItemInCartUid() const noexcept;
    void setSelectedMenuItemInCartUid(const string&);
    void resetSelectedMenuItemInCartUid();
    string getSelectedMenuItemSizeName() const noexcept;
    void setSelectedMenuItemSizeName(const string&);
    string getselectedMenuItemAddonData() const noexcept;
    void setselectedMenuItemAddonData(const string&);

    const vector<MenuItemData>& getMenuItemsData() const noexcept;
    const vector<MenuItem>& getMenuItemsInCart() const noexcept;
    const vector<MenuItemSizeData>& getMenuItemSizesData() const noexcept;
};

State& getState() noexcept;
void initializeState();
