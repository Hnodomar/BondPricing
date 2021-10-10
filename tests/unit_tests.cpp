#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <boost/python.hpp>
#include "util.hpp"
#include "flattermbond.hpp"
#include "generaltermbond.hpp"
#include "yieldcurve.hpp"

using namespace BondLibrary;

TEST_CASE("Flat Term Structure Bonds") {
    SECTION("Basic Clean Price") {
        
    }
}

TEST_CASE("General Term Structure Bonds") {
    Py_Initialize();
    boost::python::list test_list;
    YieldCurvePoint ptone(1, 3.5);
    test_list.append(ptone); // there's no better way to do this
    //test_list.append(YieldCurvePoint(2, 4.0));
    //test_list.append(YieldCurvePoint(3, 4.7));
    //test_list.append(YieldCurvePoint(4, 5.5));    
    YieldCurve yield_curve(boost::python::object(boost::python::handle<>(test_list)));
    SECTION("Basic Clean Price") {
        /*GeneralTermBond test_bond(
            100,
            5,
            4, 
            0,
            std::vector<CashFlow>{},
            0,
            yield_curve    
        );
        */
    }
    SECTION("Basic Dirty Price") {

    }
}

TEST_CASE("Yield Curves") {
    Py_Initialize();
    SECTION("Attempt to Construct Bad Yield Curve") {
        boost::python::list test_list;
        test_list.append(1);
        REQUIRE_THROWS(YieldCurve(test_list));
    }
}

