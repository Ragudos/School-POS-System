#include <contrib/utils.hpp>

char MoneyPunct::do_thousands_sep() const { return ','; }

string MoneyPunct::do_grouping() const { return "\3"; }

double calculateChange(const double& payment, const double& price) {
    return payment - price;
}

double calculateTotalOfChosenMenuItems() { throw logic_error("unimplemented"); }

/** === For now, we just turn them to string  ===  **/
string formatNumber(const int& num) { return to_string(num); }
string formatNumber(const double& num) { return formatNumber(num, 2); }
string formatNumber(const double& num, const int& precision) {
    ostringstream oss;

    oss << fixed << setprecision(precision);

    oss.imbue(locale(locale::classic(), new MoneyPunct));
    oss << num;

    return oss.str();
}
