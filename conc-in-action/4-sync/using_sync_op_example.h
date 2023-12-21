#pragma once

#include <future>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <functional>

template<typename F, typename Arg>
auto spwan_task( F && f, Arg && arg ) -> std::future<std::result_of_t<F( Arg && )>>
{
    using return_type = std::result_of_t<F( Arg && )>;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind( std::forward<F>( f ), std::forward<Arg>( arg ) ) );
    // std::packaged_task<result_type()> task( std::move( f ) );
    std::future<return_type> res = task->get_future();
    std::thread t( std::move( f ), std::move( arg ) );
    t.detach();
    return res;
}

template<typename T>
std::list<T> sequential_quick_sort( std::list<T> input )
{
    if ( input.empty() ) return input;

    std::list<T> result;
    result.splice( result.begin(), input, input.begin() );

    T const & pivot = *result.begin();
    auto divide_point = std::partition( input.begin(), input.end(), [&]( T const & t ) { return t < pivot; } );

    std::list<T> lower_part;
    lower_part.splice( lower_part.end(), input, input.begin(), divide_point );

    auto new_lower( sequential_quick_sort( std::move( lower_part ) ) );
    auto new_higher( sequential_quick_sort( std::move( input ) ) );

    result.splice( result.end(), new_higher );
    result.splice( result.begin(), new_lower );
    return result;
}

template<typename T>
std::list<T> parallel_quick_sort( std::list<T> input )
{
    if ( input.empty() ) return input;

    std::list<T> result;
    result.splice( result.begin(), input, input.begin() );

    T const & pivot = *result.begin();
    auto divide_point = std::partition( input.begin(), input.end(), [&]( T const & t ) { return t < pivot; } );

    std::list<T> lower_part;
    lower_part.splice( lower_part.end(), input, input.begin(), divide_point );

    std::future<std::list<T>> new_lower( std::async( &parallel_quick_sort<T>, std::move( lower_part ) ) );
    auto new_higher( parallel_quick_sort( std::move( input ) ) );

    result.splice( result.end(), new_higher );
    result.splice( result.begin(), new_lower.get() );
    return result;
}

template<typename T>
std::vector<T> sequential_quick_sort( std::vector<T> input )
{
    if ( input.empty() ) return input;

    std::vector<T> result;
    result.emplace_back( *input.begin() );
    input.erase( input.begin() );

    T const & pivot = *result.begin();
    auto divide_point = std::partition( input.begin(), input.end(), [&]( T const & t ) { return t < pivot; } );

    std::vector<T> lower_part;
    for ( auto it = input.begin(); it != input.end(); ) {
        if ( *it < pivot ) {
            lower_part.emplace_back( *it );
            it = input.erase( it );
        } else {
            ++it;
        }
    }
    auto new_lower = sequential_quick_sort( std::move( lower_part ) );
    auto new_higher = sequential_quick_sort( std::move( input ) );

    result.insert( result.end(), new_higher.begin(), new_higher.end() );
    result.insert( result.begin(), new_lower.begin(), new_lower.end() );
    return result;
}

template<typename T>
std::vector<T> vparallel_quick_sort( std::vector<T> input )
{
    if ( input.empty() ) return input;

    std::vector<T> result;
    result.emplace_back( *input.begin() );
    input.erase( input.begin() );

    T const & pivot = *result.begin();
    auto divide_point = std::partition( input.begin(), input.end(), [&]( T const & t ) { return t < pivot; } );

    std::vector<T> lower_part;
    for ( auto it = input.begin(); it != input.end(); ) {
        if ( *it < pivot ) {
            lower_part.emplace_back( *it );
            it = input.erase( it );
        } else {
            ++it;
        }
    }
    std::future<std::vector<T>> new_lower( std::async( &vparallel_quick_sort<T>, std::move( lower_part ) ) );
    auto new_higher = vparallel_quick_sort( std::move( input ) );

    result.insert( result.end(), new_higher.begin(), new_higher.end() );
    auto res = new_lower.get();
    result.insert( result.begin(), res.begin(), res.end() );
    return result;
}

void fp_with_futures();

