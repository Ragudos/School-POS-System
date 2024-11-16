#include <contrib/menu.hpp>

MenuItemSizes MenuItemSizeData::getSize() const noexcept { return size; }

double MenuItemSizeData::getPrice() const noexcept { return additionalPrice; }

string MenuItemSizeData::getDescription() const noexcept { return description; }

MenuItemSizes MenuItemSizeData::getSize() const noexcept { return size; }

double MenuItemSizeData::getPrice() const noexcept { return additionalPrice; }

string MenuItemSizeData::getDescription() const noexcept { return description; }

MenuItemAddonData::MenuItemAddonData(const string& id,
                                     const double& additionalPrice)
    : id(id), additionalPrice(additionalPrice) {}
MenuItemAddonData::MenuItemAddonData(const string& id,
                                     const double& additionaPrice,
                                     const string& description)
    : id(id), additionalPrice(additionalPrice) {
    setDescription(description);
}

string MenuItemAddonData::getId() const noexcept { return id; }

double MenuItemAddonData::getAdditionalPrice() const noexcept {
    return additionalPrice;
}

string MenuItemAddonData::getDescription() const noexcept {
    return description;
}

void MenuItemAddonData::setDescription(const string& desc) {
    assert(desc.size() <= MAX_MENU_ADDON_DESCRIPTION_LENGTH ||
           "MenuItemAddonData::setDescription() received a long description.");

    description = desc;
}

MenuItemAddon::MenuItemAddon(const MenuItemAddonData& menuItemAddon)
    : id(menuItemAddon.getId()),
      additionalPrice(menuItemAddon.getAdditionalPrice()),
      qtyOfMenuItem(1) {}
MenuItemAddon::MenuItemAddon(const MenuItemAddonData& menuItemAddon,
                             const uint8_t& qty)
    : id(menuItemAddon.getId()),
      additionalPrice(menuItemAddon.getAdditionalPrice()),
      qtyOfMenuItem(qty) {}

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
    throw logic_error("unimplemented");
}
