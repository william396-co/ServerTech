#include <algorithm>
#include <iostream>
#include <string>

struct Baz
{
    auto foo() const
    {
        return [s = s] { std::cout << s << "\n"; };
    }
    std::string s;
};

int main()
{
    auto const f1 = Baz { "abc" }.foo();
    auto const f2 = Baz { "xyz" }.foo();

    f1();
    f2();
    return 0;
}
