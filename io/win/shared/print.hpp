#pragma once

#include <iostream>
#include <algorithm>
#include <utility>
#include <mutex>

extern std::mutex os_mutex;

#if __cplusplus < 201703L //below c++17

template<typename T>
void print(T const& t)
{
    std::cout << t;
}

template<typename T, typename... Args>
void print(T const& first, Args... args)
{
    std::cout << first;
    print(args...);
}

inline void println()
{
    std::cout << "\n";
}

template<typename T>
void println(T const& t)
{
    std::cout << t << "\n";
}

template<typename T, typename... Args>
void println(T const& first, Args... args)
{
    std::cout << first;
    println(args...);
}
#else

template<typename... Args>
void print(Args &&... args)
{    
    std::lock_guard<std::mutex> lock(os_mutex);
    (std::cout << ... << std::forward<Args>(args));
}

template<typename... Args>
void println(Args &&... args)
{
    std::lock_guard<std::mutex> lock(os_mutex);
    (std::cout << ... << std::forward<Args>(args)) << "\n";
}

#endif

#include <iterator>
#include <type_traits>

/*
 * output container
 */
template<typename Container>
void print_con(Container con)
{
    print("[ ");
    std::for_each(std::begin(con), std::end(con), [](auto const i) { print(i, ","); });
    println("]");
}

/*
 * out Array
 */
template<typename Arr>
void print_arr(Arr&& arr)
{
    print("[ ");
    for (auto const& i : std::forward<Arr>(arr))
        print(i, ",");
    println("]");
}