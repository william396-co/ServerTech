#include <functional>
#include <iostream>
#include <string>

struct Baz
{
    std::function<void()> foo()
    {
        return [=] { std::cout << s << "\n"; };
    }
    std::string s;
};

int main()
{

    auto f1 = Baz { "abc" }.foo();
    auto f2 = Baz { "xyz" }.foo();

    f1();
    f2();

    return 0;
}
