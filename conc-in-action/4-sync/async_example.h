#pragma once

#include <future>
#include <string>
#include <iostream>

struct X
{
    void foo( int id, std::string const & msg )
    {
        std::cout << __PRETTY_FUNCTION__ << "(" << id << "," << msg << ")\n";
    }
    std::string bar( std::string const & msg )
    {
        std::cout << __PRETTY_FUNCTION__ << "(" << msg << ")\n";
        return msg;
    }
};

struct Y
{
    double operator()( double d )
    {
        std::cout << __PRETTY_FUNCTION__ << "(" << d << ")\n";
        return d * 100;
    }
};

X baz( X & x );

class move_only
{
public:
    move_only();
    move_only( move_only && other ) noexcept {}
    move_only & operator=( move_only && other ) noexcept
    {
        return *this;
    }

    move_only( move_only const & ) = delete;
    move_only & operator=( move_only const & ) = delete;
    void operator()();
};

void async_example();

void async_example2();
