#ifndef BOND_HPP
#define BOND_HPP

#include <cstdint>
#include <vector>
#include <algorithm>
#include <cassert>
#include <optional>

#include "cashflow.hpp"
#include "util.hpp"

namespace BondLibrary {
using Date = int32_t; // date is number of days since unix epoch
using CashFlows = std::vector<CashFlow>;
using CashFlowOpt = std::optional<const CashFlow&>;
class Bond {
public:
    Bond(
        double face_value,
        const Date maturity_date,
        const Date issue_date,
        const CashFlows& cashflows,
        const Date settlement_date = Utils::getCurrentDaysSinceEpoch() + 2
    );
    double cleanPrice() const;
    double dirtyPrice() const;
    double accruedAmount(Date settlement) const; // UK Corporate Bond day-count convention: https://docs.londonstockexchange.com/sites/default/files/documents/accrued-interest-corp-supra.pdf
    double yield() const;
    CashFlowOpt getCashFlow(Date date = Utils::getCurrentDaysSinceEpoch()) const;
    CashFlowOpt getNextCashFlow(const CashFlow& cashflow) const;
    CashFlowOpt getPreviousCashFlow(const CashFlow& cashflow) const;
    bool isExpired() const;
private:    
    double face_value_;
    Date maturity_date_;
    Date issue_date_;
    Date settlement_date_;
    CashFlows cashflows_;
};

}

#endif
