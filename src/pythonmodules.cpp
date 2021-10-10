#include <boost/python.hpp>
#include "yieldcurve.hpp"
#include "flattermbond.hpp"
#include "util.hpp"
#include "generaltermbond.hpp"

using namespace boost::python;

struct BaseBondWrapper : ::BondLibrary::BaseBond, wrapper<BondLibrary::BaseBond> {
    BaseBondWrapper(const double f, const double c,const  BondLibrary::Date md, 
     const BondLibrary::Date id, const BondLibrary::CashFlowsPy& cfs, const BondLibrary::Date sd)
        : ::BondLibrary::BaseBond(f, c, md, id, cfs, sd)
    {}
    double duration(const double rate, const BondLibrary::Date date) const {
        return this->get_override("duration")(rate, date);
    }
    double notionalPresentValue(const double rate, BondLibrary::Date date) const {
        return this->get_override("notionalPresentValue")(rate, date);
    }
};

BOOST_PYTHON_MODULE(BondPricing) {
    class_<BondLibrary::YieldCurvePoint>("YieldCurvePoint", init<int, double>())
        .def_readwrite("maturity", &BondLibrary::YieldCurvePoint::maturity)
        .def_readwrite("bond_yield", &BondLibrary::YieldCurvePoint::yield)
        .def("__eq__", &BondLibrary::YieldCurvePoint::operator==);
    class_<BondLibrary::YieldCurve>("YieldCurve", init<list&>())
        .def("addToYieldCurve", &BondLibrary::YieldCurve::addToYieldCurve)
        .def("removeFromYieldCurve", &BondLibrary::YieldCurve::removeFromYieldCurve);
    class_<BondLibrary::CashFlow>("CashFlow", init<double, int32_t>());
    class_<BaseBondWrapper, boost::noncopyable>("BaseBond", init<double, double, int, int, list, int>())
        .def("duration", pure_virtual(&BondLibrary::BaseBond::duration))
        .def("notionalPresentValue", pure_virtual(&BondLibrary::BaseBond::notionalPresentValue));
    class_<BondLibrary::FlatTermBond, bases<BaseBondWrapper>>(
        "FlatTermBond", init<double, double, int32_t, int32_t, list, int32_t>())
        .def("cleanPrice", &BondLibrary::FlatTermBond::cleanPrice)
        .def("dirtyPrice", &BondLibrary::FlatTermBond::dirtyPrice)
        .def("dirtyPriceFromCleanPrice", &BondLibrary::FlatTermBond::dirtyPriceFromCleanPrice)
        .def("duration", &BondLibrary::FlatTermBond::duration)
        .def("isExpired", &BondLibrary::BaseBond::isExpired);
}
