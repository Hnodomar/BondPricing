import pytest
import sys, os
sys.path.append(os.path.join(os.path.dirname(sys.path[0]),'build'))
from BondPricing import YieldCurvePoint


def main():
    test = YieldCurvePoint(1, 2.0)
    print(test.maturity)
    print(test.bond_yield)

if __name__ == "__main__":
    main()

