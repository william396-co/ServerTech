#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef PARALLEL
#include <execution>
namespace execution = std::execution;
#else
enum class execution
{
    seq,
    unseq,
    par_unseq,
    par
};
#endif

int main()
{
    std::vector<int> myVec { 1, 2, 3, 4, 5 };

    try {
        std::for_each( myVec.begin(), myVec.end(), []( int ) {
            throw std::runtime_error( "Without execution policy" );
        } );
    } catch ( std::runtime_error const & e ) {
        std::cout << e.what() << "\n";
    }

    try {
        std::for_each( execution::seq, myVec.begin(), myVec.end(), []( int ) {
            throw std::runtime_error( "With execution policy" );
        } );
    } catch ( std::runtime_error const & e ) {
        std::cout << e.what() << "\n";
    } catch ( ... ) {
        std::cout << "Catch-all exceptions\n";
    }
    return 0;
}
