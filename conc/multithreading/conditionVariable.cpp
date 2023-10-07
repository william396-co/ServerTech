#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>

std::mutex mutex_;
std::condition_variable condVar;

bool dataReady { false };

void doTheWork()
{
    std::cout << "Processing shared data.\n";
}

void waitingForWork()
{
    std::cout << "Worker: waiting  for work\n";
    std::unique_lock lck( mutex_ );                // unique_lock is needed(lock mutex)
    condVar.wait( lck, [] { return dataReady; } ); //  unlock-susupend, wakeup-lock

    doTheWork();
    std::cout << "Work done\n";
}

void setDataReady()
{
    {
        std::lock_guard lock( mutex_ );
        dataReady = true;
    }

    std::cout << "Sender: Data is ready.\n";
    condVar.notify_one();
}

int main()
{
    //    std::cout << std::endl;

    std::thread t1( waitingForWork );
    std::thread t2( setDataReady );

    t1.join();
    t2.join();

    return 0;
}
