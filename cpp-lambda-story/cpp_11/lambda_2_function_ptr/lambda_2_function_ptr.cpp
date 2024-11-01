#include <iostream>

void callWith10( void ( *bar )( int ) )
{
    bar( 10 );
}

int main()
{
    struct
    {
        using f_ptr = void ( * )( int );

        void operator()( int s ) const { return call( s ); }
        operator f_ptr() const { return &call; }

    private:
        static void call( int s ) { std::cout << s << "\n"; }
    } baz;

    callWith10( baz );

    callWith10( []( int x ) { std::cout << x << "\n"; } );

    return 0;
}
