#include <iostream>
#include <type_traits>

// 顺序执行的代码
template<typename T>
struct RemoveReferenceConst_
{
private:
    using inter_type = typename std::remove_reference<T>::type;

public:
    using type = typename std::remove_const<inter_type>::type;
};

template<typename T>
using RemoveReferenceConst = typename RemoveReferenceConst_<T>::type;

// 分支执行的代码
// std::conditional<B,T,F>
namespace example1 {
// partial specialization
struct A;
struct B;

template<typename T>
struct Fun_
{
    constexpr static size_t value = 0;
};

template<>
struct Fun_<A>
{
    constexpr static size_t value = 1;
};

template<>
struct Fun_<B>
{
    constexpr static size_t value = 2;
};

} // namespace example1

namespace example2 {
struct A;
struct B;

template<typename T>
constexpr size_t Fun = 0;

template<>
constexpr size_t Fun<A> = 1;

template<>
constexpr size_t Fun<B> = 2;

} // namespace example2

namespace example3 {

template<typename TW>
struct Wrapper
{
    template<typename T, typename TDummy = void>
    struct Fun_
    {
        constexpr static size_t value = 0;
    };

    // 这里引入了一个伪参数 TDummy，用于将原有的完全特化修改为部
    // 分特化。这个参数有一个默认值void，
    template<typename TDummy>
    struct Fun_<int, TDummy>
    {
        constexpr static size_t value = 1;
    };
};
} // namespace example3

namespace example4 {
// SFINAE

template<bool Check, std::enable_if_t<Check> * = nullptr>
auto fun()
{
    return (int)3.14;
}

template<bool Check, std::enable_if_t<!Check> * = nullptr>
auto fun()
{
    return (double)3.14;
}

template<bool Check>
auto wrap2()
{
    return fun<Check>();
}

} // namespace example4

namespace example5 {
// if constexpr

template<bool Check>
auto fun()
{
    if constexpr ( Check ) {
        return (int)3.14;
    } else {
        return (double)3.14;
    }
}
} // namespace example5

namespace example6 {

template<size_t Input>
constexpr size_t OnesCount = ( Input % 2 ) + OnesCount<( Input / 2 )>;

template<>
constexpr size_t OnesCount<0> = 0;

} // namespace example6

namespace example7 {

template<size_t... Inputs>
constexpr size_t Accumulate = 0;

template<size_t CurInput, size_t... Inputs>
constexpr size_t Accumulate<CurInput, Inputs...>
    = CurInput + Accumulate<Inputs...>;

} // namespace example7

namespace example8 {

template<size_t... values>
constexpr size_t fun()
{
    return ( 0 + ... + values );
}

} // namespace example8

namespace example9 {
#define REFACTOR
#ifndef REFACTOR
template<size_t A>
struct Wrap_
{
    template<size_t ID, typename TDummy = void>
    struct imp
    {
        constexpr static size_t value = ID + imp<ID - 1>::value;
    };

    template<typename TDummy>
    struct imp<0, TDummy>
    {
        constexpr static size_t value = 0;
    };

    template<size_t ID>
    constexpr static size_t value = imp<A + ID>::value;
};
#else
template<size_t ID>
struct imp
{
    constexpr static size_t value = ID + imp<ID - 1>::value;
};
template<> struct imp<0>
{
    constexpr static size_t value = 0;
};

template<size_t A>
struct Wrap_
{
    template<size_t ID>
    constexpr static size_t value = imp<A + ID>::value;
};
#endif
} // namespace example9

int main()
{

    {
        RemoveReferenceConst<int const &> h = 3; // h i int

        std::cout << "h = " << h << "\n";
    }
    {
        std::conditional<true, int, float>::type x = 3;
        std::conditional_t<false, int, float> y = 1.0f;

        std::cout << "x=" << x << " y=" << y << "\n";
    }

    {
        using namespace example1;
        constexpr size_t h = Fun_<B>::value; // h = 2
        std::cout << "h = " << h << "\n";
    }

    {
        using namespace example2;
        constexpr size_t h = Fun<A>;
        std::cout << "h = " << h << "\n";
    }

    {
        using namespace example4;
        std::cerr << wrap2<false>() << std::endl;
    }

    {
        using namespace example5;
        std::cerr << fun<false>() << std::endl;
    }

    {
        using namespace example6;
        constexpr size_t res = OnesCount<45>;
        std::cout << "res = " << res << " OnesCount<42> = " << OnesCount<42> << "\n";

        constexpr size_t x1 = OnesCount<7>;
        constexpr size_t x2 = OnesCount<15>;
    }

    {
        using namespace example7;
        constexpr size_t res = Accumulate<1, 2, 3, 4, 5>;
        std::cout << "Accumulate<1,2,3,4,5> = " << res << "\n";
    }

    {
        using namespace example8;
        constexpr size_t res = fun<1, 2, 3, 4, 5, 6, 7, 8>();
        std::cout << "fun<1,2,3,4,5,6,7,8> = " << res << "\n";
    }

    {
        using namespace example9;
        std::cerr << Wrap_<3>::value<2> << "\n";
        std::cerr << Wrap_<10>::value<2> << "\n";
    }
    return 0;
}
