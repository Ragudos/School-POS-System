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
#include <string>
#include <utils.hpp>
#include <vector>

using namespace std;
using namespace filesystem;
using namespace string_utils;

enum OrderState { PENDING, FINISHED, CANCELLED };

class Order {
   private:
    vector<MenuItem> items;
    double totalPrice;
    string orderUid;
    OrderState orderState;

    double calculateTotalPrice();

   public:
    Order(const vector<MenuItem>&);
    Order(const vector<MenuItem>&, const OrderState&);

    const vector<MenuItem>& getItems() const noexcept;

    double getTotalPrice() const noexcept;

    string getOrderUid() const noexcept;

    OrderState getOrderState() const noexcept;
    string getOrderStateString() const noexcept;
    void updateOrderState(const OrderState&) noexcept;
};

string orderStateToString(const OrderState&) noexcept;
OrderState orderStateFromString(const string&);
Order getOrder(const string&);
void saveOrder(const Order& order);
