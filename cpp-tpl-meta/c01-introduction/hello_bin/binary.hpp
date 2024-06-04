#pragma once

#ifdef USE_META

template<unsigned long N>
struct binary
{
    static unsigned const value = binary<N / 10>::value << 1 // perpend higher bits
        | N % 10;                                            // to lowest bit
};

template<>
struct binary<0>
{
    static unsigned const value = 0;
};

unsigned const one = binary<1>::value;
unsigned const three = binary<111>::value;
unsigned const five = binary<101>::value;
unsigned const seven = binary<111>::value;
unsigned const nine = binary<1001>::value;

#else
unsigned binary( unsigned long N )
{
    unsigned result = 0;
    for ( unsigned bit = 0x1; N; N /= 10, bit <<= 1 ) {
        if ( N % 10 )
            result += bit;
    }
    return result;
}
#endif
