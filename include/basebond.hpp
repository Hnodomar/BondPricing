#ifndef BASE_BOND_HPP
#define BASE_BOND_HPP

#include <cstdint>
#include <cassert>
#include <cmath>
#include <vector>
#include <optional>
#include <algorithm>
#include <boost/python.hpp>

#include "cashflow.hpp"
#include "util.hpp"

namespace BondLibrary {
using CashFlows = std::vector<CashFlow>;
using CashFlowsPy = boost::python::list;
using CashFlowOpt = std::optional<const CashFlow>;
class BaseBond {
public:
    BaseBond(
        double face_value,
        double coupon,
        const Utils::Date maturity_date,
        const Utils::Date issue_date,
        const CashFlowsPy& cashflows,
        const Utils::Date settlement_date,
        const Utils::DayCountConvention daycount_convention
    );
    virtual ~BaseBond() {}
    double accruedAmount(Utils::Date settlement) const;
   // double yieldToMaturity(const double bond_price) const {return yieldToMaturity(bond_price, issue_date_);}
    double yieldToMaturity(const double bond_price, const Utils::Date date) const;
    double getCouponRate() const;
    double getCurrentYield(double market_price) const;
    double modifiedDuration(const double rate, const Utils::Date date) const;
    bool isExpired() const;
    CashFlowOpt getCashFlow(Utils::Date date) const;
    CashFlowOpt getNextCashFlow(const CashFlow& cashflow) const;
    CashFlowOpt getPreviousCashFlow(const CashFlow& cashflow) const;
    virtual double duration(const double rate, const Utils::Date date) const = 0;
    double notionalPresentValue(const double rate, Utils::Date date) const; 
protected:
    bool outOfRangeOrSlowConvergence(
        double rate_approx,
        double dfroot, 
        double froot, 
        double xh, 
        double xl, 
        double dx_old
    ) const;
    int getCouponFrequency(const Utils::Date& date) const;
    double face_value_;
    double coupon_;
    Utils::Date maturity_date_;
    Utils::Date issue_date_;
    Utils::Date settlement_date_;
    CashFlows cashflows_;
    Utils::DayCountConvention daycount_convention_;
};
}

#endif
