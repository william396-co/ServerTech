#include "ThreadPool.h"
#include <stdexcept>

ThreadPool::ThreadPool( int size )
    : stop { false }
{
    for ( int i = 0; i != size; ++i ) {
        threads.emplace_back( std::thread( [this]() {
            while ( true ) {
                Task task;
                {
                    std::unique_lock lock( tasks_mtx );
                    cv.wait( lock, [this]() {
                        return stop || !tasks.empty();
                    } );
                    if ( stop && tasks.empty() ) return;

                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        } ) );
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::scoped_lock lock( tasks_mtx );
        stop = true;
    }

    cv.notify_all();
    for ( auto & it : threads ) {
        if ( it.joinable() )
            it.join();
    }
}

void ThreadPool::add( Task task )
{
    {
        std::scoped_lock lock( tasks_mtx );
        if ( stop )
            throw std::runtime_error( "ThreadPool already stop, can't add task any more" );
        tasks.emplace( task );
    }

    cv.notify_one();
}
