#include <boost/python.hpp>
#include <Python.h>
#include "yieldcurve.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(BondPricing) {
    class_<BondLibrary::YieldCurvePoint>("YieldCurvePoint", init<int, double>())
        .def_readwrite("maturity", &BondLibrary::YieldCurvePoint::maturity)
        .def_readwrite("bond_yield", &BondLibrary::YieldCurvePoint::yield)
        .def("__eq__", &BondLibrary::YieldCurvePoint::operator==);
}
