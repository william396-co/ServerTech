#include <iostream>

struct always_int
{
    typedef int type;
};

int main()
{

    std::cout << "Nullary Metafunctions\n";

    always_int::type i = 10;
    std::cout << i << "\n";

    return 0;
}
