#pragma once
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)

#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

#include <contrib/menu.hpp>
#include <memory>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

class State {
   private:
    string selectedMenuItemId;
    vector<MenuItem> menuItems;
    vector<MenuItemSizeData> menuItemSizes;
    vector<MenuItemAddonData> menuItemAddons;

   public:
    void appendMenuItem(const MenuItem&);
    void removeMenuItemWithId(const string&);

    /**
     *
     * How many items have a qty > 0
     */
    int amountOfDistinctChosenItems() const noexcept;

    optional<MenuItem*> getMenuItemWithId(const string&);

    string getSelectedMenuItemId() const noexcept;
    void setSelectedMenuItemId(const string&);

    const vector<MenuItem>& getMenuItems() const noexcept;
};

State& getState() noexcept;
void initializeState();
