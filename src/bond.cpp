#include "bond.hpp"

using namespace BondLibrary;

Bond::Bond(double face_value, const Date maturity_date,
 const Date issue_date, const CashFlows& cashflows, const Date settlement_date)
  : face_value_(face_value)
  , maturity_date_(maturity_date)
  , issue_date_(issue_date)
  , settlement_date_(settlement_date)
  , cashflows_(cashflows) {
    std::sort(cashflows_.begin(), cashflows_.end());
    assert(issue_date_ < cashflows[0].due_date 
        && "Issue date must be earlier than first payment date");
    assert(maturity_date_ > issue_date_
        && "Maturity date must be later than issue date");
}

bool Bond::isExpired() const {
    if (cashflows_.back().due_date < Utils::getCurrentDaysSinceEpoch())
        return true;
    return false;
}

double Bond::cleanPrice() const {
    
}

double Bond::dirtyPrice() const {
    if (isExpired())
        return 0.0;
    
}

double Bond::accruedAmount(Date settlement) const {
    if (Utils::getCurrentDaysSinceEpoch() > settlement) 0.0;
    if (isExpired()) return 0.0;
    const auto curr_cashflow = getCashFlow(settlement);
    if (!curr_cashflow) return 0.0;
    if (settlement == curr_cashflow->due_date) {
        return 0.0;
    }
    const auto prev_cashflow = getNextCashFlow(*curr_cashflow);
    int32_t days_into_period = 0;
    if (!prev_cashflow) 
        days_into_period = issue_date_;
    else 
        days_into_period = settlement - prev_cashflow->due_date;
    int32_t cflow_period = curr_cashflow->due_date - (prev_cashflow ? prev_cashflow->due_date : issue_date_);
    return curr_cashflow->cashflow * (days_into_period / cflow_period);
}

CashFlowOpt Bond::getCashFlow(Date date) const {
    for (auto i = 0; i < cashflows_.size(); ++i) {
        if (date <= cashflows_[i].due_date)
            return cashflows_[i];
    }
    return std::nullopt;
}

CashFlowOpt Bond::getNextCashFlow(const CashFlow& cashflow) const {
    assert(cashflows_.size() > 1
        && "Number of cashflows must be larger than one to fetch next cashflow");
    for (auto i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date == cashflow.due_date) {
            if (i == cashflows_.size() - 1) return std::nullopt;
            return cashflows_[i + 1];
        }
    }
    return std::nullopt;
}

CashFlowOpt Bond::getPreviousCashFlow(const CashFlow& cashflow) const {
    assert(cashflows_.size() > 1
        && "Number of cashflows must be larger than one to fetch previous cashflow");
    for (auto i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date == cashflow.due_date) {
            if (i == 0) return std::nullopt;
            return cashflows_[i - 1];
        }
    }
    return std::nullopt;
}
