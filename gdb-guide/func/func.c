#include <stdio.h>
#include <pthread.h>

void * thread_func( void * arg )
{
    printf( "[%s] is running\n", (const char *)arg );
    while ( 1 ) {
        sleep( 10 );
    }
}

int main()
{

    pthread_t t1, t2;

    pthread_create( &t1, NULL, thread_func, "Thread 1" );
    pthread_create( &t2, NULL, thread_func, "Thread 2" );

    sleep( 10000 );

    return 0;
}
