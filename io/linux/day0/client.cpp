#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring> // bzero

int main( int argc, char ** argv )
{

    if ( argc != 3 ) {
        std::cout << "client Usage<ip><port>\n";
        return 1;
    }

    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    struct sockaddr_in serv_addr;
    bzero( &serv_addr, sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr( argv[1] );
    serv_addr.sin_port = htons( atoi( argv[2] ) );

    // bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作

    connect( sockfd, (sockaddr *)&serv_addr, sizeof( serv_addr ) );

    return 0;
}
