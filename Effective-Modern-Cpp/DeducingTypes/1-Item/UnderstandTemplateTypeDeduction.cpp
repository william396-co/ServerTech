#include <iostream>
#include <string>

template<typename T>
void f( T const & param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

/*
 * template<typename T>
 * void f(ParamType param)
 * {
 *
 * }
 */

/*
 * Case 1: ParamType is a Reference or Pointer, but not a Universal Reference
 * 1 If expr's type is a reference, ignore the reference part
 * 2 Then pattern-match expr's type against ParamType to determine T
 */

template<typename T>
void f11( T & param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

template<typename T>
void f12( T * param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void case1()
{
    {
        std::cout << " Case 11 \n";
        int x = 27;         // x is an int
        const int cx = x;   // cx is a const int
        const int & rx = x; // rx is a reference to x as a const int
        f11( x );           // T is int, param's type is int&
        f11( cx );          // T is const int, param's type is const int&
        f11( rx );          // T is const int, param's type is const int&
    }
    {
        std::cout << "Case 12\n";

        int x = 27;
        const int * px = &x; // px is ptr to x as a const int
        f12( &x );           // T is int, param's type is int*
        f12( px );           // T is const int, param's type is const int*
    }
}

/*
 * Case 2: ParamType is a Universal Reference
 *
 * If expr's is an lvalue, both T and ParamType are deduced to be lvalue references
 * That's doubly unusual,First, it's the only situation in template type deduction
 * where T is deduced to be a reference, Second , although ParamType is declared
 * using the syntax for an rvalue reference, its deduced type is an lvalue reference.
 *
 * It expr is an rvalue, the "normal" rules(Case 1) apply
 */

template<typename T>
void f2( T && param ) // param is now a auniversal reference
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void case2()
{
    std::cout << "Case 2\n";
    int x = 42;
    const int cx = x;
    const int & rx = x;

    f2( x );  // lvalue ,T is int&, param's type is also int&
    f2( cx ); // lvalue ,T is const int&, param's type is also cont int&
    f2( rx ); // lvalue, T is const int&, param's type is also const int&
    f2( 27 ); // rvalue. T is int, param's type is therefore int&&
}

/*
 * Case 3: ParamType is Neither a Pointer nor a reference
 */

// As before,. if expr's type is a reference, ignore the reference part
// If, after ignoring expr's reference-ness, expr is const, ignore that, too, If it's
// volatile ,also ignore that(volatile objects are uncommon,They're generally
// used only for implementating device drivers)

template<typename T>
void f3( T param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}

void someFunc( int x, double y )
{
}

template<typename T>
void f31( T param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}
template<typename T>
void f32( T & param )
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
}
template<typename T, std::size_t N>
constexpr std::size_t arraySize( T ( & )[N] ) noexcept
{
    return N;
}
void case3()
{
    std::cout << "case 3\n";
    { // ignore cv(const/volatile)
        int x = 27;
        const int cx = x;
        const int & rx = x;

        f3( x ); // T's and param's type are both int

        f3( cx ); // T's and param's type are both int

        f3( rx ); // T's and param's type are both int
    }
    { // array decay to pointer

        const char name[] = "J.P.Briggs";
        f3( name );  // pass pointer to f3
        f32( name ); // pass array of f32

        const char * ptrToName = name;
        f3( ptrToName );

        int keyVals[] = { 1, 3, 5, 7, 9, 22, 35 };
        int mappedVals[arraySize( keyVals )];

        f3( keyVals );  // pointer
        f32( keyVals ); // array

        f3( mappedVals );  // pointer
        f32( mappedVals ); // array
    }

    {                    // Function Arguments
        f31( someFunc ); // param deduced as ptr-to-func,param type is void(*)(int,double)
        f32( someFunc ); // param deduced as ref-to-func,param type is void(&)(int,double)
    }

    const char * const ptr = "Fun with pointers";
    f3( ptr ); // T's and param's type are both const char*
}

int main()
{

    f( 21 );
    f( "hello" );
    std::string str = "hello";
    f( str );

    case1();
    case2();
    case3();

    return 0;
}
