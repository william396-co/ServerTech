#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int main( int argc, char ** argv )
{
    if ( argc != 2 ) {
        std::cout << " server: Usage<port>\n";
        return 1;
    }

    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    struct sockaddr_in s_addr;
    // bzero( &s_addr, sizeof( s_addr ) );
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    s_addr.sin_port = htons( atoi( argv[1] ) );

    bind( sockfd, (sockaddr *)&s_addr, sizeof( s_addr ) );

    listen( sockfd, SOMAXCONN );

    struct sockaddr_in c_addr;
    socklen_t c_addr_len = sizeof( c_addr );
    //    bzero( &c_addr, c_addr_len );

    while ( true ) {
        int client_fd = accept( sockfd, (sockaddr *)&c_addr, &c_addr_len );

        std::cout << "new client fd: " << client_fd << " Ip :" << inet_ntoa( c_addr.sin_addr ) << " Port:" << ntohs( c_addr.sin_port ) << std::endl;
    }

    return 0;
}
