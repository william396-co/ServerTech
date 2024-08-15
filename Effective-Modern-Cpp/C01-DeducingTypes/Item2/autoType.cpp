#include <iostream>
#include <typeinfo>

template<typename T>
void func_for_x( T p )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template<typename T>
void func_for_cx( T const p )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template<typename T>
void func_for_rx( T const & p )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void someFunc( int, double )
{
}

template<typename T>
void f( T param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

int main()
{
    auto x = 27;
    const auto cx = x;
    const auto & rx = x;

    func_for_x( 27 );
    func_for_cx( x );
    func_for_rx( x );

    /*
     * Case 1: The type specifier a pointer or reference, but not a universal reference
     * Case 2: The type specifier is a universal reference
     * Case 3: The type specifier is neither a pointer or reference
     */
    {
        auto x = 27;         // case 3(x is neither ptr nor  reference)
        const auto cx = x;   // case 3(cx isn't either)
        const auto & rx = x; // case 1(rx is a non-universal ref)

        auto && uref1 = x;  // x is int an lvalue, so uref1's type is int&
        auto && uref2 = cx; // cx is const int and lvalue, so uref2's type is const int&
        auto && uref3 = 27; // 27 is int and rvalue, so uref3's type is int&&

        const char name[] = "R.N.Briggs";

        auto arr1 = name;   // arr1's type is const char*
        auto & arr2 = name; // arr2's type is const char(&)[13]

        auto func1 = someFunc;   // func1's type is void(*)(int,double)
        auto & func2 = someFunc; // func2's type is void(&)(int,double)
    }

    {
        auto x1 = 27;  // type is int, value is 27
        auto x2( 27 ); // ditto

        auto x3 = { 27 }; // type is std::initializer_list<int>, value is {27}

        auto x4 { 27 }; // ditto

        auto x = { 11, 23, 9 };

        f( x ); // type is std::initializer_list<int>
                //        f( { 2, 3 } ); // Error, can't deduce type for T
    }

    return 0;
}
