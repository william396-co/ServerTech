#include <iostream>

struct Baz
{
    auto foo()
    {
        return [*this] { std::cout << s << "\n"; };
    }
    std::string s;
};

int main()
{

    const auto f1 = Baz { "xyz" }.foo();
    const auto f2 = Baz { "abc" }.foo();

    f1();
    f2();

    return 0;
}
