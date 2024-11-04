#include "util.h"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

void errif( bool cond, const char * message )
{
    if ( cond ) {
        perror( message );
        exit( EXIT_FAILURE );
    }
}

/********************************
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - Open connection to server at <hostname, port> and
 *     return a socket descriptor ready for reading and writing. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_clientfd */
int open_clientfd( const char * hostname, const char * port )
{
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset( &hints, 0, sizeof( struct addrinfo ) );
    hints.ai_socktype = SOCK_STREAM; /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV; /* ... using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG; /* Recommended for connections */
    if ( ( rc = getaddrinfo( hostname, port, &hints, &listp ) ) != 0 ) {
        fprintf( stderr, "getaddrinfo failed (%s:%s): %s\n", hostname, port, gai_strerror( rc ) );
        return -2;
    }

    /* Walk the list for one that we can successfully connect to */
    for ( p = listp; p; p = p->ai_next ) {
        /* Create a socket descriptor */
        if ( ( clientfd = socket( p->ai_family, p->ai_socktype, p->ai_protocol ) ) < 0 )
            continue; /* Socket failed, try the next */

        /* Connect to the server */
        if ( connect( clientfd, p->ai_addr, p->ai_addrlen ) != -1 )
            break;                                                       /* Success */
        if ( close( clientfd ) < 0 ) { /* Connect failed, try another */ // line:netp:openclientfd:closefd
            fprintf( stderr, "open_clientfd: close failed: %s\n", strerror( errno ) );
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo( listp );
    if ( !p ) /* All connects failed */
        return -1;
    else /* The last connect succeeded */
        return clientfd;
}

/* $end open_clientfd */

/*
 * open_listenfd - Open and return a listening socket on port. This
 *     function is reentrant and protocol-independent.
 *
 *     On error, returns:
 *       -2 for getaddrinfo error
 *       -1 with errno set for other errors.
 */
/* $begin open_listenfd */
int open_listenfd( const char * port )
{
    struct addrinfo hints, *listp, *p;
    int listenfd, rc, optval = 1;

    /* Get a list of potential server addresses */
    memset( &hints, 0, sizeof( struct addrinfo ) );
    hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
    hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
    if ( ( rc = getaddrinfo( NULL, port, &hints, &listp ) ) != 0 ) {
        fprintf( stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror( rc ) );
        return -2;
    }

    /* Walk the list for one that we can bind to */
    for ( p = listp; p; p = p->ai_next ) {
        /* Create a socket descriptor */
        if ( ( listenfd = socket( p->ai_family, p->ai_socktype, p->ai_protocol ) ) < 0 )
            continue; /* Socket failed, try the next */

        /* Eliminates "Address already in use" error from bind */
        setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, // line:netp:csapp:setsockopt
            (const void *)&optval,
            sizeof( int ) );

        /* Bind the descriptor to the address */
        if ( bind( listenfd, p->ai_addr, p->ai_addrlen ) == 0 )
            break;                     /* Success */
        if ( close( listenfd ) < 0 ) { /* Bind failed, try the next */
            fprintf( stderr, "open_listenfd close failed: %s\n", strerror( errno ) );
            return -1;
        }
    }

    /* Clean up */
    freeaddrinfo( listp );
    if ( !p ) /* No address worked */
        return -1;

    /* Make it a listening socket ready to accept connection requests */
    if ( listen( listenfd, SOMAXCONN ) < 0 ) {
        close( listenfd );
        return -1;
    }
    return listenfd;
}
/* $end open_listenfd */

