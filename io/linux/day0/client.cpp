#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring> // bzero

int main()
{

    std::cout << "client\n";

    int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

    struct sockaddr_in serv_addr;
    bzero( &serv_addr, sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    serv_addr.sin_port = htons( 9527 );

    // bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)); 客户端不进行bind操作

    connect( sockfd, (sockaddr *)&serv_addr, sizeof( serv_addr ) );

    return 0;
}
