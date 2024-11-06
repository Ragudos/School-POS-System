#include <contrib.hpp>
#include <state.hpp>

using namespace std;

MenuItem::MenuItem(string id, double price) : id(id), price(price), qty(0) {}
MenuItem::MenuItem(string id, double price, string desc)
    : id(id), price(price), description(desc), qty(0) {}
// hi
string MenuItem::getId() const noexcept { return id; }

void MenuItem::setDescription(string desc) { description = desc; }

string MenuItem::getDescription() const noexcept { return description; }

uint8_t MenuItem::getQty() const noexcept { return qty; }

void MenuItem::increaseQty(uint8_t amount) {
   qty+=amount;
}

void MenuItem::decreaseQty(uint8_t amount) {
    if(qty==0)
        return;
    if(qty<amount)
        throw logic_error("qty<amount");

    qty-=amount;
}

void MenuItem::resetQty() noexcept { qty=0; }

double MenuItem::getPrice() const noexcept { return price; }

double MenuItem::calculateSubTotal() const noexcept {
    throw logic_error("unimplemented");
}

double calculateChange(double givenCash, double price) {
    return givenCash - price;
}

double calculateTotalOfChosenMenuItems(const vector<MenuItem>& items) {
    double total = 0.0;
    for (const auto& item : items) {
        total += item.calculateSubTotal();
    }
    return total;
}

void State::appendMenuItem(MenuItem menuItem) { menuItems.push_back(menuItem); }

void State::removeMenuItemWithId(string itemId) {
    throw logic_error("unimplemented");
}

optional<const MenuItem*> State::getMenuItemWithId(const string& itemId) {
    for (size_t i = 0, l = menuItems.size(); i < l; ++i) {
        const MenuItem* item = &menuItems.at(i);

        if (item->getId() == itemId) {
            return item;
        }
    }

    return nullopt;
}

void State::setSelectedMenuItemId(string id) {
    for (const auto& item : menuItems) {
        if (item.getId() == id) {
            selectedMenuItemId = id;
            return;
        }
    }

    assert(false || !"State::setSelectedMenuItemId() received an id that does not exist in menuItems!");
}

string State::getSelectedMenuItemId() const noexcept {
    return selectedMenuItemId;
}

const vector<MenuItem>& State::getMenuItems() const noexcept {
    return menuItems;
}

string formatNumber(double num) { return to_string(num); }
