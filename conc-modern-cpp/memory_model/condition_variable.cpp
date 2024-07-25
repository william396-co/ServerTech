
#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>


std::vector<int> mySharedWork;
std::mutex mutex_;
std::condition_variable condVar;

bool dataReady { false };

void waitingForWork(){
    std::cout << "waiting \n";
    std::unique_lock lck( mutex_ );
    condVar.wait( lck, [] { return dataReady; } );
    mySharedWork[1] = 2;
    std::cout << "work done\n";
}

void setDataRead(){
    mySharedWork = { 1, 0, 3 };
    {
        std::lock_guard lock( mutex_ );
        dataReady = true;
    }
    std::cout << "Data prepared\n";
    condVar.notify_one();
}

int main(){

   std::cout << std::endl;

    std::thread t1( waitingForWork );
    std::thread t2( setDataRead );

    t1.join();
    t2.join();

    for ( auto const & v : mySharedWork ) {
        std::cout << v << "\n";
    }



    return 0;
}
