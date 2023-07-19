#include <sys/types.h>
#include <sys/socket.h>
#include <neinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdilb.h>

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
    fd_set exceptions_fds;
    FD_ZERO( &read_fds );
    FD_ZERO( &exception_fds );

    while ( 1 ) {

        memset( buf, 0, sizeof( buf ) );
        FD_SET( connfd, &read_fds );
        FD_SET( connfd, &exceptions_fds );
    }
}
