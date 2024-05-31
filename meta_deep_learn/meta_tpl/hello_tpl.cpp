#include <iostream>

#include <type_traits>

/*
 * 模板可以作为元函数的输入参数
 */

namespace sample1 {

template<template<typename> class T1, typename T2>
struct Fun_
{
    using type = typename T1<T2>::type;
};

template<template<typename> class T1, typename T2>
using Fun = typename Fun_<T1, T2>::type;
} // namespace sample1

/*
 *模板作为元函数的输出
 */

namespace sample2 {

template<bool AddOrRemoveRef> struct Fun_;

template<>
struct Fun_<true>
{
    template<typename T>
    using type = std::add_lvalue_reference<T>;
};

template<>
struct Fun_<false>
{
    template<typename T>
    using type = std::remove_reference<T>;
};

template<typename T>
template<bool AddOrRemoveRef>
using Fun = typename Fun_<AddOrRemoveRef>::template type<T>;

template<typename T>
using Res_ = Fun<false>;

} // namespace sample2

int main()
{

    {
        using namespace sample1;
        Fun<std::remove_reference, int &> h = 3;
        std::cout << h << "\n"; // h is int without reference
    }

    {
        using namespace sample2;
        Res_<int &>::type h1 = 3; // remove_reference
        std::cout << h1 << "\n";
    }

    return 0;
}
