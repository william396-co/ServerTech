#include <iostream>
#include <string>

struct Baz
{

    void foo()
    {
        [this]() { std::cout << s << "\n"; }();
    }

    std::string s;
};

int main()
{

    Baz b;
    b.foo();
}
