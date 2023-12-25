#pragma once

#include "queue.h"
#include "templateDispatcher.h"
#include <memory>

namespace messaging {

class close_queue
{};

class dispatcher
{
public:
    dispatcher( dispatcher && other ) noexcept
        : q { other.q }, chained { other.chained }
    {
        other.q = nullptr;
        other.chained = true;
    }

    explicit dispatcher( queue * q_ )
        : q { q_ }, chained { false } {}

    template<typename Message, typename Func>
    TemplateDispatcher<dispatcher, Message, Func> handle( Func && f )
    {
        return TemplateDispatcher<dispatcher, Message, Func>( q, this, std::forward<Func>( f ) );
    }

    ~dispatcher() noexcept( false )
    {
        if ( !chained ) {
            wait_and_dispatch();
        }
    }
    dispatcher( dispatcher const & ) = delete;
    dispatcher & operator=( dispatcher const & ) = delete;

private:
    template<typename Dispatcher, typename Msg, typename Func>
    friend class TemplateDispatcher;

    void wait_and_dispatch()
    {
        while ( true ) {
            auto msg = q->wait_and_pop();
            dispatch( msg );
        }
    }
    bool dispatch( std::shared_ptr<message_base> const & msg )
    {
        if ( dynamic_cast<wrapped_message<close_queue> *>( msg.get() ) ) {
            throw close_queue();
        }
        return false;
    }

private:
    queue * q;
    bool chained;
};
} // namespace messaging
