#ifndef YIELD_CURVE_HPP
#define YIELD_CURVE_HPP

#include <type_traits>
#include <cassert>
#include <vector>

#include "bond.hpp"

namespace BondLibrary {
class YieldCurve {
    struct YieldCurvePoint {
        YieldCurvePoint(int maturity, double yield)
            : maturity(maturity), yield(yield) {}
        int maturity;
        double yield;
    };
public:
    template<typename ...Args> YieldCurve(Args ...args) {
        assert((std::is_same<Args, YieldCurvePoint> 
            && "Yield Curve must be constructed with yield curve points"))...;
        yield_curve_.push_back(args)...;
    }
    template<typename ...Args> void addToYieldCurve(Args&& ...args) {
        assert((std::is_same<Args, YieldCurvePoint> && "Can only add points to Yield Curve"))...;
        yield_curve_.push_back(args)...;
    }
    double estimateBondPrice(const Bond& bond) const;
private:
    std::vector<YieldCurvePoint> yield_curve_;
};
}
#endif
