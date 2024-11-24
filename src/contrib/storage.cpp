#include <contrib/storage.hpp>

Order::Order(const vector<MenuItem>& menuItems)
    : items(menuItems), orderState(OrderState::PENDING) {
    totalPrice = calculateTotalPrice();
    orderUid = genRandomID(8);
};

Order::Order(const vector<MenuItem>& menuItems, const OrderState& orderS)
    : items(menuItems), orderState(orderS) {
    totalPrice = calculateTotalPrice();
    orderUid = genRandomID(8);
};

// TODO: include add-ons price
double Order::calculateTotalPrice() {
    double total = 0.0;

    for (auto& item : items) {
        total += (item.getBasePrice() +
                  getAdditionalPriceForMenuItemSize(item.getSize())) *
                 item.getQty();
    }

    return total;
}

const vector<MenuItem>& Order::getItems() const noexcept { return items; }

double Order::getTotalPrice() const noexcept { return totalPrice; }

string Order::getOrderUid() const noexcept { return orderUid; }

OrderState Order::getOrderState() const noexcept { return orderState; }
string Order::getOrderStateString() const noexcept {
    return orderStateToString(orderState);
}
void Order::updateOrderState(const OrderState& orderS) noexcept {
    orderState = orderS;
}

string orderStateToString(const OrderState& orderState) noexcept {
    switch (orderState) {
        case OrderState::PENDING:
            return "PENDING";
        case OrderState::FINISHED:
            return "FINISHED";
        case OrderState::CANCELLED:
            return "CANCELLED";
    }

    return "";
}

OrderState orderStateFromString(const string& orderState) {
    if (orderState == "PENDING") {
        return OrderState::PENDING;
    }

    if (orderState == "FINISHED") {
        return OrderState::FINISHED;
    }

    if (orderState == "CANCELLED") {
        return OrderState::CANCELLED;
    }

    assert(false);
}

void saveOrder(const Order& order) {
    if (!exists("../storage")) {
        create_directories("../storage");
    }

    bool fileExists = exists("../storage/orders.csv");

    ofstream file("../storage/orders.csv", ios::app);

    assert(file.is_open());

    if (!fileExists) {
        file << "Date Created,Order Uid,Item "
                "Uid,Name,Size,Quantity,Subtotal,Total,Remarks,Order State\n";
    }

    for (auto& item : order.getItems()) {
        file << getCurrentDate() << "," << order.getOrderUid() << ","
             << item.getUid() << "," << item.getName() << ","
             << toString(item.getSize()) << "," << formatNumber(item.getQty())
             << "," << formatDoublePrecision(item.calculateSubtotal()) << ","
             << formatDoublePrecision(order.getTotalPrice()) << ",";

        if (item.getRemarks().has_value()) {
            file << item.getRemarks().value();
        }

        file << "," << order.getOrderStateString() << "\n";
    }

    file.close();
}
