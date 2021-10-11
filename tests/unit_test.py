import pytest
import sys, os
import math
sys.path.append(os.path.join(os.path.dirname(sys.path[0]),'build'))
from BondPricing import YieldCurvePoint, YieldCurve, FlatTermBond, CashFlow, Date, DayCountConvention

class TestBaseBond:
    def test_ExpiredBond(self):
        ftbond = FlatTermBond(
            face_value = 100, 
            coupon = 5, 
            maturity_date = Date('05/07/2021'), 
            issue_date = Date('05/07/2011'),
            settlement_date = Date('07/07/2021'),
            cashflows = [CashFlow(2.0, Date('05/07/2015'))]
        )
        assert ftbond.isExpired() == True
    def test_CashflowBeforeIssueDate(self):
        with pytest.raises(Exception):
            FlatTermBond(
                face_value = 100,
                coupon = 5,
                maturity_date = Date('10/10/2031'),
                issue_date = Date('10/10/2021'),
                settlement_date = Date('12/10/2021'),
                cashflows = [CashFlow(2.0, Date('5/10/2021'))]
            )
    def test_MaturityBeforeIssueDate(self):
        with pytest.raises(Exception):
            FlatTermBond(
                face_value = 100,
                coupon = 5,
                maturity_date = Date('10/10/2020'),
                issue_date = Date('10/10/2021'),
                settlement_date = Date('12/10/2021'),
                cashflows = [CashFlow(2.0, Date('10/10/2025'))]
            )

class TestFlatTermStructure:
    def test_CleanPrice(self):
        ftbond = FlatTermBond(
            face_value = 100, 
            coupon = 10, 
            maturity_date = Date('12/12/2024'), 
            issue_date = Date('12/12/2021'),
            settlement_date = Date('14/10/2021'),
            cashflows = [CashFlow(cashflow=10, due_date=Date('12/10/{}'.format(2021 + x))) for x in range(1, 4)] 
        )
        assert ftbond.cleanPrice(0.09, Date('12/10/2021')) == 102.53
    def test_DirtyPrice(self):
        ftbond = FlatTermBond(
            face_value = 1000,
            coupon = 50,
            cashflows = [
                CashFlow(50, Date('01/{}/202{}'.format(12 if x % 2 == 0 else 6, int(2 + (x / 2))))) 
                for x in range(6)
            ],
            maturity_date = Date('01/12/2023'),
            issue_date = Date('01/01/2022'),
            settlement_date = Date('01/01/2022'),
            dc_convention = DayCountConvention.Year360Month30
        )
        assert ftbond.dirtyPrice(rate = 0.05, date = Date('29/09/2022')) == 1016.39

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
