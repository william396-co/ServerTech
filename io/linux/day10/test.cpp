#include "src/InetAddress.h"
#include "src/Socket.h"
#include "src/ThreadPool.h"
#include "src/Buffer.h"

#include <functional>
#include <memory>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

constexpr auto ip = "127.0.0.1";
constexpr auto port = 9527;

void oneClient( int msgs, int wait )
{
    std::unique_ptr<Socket> s = std::make_unique<Socket>();
    InetAddress addr { ip, port };
    s->connect( addr );

    int fd = s->getFd();
    std::unique_ptr<Buffer> sendBuffer = std::make_unique<Buffer>();
    std::unique_ptr<Buffer> readBuffer = std::make_unique<Buffer>();

    sleep( wait );
    int count = 0;
    while ( count < msgs ) {
        sendBuffer->setBuf( "I'm client!" );
        ssize_t write_bytes = write( fd, sendBuffer->c_str(), sendBuffer->size() );
        if ( write_bytes == -1 ) {
            printf( "socket already disconnected, can't write any more!\n" );
            break;
        }

        int already_read = 0;
        char buf[1024];
        while ( true ) {
            bzero( &buf, sizeof( buf ) );
            ssize_t read_bytes = read( fd, buf, sizeof( buf ) );

            if ( read_bytes > 0 ) {
                readBuffer->append( buf, read_bytes );
                already_read += read_bytes;
            } else if ( read_bytes == 0 ) {
                printf( "server disconnected!\n" );
                exit( 1 );
            }
            if ( already_read >= sendBuffer->size() ) {
                printf( "count:%d,message from server:%s\n", count++, readBuffer->c_str() );
                break;
            }
        }
        readBuffer->clear();
    }
}

int main( int argc, char * argv[] )
{
    int threads = 100;
    int msgs = 100;
    int wait = 0;
    int o;
    const char * optstring = "t:m:w";
    while ( ( o = getopt( argc, argv, optstring ) ) != -1 ) {
        switch ( o ) {
            case 't':
                threads = std::stoi( optarg );
                break;
            case 'm':
                msgs = std::stoi( optarg );
                break;
            case 'w':
                wait = std::stoi( optarg );
                break;
            case '?':
                printf( "error optop:%c\n", optopt );
                printf( "error opterr:%d\n", opterr );
                break;
        }
    }

    std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>( threads );
    std::function<void()> func = std::bind( oneClient, msgs, wait );
    for ( int i = 0; i < threads; ++i ) {
        pool->add( oneClient, msgs, wait );
    }

    return 0;
}
