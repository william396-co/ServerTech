#include "task.h"

int main()
{
    manual_executor ex;

    ex.sync_wait( loop_synchronously( 100 ) );
    ex.sync_wait( loop_synchronously( 1000 ) );
    ex.sync_wait( loop_synchronously( 100000 ) );
    ex.sync_wait( loop_synchronously( 1000000 ) ); // stack overflow

    return 0;
}
