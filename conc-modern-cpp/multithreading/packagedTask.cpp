#include <utility>
#include <future>
#include <iostream>
#include <thread>
#include <deque>

class SumUp
{

public:
    int operator()( int beg, int end )
    {
        long long int sum { 0 };
        for ( int i = beg; i != end; ++i ) {
            sum += i;
        }
        return sum;
    }
};

using sumTask = std::packaged_task<int( int, int )>;
using sumResult = std::future<int>;

int main()
{

    SumUp s1;
    SumUp s2;
    SumUp s3;
    SumUp s4;

    // wrap the tasks
    sumTask t1( s1 );
    sumTask t2( s2 );
    sumTask t3( s3 );
    sumTask t4( s4 );

    // create the futures
    sumResult r1 = t1.get_future();
    sumResult r2 = t2.get_future();
    sumResult r3 = t3.get_future();
    sumResult r4 = t4.get_future();

    // push the tasks on the container
    std::deque<sumTask> allTasks;
    allTasks.push_back( std::move( t1 ) );
    allTasks.push_back( std::move( t2 ) );
    allTasks.push_back( std::move( t3 ) );
    allTasks.push_back( std::move( t4 ) );

    int begin { 1 };
    int increment { 2500 };
    int end = begin + increment;

    // perform each calculation in a separate thread
    while ( !allTasks.empty() ) {
        sumTask myTask = std::move( allTasks.front() );
        allTasks.pop_front();

        std::thread sumThread( std::move( myTask ), begin, end );
        begin = end;
        end += increment;
        sumThread.detach();
    }

    // pick up the results
    auto sum = r1.get() + r2.get() + r3.get() + r4.get();

    std::cout << " sum of 0 ... 10000 = " << sum << "\n";

    return 0;
}
