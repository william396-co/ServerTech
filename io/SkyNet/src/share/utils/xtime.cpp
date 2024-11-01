
#include "xtime.h"

namespace utils {

// seconds
time_t time()
{
    using namespace std::chrono;
    return duration_cast<seconds>( system_clock::now().time_since_epoch() ).count();
}
// milliseconds
time_t now()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
}
// microseconds
time_t current()
{
    using namespace std::chrono;
    return duration_cast<microseconds>( system_clock::now().time_since_epoch() ).count();
}

// 获取零点时间(offset 逻辑偏移 5: 凌晨5点)
time_t getZeroTimeStamp( time_t timestamp, int offset )
{
    if ( timestamp == 0 ) {
        timestamp = time();
    }

    struct tm zero_tm;
    ::localtime_r( &timestamp, &zero_tm );

    zero_tm.tm_min = zero_tm.tm_sec = 0;

    if ( zero_tm.tm_hour >= offset ) {
        zero_tm.tm_hour = offset;
    } else { // forward one day
        zero_tm.tm_hour = offset;
        return ::mktime( &zero_tm ) - OneDay;
    }

    return ::mktime( &zero_tm );
}

// 是否跨天(offset 逻辑偏移 5: 凌晨5点)
bool isOverDay( time_t timestamp, int offset )
{
    if ( timestamp == 0 ) return true;
    auto timestamp_zero = getZeroTimeStamp( timestamp, offset );

    if ( timestamp > timestamp_zero + OneDay )
        return true;

    auto now = time();
    if ( now > timestamp_zero + OneDay ) {
        return true;
    }

    return false;
}

bool isLeapYear( int year )
{
    return ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0;
}
} // namespace utils
