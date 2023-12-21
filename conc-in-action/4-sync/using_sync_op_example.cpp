#include "using_sync_op_example.h"

#include <future>
#include <thread>
#include <chrono>
#include <string>

void fp_with_futures()
{
    std::list<int> li = { 5, 7, 3, 4, 1, 9, 2, 8, 10, 6 };
    {
        auto sl = sequential_quick_sort( li );
        std::cout << "after sequential_quick_sort\n";
        for ( auto const & l : sl ) {
            std::cout << l << "\t";
        }
        std::cout << "\n";
    }
    {
        auto sl = parallel_quick_sort( li );
        std::cout << "after parallel_quick_sort\n";
        for ( auto const & l : sl ) {
            std::cout << l << "\t";
        }
        std::cout << "\n";
    }

    {
        std::vector<int> vi = { 5, 7, 3, 4, 1, 9, 2, 8, 10, 6 };
        auto svi = sequential_quick_sort( vi );
        std::cout << "after sequential_quick_sort\n";
        for ( auto const & i : svi ) {
            std::cout << i << "\t";
        }
        std::cout << "\n";
    }
    {
        std::vector<int> vi = { 5, 7, 3, 4, 1, 9, 2, 8, 10, 6 ,5};
        auto svi = vparallel_quick_sort( vi );
        std::cout << "after vparallel_quick_sort\n";
        for ( auto const & i : svi ) {
            std::cout << i << "\t";
        }
        std::cout << "\n";
    }
    std::cout << __PRETTY_FUNCTION__ << "\n";
}
