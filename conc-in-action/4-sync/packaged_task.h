#pragma once

#include <future>
#include <deque>
#include <thread>
#include <utility>

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;

bool gui_shutdown_message_received();
void get_and_process_gui_message();

void gui_thread()
{
    while ( !gui_shutdown_message_received() ) {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard lock( m );
            if ( tasks.empty() )
                continue;

            task = std::move( tasks.front() );
            tasks.pop_front();
        }
        task(); // execute
    }
}

std::thread gui_bg_thread( gui_thread );
template<typename F>
std::future<void> post_task_for_gui_thread( F && f )
{
    std::packaged_task<void()> task( std::forward<F>( f ) );
    std::future<void> res = task.get_future();
    std::lock_guard lock( m );
    tasks.push_back( std::move( task ) );
    return res;
}
