#include <iostream>

int main()
{

#define USE_CAP_REF
#ifndef USE_CAP_REF

    int x = 30;
    int y = 12;

    const auto foo = [z = x + y] { std::cout << z << "\n"; };

    x = 0;
    y = 0;
    foo(); // 42

#else
    int x = 30;

    const auto foo = [&z = x] { std::cout << z << "\n"; };

    foo(); // 30
    x = 0;
    foo(); // 0

#endif

    return 0;
}
