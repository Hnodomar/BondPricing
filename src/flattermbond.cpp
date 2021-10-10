#include "flattermbond.hpp"

using namespace BondLibrary;

double FlatTermBond::cleanPrice(const double rate, const Date date) const {
    if (isExpired()) return 0.0;
    return notionalPresentValue(rate, date);
}

double FlatTermBond::dirtyPrice(const double rate, const Date date) const {
    if (isExpired()) return 0.0;
    return notionalPresentValue(rate, date) + accruedAmount(date);
}

double FlatTermBond::dirtyPriceFromCleanPrice(const double market_price, const Date date) const {
    if (isExpired()) return 0.0;
    return market_price + accruedAmount(date);
}

double FlatTermBond::duration(const double rate, const Date date) const {
    size_t t = 1;
    double duration = 0.0;
    for (const auto& cashflow : cashflows_) {
        if (date > cashflow.due_date) continue;
        duration += (t * coupon_) / pow((1 + rate), t);
        ++t;
    }
    return duration / notionalPresentValue(rate, date);
}

double FlatTermBond::notionalPresentValue(const double rate, Date date) const {
    size_t i = 0;
    for (; i < cashflows_.size(); ++i)
        if (date <= cashflows_[i].due_date) break;
    if (i == cashflows_.size()) return 0.0;
    double unoccurred_payments = cashflows_.size() - 1 - i;
    return (rate * face_value_) 
        * ((1 - pow((1 + rate), -unoccurred_payments)) / rate)
        + (face_value_ + (rate * face_value_)) * pow((1 + rate), -unoccurred_payments);
}
