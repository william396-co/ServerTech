#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

std::mutex coutMutex;

class Worker
{
public:
    Worker( std::string const & n )
        : name { n } {}
    void operator()()
    {
        for ( int i = 1; i <= 3; ++i ) {
            std::this_thread::sleep_for( std::chrono::milliseconds { 200 } );
            std::scoped_lock lock( coutMutex );
            std::cout << name << ": "
                      << "Work " << i << " done!!!\n";
        }
    }

private:
    std::string name;
};

int main()
{

    std::cout << "Boss: Let's start working.\n\n";

    std::thread herb( Worker( "Herb" ) );
    std::thread andrei( Worker( "  Andrei" ) );
    std::thread scott( Worker( "    Scott" ) );
    std::thread bjarne( Worker( "     Bjarne" ) );
    std::thread bart( Worker( "       Bart" ) );
    std::thread jenne( Worker( "         Jenne" ) );

    herb.join();
    andrei.join();
    scott.join();
    bjarne.join();
    bart.join();
    jenne.join();

    std::cout << "\nBoss: Let's go home.\n";

    return 0;
}
