#include <iostream>
#include <thread>

void helloFunc()
{
    std::cout << "Hello from a function\n";
}

class HelloFuncObj
{
public:
    void operator()() const
    {
        std::cout << "Hello from a function object\n";
    }
};

int main()
{

    std::thread t1( helloFunc );

    HelloFuncObj helloFuncObj;
    std::thread t2( helloFuncObj );

    std::thread t3( [] {
        std::cout << "Hello from a lambda\n";
    } );

    t1.join();
    t2.join();
    t3.join();

    std::cout << "==================\n";
    return 0;
}
