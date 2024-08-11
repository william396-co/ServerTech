#include <iostream>
void bar()
{
    static int static_int = 10;
    std::cout << static_int << std::endl;
    auto foo = [=]() mutable noexcept { ++static_int; };
    foo();
    std::cout << static_int << std::endl;
    const auto increase = []() noexcept { ++static_int; };
    increase();
    std::cout << static_int << std::endl;
    const auto moreIncrease = [static_int]() noexcept { ++static_int; };
    moreIncrease();
    std::cout << static_int << std::endl;
}

int main()
{
    bar();

    return 0;
}
