#include <iostream>
#include <string>
#include <mutex>
#include <thread>

std::mutex coutMutex;

thread_local std::string s( "hello from " );

void addThreadLocal( std::string const & s2 )
{
    s += s2;
    std::lock_guard lock( coutMutex );
    std::cout << s << "\n";
    std::cout << "&s: " << &s << "\n";
}

int main()
{

    std::thread t1( addThreadLocal, "t1" );
    std::thread t2( addThreadLocal, "t2" );
    std::thread t3( addThreadLocal, "t3" );
    std::thread t4( addThreadLocal, "t4" );

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << s << " &s:" << &s << "\n";
    return 0;
}
