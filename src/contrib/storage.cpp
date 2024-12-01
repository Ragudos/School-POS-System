#include <contrib/storage.hpp>

Order::Order(const vector<MenuItem>& menuItems)
    : items(menuItems),
      dateCreated(parseDate(getCurrentDate())),
      orderState(OrderState::PENDING) {
    totalPrice = calculateTotalPrice();
    orderUid = genRandomID(8);
};

Order::Order(const vector<MenuItem>& menuItems, const string& uid)
    : items(menuItems),
      orderUid(uid),
      dateCreated(parseDate(getCurrentDate())),
      orderState(OrderState::PENDING) {
    totalPrice = calculateTotalPrice();
    orderUid = genRandomID(8);
};

Order::Order(const vector<MenuItem>& menuItems, const string& uid,
             const tm& cat)
    : items(menuItems),
      orderUid(uid),
      dateCreated(cat),
      orderState(OrderState::PENDING) {
    totalPrice = calculateTotalPrice();
};

Order::Order(const vector<MenuItem>& menuItems, const string& uid,
             const tm& cat, const OrderState& orderS)
    : items(menuItems), orderUid(uid), dateCreated(cat), orderState(orderS) {
    totalPrice = calculateTotalPrice();
};

Order::Order(const vector<MenuItem>& menuItems, const string& uid,
             const tm& cat, const OrderState& orderS, const double& totalP,
             const double& vat)
    : items(menuItems),
      orderUid(uid),
      dateCreated(cat),
      orderState(orderS),
      totalPrice(totalP),
      VAT(vat){};

// TODO: include add-ons price
double Order::calculateTotalPrice() {
    double total = 0.0;

    for (auto& item : items) {
        total += (item.getBasePrice() +
                  getAdditionalPriceForMenuItemSize(item.getSize())) *
                 item.getQty();
    }

    VAT = TAX * total;
    total += VAT;

    return total;
}

const vector<MenuItem>& Order::getItems() const noexcept { return items; }

double Order::getTotalPrice() const noexcept { return totalPrice; }

string Order::getOrderUid() const noexcept { return orderUid; }

tm Order::createdAt() const noexcept { return dateCreated; }

OrderState Order::getOrderState() const noexcept { return orderState; }
string Order::getOrderStateString() const noexcept {
    return orderStateToString(orderState);
}
void Order::updateOrderState(const OrderState& orderS) noexcept {
    orderState = orderS;
}

double Order::getVAT() const noexcept { return VAT; }

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

    assert(false || "Invalid string");
}

map<string, double> getDailySales() {
    map<string, double> dailySales;

    ifstream file("../storage/orders.csv");

    assert(file.is_open());

    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream lineSS(line);
        string cell;
        vector<string> row;

        while (getline(lineSS, cell, ',')) {
            row.push_back(cell);
        }

        if (row.empty()) {
            continue;
        }

        string createdAt = row.at(2);
        double orderTotal = stod(row.at(8));

        dailySales[createdAt] += orderTotal;
    }

    return dailySales;
}

map<string, double> getMonthlySales() {
    map<string, double> monthlySales;

    ifstream file("../storage/orders.csv");

    assert(file.is_open());

    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream lineSS(line);
        string cell;
        vector<string> row;

        while (getline(lineSS, cell, ',')) {
            row.push_back(cell);
        }

        if (row.empty()) {
            continue;
        }

        string createdAt = row.at(2).substr(0, 7);
        double orderTotal = stod(row.at(8));

        monthlySales[createdAt] += orderTotal;
    }

    return monthlySales;
}

map<string, double> getYearlySales() {
    map<string, double> yearlySales;

    ifstream file("../storage/orders.csv");

    assert(file.is_open());

    string line;

    getline(file, line);

    while (getline(file, line)) {
        stringstream lineSS(line);
        string cell;
        vector<string> row;

        while (getline(lineSS, cell, ',')) {
            row.push_back(cell);
        }

        if (row.empty()) {
            continue;
        }

        string createdAt = row.at(2).substr(0, 4);
        double orderTotal = stod(row.at(8));

        yearlySales[createdAt] += orderTotal;
    }

    return yearlySales;
}

optional<Order> getOrder(const string& orderUid) {
    vector<MenuItem> menuItems;
    OrderState orderState = OrderState::PENDING;
    tm dateCreated = {};
    double totalPrice = 0;
    double VAT = 0;

    ifstream file("../storage/orders.csv");

    assert(file.is_open());

    string line;

    // skip headers
    getline(file, line);

    while (getline(file, line)) {
        stringstream lineSS(line);
        string cell;
        vector<string> row;

        while (getline(lineSS, cell, ',')) {
            row.push_back(cell);
        }

        if (row.empty()) {
            continue;
        }

        string orderId = row.at(0);

        if (orderId != orderUid) {
            continue;
        }

        string itemId = row.at(1);
        string itemName = row.at(3);
        // todo: handle invalid value with try-catch
        double basePrice = stod(row.at(4));
        MenuItemSizes itemSize = fromString(row.at(5));
        uint8_t itemQty = stoul(row.at(6));
        optional<string> remarks =
            row.at(10).empty() ? nullopt : optional(row.at(9));

        // if first time appending
        if (menuItems.empty()) {
            orderState = orderStateFromString(row.at(10));
            dateCreated = parseDate(row.at(2));
            totalPrice = stod(row.at(8));
            VAT = stod(row.at(9));
        }

        MenuItem item(itemId, itemName, basePrice, itemSize, itemQty, remarks);

        menuItems.push_back(item);
    }

    if (menuItems.empty()) {
        return nullopt;
    }

    Order order(menuItems, orderUid, dateCreated, orderState, totalPrice, VAT);

    return order;
}

void saveOrder(const Order& order) {
    if (!exists("../storage")) {
        create_directories("../storage");
    }

    bool fileExists = exists("../storage/orders.csv");

    ofstream file("../storage/orders.csv", ios::app);

    assert(file.is_open());

    if (!fileExists) {
        file
            << "Order Uid,Item Uid,Date Created,"
               "Name,Base Price,Size,Quantity,Subtotal,Total,VAT,Remarks,Order "
               "State\n";
    }

    for (auto& item : order.getItems()) {
        file << order.getOrderUid() << "," << item.getUid() << ","
             << parseDate(order.createdAt()) << "," << item.getName() << ","
             << formatDoublePrecision(item.getBasePrice()) << ","
             << toString(item.getSize()) << "," << formatNumber(item.getQty())
             << "," << formatDoublePrecision(item.calculateSubtotal()) << ","
             << formatDoublePrecision(order.getTotalPrice()) << ","
             << formatDoublePrecision(order.getVAT()) << ",";

        if (item.getRemarks().has_value()) {
            file << item.getRemarks().value();
        }

        file << "," << order.getOrderStateString() << "\n";
    }

    file.close();
}
