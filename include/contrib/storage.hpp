#pragma once

#if defined(LINUX_PLATFORM) || defined(MAC_PLATFORM)
#include <unistd.h>

#elif defined(WINDOWS_PLATFORM)
#include <windows.h>

#else
#error "Unsupported Platform!"

#endif

#include <cassert>
#include <contrib/menu.hpp>
#include <contrib/utils.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utils.hpp>
#include <vector>

using namespace std;
using namespace filesystem;
using namespace string_utils;

const double TAX = 0.12;

enum OrderState { PENDING, FINISHED, CANCELLED };

class Order {
   private:
    vector<MenuItem> items;
    string orderUid;
    tm dateCreated;
    OrderState orderState;
    double totalPrice;
    double VAT;

    double calculateTotalPrice();

   public:
    Order(const vector<MenuItem>&);
    Order(const vector<MenuItem>&, const string&);
    Order(const vector<MenuItem>&, const string&, const tm&);
    Order(const vector<MenuItem>&, const string&, const tm&, const OrderState&);
    Order(const vector<MenuItem>&, const string&, const tm&, const OrderState&,
          const double&, const double&);

    const vector<MenuItem>& getItems() const noexcept;

    double getTotalPrice() const noexcept;

    string getOrderUid() const noexcept;

    tm createdAt() const noexcept;

    OrderState getOrderState() const noexcept;
    string getOrderStateString() const noexcept;
    void updateOrderState(const OrderState&) noexcept;

    double getVAT() const noexcept;
};

string orderStateToString(const OrderState&) noexcept;
OrderState orderStateFromString(const string&);

map<string, double> getDailySales();
map<string, double> getMonthlySales();
map<string, double> getYearlySales();
optional<Order> getOrder(const string&);
void saveOrder(const Order& order);
