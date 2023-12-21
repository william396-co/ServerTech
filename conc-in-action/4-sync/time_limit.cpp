#include "time_limit.h"

#include <iostream>
using namespace std::chrono;
using namespace std::chrono_literals;

auto one_day = 24h;
auto half_an_hour = 30min;
auto max_time_between_messages = 30ms;

void chrono_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";

    std::chrono::milliseconds ms { 54802 };
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>( ms );

    std::cout << ms << "=" << s << "\n";

    std::cout << "one_day = " << one_day << "\n";
}
