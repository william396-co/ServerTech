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
#define BUFFER_SIZE 1024

struct fds
{
    int epollfd;
    int sockfd;
};

// set fd nonblock mode
int setnonblocking( int fd )
{
    int old_opt = fcntl( fd, F_GETFL );
    int new_opt = old_opt | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_opt );
    return old_opt;
}
/* 将fd上的EPOLLIN和EPOLLET事件注册到epollfd指示的epoll内核时间表中，参数one_shot指定是否对注册fd上的EPOLLONESHOT事件 */
void addfd( int epollfd, int fd, int oneshot )
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if ( oneshot ) {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}
/* 重置fd上的事件。这样操作之后，尽管fd上的EPOLLLONGSHOT事件被注册，但是操作系统仍然会触发fd上的EPOLLIN事件，且只触发一次 */
void reset_oneshot( int epollfd, int fd )
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

/* 工作线程 */
void * worker( void * arg )
{
    int sockfd = ( (struct fds *)arg )->sockfd;
    int epollfd = ( (struct fds *)arg )->epollfd;
    printf( "start new thread to receive data on fd:%d\n", sockfd );

    char buf[BUFFER_SIZE];
    memset( buf, 0, BUFFER_SIZE );

    // 循环读取sockfd上的数据,直到遇到EAGAIN错误
    while ( 1 ) {

        int ret = recv( sockfd, buf, BUFFER_SIZE - 1, 0 );
        if ( 0 == ret ) {
            close( sockfd );
            printf( "foreiner closed the connection\n" );
            break;
        } else if ( ret < 0 ) {
            if ( errno == EAGAIN ) {
                reset_oneshot( epollfd, sockfd );
                printf( "read later\n" );
                break;
            }
        } else {
            printf( "get content: %s\n", buf );
            // 休眠5s 模拟数据处理过程
            sleep( 5 );
        }
    }
    printf( "end thread receiving data on fd: %d\n", sockfd );
    return NULL;
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
    assert( ret != -2 && "listen failed" );

    struct epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd, 0 ); /* 注意，监听socket上是不能注册EPOLLONESHOT事件的，否则应用程序只能处理一个客户端连接，因为后续的客户连接请求将不再触发listenfd上的EPOLLIN事件 */

    while ( 1 ) {
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        if ( ret < 0 ) {
            printf( "epoll failure\n" );
            break;
        }

        for ( int i = 0; i < ret; ++i ) {
            int sockfd = events[i].data.fd;
            if ( sockfd == listenfd ) {
                struct sockaddr_in laddr;
                socklen_t addr_len = sizeof( laddr );
                int connfd = accept( listenfd, (struct sockaddr *)&laddr, &addr_len );

                /* 对每个非监听文件描述符都注册EPOLLONESHOT事件  */
                addfd( epollfd, connfd, 1 );

            } else if ( events[i].events & EPOLLIN ) {
                pthread_t thread;
                struct fds fds_new;
                fds_new.epollfd = epollfd;
                fds_new.sockfd = sockfd;
                /* 新启动一个工作线程为sockfd服务 */
                pthread_create( &thread, NULL, worker, (void *)&fds_new );
            } else {
                printf( "something else happened\n" );
            }
        }
    }
    close( listenfd );
    return 0;
}
