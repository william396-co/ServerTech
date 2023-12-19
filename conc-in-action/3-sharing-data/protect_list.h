#pragma once

#include <thread>
#include <mutex>
#include <algorithm>
#include <list>

std::list<int> some_list;
std::mutex some_mtx;

void add_to_list( int new_val )
{
    std::lock_guard lock( some_mtx );
    some_list.emplace_back( new_val );
}

bool list_contains( int val )
{
    std::lock_guard lock( some_mtx );
    return std::find( some_list.begin(), some_list.end(), val ) != some_list.end();
}
