#include <iostream>
#include <mutex>

class MySingleton
{

public:
    MySingleton( MySingleton const & ) = delete;
    MySingleton & operator=( MySingleton const & ) = delete;

    static MySingleton * getInstance()
    {
        call_once( initInstanceFlag, MySingleton::initSingleton );
        return instance;
    }

    static void initSingleton()
    {
        instance = new MySingleton();
    }

private:
    static std::once_flag initInstanceFlag;
    static MySingleton * instance;
    MySingleton() = default;
};

MySingleton * MySingleton::instance = nullptr;
std::once_flag MySingleton::initInstanceFlag;

int main()
{

    std::cout << "MySingleton::getInstance(): " << MySingleton::getInstance() << "\n";
    std::cout << "MySingleton::getInstance(): " << MySingleton::getInstance() << "\n";

    return 0;
}
