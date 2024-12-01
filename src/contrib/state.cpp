#include <contrib/state.hpp>

static unique_ptr<State> state;

State& getState() noexcept { return *state; }

void initializeState() {
    assert(!state || !"State must only be initialized once");

    state = make_unique<State>();

	initializeMenuItemSelectData();
	initializeMenuItemSizesSelectData();
    initializeMenuItemAddonSelectData();
    initializeAdminMenuOptions();
}

void initializeMenuItemSizesSelectData() {
	State& state = getState();

    MenuItemSizeData tall(
        MenuItemSizes::TALL, "A tall size",
        getAdditionalPriceForMenuItemSize(MenuItemSizes::TALL));
    MenuItemSizeData venti(
        MenuItemSizes::VENTI, "A venti size",
        getAdditionalPriceForMenuItemSize(MenuItemSizes::GRANDE));
    MenuItemSizeData grande(
        MenuItemSizes::GRANDE, "A grande size",
        getAdditionalPriceForMenuItemSize(MenuItemSizes::VENTI));
    MenuItemSizeData trenta(
        MenuItemSizes::TRENTA, "A trenta size",
        getAdditionalPriceForMenuItemSize(MenuItemSizes::TRENTA));

    state.appendMenuItemSizeData(tall);
    state.appendMenuItemSizeData(venti);
	state.appendMenuItemSizeData(grande);
	state.appendMenuItemSizeData(trenta);
}

void initializeMenuItemSelectData() {
	State& state = getState();

    MenuItemData item1("cafe-americano", 90,
                       "A classic coffee made with rich espresso and hot "
                       "water, offering a bold and robust flavor.");
    MenuItemData item2(
        "cafe-latte", 110,
        "Smooth and creamy espresso blended with steamed milk, topped with a "
        "light layer of foam for a comforting coffee experience.");
    MenuItemData item3(
        "cappucino", 110,
        "A balanced mix of rich espresso, steamed milk, and a generous topping "
        "of velvety foam, perfect for coffee enthusiasts.");
    MenuItemData item4(
        "iced-americano", 90,
        "A refreshing twist on a classic, with espresso poured over "
        "ice and water for a crisp, bold flavor.");
    MenuItemData item5(
        "iced-cafe-latte", 110,
        "Chilled espresso combined with cold milk and served over "
        "ice, offering a smooth and creamy refreshment.");
    MenuItemData item6(
        "iced-spanish-latte", 130,
        "A luxurious mix of espresso, sweetened milk, and a touch "
        "of ice, delivering a rich and indulgent flavor profile.");
    MenuItemData item7(
        "coffee-jelly", 130,
        "A delightful treat of sweetened coffee jelly paired with "
        "creamy milk and espresso for a unique coffee experience.");
    MenuItemData item8(
        "caramel-bliss", 130,
        "A heavenly blend of espresso, milk, and caramel, topped "
        "with whipped cream for a sweet and indulgent delight.");
    MenuItemData item9(
        "mocha-frappe", 130,
        "A chilled and creamy coffee treat combining espresso, chocolate, and "
        "milk, blended with ice for a decadent, chocolatey flavor.");
    MenuItemData item10(
        "java-chip", 130,
        "A delightful fusion of espresso, milk, chocolate, and crunchy "
        "coffee-infused chips, blended for a rich, textural experience.");

    state.appendMenuItemData(item1);
    state.appendMenuItemData(item2);
    state.appendMenuItemData(item3);
    state.appendMenuItemData(item4);
    state.appendMenuItemData(item5);
    state.appendMenuItemData(item6);
    state.appendMenuItemData(item7);
    state.appendMenuItemData(item8);
    state.appendMenuItemData(item9);
    state.appendMenuItemData(item10);
}

void initializeMenuItemAddonSelectData()
{
    State& state = getState();

    MenuItemAddonData addon1(
        "EXPRESSO SHOT", 30);
    MenuItemAddonData addon2(
        "WHIPPED CREAM", 25);
    MenuItemAddonData addon3(
        "MILK", 25);
    MenuItemAddonData addon4(
        "CHOCOLATE SYRUP", 25);
    MenuItemAddonData addon5(
        "CARAMEL DRIZZLE", 25);

    state.appendMenuItemAddonData(addon1);
    state.appendMenuItemAddonData(addon2);
	state.appendMenuItemAddonData(addon3);
	state.appendMenuItemAddonData(addon4);
    state.appendMenuItemAddonData(addon5);
}

