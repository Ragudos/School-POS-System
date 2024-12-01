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
#include <contrib/storage.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

using namespace std;

void initializeMenuItemSelectData();
void initializeMenuItemSizesSelectData();
void initializeMenuItemAddonSelectData();
void initializeAdminMenuOptions();

/*struct AdminCredentials {
   private:
    string username;
    string password;

   public:
    string getUsername() const noexcept;
    void setUsername(const string&);

    string getPassword() const noexcept;
    void setPassword(const string&);
};*/

struct AdminMenuOption {
   private:
    string name;
    string description;

   public:
    AdminMenuOption(const string&, const string&);

    string getName() const noexcept;
    void setName(const string&);

    string getDescription() const noexcept;
    void setDescription(const string&);
};

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

    string selectedCartMenuItemInOrderConfirmation;

    optional<Order> orderInfo;

    string selectedAdminMenuOptionName;
    vector<AdminMenuOption> adminMenuOptions;

    /*AdminCredentials adminLoginInput;
    string adminLoginError;*/

    string orderIdInput;

   public:
    void appendMenuItemData(const MenuItemData&);
    void removeMenuItemDataWithName(const string&);

    void appendMenuItemToCart(const MenuItem&);
    void removeMenuItemFromCartWithUid(const string&);

    void appendMenuItemSizeData(const MenuItemSizeData&);
    void removeMenuItemSizeData(const MenuItemSizes&);

    void appendMenuItemAddonData(const MenuItemAddonData&);
    void removeMenuItemAddonData(const string&);

    void appendAdminMenuOption(const AdminMenuOption&);
    void removeAdminMenuOption(const string&);

    optional<MenuItemData> getMenuItemDataWithName(const string&);
    optional<MenuItem*> getMenuItemWithUid(const string&);
    optional<MenuItemSizeData> getSelectedMenuItemSizeName(const string&);
    optional<AdminMenuOption> getSelectedAdminMenuOptionName(const string&);
    optional<MenuItemAddonData> getMenuItemAddonDataWithName(const string&) const;

    string getSelectedMenuItemDataName() const noexcept;
    void setSelectedMenuItemDataName(const string&);

    string getSelectedMenuItemInCartUid() const noexcept;
    void setSelectedMenuItemInCartUid(const string&);
    void resetSelectedMenuItemInCartUid();

    string getSelectedMenuItemSizeName() const noexcept;
    void setSelectedMenuItemSizeName(const string&);
    void resetSelectedMenuItemSizeName();

    string getselectedMenuItemAddonData() const noexcept;
    void setselectedMenuItemAddonData(const string&);

    string getSelectedAdminMenuOptionName() const noexcept;
    void setSelectedAdminMenuOptionName(const string&);

    optional<Order> getOrderInfo() const noexcept;
    void setOrderInfo(const Order&) noexcept;
    void resetOrderInfo() noexcept;

    string getOrderIdInput() const noexcept;
    void setOrderIdInput(const string&);

    /*AdminCredentials getAdminLoginInput() const noexcept;
    void setAdminLoginInput(const AdminCredentials&);
    void resetAdminLoginInput() const noexcept;

    string getAdminLoginError() const noexcept;
    void setAdminLoginError(const string&);
    void resetAdminLoginError();*/

    const vector<MenuItemData>& getMenuItemsData() const noexcept;

    const vector<MenuItem>& getMenuItemsInCart() const noexcept;
    void clearMenuItemsInCart() noexcept;

    const vector<MenuItemSizeData>& getMenuItemSizesData() const noexcept;

    const vector<AdminMenuOption>& getAdminMenuOptions() const noexcept;

    const vector<MenuItemAddonData>& getMenuItemAddonData() const noexcept;
};

State& getState() noexcept;
void initializeState();
