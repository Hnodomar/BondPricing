#ifndef YIELD_CURVE_HPP
#define YIELD_CURVE_HPP

#include <type_traits>
#include <cassert>
#include <vector>

#include "bond.hpp"

namespace BondLibrary {
class YieldCurve {
public:
    template<typename ...Args> YieldCurve(Args ...args) {
        assert((std::is_same<Args, double> 
            && "Yield Curve must be constructed with yields"))...;
        curve_bonds_.push_back(args)...;
    }
    template<typename ...Args> void addToYieldCurve(Args&& ...args) {
        assert((std::is_same<Args, Bond> && "Can only add Bonds to Yield Curve"))...;
        curve_bonds_.push_back(args)...;
    }
    double estimateBondPrice(const Bond& bond) const;
    double getForwardRateCurve() const;
    double getSpotRateCurve() const;
private:
    std::vector<Bond> curve_bonds_;
    std::vector<double> curve_bond_yields_;
};
}
#endif
