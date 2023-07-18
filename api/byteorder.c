#include <stdio.h>

int main()
{
    union
    {
        short value;
        char union_bytes[sizeof( short )];
    } test;

    test.value = 0x0102;
    if ( ( test.union_bytes[0] == 0x01 ) && ( test.union_bytes[1] == 0x02 ) ) {
        printf( "big endian\n" );
    } else if ( ( test.union_bytes[0] = 0x02 ) && ( test.union_bytes[1] == 0x01 ) ) {
        printf( "little endian\n" );
    } else {
        printf( "unknown...\n" );
    }
    return 0;
}
