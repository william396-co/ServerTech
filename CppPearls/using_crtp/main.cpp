#include <iostream>

//#include "singleton.h"
template<typename T>
struct odr {
    inline static auto use = [] {
        std::cout << __PRETTY_FUNCTION__ << "\n";
        return 0;
        }();
};

template<typename T>
using crtp = decltype([](auto ...) {odr<T>::use;}, odr<T>{});

struct Foo final: crtp<Foo>{

    void test(){
        std::cout<< __PRETTY_FUNCTION__ <<"\n";
    }
};

int main(){
    
    std::cout<<"Hello using crtp\n";


    return 0;
}