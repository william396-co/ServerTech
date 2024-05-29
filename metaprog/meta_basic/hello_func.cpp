#include <iostream>
#include <type_traits>

template<typename T>
struct Fun_
{
    using type = T;
};

template<>
struct Fun_<int>
{
    using type = unsigned int;
};

template<>
struct Fun_<long>
{
    using type = unsigned long;
};

template<typename T>
using Fun = typename Fun_<T>::type;

template<int a, int b>
struct Add_
{
    constexpr static int value = a + b;
};

template<int a, int b>
constexpr int Add = a + b;

int main()
{
    {
        constexpr int x1 = Add_<2, 3>::value;
        constexpr int x2 = Add<2, 3>;

        std::cout << x1 << " " << x2 << "\n";
    }

    Fun<int> h1 = 2;

    Fun_<int>::type h2 = 3;
    std::cout << h1 << " " << h2 << "\n";

    std::remove_reference<int &>::type h3 = 5;
    std::remove_reference_t<int &> h4 = 6;

    std::cout << h3 << " " << h4 << "\n";

    return 0;
}
