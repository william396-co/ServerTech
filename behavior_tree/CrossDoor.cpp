#include "CrossDoor.h"
#include "../util/print.hpp"

brain::Status CrossDoor::isDoorClose()
{
    _door_open ? printEx( "Door is Open" ) : printEx( "Door is Closed" );

    return _door_open ? brain::Status::Success : brain::Status::Failure;
}

brain::Status CrossDoor::passThroughDoor()
{
    _door_open ? printEx( __FUNCTION__, " Failure" ) : printEx( __FUNCTION__, " Success" );
    return _door_open ? brain::Status::Success : brain::Status::Failure;
}

brain::Status CrossDoor::pickLock()
{
    if ( ++_pick_attemps > 3 ) {
        _door_locked = false;
        _door_open ? printEx( "This is Open" ) : printEx( "Door is Closed" );
        return brain::Status::Success;
    }
    _door_open ? printEx( "Door is Open" ) : printEx( "Door is Closed" );
    return brain::Status::Failure;
}

brain::Status CrossDoor::openDoor()
{
    _door_locked ? printEx( __FUNCTION__, " Failure" ) : printEx( __FUNCTION__, " Success" );
    return _door_locked ? brain::Status::Failure : brain::Status::Success;
}

brain::Status CrossDoor::smashDoor()
{
    _door_locked = false;
    printEx( __FUNCTION__, " Success" );
    printEx( "Door is Open" );
    return brain::Status::Success;
}
