#include "generaltermbond.hpp"

using namespace BondLibrary;

GeneralTermBond::GeneralTermBond(double face_value, double coupon, const Date maturity_date,
 const Date issue_date, const CashFlowsPy& cashflows, const Date settlement_date, YieldCurve& yield_curve)
  : BaseBond(face_value, coupon, maturity_date, issue_date, cashflows, settlement_date)
  , yield_curve_(yield_curve)
{}

double GeneralTermBond::cleanPrice(const Date date) const {
    if (isExpired()) return 0.0;
    return notionalPresentValue(0, date);
}

double GeneralTermBond::dirtyPrice(const Date date) const {
    if (isExpired()) return 0.0;
    return notionalPresentValue(0, date) + accruedAmount(date);
}

double GeneralTermBond::dirtyPrice(const double market_price, const Date date) const {
    if (isExpired()) return 0.0;
    return market_price + accruedAmount(date);
}

double GeneralTermBond::notionalPresentValue(const double, Date date) const {
    double npv = 0.0;
    Date time = 0;
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (date > cashflows_[i].due_date) continue;
        time = cashflows_[i].due_date;
        npv += discountFactorFromYield(performLinearInterpolation(time), time) * cashflows_[i].cashflow;
    }
    return npv;
}

double GeneralTermBond::duration(const double, Date date) const {
    double s = 0.0, d1 = 0.0;
    double dfactor = 0.0;
    Date time = 0;
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (date > cashflows_[i].due_date) continue;
        time = cashflows_[i].due_date;
        dfactor = discountFactorFromYield(performLinearInterpolation(time), time);
        s += cashflows_[i].cashflow * dfactor;
        d1 += time * cashflows_[i].cashflow * dfactor;
    }
    return d1 / s;
}

double GeneralTermBond::duration(const Date date) const {
    return duration(0, date);
}

double GeneralTermBond::discountFactorFromYield(const double rate, const Date time) const {
    return exp(-rate * time);
}

double GeneralTermBond::performLinearInterpolation(const Date time) const {
    const auto yield_curve = yield_curve_.getYieldCurve(); // Yields are in increasing time to maturity order
    if (yield_curve.size() < 1) return 0.0;
    auto t_min = yield_curve[0].maturity;
    const auto& curve_max = yield_curve[yield_curve.size() - 1];
    if (time <= t_min) {
        return yield_curve[0].yield;
    }    
    else if (time >= curve_max.maturity) {
        return curve_max.yield;
    }
    int t = 1;
    while (t < yield_curve.size() && time > yield_curve[t].maturity)
        ++t;
    double lambda = (yield_curve[t].maturity - time) / (yield_curve[t].maturity - yield_curve[t - 1].maturity);
    return yield_curve[t - 1].yield * lambda + yield_curve[t].yield * (1.0 - lambda);
}

/*
double Bond::yieldToMaturity(const double bond_price, const Date date) const {
    double low_bound = 0, up_bound = 1.0;
    size_t max_iterations = 200;
    double precision = 1e-5;
    while (discreteBondPrice(up_bound, date) > bond_price)
        up_bound *= 2;
    double rate_approx = 0.5 * up_bound;
    for (auto i = 0; i < max_iterations; ++i) {
        double diff = discreteBondPrice(rate_approx, date) - bond_price;
        if (fabs(diff) < precision)
            return rate_approx;
        if (diff > 0.0)
            low_bound = rate_approx;
        else
            up_bound = rate_approx;
        rate_approx = 0.5 * (up_bound + low_bound);
    }
    return rate_approx;
}*/
