#include <iostream>
#include <type_traits>

int main()
{

    const int x = 10;
    [x]() mutable {
        std::cout << std::is_const<decltype( x )>::value << "\n";
      //  x = 11; //Error
    }();

    return 0;
}
