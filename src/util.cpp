#include "util.hpp"

using namespace BondLibrary::Utils;

int32_t getCurrentDaysSinceEpoch() {
    using namespace std::chrono;
    using days = duration<int, std::ratio_multiply<
        hours::period, std::ratio<24>
    >::type>;
    return duration_cast<days>(
        system_clock::now().time_since_epoch()
    ).count();
}
