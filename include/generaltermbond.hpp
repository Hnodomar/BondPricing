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
        const Utils::Date maturity_date,
        const Utils::Date issue_date,
        const CashFlowsPy& cashflows,
        const Utils::Date settlement_date,
        YieldCurve& yield_curve
    );
    double cleanPrice(const Utils::Date date) const;
    double dirtyPrice(const Utils::Date date) const;
    double dirtyPrice(const double market_price, const Utils::Date date) const;
    double duration(const Utils::Date date) const;
    void setYieldCurve(YieldCurve& yc) const {yield_curve_ = yc;}
    YieldCurve& getYieldCurve() const {return yield_curve_;}
private:
    double discountFactorFromYield(const double rate, const Utils::Date time) const;
    double duration(const double rate, const Utils::Date date) const override;
    double notionalPresentValue(const double rate, Utils::Date date) const override;
    double performLinearInterpolation(const Utils::Date time) const;
    YieldCurve& yield_curve_;
};
}

#endif
