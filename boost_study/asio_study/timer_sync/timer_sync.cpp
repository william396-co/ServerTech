#include <iostream>
#include <boost/asio.hpp>

int main()
{

    std::cout << "Timer sync start\n";
    boost::asio::io_context io;
    boost::asio::steady_timer t( io, boost::asio::chrono::seconds( 5 ) );

    t.wait();
    std::cout << "Hello World\n";

    return 0;
}
