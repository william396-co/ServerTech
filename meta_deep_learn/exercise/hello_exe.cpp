#include <iostream>

namespace exe1 {

template<typename T>
constexpr size_t SizeofT = sizeof( T );
}

template<typename T, size_t N>
constexpr bool IsEqual = sizeof( T ) == N;

int main()
{

    {
        using namespace exe1;
        std::cout << SizeofT<int> << "\n";

        std::cout << IsEqual<int, 4> << "\n";
    }

    return 0;
}
