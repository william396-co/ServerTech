#include <iostream>
#include <iterator>

template<class ForwardIterator1, class ForwardIterator2>
void iter_swap( ForwardIterator1 i1, ForwardIterator2 i2 )
{
    typename std::iterator_traits<ForwardIterator1>::value_type tmp = *i1;
    *i1 = *i2;
    *i2 = tmp;
}

void f( int * p1, int * p2 )
{
    iter_swap( p1, p2 );
}

int main()
{
    int a = 3, b = 4;

    std::cout << "a = " << a << " b = " << b << "\n";
    f( &a, &b );
    std::cout << "a = " << a << " b = " << b << "\n";

    return 0;
}
