#include "CrossDoor.h"
#include "../util/print.hpp"

brain::Status CrossDoor::isDoorClosed()
{
    _door_open ? printlnEx( "Door is Open" ) : printlnEx( "Door is Closed" );

    return _door_open ? brain::Status::Success : brain::Status::Failure;
}

brain::Status CrossDoor::passThroughDoor()
{
    _door_open ? printlnEx( __FUNCTION__, " Failure" ) : printlnEx( __FUNCTION__, " Success" );
    return _door_open ? brain::Status::Success : brain::Status::Failure;
}

brain::Status CrossDoor::pickLock()
{
    if ( ++_pick_attemps > 3 ) {
        _door_locked = false;
        _door_open ? printlnEx( "This is Open" ) : printlnEx( "Door is Closed" );
        return brain::Status::Success;
    }
    _door_open ? printlnEx( "Door is Open" ) : printlnEx( "Door is Closed" );
    return brain::Status::Failure;
}

brain::Status CrossDoor::openDoor()
{
    _door_locked ? printlnEx( __FUNCTION__, " Failure" ) : printlnEx( __FUNCTION__, " Success" );
    return _door_locked ? brain::Status::Failure : brain::Status::Success;
}

brain::Status CrossDoor::smashDoor()
{
    _door_locked = false;
    printlnEx( __FUNCTION__, " Success" );
    printlnEx( "Door is Open" );
    return brain::Status::Success;
}
