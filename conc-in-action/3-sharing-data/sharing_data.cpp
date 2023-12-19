#include <thread>
#include <string>
#include <iostream>
#include <vector>

#include "threadsafe_stack.h"
#include "random_util.h"

threadsafe_stack<std::string> stk;

void do_push()
{
    for ( auto i = 0; i != 100; ++i ) {
        std::string msg = random_string( 10, 20 );
        stk.push( msg );
        std::cout << __PRETTY_FUNCTION__ << "(" << msg << ")\n";
    }
}

void do_pop()
{
    while ( true ) {
        if (
            !stk.empty() ) {
            std::string cur;
            stk.pop( cur );
            std::cout << __PRETTY_FUNCTION__ << "(" << cur << ")\n";
        }
    }
}

void threadsafe_stack_test()
{
    std::vector<std::jthread> producers;
    for ( auto i = 0; i != 3; ++i ) {
        producers.emplace_back( do_push );
    }

    std::jthread cosumer( do_pop );
}

int main()
{

    //    threadsafe_stack_test();

    return 0;
}
