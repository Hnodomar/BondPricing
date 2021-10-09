#ifndef BASE_BOND_HPP
#define BASE_BOND_HPP

#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <optional>
#include <algorithm>

#include "cashflow.hpp"
#include "util.hpp"

namespace BondLibrary {
using Date = int32_t; // date is number of days since unix epoch.. user must translate day-count conventions themselves
using CashFlows = std::vector<CashFlow>;
using CashFlowOpt = std::optional<const CashFlow&>;
class BaseBond {
public:
    BaseBond(
        double face_value,
        double coupon,
        const Date maturity_date,
        const Date issue_date,
        const CashFlows& cashflows,
        const Date settlement_date = Utils::getCurrentDaysSinceEpoch() + 2
    );
    virtual ~BaseBond();
    double accruedAmount(Date settlement) const;
    double yieldToMaturity(const double bond_price) const {return yieldToMaturity(bond_price, issue_date_);}
    double yieldToMaturity(const double bond_price, const Date date) const;
    double getCouponRate() const;
    double getCurrentYield(double market_price) const;
    double modifiedDuration(const double rate, const Date date) const;
    bool isExpired() const;
    CashFlowOpt getCashFlow(Date date = Utils::getCurrentDaysSinceEpoch()) const;
    CashFlowOpt getNextCashFlow(const CashFlow& cashflow) const;
    CashFlowOpt getPreviousCashFlow(const CashFlow& cashflow) const;
    virtual double duration(const double rate, const Date date) const = 0;
protected:
    virtual double notionalPresentValue(const double rate, Date date) const = 0; 
    bool outOfRangeOrSlowConvergence(
        double rate_approx,
        double dfroot, 
        double froot, 
        double xh, 
        double xl, 
        double dx_old
    ) const;
    double face_value_;
    double coupon_;
    Date maturity_date_;
    Date issue_date_;
    Date settlement_date_;
    CashFlows cashflows_;
};
}

#endif
