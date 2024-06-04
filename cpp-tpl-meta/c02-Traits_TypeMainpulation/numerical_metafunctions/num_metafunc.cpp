#include <iostream>

struct five
{
    static int const value = 5;
};

int main()
{

    std::cout << five::value << "\n";

    return 0;
}
