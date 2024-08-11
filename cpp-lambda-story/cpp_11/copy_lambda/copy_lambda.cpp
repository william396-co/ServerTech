#include <type_traits>

int main()
{

    const auto firstLam = []( int x ) noexcept { return x * 2; };
    const auto secondLam = firstLam;

    static_assert( std::is_same<decltype( firstLam ), decltype( secondLam )>::value, "must be the same" );
    return 0;
}
