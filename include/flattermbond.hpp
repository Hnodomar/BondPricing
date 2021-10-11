#ifndef FLAT_TERM_BOND_HPP
#define FLAT_TERM_BOND_HPP

#include "basebond.hpp"

namespace BondLibrary {
class FlatTermBond : public BaseBond {
public:
    FlatTermBond(
        double face_value, 
        double coupon, 
        const Utils::Date maturity_date,
        const Utils::Date issue_date, 
        const CashFlowsPy& cashflows, 
        Utils::Date settlement_date
    );
    double cleanPrice(const double rate, const Utils::Date date) const;
    double dirtyPrice(const double rate, const Utils::Date date) const;
    double dirtyPriceFromCleanPrice(const double market_price, const Utils::Date date) const;
    double duration(const double rate, const Utils::Date date) const override;
private:
    double notionalPresentValue(const double rate, Utils::Date date) const override;
};
}

#endif
