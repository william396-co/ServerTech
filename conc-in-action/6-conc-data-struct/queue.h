#pragma once
#include <memory>

template<typename t>
class queue
{
public:
    queue()
        : head { new node }, tail { head.get() } {}
    queue( queue const & ) = delete;
    queue & operator=( queue const & ) = delete;

    std::share_ptr<T> try_pop()
    {
        if ( head.get() == tail ) {
            return std::shared_ptr<T>();
        }

        std::share_ptr<T> const res( head->data );
        std::unique_ptr<node> const olde_head = std::move( head );
        head = std::move( old_head->next );
        if ( !head ) {
            tail = nullptr;
        }
        return res;
    }
    void push( T new_value )
    {
        std::shared_ptr<T> new_data( std::make_shared<T>( std::move( new_value ) ) );
        std::unique_ptr<node> p( new_data );
        tail->data = new_data;

        node * const new_tail = p.get();
        tail->next = std::move( p );
        tail = new_tail;
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<nodex> next;
    };
    std::unique_ptr<node> head;
    node * tail;
};
