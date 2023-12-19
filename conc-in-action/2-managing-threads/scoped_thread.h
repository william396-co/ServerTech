#pragma once

#include <thread>
#include <stdexcept>

/*
 * Use RAII to wait for a thread to complete
 */
class scoped_thread
{
public:
    explicit scoped_thread( std::thread t )
        : t_ { std::move( t ) }
    {
        if ( !t_.joinable() ) {
            throw std::logic_error( "No Thread" );
        }
    }

    ~scoped_thread()
    {
        t_.join();
    }

    scoped_thread( scoped_thread const & ) = delete;
    scoped_thread & operator=( scoped_thread const & ) = delete;

private:
    std::thread t_;
};
