#include <iostream>

#include <type_traits>


void exe1()
{
    std::cout << __FUNCTION__ << "\n";

}

int main()
{

    exe1();

    return 0;
}
