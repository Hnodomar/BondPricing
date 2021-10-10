#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <chrono>
#include <string_view>

namespace BondLibrary {
namespace Utils {

enum class DayCountConvention {
    Year360Month30,
    Year365Month30,
    Year360MonthActual,
    Year365MonthActual,
    YearActualMonthActual
};

struct Date {
    Date(int day, int month, int year)
        : day(day), month(month), year(year)
    {}
    int day;
    int month;
    int year;
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
template<typename VectorType>
inline void convertPythonListToVector(const boost::python::list& list, 
 std::vector<VectorType>& vector, const std::string& msg) {
    try {
        const boost::python::ssize_t len = boost::python::len(list);
        for (auto i = 0; i < len; ++i) {
            auto element = boost::python::extract<CashFlow>(list[i]);
            if (element.check()) {
                vector.push_back(element);
            }
            else {
                throw std::runtime_error(msg);
            }
        }
    }
    catch (boost::python::error_already_set) {
        PyErr_Print();
    }
}

}
}

#endif
