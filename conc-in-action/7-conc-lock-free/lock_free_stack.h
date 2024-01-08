#pragma once

#include <atomic>
#include <memory>

#define USE_OPT

#ifndef USE_OPT
template<typename T>
class lock_free_stack
{
public:
    void push( T const & data )
    {
        node * const new_node = new node( data );
        new_node->next = head.load();
        while ( !head.compare_exchange_weak( new_node->next, new_node ) )
            ;
    }

    std::shared_ptr<T> pop()
    {
        node * old_head = head.load();
        while ( old_head && !head.compare_exchange_weak( old_head, old_head->next ) )
            ;
        return old_head ? old_head->data : std::shared_ptr<T>();
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        node * next;
        explicit node( T const & data_ )
            : data { std::make_shared<T>( data_ ) } {}
    };
    std::atomic<node *> head;
};

#else
#define USE_HAZARD_POINTER
#ifndef USE_HAZARD_POINTER

template<typename T>
class lock_free_stack
{
public:
    void push( T const & data )
    {
        node * const new_node = new node( data );
        new_node->next = head.load();
        while ( !head.compare_exchange_weak( new_node->next, new_node ) )
            ;
    }

    std::shared_ptr<T> pop()
    {
        ++threads_in_pop;
        node * old_head = head.load();
        while ( old_head && !head.compare_exchange_weak( old_head, old_head->next ) )
            ;
        std::shared_ptr<T> res;
        if ( olde_head ) {
            res.swap( old_head->data );
        }
        try_reclaim( old_head );
        return res;
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        node * next;
        explicit node( T const & data_ )
            : data { std::make_shared<T>( data_ ) } {}
    };
    std::atomic<node *> head;

private:
    std::atomic<unsigned> threads_in_pop;
    std::atomic<node *> to_be_deleted;
    void try_reclaim( node * old_head )
    {
        if ( threads_in_pop == 1 ) {
            node * nodes_to_delete = to_be_deleted.exchange( nullptr );
            if ( !--threads_in_pop ) {
                delete_nodes( nodes_to_delete );
            } else if ( nodes_to_delete ) {
                chain_pending_nodes( nodes_to_delete );
            }
            delete old_head;
        } else {
            chain_pending_node( olde_head );
            --threads_in_pop;
        }
    }

    void chain_pending_nodes( node * nodes )
    {
        node * last = nodes;
        while ( node * const next = last->next ) {
            last = next;
        }
        chain_pending_nodes( nodes, last );
    }

    void chain_pending_nodes( node * first, node * last )
    {
        last->next = to_be_deleted;
        while ( !to_be_deleted.compare_exchange_weak( last->next, first ) )
            ;
    }
    void chain_pending_node( node * n )
    {
        chain_pending_nodes( n, n );
    }
    static void delete_nodes( node * nodes )
    {
        while ( nodes ) {
            node * next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }
};

#else
#include "hazard_pointer.h"

template<typename T>
class lock_free_stack
{
public:
    void push( T const & data )
    {
        node * const new_node = new node( data );
        new_node->next = head.load();
        while ( !head.compare_exchange_weak( new_node->next, new_node ) )
            ;
    }

    std::shared_ptr<T> pop()
    {
        std::atomic<void *> & hp = get_hazard_pointer_for_current_thread();
        node * old_head = head.load();
        do {
            node * temp;
            do {
                temp = old_head;
                hp.store( old_head );

                old_head = head.load();
            } while ( old_head != temp );

        } while ( old_head && !head.compare_exchange_weak( old_head, old_head->next ) );
        hp.store( nullptr );
        std::shared_ptr<T> res;
        if ( old_head ) {
            res.swap( old_head->data );
            if ( outstanding_hazard_pointers_for( old_head ) ) {
                reclaim_later( old_head );
            } else {
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }
        return res;
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        node * next;
        explicit node( T const & data_ )
            : data { std::make_shared<T>( data_ ) } {}
    };
    std::atomic<node *> head;
};

#endif
#endif

void lock_free_stack_example();
