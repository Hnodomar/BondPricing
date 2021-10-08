#ifndef BOND_HPP
#define BOND_HPP

#include <cstdint>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <optional>

#include "cashflow.hpp"
#include "util.hpp"

namespace BondLibrary {
using Date = int32_t; // date is number of days since unix epoch.. user must translate day-count conventions themselves
using CashFlows = std::vector<CashFlow>;
using CashFlowOpt = std::optional<const CashFlow&>;
class Bond {
public:
    Bond(
        double face_value,
        double coupon,
        const Date maturity_date,
        const Date issue_date,
        const CashFlows& cashflows,
        const Date settlement_date = Utils::getCurrentDaysSinceEpoch() + 2
    );
    double cleanPrice(
        const double yield,
        const Date date,
        const double frequency
    ) const;
    double dirtyPrice(
        const double yield, 
        const Date date, 
        const double frequency,
        const Date settlement_date = Utils::getCurrentDaysSinceEpoch() + 2
    ) const;
    double dirtyPrice(
        const double market_price,
        const Date settlement_date = Utils::getCurrentDaysSinceEpoch() + 2
    );
    double yieldToMaturity(const double bond_price) {return yieldToMaturity(bond_price, issue_date_);}
    double yieldToMaturity(
        const double bond_price,
        const Date date
    ) const;
    double modifiedDuration(const double rate, const Date date) const;
    double duration(const double rate, const Date date) const;
    double getCouponRate() const;
    double getCurrentYield(double market_price) const;
    double accruedAmount(Date settlement) const;
    double yield() const;
    CashFlowOpt getCashFlow(Date date = Utils::getCurrentDaysSinceEpoch()) const;
    CashFlowOpt getNextCashFlow(const CashFlow& cashflow) const;
    CashFlowOpt getPreviousCashFlow(const CashFlow& cashflow) const;
    bool isExpired() const;
private:    
    enum class TermStructure { Flat, General };
    double discreteBondPrice(const double rate, Date date) const;
    bool outOfRangeOrSlowConvergence(double rate_approx, double dfroot, double froot, double xh, double xl, double dx_old) const;
    double face_value_;
    double coupon_;
    Date maturity_date_;
    Date issue_date_;
    Date settlement_date_;
    CashFlows cashflows_;
};

}

#endif
