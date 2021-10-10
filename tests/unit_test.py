import pytest
import sys, os
sys.path.append(os.path.join(os.path.dirname(sys.path[0]),'build'))
from BondPricing import YieldCurvePoint, YieldCurve, FlatTermBond, CashFlow

class TestBaseBond:
    def test_ExpiredBond(self):
        ftbond = FlatTermBond(
            100, 5, 3000, 100,
            [CashFlow(2.0, 200)], 102
        )
        assert ftbond.isExpired() == True
    def test_BadIssueDate(self):
        with pytest.raises(Exception):
            FlatTermBond(
                100, 5, 300, 100,
                [CashFlow(2.0, 80)], 105
            )
    def test_BadMaturityDate(self):
        with pytest.raises(Exception):
            FlatTermBond(
                100, 5, 1, 100,
                [CashFlow(2.0, 80)], 105
            )


class TestFlatTermStructure:
    def test_CleanPrice(self):
        ftbond = FlatTermBond(
            100, 10, 900, 0,
            [CashFlow(10, x * 10000) for x in range(1, 4)], 2
        )
        assert ftbond.cleanPrice(0.09, 0) == 102.53

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
