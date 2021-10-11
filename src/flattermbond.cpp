#include "flattermbond.hpp"
#include <iostream>

using namespace BondLibrary;

FlatTermBond::FlatTermBond(double face_value, double coupon, const Utils::Date maturity_date,
 const Utils::Date issue_date, const CashFlowsPy& cashflows, Utils::Date settlement_date,
 const Utils::DayCountConvention daycount_convention)
    : BaseBond(face_value, coupon, maturity_date, issue_date, cashflows, settlement_date, daycount_convention)
{}

double FlatTermBond::cleanPrice(const double rate, const Utils::Date date) const {
    //if (isExpired()) return 0.0;
    return notionalPresentValue(rate, date);
}

double FlatTermBond::dirtyPrice(const double rate, const Utils::Date date) const {
    //if (isExpired()) return 0.0;
    return notionalPresentValue(rate, date) + accruedAmount(date);
}

double FlatTermBond::dirtyPriceFromCleanPrice(const double market_price, const Utils::Date date) const {
    //if (isExpired()) return 0.0;
    return market_price + accruedAmount(date);
}

double FlatTermBond::duration(const double rate, const Utils::Date date) const {
    size_t t = 1;
    double duration = 0.0;
    for (const auto& cashflow : cashflows_) {
        if (cashflow.due_date < date) continue;
        duration += (t * cashflow.cashflow) / pow((1 + rate), t);
        ++t;
    }
    return round((duration / notionalPresentValue(rate, date)) * 100) / 100;
}

double FlatTermBond::notionalPresentValue(const double rate, Utils::Date date) const {
    double npv = 0.0;
    auto t = 1;
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date < date) continue;
        npv += cashflows_[i].cashflow / (pow(1 + rate, t++));
    }
    return round(npv * 100.0) / 100.0; 
}
