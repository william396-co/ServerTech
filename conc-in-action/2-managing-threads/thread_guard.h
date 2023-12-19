#pragma once

#include <thread>

/*
 * Use RAII to wait for a thread to complete
 */
class thread_guard
{
public:
    explicit thread_guard( std::thread & t )
        : t_ { t } {}

    ~thread_guard()
    {
        if ( t_.joinable() ) {
            t_.join();
        }
    }

    thread_guard( thread_guard const & ) = delete;
    thread_guard & operator=( thread_guard const & ) = delete;

private:
    std::thread & t_;
};
