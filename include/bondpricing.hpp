#ifndef BOND_PRICING_HPP
#define BOND_PRICING_HPP

#include <cmath>
#include <limits>
#include <vector>

class BondPricer {
public:
    double yieldToMaturity(const std::vector<double>& times, 
     const std::vector<double>& cashflows, const double bond_price) const { // implement secant method
        double low_bound = 0, up_bound = 1.0;
        while (discreteBondPrice(times, cashflows, up_bound) > bond_price)
            up_bound *= 2;
        double rate_approx = 0.5 * up_bound;
        for (auto i = 0; i < max_iterations_; ++i) {
            double diff = discreteBondPrice(times, cashflows, rate_approx) - bond_price;
            if (fabs(diff) < precision_)
                return rate_approx;
            if (diff > 0.0)
                low_bound = rate_approx;
            else
                up_bound = rate_approx;
            rate_approx = 0.5 * (up_bound + low_bound);
        }
        return rate_approx;
    }
private:
    double discreteBondPrice(const std::vector<double>& times, 
     const std::vector<double>& cashflows, const double interest) const {
        double price = 0;
        for (auto i = 0; i < times.size(); ++i)
            price += cashflows[i] / pow(interest + 1, times[i]);
        return price;
    }
    double discreteBondDuration(const std::vector<double>& times,
     const std::vector<double>& cashflows, const double bond_price) const {
        double weighted_maturity = 0, interest = yieldToMaturity(times, cashflows, bond_price);
        double price = discreteBondPrice(times, cashflows, interest);
        for (auto i = 0; i < times.size(); ++i)
            weighted_maturity += times[i] * cashflows[i] / pow(1 + interest, times[i]);
        return weighted_maturity / price;
    }
    double modifiedBondDuration(const std::vector<double>& times,
     const std::vector<double>& cashflows, const double bond_price) const {
        double duration = discreteBondPrice(times, cashflows, bond_price);
        return duration / (1 + yieldToMaturity(times, cashflows, bond_price));
    }
    const std::size_t max_iterations_ = 200;
    const double precision_ = 1e-5;
};


#endif
