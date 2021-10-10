#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <chrono>

namespace BondLibrary {
namespace Utils {

enum class DayCountConvention {
    Year360Month30,
    Year365Month30,
    Year360MonthActual,
    Year365MonthActual,
    YearActualMonthActual
};

inline int32_t getCurrentDaysSinceEpoch() { // current date is number of days since UNIX epoch
    using namespace std::chrono;
    using days = duration<int, std::ratio_multiply<
        hours::period, std::ratio<24>
    >::type>;
    return duration_cast<days>(
        system_clock::now().time_since_epoch()
    ).count();
}


}
}

#endif
