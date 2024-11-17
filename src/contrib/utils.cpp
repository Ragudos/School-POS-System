#include <contrib/utils.hpp>

double calculateChange(const double& payment, const double& price) {
    return payment - price;
}

double calculateTotalOfChosenMenuItems() { throw logic_error("unimplemented"); }

/** === For now, we just turn them to string  ===  **/
string formatNumber(const int& num) { return to_string(num); }
string formatNumber(const double& num) { return to_string(num); }
string formatNumber(const double& num, const int& precision) {
    return to_string(num);
}
