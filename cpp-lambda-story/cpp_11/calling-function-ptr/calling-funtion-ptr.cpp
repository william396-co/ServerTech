
template<typename F>
void call_function( F f )
{
    f( 10 );
}

int main()
{

    call_function( static_cast<int ( * )( int )>( []( int x ) { return x + 2; } ) );

    call_function( static_cast<int ( * )( int )>( []( int x ) { return x * 2; } ) );

    return 0;
}
