#pragma once

#include <thread>

class joining_thread
{
public:
    template<typename F, typename... Args>
    explicit joining_thread( F && f, Args &&... args )
        : t { std::forward<F>( f ), std::forward<Args>( args )... }
    {
    }
    explicit joining_thread( std::thread t_ ) noexcept
        : t { std::move( t_ ) } {}

    joining_thread & operator=( std::thread t_ ) noexcept
    {
        if ( joinable() ) {
            join();
        }
        t = std::move( t_ );
        return *this;
    }

    joining_thread( joining_thread && other ) noexcept
        : t { std::move( other.t ) } {}
    joining_thread & operator=( joining_thread && other ) noexcept
    {
        if ( joinable() ) {
            join();
        }

        t = std::move( other.t );
        return *this;
    }

    ~joining_thread() noexcept
    {
        if ( joinable() ) {
            join();
        }
    }
    void swap( joining_thread & other ) noexcept
    {
        t.swap( other.t );
    }
    bool joinable() const noexcept
    {
        return t.joinable();
    }
    void join()
    {
        t.join();
    }

    void detach()
    {
        t.detach();
    }

    std::thread & as_thread() noexcept
    {
        return t;
    }
    const std::thread & as_thread() const noexcept
    {
        return t;
    }

private:
    std::thread t;
};
