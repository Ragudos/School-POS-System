#include <contrib/menu.hpp>

MenuItemSizeData::MenuItemSizeData(const MenuItemSizes& s, const string& desc) {
    size = s;
    description = desc;
}

MenuItemSizes MenuItemSizeData::getSize() const noexcept { return size; }

string toString(MenuItemSizes size) noexcept {
    switch (size) {
        case MenuItemSizes::TALL:
            return "TALL";
        case MenuItemSizes::GRANDE:
            return "GRANDE";
        case MenuItemSizes::VENTI:
            return "VENTI";
        case MenuItemSizes::TRENTA:
            return "TRENTA";
    }

    return "";
}

MenuItemAddonData::MenuItemAddonData(const string& n, const double& p)
    : name(n), price(p) {}
MenuItemAddonData::MenuItemAddonData(const string& n, const double& p,
                                     const string& desc)
    : name(n), price(p) {
    setDescription(desc);
}

string MenuItemAddonData::getName() const noexcept { return name; }

double MenuItemAddonData::getPrice() const noexcept { return price; }

string MenuItemAddonData::getDescription() const noexcept {
    return description;
}

void MenuItemAddonData::setDescription(const string& desc) {
    assert(desc.size() <= 50);

    description = desc;
}

MenuItemAddon::MenuItemAddon(const string& menuItemUid,
                             const MenuItemAddonData& data)
    : menuItemUid(menuItemUid),
      name(data.getName()),
      price(data.getPrice()),
      qty(1) {}

string MenuItemAddon::getName() const noexcept { return name; }

double MenuItemAddon::getPrice() const noexcept { return price; }

string MenuItemAddon::getMenuItemUid() const noexcept { return menuItemUid; }

MenuItemData::MenuItemData(const string& n, const double& bp)
    : name(kebabToPascal(n)), basePrice(bp) {}
MenuItemData::MenuItemData(const string& n, const double& bp,
                           const string& desc)
    : name(kebabToPascal(n)), basePrice(bp) {
    setDescription(desc);
}

string MenuItemData::getName() const noexcept { return name; }

string MenuItemData::getDescription() const noexcept { return description; }

void MenuItemData::setDescription(const string& desc) {
    assert(desc.size() <= 50);

    description = desc;
}

double MenuItemData::getBasePrice() const noexcept { return basePrice; }

MenuItem::MenuItem(const MenuItemData& data)
    : uid(genRandomID(8)),
      name(data.getName()),
      basePrice(data.getBasePrice()),
      size(MenuItemSizes::TALL),
      qty(1) {}

MenuItem::MenuItem(const MenuItemData& data, const MenuItemSizes& size)
    : uid(genRandomID(8)),
      name(data.getName()),
      basePrice(data.getBasePrice()),
      size(size),
      qty(1) {}

string MenuItem::getUid() const noexcept { return uid; }

string MenuItem::getName() const noexcept { return name; }

double MenuItem::getBasePrice() const noexcept { return basePrice; }

MenuItemSizes MenuItem::getSize() const noexcept { return size; }
void MenuItem::setSize(MenuItemSizes s) noexcept { size = s; }

uint8_t MenuItem::getQty() const noexcept { return qty; }

void MenuItem::increaseQty(const uint8_t& amount) { qty += amount; }

void MenuItem::decreaseQty(const uint8_t& amount) {
    if (qty == 0) {
        return;
    }

    assert(amount <= qty);

    qty -= amount;
}

void MenuItem::resetQty() noexcept { qty = 0; }

optional<string> MenuItem::getRemarks() const noexcept { return remarks; }

void MenuItem::setRemarks(const string& remark) {
    assert(remark.size() <= 50);

    remarks = remark;
}
