#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <stdexcept>

constexpr auto max_hazard_pointers = 100;
struct hazard_pointer
{
    std::atomic<std::thread::id> id;
    std::atomic<void *> pointer;
};
hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner
{
public:
    hp_owner( hp_owner const & ) = delete;
    hp_owner & operator=( hp_owner const & ) = delete;

    hp_owner()
        : hp { nullptr }
    {
        for ( auto i = 0; i != max_hazard_pointers; ++i ) {
            std::thread::id old_id;
            if ( hazard_pointers[i].id.compare_exchange_strong( old_id, std::this_thread::get_id() ) ) {
                hp = &hazard_pointers[i];
                break;
            }
        }
        if ( !hp ) {
            throw std::runtime_error( "No Hazard pointers available" );
        }
    }
    ~hp_owner()
    {
        hp->pointer.store( nullptr );
        hp->id.store( std::thread::id() );
    }
    std::atomic<void *> & get_pointer()
    {
        return hp->pointer;
    }

private:
    hazard_pointer * hp;
};

inline std::atomic<void *> & get_hazard_pointer_for_current_thread()
{
    thread_local static hp_owner hazard;
    return hazard.get_pointer();
}

inline bool outstanding_hazard_pointers_for( void * p )
{
    for ( auto i = 0; i != max_hazard_pointers; ++i ) {
        if ( hazard_pointers[i].pointer.load() == p ) {
            return true;
        }
    }
    return false;
}

template<typename T>
void do_delete( void * p )
{
    delete static_cast<T *>( p );
}

struct data_to_reclaim
{
    void * data;
    std::function<void( void * )> deleter;
    data_to_reclaim * next;
    template<typename T>
    data_to_reclaim( T * p )
        : data { p }, deleter { &do_delete<T> }, next { nullptr }
    {
    }
    ~data_to_reclaim()
    {
        deleter( data );
    }
};

std::atomic<data_to_reclaim *> nodes_to_reclaim;
void add_to_reclaim_list( data_to_reclaim * node )
{
}
template<typename T>
void reclaim_later( T * data )
{
}

void delete_nodes_with_no_hazards()
{
}
