#pragma once
#include "queue.h"

namespace messaging {

class sender
{
public:
    sender()
        : q { nullptr } {}
    explicit sender( queue * q_ )
        : q { q_ } {}

    template<typename Message>
    void send( Message const & msg )
    {
        if ( q ) {
            q->push( msg );
        }
    }

private:
    queue * q;
};

} // namespace messaging
