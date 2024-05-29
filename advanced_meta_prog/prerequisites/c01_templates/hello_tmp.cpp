#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

 #define TMP

#ifndef TMP

class sq
{
    double s_;

public:
    explicit sq( double x )
        : s_( x * x ) {}

    operator double() const
    {
        return s_;
    }
};
#else

class sq_t
{
public:
    using value_type = double;
    value_type operator()( double x ) const
    {
        return x * x;
    }
};

const sq_t sq = sq_t();
#endif

template<typename T>
void f( T x )
{
    std::cout << __FUNCTION__ << "(" << x << ")\n";
}

int main()
{

    double a = 3.0, b = 4.0, c = 5.0;

#ifndef TMP

    {

        f( cos( 3.14 ) );
        f( sq( 3.14 ) );

        std::vector<double> v { 1.0, 2.0, 3.0 };
      //  std::transform( v.begin(), v.end(), v.begin(), sq );

        std::for_each( v.begin(), v.end(), []( auto i ) { std::cout << i << "\t"; } );
        std::cout << std::endl;
    }

#else
    {
        f( cos( 3.14 ) );
        f( sq( 3.14 ) );

        std::vector<double> v { 1.0, 2.0, 3.0 };
        std::transform( v.begin(), v.end(), v.begin(), sq );

        std::for_each( v.begin(), v.end(), []( auto i ) { std::cout << i << "\t"; } );
        std::cout << std::endl;

        double x1 = ( -b + sqrt( b * b - 4 * a * c ) ) / ( 2 * a );
        double x2 = ( -b + sqrt( sq( b ) - 4 * a * c ) ) / ( 2 * a );

        std::cout << x1 << " " << x2 << "\n";
    }

#endif
    return 0;
}
