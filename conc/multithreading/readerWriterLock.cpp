#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>
#include <chrono>

std::map<std::string, int> teleBook {
    { "Dijkstra", 1972 }, { "Scott", 1976 }, { "Ritchie", 1983 }
};

std::shared_timed_mutex teleBookMutex;

void addToTeleBook( std::string const & name, int tele )
{
    std::lock_guard<std::shared_timed_mutex> writerLock( teleBookMutex );
    std::cout << "\nSTARTING UPDATE " << name;
    std::this_thread::sleep_for( std::chrono::milliseconds { 500 } );
    teleBook[name] = tele;
    std::cout << " ...ENDING UPDATE " << name << "\n";
}

void printNumber( std::string const & name )
{
    std::shared_lock readerlock( teleBookMutex );
    auto it = teleBook.find( name );
    if ( it == teleBook.end() ) {
        std::cout << name << " not found\n";
    } else {
        std::cout << it->first << ":" << it->second << "\n";
    }
}

int main()
{

    std::thread reader1( [] { printNumber( "Scott" ); } );
    std::thread reader2( [] { printNumber( "Ritchie" ); } );
    std::thread w1( [] { addToTeleBook( "Scott", 1968 ); } );

    std::thread reader3( [] { printNumber( "Dijkstra" ); } );
    std::thread reader4( [] { printNumber( "Scott" ); } );
    std::thread w2( [] { addToTeleBook( "Bjarne", 1965 ); } );

    std::thread reader5( [] { printNumber( "Scott" ); } );
    std::thread reader6( [] { printNumber( "Ritchie" ); } );
    std::thread reader7( [] { printNumber( "Scott" ); } );
    std::thread reader8( [] { printNumber( "Bjarne" ); } );

    reader1.join();
    reader2.join();
    reader3.join();
    reader4.join();
    reader5.join();
    reader6.join();
    reader7.join();
    reader8.join();

    w1.join();
    w2.join();

    std::cout << "\nThe new telephone book\n";
    for ( auto const & it : teleBook ) {
        std::cout << it.first << ": " << it.second << "\n";
    }

    return 0;
}
