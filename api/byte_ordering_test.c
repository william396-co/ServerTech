#include <stdio.h>

int is_little_endian()
{
    union Test
    {
        int a;
        char b;
    } t = { 0x1 };

    return t.b == 0x1;
}
/* low address save low  */

int is_little_endian2()
{
    int a = 0x12345678;
    return (char)a==0x78;
}

int main()
{
    printf( "Is little endian = %d\n ", is_little_endian() );
    printf( "Is little endian = %d\n ", is_little_endian2() );
    return 0;
}
