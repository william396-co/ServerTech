#include <type_traits>

int main()
{

    const auto oneLam = []( int x ) noexcept { return x * 2; };
    const auto twoLam = []( int x ) noexcept { return x * 2; };

    static_assert( !std::is_same<decltype( oneLam ), decltype( twoLam )>::value, "must be different!" );

    return 0;
}
