#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

// set fd nonblock mode
int setnonblocking( int fd )
{
    int old_opt = fcntl( fd, F_GETFL );
    int new_opt = old_opt | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_opt );
    return old_opt;
}

// 将文件描述符fd上的EPOLLIN注册到epollfd指示的epoll内核事件表中,参数enable_et指定是否对fd启用ET模式
void addfd( int epollfd, int fd, int enable_et )
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if ( enable_et ) {
        event.events |= EPOLLET;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

// LT 模式的工作流程
void lt( struct epoll_event * events, int number, int epollfd, int listenfd )
{
    char buf[BUFFER_SIZE];
    for ( int i = 0; i < number; ++i ) {
        int sockfd = events[i].data.fd;
        if ( sockfd == listenfd ) {
            struct sockaddr_in faddr;
            socklen_t faddrlen = sizeof( faddr );
            int connfd = accept( listenfd, (struct sockaddr *)&faddr, &faddrlen );
            addfd( epollfd, connfd, 0 );
        } else if ( events[i].events & EPOLLIN ) {
            // 只要socket读缓存中还有未读出的数据，这段代码就被触发
            printf( "event trigger once\n" );
            memset( buf, 0, sizeof( buf ) );
            int ret = recv( sockfd, buf, sizeof( buf ) - 1, 0 );
            if ( ret <= 0 ) {
                close( sockfd );
                continue;
            }
            printf( "get %d bytes of contents:[ %s]\n", ret, buf );
        } else {
            printf( "something else happened\n" );
        }
    }
}

// ET模式的工作流程
void et( struct epoll_event * events, int number, int epollfd, int listenfd )
{
    char buf[BUFFER_SIZE];
    for ( int i = 0; i < number; ++i ) {
        int sockfd = events[i].data.fd;
        if ( sockfd == listenfd ) {
            struct sockaddr_in faddr;
            socklen_t faddrlen = sizeof( faddr );
            int connfd = accept( listenfd, (struct sockaddr *)&faddr, &faddrlen );
            addfd( epollfd, connfd, 1 ); /*  对connfd开启ET模式 */
        } else if ( events[i].events & EPOLLIN ) {
            /* 这段代码不会被重复触发，所以我们循环读取数据，以确保把socket读缓存中的所有数据读出 */
            printf( "event trigger once\n" );
            while ( 1 ) {
                memset( buf, 0, sizeof( buf ) );
                int ret = recv( sockfd, buf, sizeof( buf ) - 1, 0 );
                if ( ret < 0 ) {
                    /*  对于非阻塞IO，下面的条件成立表示数据已经全部读取完毕，此后，epoll就能再次触发sockfd上的EPOLLIN事件，以驱动下一次读操作*/
                    if ( ( errno == EAGAIN ) || ( errno == EWOULDBLOCK ) ) {
                        printf( "read later\n" );
                        break;
                    }
                    close( sockfd );
                    break;
                } else if ( ret == 0 ) {
                    close( sockfd );
                } else {
                    printf( "get %d bytes of contents: [%s]\n", ret, buf );
                }
            }
        } else {
            printf( "something else happened\n" );
        }
    }
}

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
    assert( listenfd > 0 && "socket failed" );

    ret = bind( listenfd, (struct sockaddr *)&addr, sizeof( addr ) );
    assert( ret != -1 && "bind failed" );

    ret = listen( listenfd, 5 );
    assert( ret != -1 && "listen failed" );

    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd, 1 );

    while ( 1 ) {
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if ( ret < 0 ) {
            printf( "epoll failure\n" );
            break;
        }
        //        lt( events, ret, epollfd, listenfd ); /* LT 模式*/
        et( events, ret, epollfd, listenfd ); /* ET 模式*/
    }
    close( listenfd );
    return 0;
}
