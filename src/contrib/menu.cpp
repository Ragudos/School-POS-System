
#include <contrib/menu.hpp>

 MenuItemSizes MenuItemSizeData::getSize() const noexcept {return size; }

 double MenuItemSizeData::getPrice() const noexcept{return additionalPrice;}

 string MenuItemSizeData::getDescription() const noexcept{return description;}

MenuItem::MenuItem(const string& id, const double& price)
    : id(id), price(price), qty(0) {}
MenuItem::MenuItem(const string& id, const double& price,
                   const string& description)
    : id(id), price(price), qty(0) {
    setDescription(description);
}

string MenuItem::getId() const noexcept { return id; }

string MenuItem::getDescription() const noexcept { return description; }

void MenuItem::setDescription(const string& desc) {
    assert(desc.size() <= MAX_MENU_ITEM_DESCRIPTION_LENGTH ||
           "MenuItem::setDescription() max description size.");

    description = desc;
}

uint8_t MenuItem::getQty() const noexcept { return qty; }

void MenuItem::increaseQty(uint8_t amount) {
    assert(qty < 256 || "MenuItem::increaseQty() max qty reached!");

    qty += amount;
}

void MenuItem::decreaseQty(uint8_t amount) {
    if (qty == 0) {
        return;
    }

    assert(qty >= amount ||
           "MenuItem::decreaseQty() amount exceeds current qty!");

    qty -= amount;
}

void MenuItem::resetQty() noexcept { qty = 0; }

double MenuItem::getPrice() const noexcept { return price; }

double MenuItem::calculateSubTotal() const noexcept {
    price*=;
}
