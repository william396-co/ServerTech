//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2024 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <functional>
#include <iostream>
#include <boost/asio.hpp>

#define USE_BOOST_BIND
#ifdef USE_BOOST_BIND
#include <boost/bind/bind.hpp>
#endif

void print( const boost::system::error_code & /*e*/,
    boost::asio::steady_timer * t, int * count )
{
    if ( *count < 5 ) {
        std::cout << *count << std::endl;
        ++( *count );

        t->expires_at( t->expiry() + boost::asio::chrono::seconds( 1 ) );
#ifndef USE_BOOST_BIND
        t->async_wait( std::bind( print, std::placeholders::_1, t, count ) );
#else
        t->async_wait( boost::bind( print, boost::asio::placeholders::error, t, count ) );
#endif
    }
}

int main()
{
    boost::asio::io_context io;

    int count = 0;
    boost::asio::steady_timer t( io, boost::asio::chrono::seconds( 1 ) );

#ifndef USE_BOOST_BIND
    t.async_wait( std::bind( print, std::placeholders::_1, &t, &count ) );
#else
    t.async_wait( boost::bind( print, boost::asio::placeholders::error, &t, &count ) );
#endif

    io.run();

    std::cout << "Final count is " << count << std::endl;

    return 0;
}
