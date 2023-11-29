// sync_example.cpp: 定义应用程序的入口点。
//

#include "sync_example.h"
#include "safeQueue.h"

using namespace std;

#include <functional>
#include <future>
#include <thread>

using Func = std::function<void()>;
using FuncList = SafeQueue<Func>;

class Server
{
public:
    template<typename F, typename... Args>
    auto add_func( F && f, Args &&... args ) -> std::future<decltype( f( args... ) )>
    {

        std::function<decltype( f( args... ) )()> func = std::bind( std::forward<F>( f ), std::forward<Args>( args )... );

        auto task_ptr = std::make_shared<std::packaged_task<decltype( f( args... ) )()>>( func );

        // wrap packaged_task into void function
        Func wrap_func = [task_ptr]() {
            ( *task_ptr )();
        };

        m_funcList.enqueue( wrap_func );
        return task_ptr->get_future();
    }
    void run()
    {
        while ( true ) {
            std::this_thread::sleep_for( std::chrono::milliseconds { 300 } ); // idle time
            std::cout << "funclist size: " << m_funcList.size() << std::endl;

            Func func;
            while ( !m_funcList.empty() ) {
                auto dequeued = m_funcList.dequeue( func );
                if ( dequeued ) {
                    func();
                }
            }
        }
    }

private:
    FuncList m_funcList;
};

//#define SYNC_EXEC

int add( int a, int b )
{
#ifndef SYNC_EXEC
    std::cout << a << "+" << b << " = " << a + b << std::endl;
#endif
    return a + b;
}

int mul( int a, int b )
{
#ifndef SYNC_EXEC
    std::cout << a << "*" << b << " = " << a * b << std::endl;
#endif
    return a * b;
}

constexpr auto RUN_TIMES = 100;

int main()
{
    Server s;

    std::thread t1( [&s] {
        for ( int i = 0; i != RUN_TIMES; ++i ) {
            std::cout << "threadId: " << std::this_thread::get_id() << " add_func(" << i << ")\n";
            auto res = s.add_func( &add, 10 + i * 12, 20 + i * 23 );
#ifdef SYNC_EXEC
            std::cout << res.get() << "=(" << 10 + i * 12 << " + " << 20 + i * 23 << ")\n";
#else
            std::this_thread::sleep_for( std::chrono::milliseconds { 20 } );
#endif
        }
    } );

    std::thread t2( [&s] {
        for ( int i = 0; i != RUN_TIMES; ++i ) {
            std::cout << "threadId: " << std::this_thread::get_id() << " add_func(" << i << ")\n";
            auto res = s.add_func( &mul, 10 + i * 10, 20 + i * 42 );
#ifdef SYNC_EXEC
            std::cout << res.get() << "=(" << 10 + i * 10 << " * " << 20 + i * 42 << ")\n";
#else
            std::this_thread::sleep_for( std::chrono::milliseconds { 50 } );
#endif
        }
    } );
    s.run();

    t1.join();
    t2.join();

    return 0;
}
