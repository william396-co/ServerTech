#pragma once
#include <memory>

#include "queue.h"
#include "dispatcher.h"

namespace messaging {

template<typename PreviousDispatcher, typename Msg, typename Func>
class TemplateDispatcher
{
private:
    TemplateDispatcher( TemplateDispatcher const & ) = delete;
    TemplateDispatcher & operator=( TemplateDispatcher const & ) = delete;

    template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
    friend class TemplateDispatcher;

    void wait_and_dispatch()
    {
        while ( true ) {
            auto msg = q->wait_and_pop();
            if ( dispatch( msg ) )
                break;
        }
    }

    bool dispatch( std::shared_ptr<message_base> const & msg )
    {
        if ( wrapped_message<Msg> * wrapper = dynamic_cast<wrapped_message<Msg> *>( msg.get() ) ) {
            f( wrapper->contents );
            return true;
        } else {
            return prev->dispatch( msg );
        }
    }

public:
    TemplateDispatcher( TemplateDispatcher && other ) noexcept
        : q { other.q }, prev { other.prev },
          f( std::move( other.f ) )
    {
        prev->chained = true;
    }

    TemplateDispatcher( queue * q_, PreviousDispatcher * prev_, Func && f_ )
        : q { q_ }, prev { prev_ }, f( std::forward<Func>( f_ ) ), chained { false }
    {
        prev->chained = true;
    }
    template<typename OtherMsg, typename OtherFunc>
    TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
    handle( OtherFunc && of )
    {
        return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>( q, this, std::forward<OtherFunc>( of ) );
    }

    ~TemplateDispatcher() noexcept( false )
    {
        if ( !chained ) {
            wait_and_dispatch();
        }
    }

private:
    queue * q;
    PreviousDispatcher * prev;
    Func f;
    bool chained;
};

} // namespace messaging
