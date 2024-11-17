#include <contrib/state.hpp>

static unique_ptr<State> state;

State& getState() noexcept { return *state; }

void initializeState() {
    assert(!state || !"State must only be initialized once");

    state = make_unique<State>();

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

    state->appendMenuItemData(item1);
    state->appendMenuItemData(item2);
    state->appendMenuItemData(item3);
    state->appendMenuItemData(item4);
    state->appendMenuItemData(item5);
    state->appendMenuItemData(item6);
    state->appendMenuItemData(item7);
    state->appendMenuItemData(item8);
    state->appendMenuItemData(item9);
    state->appendMenuItemData(item10);
}

void State::appendMenuItemData(const MenuItemData& menuItem) {
    for (auto item : menuItemsData) {
        if (item.getName() == menuItem.getName()) {
            throw logic_error("menuItemData already exists.");
        }
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
        if (product.getUid() == item.getUid()) {
            throw logic_error("item already exists");
        }
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

void State::resetSelectedMenuItemInCardUid() { selectedMenuItemInCartUid = ""; }

const vector<MenuItemData>& State::getMenuItemsData() const noexcept {
    return menuItemsData;
}

const vector<MenuItem>& State::getMenuItemsInCart() const noexcept {
    return cart;
}
