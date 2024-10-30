#pragma once

#include <chrono>
#include <string>

namespace utils {

constexpr auto OneSecond = 1;
constexpr auto OneMinute = OneSecond * 60;
constexpr auto OneHour = OneMinute * 60;
constexpr auto OneDay = OneHour * 24;
constexpr auto OneWeek = OneDay * 7;

// seconds
inline time_t time()
{
    using namespace std::chrono;
    return duration_cast<seconds>( system_clock::now().time_since_epoch() ).count();
}
// milliseconds
inline time_t now()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
}
// microseconds
inline time_t current()
{
    using namespace std::chrono;
    return duration_cast<microseconds>( system_clock::now().time_since_epoch() ).count();
}

inline std::string getDateTime( time_t timestamp )
{
    struct tm tm_info;
    ::localtime_r( &timestamp, &tm_info );

    char buf[1024];
    strftime( buf, sizeof( buf ) - 1, "%F %T", &tm_info );
    return buf;
}

// 获取零点时间(offset 逻辑偏移 5: 凌晨5点)
inline time_t getZeroTimeStamp( time_t timestamp, int offset )
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
inline bool isOverDay( time_t timestamp, int offset )
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

inline bool isLeapYear( int year )
{
    return ( year % 4 == 0 && year % 100 != 0 ) || year % 400 == 0;
}
}

} // namespace utils
