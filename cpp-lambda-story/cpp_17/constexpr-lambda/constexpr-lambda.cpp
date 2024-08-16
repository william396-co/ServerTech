#include <array>
#include <cassert>

template<typename Range, typename Func, typename T>
constexpr T SimpleAccumulate( Range && range, Func func, T init )
{
    for ( auto && elem : range ) {
        init += func( elem );
    }
    return init;
}

int main()
{
    constexpr std::array arr = { 1, 2, 3 };

    constexpr auto sum = SimpleAccumulate(
        arr, []( auto i ) { return i * i; }, 0 );

    static_assert( sum == 14 );
    return 0;
}
