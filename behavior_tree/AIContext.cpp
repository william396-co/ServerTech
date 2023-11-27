#include "AIContext.h"
#include <cassert>

#include "CrossDoor.h"

AIContext::AIContext( CrossDoor * cd )
    : m_CrossDoor { cd }
{
    assert( m_CrossDoor != nullptr );
}

AIContext::~AIContext()
{
    m_CrossDoor = nullptr;
}

// SUCCESS if _door_open == true
brain::Status AIContext::isDoorClosed()
{
    return m_CrossDoor->isDoorClosed();
}
// SUCCESS if _door_open == true
brain::Status AIContext::passThroughDoor()
{
    return m_CrossDoor->passThroughDoor();
}
// After 3 attempts, will open a locked door
brain::Status pickLock()
{
    return m_CrossDoor->pickLock();
}
// FAILURE if door locked
brain::Status openDoor()
{
    return m_CrossDoor->openDoor();
}
// WILL always open a door
brain::Status smashDoor()
{
    return m_CrossDoor->smashDoor();
}

