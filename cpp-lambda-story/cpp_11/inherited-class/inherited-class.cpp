#include <iostream>

struct BaseInt
{
    void Func( int ) { std::cout << "BaseInt...\n"; }
};

struct BaseDouble
{
    void Func( double ) { std::cout << "BaseDouble...\n"; }
};

struct Derived : public BaseInt, BaseDouble
{
    using BaseDouble::Func;
    using BaseInt::Func;
};

int main()
{

    Derived d;
    d.Func( 10.0 );
    d.Func( 10 );
    return 0;
}
