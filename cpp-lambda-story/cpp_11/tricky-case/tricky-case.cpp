#include <type_traits>

int main()
{
    // function point equal to +lambda
    auto funcPtr = +[] {};
    static_assert( std::is_same<decltype( funcPtr ), void ( * )()>::value );

    return 0;
}
