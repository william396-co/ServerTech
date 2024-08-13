#include <iostream>
#include <vector>

int main()
{

    using TFunc = void ( * )( int & );

    std::vector<TFunc> ptrFuncVec;

    ptrFuncVec.push_back( []( int & x ) { std::cout << x << "\n"; } );
    ptrFuncVec.push_back( []( int & x ) { x *= 2; } );
    ptrFuncVec.push_back( ptrFuncVec[0] );

    int x = 10;
    for ( auto const & entry : ptrFuncVec ) {
        entry( x );
    }

    return 0;
}
