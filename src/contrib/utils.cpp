#include <contrib/utils.hpp>

double calculateChange(const double& payment, const double& price) {
    return payment - price;
}

double calculateTotalOfChosenMenuItems(const vector<MenuItem>& items) {
    double total = 0.0;

    for (const auto& item : items) {
        total += item.calculateSubTotal();
    }

    return total;
}

/** === For now, we just turn them to string  ===  **/
string formatNumber(const int& num) { return to_string(num); }
string formatNumber(const double& num) { return to_string(num); }
string formatNumber(const double& num, const int& precision) {
    return to_string(num);
}
