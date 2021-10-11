#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <chrono>
#include <string_view>
#include <ctime>

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
    Date(const std::string& date_str) {
        if (date_str.empty())
            throw std::runtime_error("Cannot construct date object from empty string");
        size_t last_pos = 0;
        size_t pos = 0;
        int date_element = 0;
        for (size_t i = 0; i < 3; ++i) {
            pos = date_str.find('/', last_pos);
            if (i != 2 && pos == std::string::npos)
                throw std::runtime_error("Date object not constructed with correct format");
            switch (i) {
                case 0:
                    date_element = std::stoi(date_str.substr(last_pos, pos - last_pos));
                    if (date_element < 1 || date_element > 31)
                        throw std::runtime_error("Date object constructed with bad day number");
                    day = date_element;
                    break;
                case 1:
                    date_element = std::stoi(date_str.substr(last_pos, pos - last_pos));
                    if (date_element < 1 || date_element > 12)
                        throw std::runtime_error("Date object constructed with bad month number");
                    month = date_element;
                    break;
                case 2:
                    date_element = std::stoi(date_str.substr(last_pos));
                    if (date_element < 1)
                        throw std::runtime_error("Date object constructed with bad year number");
                    year = date_element;
                    break;
                default:
                    break;
            }
            last_pos = pos + 1;
        }
    }
    bool operator<(const Date& rhs) const {
        if (year < rhs.year) 
            return true;
        if (year == rhs.year) {
            if (month < rhs.month)
                return true;
            if (month == rhs.month) {
                if (day < rhs.day)
                    return true;
            }
        }
        return false;
    }
    bool operator==(const Date& rhs) const {
        return day == rhs.day && month == rhs.month && year == rhs.year;
    }
    friend std::ostream& operator<<(std::ostream& lhs, const Date& rhs) {
        lhs << rhs.year << " " << rhs.month << " " << rhs.day << std::endl;
        return lhs;
    }
    int day;
    int month;
    int year;
};

inline Date getCurrentDate() { 
    std::time_t t = std::time(0);
    char buf[20] = {0};
    strftime(buf, sizeof(buf), "%d/%m/%Y", std::localtime(&t));
    return Date(buf);
}

inline int getJulianDayNumber(const Date& date) {
    return 367 * date.year - (7 * (date.year + 5001 + (date.month - 9) / 7) / 4)
        + (275 * date.month) / 9 + date.day + 1729777;
}
}
}

#endif
