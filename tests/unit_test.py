import pytest
import sys, os
sys.path.append(os.path.join(os.path.dirname(sys.path[0]),'build'))
from BondPricing import YieldCurvePoint, YieldCurve, FlatTermBond, CashFlow

class TestFlatTermStructure:
    def test_CleanPrice(self):
        assert 1 == 1
    def test_arbTest(self):
        ftbond = FlatTermBond(
            100,
            5,
            3000,
            100,
            [CashFlow(2.0, 200)],
            2000
        )
        assert ftbond.isExpired() == True

class TestGeneralTermStucture:
    def test_CleanPrice(self):
        assert 2 == 2
    def test_DirtyPrice(self):
        assert 2 == 2

class TestYieldCurves:
    def test_BadConstruction(self):
        with pytest.raises(Exception):
            YieldCurve([YieldCurvePoint(1, 2.0), 2])
    def test_BadAdd(self):
        with pytest.raises(Exception):
            curve = YieldCurve([YieldCurvePoint(1, 2.0)])
            curve.addToYieldCurve([YieldCurvePoint(2, 3.0), 2])
    def test_GoodConstruction(self):
        try:
            curve = YieldCurve([
                YieldCurvePoint(1, 2.0), 
                YieldCurvePoint(2, 3.0),
                YieldCurvePoint(3, 4.0)
            ])
            curve.addToYieldCurve([
                YieldCurvePoint(4, 5.0), 
                YieldCurvePoint(5, 6.0),
                YieldCurvePoint(6, 7.0)
            ])
        except Exception:
            pytest.fail("GoodConstruction failed")
    def test_RemoveFromCurve(self):
        try:
            curve = YieldCurve([
                YieldCurvePoint(1, 2.0), 
                YieldCurvePoint(2, 3.0),
                YieldCurvePoint(3, 4.0)
            ])
            curve.removeFromYieldCurve(YieldCurvePoint(2, 3.0))
        except Exception:
            pytest.fail("RemoveFromCurve failed")
    def test_BadRemoveFromCurve(self):
        with pytest.raises(Exception):
            curve = YieldCurve([YieldCurvePoint(1, 2.0)])
            curve.removeFromYieldCurve(1)
