#include <contrib.hpp>

using namespace std;

MenuItem::MenuItem(string id) : id(id) {}

string MenuItem::getId() const noexcept { throw logic_error("unimplemented"); }

void MenuItem::setDescription(string desc) {
    throw logic_error("unimplemented");
}

string MenuItem::getDescription() const noexcept {
    throw logic_error("unimplemented");
}

uint8_t MenuItem::getQty() const noexcept {
    throw logic_error("unimplemented");
}

void MenuItem::increaseQty(uint8_t amount) {
    throw logic_error("unimplemented");
}

void MenuItem::decreaseQty(uint8_t amount) {
    throw logic_error("unimplemented");
}

void MenuItem::resetQty() noexcept { throw logic_error("unimplemented"); }

double MenuItem::getPrice() const noexcept {
    throw logic_error("unimplemented");
}

double MenuItem::calculateSubTotal() const noexcept {
    throw logic_error("unimplemented");
}

double calculateChange(double givenCash, double price) {
    throw logic_error("unimplemented");
}

double calculateTotalOfChosenMenuItems(const vector<MenuItem>&) {
    throw logic_error("unimplemented");
}

string formatNumber(double num) { throw logic_error("unimplemented"); }