void initializeAdminMenuOptions() {
    State& state = getState();

    AdminMenuOption option1("Review Order",
                            "Review an order by ID and edit it.");
    AdminMenuOption option2("Daily Sales", "Sales information daily.");
    AdminMenuOption option3("Monthly Sales", "Sales information monthly.");
    AdminMenuOption option4("Yearly Sales", "Sales information yearly.");

    state.appendAdminMenuOption(option1);
    state.appendAdminMenuOption(option2);
    state.appendAdminMenuOption(option3);
    state.appendAdminMenuOption(option4);
}

AdminMenuOption::AdminMenuOption(const string& n, const string& desc)
    : name(n), description(desc) {}

string AdminMenuOption::getName() const noexcept { return name; }
void AdminMenuOption::setName(const string& n) { name = n; }

string AdminMenuOption::getDescription() const noexcept { return description; }
void AdminMenuOption::setDescription(const string& desc) { description = desc; }

/*string AdminCredentials::getUsername() const noexcept { return username; }

void AdminCredentials::setUsername(const string& name) { username = name; }

string AdminCredentials::getPassword() const noexcept { return password; }

void AdminCredentials::setPassword(const string& pass) { password = pass; }*/

void State::appendMenuItemData(const MenuItemData& menuItem) {
    for (auto item : menuItemsData) {
        assert(item.getName() != menuItem.getName());
    }

    menuItemsData.push_back(menuItem);
}

void State::removeMenuItemDataWithName(const string& itemName) {
    menuItemsData.erase(remove_if(menuItemsData.begin(), menuItemsData.end(),
                                  [&itemName](const MenuItemData& data) {
                                      return data.getName() == itemName;
                                  }),
                        menuItemsData.end());
}

void State::appendMenuItemToCart(const MenuItem& item) {
    for (auto product : cart) {
        assert(product.getUid() != item.getUid());
    }

    cart.push_back(item);
}

void State::removeMenuItemFromCartWithUid(const string& uid) {
    cart.erase(remove_if(cart.begin(), cart.end(),
                         [&uid](const MenuItem& item) {
                             return item.getUid() == uid;
                         }),
               cart.end());
}

void State::appendMenuItemSizeData(const MenuItemSizeData& sizeData) {
	for (auto size : menuItemSizesData) {
		assert(size.getSize() != sizeData.getSize());
	}

	menuItemSizesData.push_back(sizeData);
}

void State::removeMenuItemSizeData(const MenuItemSizes& size) {
	menuItemSizesData.erase(remove_if(menuItemSizesData.begin(), menuItemSizesData.end(),
                         [&size](const MenuItemSizeData& sizeData) {
                             return sizeData.getSize() == size;
                         }),
               menuItemSizesData.end());
}

void State::appendMenuItemAddonData(const MenuItemAddonData& n)
{
    for (auto& name : menuItemAddonData) {
        assert(name.getName() != n.getName());
    }

    menuItemAddonData.push_back(n);
}

void State::removeMenuItemAddonData(const string& n)
{
    menuItemAddonData.erase(
        remove_if(menuItemAddonData.begin(), menuItemAddonData.end(),
                  [&n](const MenuItemAddonData& item) {
                      return item.getName() == n;
                  }),
        menuItemAddonData.end());
}

void State::appendAdminMenuOption(const AdminMenuOption& option) {
    for (auto& opt : adminMenuOptions) {
        assert(opt.getName() != option.getName());
    }

    adminMenuOptions.push_back(option);
}

void State::removeAdminMenuOption(const string& optionName) {
    adminMenuOptions.erase(
        remove_if(adminMenuOptions.begin(), adminMenuOptions.end(),
                  [&optionName](const AdminMenuOption& option) {
                      return option.getName() == optionName;
                  }),
        adminMenuOptions.end());
}

optional<MenuItemData> State::getMenuItemDataWithName(const string& itemName) {
    for (auto item : menuItemsData) {
        if (item.getName() == itemName) {
            return item;
        }
    }

    return nullopt;
}

optional<MenuItem*> State::getMenuItemWithUid(const string& uid) {
    for (size_t i = 0, l = cart.size(); i < l; ++i) {
        MenuItem* item = &cart.at(i);

        if (item->getUid() == uid) {
            return item;
        }
    }

    return nullopt;
}

