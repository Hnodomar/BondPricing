#include "yieldcurve.hpp"

using namespace BondLibrary;

double YieldCurve::estimateBondPrice(const Bond& bond) const {
    if (curve_bonds_.empty()) return 0.0;
}
