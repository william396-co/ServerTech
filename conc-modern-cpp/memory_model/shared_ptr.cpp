#include <thread>
#include <memory>

std::shared_ptr<int> ptr = std::make_shared<int>( 2024 );

void cap_by_val()
{
    for ( auto i = 0; i < 10; ++i ) {
        std::thread( [ptr] {
            std::shared_ptr<int> localPtr;
            localPtr = std::make_shared<int>( 2014 );
        } ).detach();
    }
}

void cap_by_ref()
{

    for ( auto i = 0; i < 10; i++ ) {
        std::thread( [&ptr] {
            ptr = std::make_shared<int>( 2014 );
        } ).detach();
    }
}

void cap_by_ref_safe()
{
    for ( auto i = 0; i < 10; i++ ) {
        std::thread( [&ptr] {
            auto localPtr = std::make_shared<int>( 2014 );
            std::atomic_store( &ptr, localPtr );
        } ).detach();
    }
}

int main()
{

    cap_by_val();
//    cap_by_ref();
  //  cap_by_ref_safe();
    return 0;
}
