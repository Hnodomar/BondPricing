#include "flattermbond.hpp"

using namespace BondLibrary;

FlatTermBond::FlatTermBond(double face_value, double coupon, const Date maturity_date,
 const Date issue_date, const CashFlowsPy& cashflows, Date settlement_date)
    : BaseBond(face_value, coupon, maturity_date, issue_date, cashflows, settlement_date)
{}

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
    double npv = 0.0;
    for (auto i = 0; i < cashflows_.size(); ++i) {
        if (date > cashflows_[i].due_date) continue;
        npv += cashflows_[i].cashflow / (pow(1 + rate, i));
    }
}
