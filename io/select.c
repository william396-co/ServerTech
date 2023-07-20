#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char ** argv )
{
    if ( argc != 3 ) {
        printf( "Usage: %s <Ip> <Port>\n", argv[0] );
        return 1;
    }

    const char * ip = argv[1];
    int port = atoi( argv[2] );

    int ret = 0;
    struct sockaddr_in addr;
    bzero( &addr, sizeof( addr ) );
    addr.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &addr.sin_addr );
    addr.sin_port = htons( port );

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd > 0 );
    ret = bind( listenfd, (struct sockaddr *)&addr, sizeof( addr ) );
    assert( ret != -1 );
    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    struct sockaddr_in faddr;
    socklen_t faddrlen = sizeof( faddr );
    int connfd = accept( listenfd, (struct sockaddr *)&faddr, &faddrlen );
    if ( connfd < 0 ) {
        printf( "errono is: %d\n", errno );
        close( listenfd );
    }

    char buf[1024];
    fd_set read_fds;
    fd_set exception_fds;
    FD_ZERO( &read_fds );
    FD_ZERO( &exception_fds );

    while ( 1 ) {

        memset( buf, 0, sizeof( buf ) );
        FD_SET( connfd, &read_fds );
        FD_SET( connfd, &exception_fds );

        ret = select( connfd + 1, &read_fds, NULL, &exception_fds, NULL );
        if ( ret < 0 ) {
            printf( "selection failure\n" );
            break;
        }

        // 对于可读事件
        if ( FD_ISSET( connfd, &read_fds ) ) {
            ret = recv( connfd, buf, sizeof( buf ) - 1, 0 );
            if ( ret <= 0 ) {
                break;
            }
        } /* 对于异常事件，采用带MSG_OOB标志的recv函数读取带外数据*/
        else if ( FD_ISSET( connfd, &exception_fds ) ) {
            ret = recv( connfd, buf, sizeof( buf ) - 1, MSG_OOB );
            if ( ret <= 0 ) {
                break;
            }
            printf( "get %d bytes of oob data: %s\n", ret, buf );
        }
    }

    close( connfd );
    close( listenfd );
    return 0;
}
