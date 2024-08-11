#include <iostream>

int global = 10;

int main()
{
    std::cout << global << std::endl;
    auto foo = [=]() mutable noexcept { ++global; };
    foo();
    std::cout << global << std::endl;
    const auto increaseGlobal = []() noexcept { ++global; };
    increaseGlobal();
    std::cout << global << std::endl;
    const auto moreIncreaseGlobal = [global]() noexcept { ++global; };
    moreIncreaseGlobal();
    std::cout << global << std::endl;
    return 0;
}
