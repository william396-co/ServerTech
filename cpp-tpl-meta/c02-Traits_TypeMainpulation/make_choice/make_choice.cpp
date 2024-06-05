#include <iostream>

#include <type_traits>
#include <iterator>
#include <utility> // for swap

template<bool use_swap> struct iter_swap_impl;

template<class ForwardIterator1, class ForwardIterator2>
void iter_swap( ForwardIterator1 i1, ForwardIterator2 i2 )
{
    typedef std::iterator_traits<ForwardIterator1> traits1;
    typedef typename traits1::value_type v1;
    typedef typename traits1::reference r1;

    typedef std::iterator_traits<ForwardIterator2> traits2;
    typedef typename traits2::value_type v2;
    typedef typename traits2::reference r2;

    bool const use_swap = std::is_same_v<v1, v2>
        && std::is_reference_v<r1>
        && std::is_reference_v<r2>;
    iter_swap_impl<use_swap>::do_it( i1, i2 );
};

template<>
struct iter_swap_impl<true>
{
    template<class ForwardIterator1, class ForwardIterator2>
    static void do_it( ForwardIterator1 i1, ForwardIterator2 i2 )
    {
        std::swap( *i1, *i2 );
    }
};

template<>
struct iter_swap_impl<false>
{
    template<class ForwardIterator1, class ForwardIterator2>
    static void do_it( ForwardIterator1 i1, ForwardIterator2 i2 )
    {
        typename std::iterator_traits<ForwardIterator1>::value_type tmp = *i1;
        *i1 = *i2;
        *i2 = tmp;
    }
};

int main()
{
    std::cout << "Making Choice at Complie Time\n";

    int a = 3, b = 5;
    std::cout << "a=" << a << " b=" << b << "\n";
    iter_swap( &a, &b );
    std::cout << "a=" << a << " b=" << b << "\n";

    return 0;
}
