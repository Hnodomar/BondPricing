#ifndef CASHFLOW_HPP
#define CASHFLOW_HPP

#include <cstdint>

namespace BondLibrary {
using Date = int32_t;
struct CashFlow {
    double cashflow = 0; // amount to be received
    Date due_date = 0; // date on which the cashflow is due to be paid out
    bool operator<(CashFlow rhs) {
        return rhs.due_date < due_date;
    }
};   
}

#endif
