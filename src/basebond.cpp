#include "basebond.hpp"

using namespace BondLibrary;

BaseBond::BaseBond(double face_value, double coupon, const Utils::Date maturity_date,
 const Utils::Date issue_date, const CashFlowsPy& cashflows, const Utils::Date settlement_date)
  : face_value_(face_value)
  , coupon_(coupon)
  , maturity_date_(maturity_date)
  , issue_date_(issue_date)
  , settlement_date_(settlement_date) {
    try {
        const boost::python::ssize_t len = boost::python::len(cashflows);
        for (auto i = 0; i < len; ++i) {
            auto element = boost::python::extract<CashFlow>(cashflows[i]);
            if (element.check()) {
                cashflows_.push_back(element);
            }
            else {
                throw std::runtime_error("Tried to construct bond cashflow without cashflows");
            }
        }
    }
    catch (boost::python::error_already_set) {
        PyErr_Print();
    }
    std::sort(cashflows_.begin(), cashflows_.end());
    const size_t nflows = cashflows_.size();
    if (nflows >= 2 && cashflows_[nflows -1].cashflow == cashflows_[nflows - 2].cashflow) {
        cashflows_[nflows - 1].cashflow += face_value;
    }
    if (cashflows_[0].due_date < issue_date_)
        throw std::runtime_error("Issue date must be earlier than first payment date");
    else if (maturity_date_ < issue_date_)
        throw std::runtime_error("Maturity date must be later than issue date");
}

double BaseBond::accruedAmount(Utils::Date settlement) const {
    /*if (settlement < Utils::getCurrentDate()) return 0.0;
    if (isExpired()) return 0.0;
    const auto curr_cashflow = getCashFlow(settlement);
    if (!curr_cashflow) return 0.0;
    if (settlement == curr_cashflow->due_date) {
        return 0.0;
    }
    const auto prev_cashflow = getPreviousCashFlow(*curr_cashflow);
    int32_t days_into_period = 0;
    if (!prev_cashflow) 
        days_into_period = issue_date_;
    else 
        days_into_period = settlement - prev_cashflow->due_date;
    int32_t cflow_period = curr_cashflow->due_date - (prev_cashflow ? prev_cashflow->due_date : issue_date_);
    return curr_cashflow->cashflow * (days_into_period / cflow_period);*/
}

double BaseBond::yieldToMaturity(const double bond_price, const Utils::Date date) const {
    double precision = 1e-5, froot = 0, dfroot = 0, dx = 0, dx_old = 0, xh = 0, xl = 0;
    double low_bound = 0, up_bound = 1.0;
    size_t max_iterations = 200, curr_iterations = 0;
    while (notionalPresentValue(up_bound, date) > bond_price)
        up_bound *= 2;
    double rate_approx = 0.5 * up_bound;
    dx_old = up_bound - low_bound;
    dx = dx_old; 
    froot = notionalPresentValue(rate_approx, date) - bond_price;
    dfroot = modifiedDuration(rate_approx, date);
    ++curr_iterations;
    while (curr_iterations <= max_iterations) {
        bool use_bisection = outOfRangeOrSlowConvergence(rate_approx, dfroot, froot, xh, xl, dx_old);
        if (use_bisection) {
            dx_old = dx;
            dx = (xh - xl) / 2.0;
            rate_approx = xl + dx;
        }
        else {
            dx_old = dx;
            dx = froot / dfroot;
            rate_approx -= dx;
        }
        if (std::fabs(dx) < precision) {
            return rate_approx;
        }
        froot = notionalPresentValue(rate_approx, date) - bond_price;
        dfroot = modifiedDuration(rate_approx, date);
        ++curr_iterations;
        if (froot < 0.0)
            xl = rate_approx;
        else
            xh = rate_approx;
    }
    throw std::runtime_error("Maximum iterations exceeded on yield to maturity Safe-Newton approximation");

}

double BaseBond::getCouponRate() const {
    return coupon_ / face_value_;
}

double BaseBond::getCurrentYield(double market_price) const {
    return coupon_ / market_price;
}

bool BaseBond::isExpired() const {
    if (cashflows_.back().due_date < Utils::getCurrentDate())
        return true;
    return false;
}

CashFlowOpt BaseBond::getCashFlow(Utils::Date date) const {
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (date < cashflows_[i].due_date)
            return cashflows_[i];
    }
    return std::nullopt;
}

CashFlowOpt BaseBond::getNextCashFlow(const CashFlow& cashflow) const {
    assert(cashflows_.size() > 1
        && "Number of cashflows must be larger than one to fetch next cashflow");
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date == cashflow.due_date) {
            if (i == cashflows_.size() - 1) return std::nullopt;
            return cashflows_[i + 1];
        }
    }
    return std::nullopt;
}

CashFlowOpt BaseBond::getPreviousCashFlow(const CashFlow& cashflow) const {
    assert(cashflows_.size() > 1
        && "Number of cashflows must be larger than one to fetch previous cashflow");
    for (size_t i = 0; i < cashflows_.size(); ++i) {
        if (cashflows_[i].due_date == cashflow.due_date) {
            if (i == 0) return std::nullopt;
            return cashflows_[i - 1];
        }
    }
    return std::nullopt;
}

bool BaseBond::outOfRangeOrSlowConvergence(double rate_approx, double dfroot, 
 double froot, double xh, double xl, double dx_old) const {
    return (((rate_approx - xh) * dfroot - froot) 
        * ((rate_approx - xl) * dfroot - froot) > 0.0) 
        || (std::fabs(2.0 * froot) > std::fabs(dx_old * dfroot));
}

double BaseBond::modifiedDuration(const double rate, const Utils::Date date) const {
    //return duration(rate, date) / (1 + date);
}
