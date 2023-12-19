#include <functional>
#include <future>
#include <iostream>
#include <utility>
#include <vector>

using SumTask = std::packaged_task<int( int, int )>;

void calcProducts( SumTask & task, std::vector<std::pair<int, int>> const & pairs )
{
    for ( auto const & p : pairs ) {
        auto fut = task.get_future();
        task( p.first, p.second );
        std::cout << p.first << " * " << p.second << " = " << fut.get() << "\n";
        task.reset();
    }
}

int main()
{

    std::vector<std::pair<int, int>> allPairs;
    allPairs.emplace_back( 1, 2 );
    allPairs.emplace_back( 2, 3 );
    allPairs.emplace_back( 3, 4 );
    allPairs.emplace_back( 4, 5 );

    SumTask task { []( int fir, int sec ) { return fir * sec; } };

    calcProducts( task, allPairs );

    std::cout << "finished Main thread\n";

    std::thread t( calcProducts, std::ref( task ), allPairs );
    t.join();

    return 0;
}
