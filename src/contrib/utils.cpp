#include <contrib/utils.hpp>
#define _CRT_SECURE_NO_WARNINGS

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

string formatDoublePrecision(const double& num) {
    return formatDoublePrecision(num, 2);
}
string formatDoublePrecision(const double& num, const int& precision) {
    ostringstream oss;

    oss << fixed << setprecision(precision) << num;

    return oss.str();
}

string getCurrentDate() {
    auto now = system_clock::now();
    auto time_t_now = system_clock::to_time_t(now);

    ostringstream dateStream;

    dateStream << put_time(localtime(&time_t_now), "%Y-%m-%d");

    return dateStream.str();
}
