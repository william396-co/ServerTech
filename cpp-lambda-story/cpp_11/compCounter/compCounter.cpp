#include <algorithm>
#include <iostream>
#include <vector>

int main()
{

    std::vector<int> vi { 0, 5, 2, 9, 7, 6, 1, 3, 4, 8 };

    size_t compCounter = 0;
    std::sort( vi.begin(), vi.end(), [&compCounter]( int a, int b ) noexcept {
        ++compCounter;
        return a < b;
    } );

    std::cout << "number of comparision: " << compCounter << "\n";

    for ( auto const & i : vi ) {
        std::cout << i << ",";
    }
    std::cout << "\n";

    return 0;
}
