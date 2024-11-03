#include "util.h"

#include <cstdio>
#include <cstdlib>

void errif( bool cond, const char * message )
{
    if ( cond ) {
        perror( message );
        exit( EXIT_FAILURE );
    }
}