optional<MenuItemSizeData> State::getSelectedMenuItemSizeName(const string& sizeName) {
    for (auto& item : menuItemSizesData) {
        if (item.getSize() == fromString(sizeName)) {
            return item;
        }
    }

    return nullopt;
}

optional<AdminMenuOption> State::getSelectedAdminMenuOptionName(
    const string& name) {
    for (auto& option : adminMenuOptions) {
        if (option.getName() == name) {
            return option;
        }
    }

    return nullopt;
}

optional<MenuItemAddonData> State::getMenuItemAddonDataWithName(const string& addonName) const {
    for (const auto& addon : menuItemAddonData) {
        if (addon.getName() == addonName) {
            return addon;
        }
    }
    return nullopt;
}

string State::getSelectedMenuItemDataName() const noexcept {
    return selectedMenuItemDataName;
}

void State::setSelectedMenuItemDataName(const string& name) {
    for (const auto& item : menuItemsData) {
        if (item.getName() == name) {
            selectedMenuItemDataName = name;

            return;
        }
    }

    assert(false ||
           "State::setSelectedMenuItemDataName() received a name that's not in "
           "menuItemsData");
}

string State::getSelectedMenuItemInCartUid() const noexcept {
    return selectedMenuItemInCartUid;
}

void State::setSelectedMenuItemInCartUid(const string& uid) {
    for (const auto& item : cart) {
        if (item.getUid() == uid) {
            selectedMenuItemInCartUid = uid;

            return;
        }
    }

    assert(false ||
           "State::setSelectedMenuItemInCartUid() received a uid that's not in "
           "cart");
}

void State::resetSelectedMenuItemInCartUid() { selectedMenuItemInCartUid = ""; }

string State::getSelectedMenuItemSizeName() const noexcept
{
    return selectedMenuItemSizeName;
}

void State::setSelectedMenuItemSizeName(const string& s) {
    for (const auto& size : menuItemSizesData) {
        if (size.getSize() == fromString(s)) {
            selectedMenuItemSizeName = s;

            return;
        }
    }

    assert(false ||
           "State::setSelectedMenuItemSizeName() received a name that's not in "
           "menuItemSizesData");
}

void State::resetSelectedMenuItemSizeName() { selectedMenuItemSizeName = ""; }

string State::getselectedMenuItemAddonData() const noexcept
{
    return selectedMenuItemAddonData;
}
void State::setselectedMenuItemAddonData(const string& n)
{
    for (const auto& name : menuItemAddonData) {
        if (name.getName() == n) {
            selectedMenuItemAddonData = n;

            return;
        }
    }

    assert(false ||
           "State::getselectedMenuItemAddonData() received a name that's not in "
           "menuItemAddonData");
}

string State::getSelectedAdminMenuOptionName() const noexcept {
    return selectedAdminMenuOptionName;
}

void State::setSelectedAdminMenuOptionName(const string& n) {
    for (const auto& option : adminMenuOptions) {
        if (option.getName() == n) {
            selectedAdminMenuOptionName = n;

            return;
        }
    }

    assert(
        false ||
        "State::getSelectedAdminMenuOptionName() received a name that's not in "
        "menuItemAddonData");
}

optional<Order> State::getOrderInfo() const noexcept { return orderInfo; }

void State::setOrderInfo(const Order& orderI) noexcept { orderInfo = orderI; }

void State::resetOrderInfo() noexcept { orderInfo = nullopt; }

string State::getOrderIdInput() const noexcept { return orderIdInput; }

void State::setOrderIdInput(const string& str) { orderIdInput = str; }

const vector<MenuItemData>& State::getMenuItemsData() const noexcept {
    return menuItemsData;
}

const vector<MenuItem>& State::getMenuItemsInCart() const noexcept {
    return cart;
}

void State::clearMenuItemsInCart() noexcept {
    cart.clear();
    resetSelectedMenuItemInCartUid();
}

const vector<MenuItemSizeData>& State::getMenuItemSizesData() const noexcept {
    return menuItemSizesData;
}

const vector<AdminMenuOption>& State::getAdminMenuOptions() const noexcept {
    return adminMenuOptions;
}

const vector<MenuItemAddonData>& State::getMenuItemAddonData() const noexcept {
    return menuItemAddonData;
}
