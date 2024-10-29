#pragma once
#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)

#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported platform!"

#endif

#include <contrib.hpp>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

/** === Global state === */

class State {
   private:
    string selectedMenuItemId;
    vector<MenuItem> menuItems;

   public:
    void appendMenuItem(MenuItem);
    void removeMenuItemWithId(string);

   public:
    optional<const MenuItem*> getMenuItemWithId(const string&);
    string getSelectedMenuItemId() const noexcept;
    void setSelectedMenuItemId(string);
    const vector<MenuItem>& getMenuItems() const noexcept;
};

State& getState() noexcept;
void initializeState();
