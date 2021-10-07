#ifndef BOND_PRICING_HPP
#define BOND_PRICING_HPP

#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>

template <typename Derived>
class TermStructureBase {
public:
    double getSpotRate(const double time) const {
        return static_cast<Derived*>(this)->spotRate(time);
    }
    double getForwardRate(const double time_one, const double time_two) const {
        double discount_one = getDiscount(time_one);
        double discount_two = getDiscount(time_two);
        return forwardRateFromDiscountFactors(discount_one, discount_two);
    }
    double getDiscountFactor(const double time) const {
        return discountFactorFromYield(getSpotRate(time), time);
    }
private:
    double yieldFromDiscountFactor(const double discount_factor, const double time) const {
        return (-log(discount_factor) / time);
    }
    double discountFactorFromYield(const double rate, const double time) const {
        return exp(-rate*time);
    }
    double forwardRateFromDiscountFactors(const double d_one, const double d_two, const double time) const {
        return (log(d_one / d_two)) / time;
    }
    double forwardRateFromYields(const double spot_rate_one, const double spot_rate_two,
     const double time_one, const double time_two) const {
        return spot_rate_two * time_two / (time_two - time_one) 
            - spot_rate_one * time_one / (time_two - time_one);
    }
};

class TermStructureFlat : public TermStructureBase<TermStructureFlat> {
public:
    TermStructureFlat(const double flat_rate)
        : flat_rate_(flat_rate)
    {}
    void setFlatRate(const double rate) {
        flat_rate_ = rate;
    }

private:
    double spotRate(const double time) {
        if (time >= 0) 
            return flat_rate_;
        return 0;
    }
    double flat_rate_;
};

double interpolateLinearly(const double time, const std::vector<double>& observed_times,
 const std::vector<double>& data) {
    if (observed_times.size() != data.size())
        return 0;
    std::sort(data.begin(), data.end());
    std::sort(observed_times.begin(), observed_times.end());
    if (time <= observed_times[0])
        return data[0];
    if (time >= observed_times[observed_times.size() - 1])
        return data[data.size() - 1];
    
    int t = 1;
    while (t < observed_times.size() && time > observed_times[t]) ++t;
    double lambda = (observed_times[t] - time) / (observed_times[t] - observed_times[t - 1]);
    double interpolated_result = data[t - 1] * lambda + data[t] * (1.0 - lambda);
    return interpolated_result;
}

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
