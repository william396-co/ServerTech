#include <iostream>
#include "brain.h"

#include "AIContext.h"
#include "CrossDoor.h"

using namespace brain;

struct IsDoorClosed : public brain::Leaf
{
public:
    virtual ~IsDoorClosed() {}
    virtual brain::Status execute( brain::Context * ctx ) override
    {
        return static_cast<AIContext *>( ctx )->isDoorClosed();
    }
};

struct PassThroughDoor : public brain::Leaf
{
public:
    virtual ~PassThroughDoor() {}
    virtual brain::Status execute( brain::Context * ctx ) override
    {
        return static_cast<AIContext *>( ctx )->passThroughDoor();
    }
};

class OpenDoor : public brain::Leaf
{
public:
    virtual ~OpenDoor() {}
    virtual brain::Status execute( brain::Context * ctx ) override
    {
        return static_cast<AIContext *>( ctx )->openDoor();
    }
};

class PickLock : public brain::Leaf
{
public:
    virtual ~PickLock() {}
    virtual brain::Status execute( brain::Context * ctx ) override
    {
        return static_cast<AIContext *>( ctx )->pickLock();
    }
};

class SmashDoor : public brain::Leaf
{
public:
    virtual ~SmashDoor() {}
    virtual brain::Status execute( brain::Context * ctx ) override
    {
        return static_cast<AIContext *>( ctx )->smashDoor();
    }
};

brain::Tree * make_brain( int num_attemps )
{
    return brain::Builder()
        .composite<brain::Sequence>()
        .composite<brain::Selector>()
        .decorator<brain::Inverter>()
        .leaf<IsDoorClosed>()
        .end()
        .composite<brain::Selector>()
        .leaf<OpenDoor>()
        .decorator<brain::Repeater>( num_attemps )
        .leaf<PickLock>()
        .end()
        .leaf<SmashDoor>()
        .end()
        .end()
        .end()
        .build( nullptr );
}

int main()
{
    std::cout << "open door\n";

    brain::Tree * bt = make_brain( 5 );

    CrossDoor cd;

    AIContext * ctx = new AIContext( &cd );

    bt->update( ctx );

    delete ctx;

    return 0;
}
