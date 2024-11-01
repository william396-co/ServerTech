#pragma once

#include <chrono>
#include <time.h>
#include <string>

namespace utils {

constexpr auto OneSecond = 1;
constexpr auto OneMinute = OneSecond * 60;
constexpr auto OneHour = OneMinute * 60;
constexpr auto OneDay = OneHour * 24;
constexpr auto OneWeek = OneDay * 7;

// seconds
time_t time();

// milliseconds
time_t now();

// microseconds
time_t current();

// 获取零点时间(offset 逻辑偏移 5: 凌晨5点)
time_t getZeroTimeStamp( time_t timestamp, int offset );

// 是否跨天(offset 逻辑偏移 5: 凌晨5点)
bool isOverDay( time_t timestamp, int offset );

bool isLeapYear( int year );

} // namespace utils
