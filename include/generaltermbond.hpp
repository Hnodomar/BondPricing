#ifndef GENERAL_TERM_BOND_HPP
#define GENERAL_TERM_BOND_HPP

#include <cmath>

#include "basebond.hpp"
#include "yieldcurve.hpp"

namespace BondLibrary {
class GeneralTermBond : public BaseBond {
public:
    GeneralTermBond(
        double face_value,
        double coupon,
        const Date maturity_date,
        const Date issue_date,
        const CashFlows& cashflows,
        const Date settlement_date,
        YieldCurve& yield_curve
    );
    double cleanPrice(const Date date) const;
    double dirtyPrice(const Date date) const;
    double dirtyPrice(const double market_price, const Date date) const;
    double duration(const Date date) const;
    void setYieldCurve(YieldCurve& yc) const {yield_curve_ = yc;}
    YieldCurve& getYieldCurve() const {return yield_curve_;}
private:
    double discountFactorFromYield(const double rate, const Date time) const;
    double duration(const double rate, const Date date) const override;
    double notionalPresentValue(const double rate, Date date) const override;
    double performLinearInterpolation(const Date time) const;
    YieldCurve& yield_curve_;
};
}

#endif
