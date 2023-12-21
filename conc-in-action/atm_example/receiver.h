#pragma once

#include "queue.h"
#include "sender.h"
#include "dispatcher.h"

namespace messaging {

class receiver
{
public:
    operator sender()
    {
        return sender( &q );
    }
    dispatcher wait()
    {
        return dispatcher( &q );
    }

private:
    queue q;
};
} // namespace messaging
