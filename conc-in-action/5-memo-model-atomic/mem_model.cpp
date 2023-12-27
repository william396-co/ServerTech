#include <atomic>

#include <iostream>
#include <string>
#include <cstdint>

#include "spinlock.h"
#include "atomic_example.h"

struct my_data
{
    int i;
    double d;
    unsigned bf1 : 10;
    int bf2 : 25;
    int : 0; // start a new int
    int bf4 : 9;
    int i2;
    char c1, c2;
    //    std::string s;
};

#pragma pack(8)
struct S
{
    unsigned int b : 3; // three-bit unsigned field, allowed values are 0...7
};

void bit_fields_example()
{
    std::cout << __PRETTY_FUNCTION__ << "\n";
    S s { 6 };
    std::cout << ++s.b << "\n"; // 7
    std::cout << ++s.b << "\n"; // 0

    std::cout << "sizeof(S)=" << sizeof( S ) << "\n";
}

void lock_free_test()
{
    std::cout << "atomic<int> is_always_lock_free: " << std::boolalpha << std::atomic<int>::is_always_lock_free << "\n";
    std::cout << "atomic<int> is_lock_free: " << std::boolalpha << std::atomic<int> {}.is_lock_free() << "\n";

    std::cout << "atomic<uintmax_t> is_always_lock_free: " << std::boolalpha << std::atomic<uintmax_t>::is_always_lock_free << "\n";
    std::cout << "atomic<uintmax_t> is_lock_free: " << std::boolalpha << std::atomic<uintmax_t> {}.is_lock_free() << "\n";

    //    std::cout << "atomic<std::string> is_always_lock_free: " << std::boolalpha << std::atomic<std::string>::is_always_lock_free << "\n";
    //   std::cout << "atomic<std::string> is_lock_free: " << std::boolalpha << std::atomic<std::string> {}.is_lock_free() << "\n";
}

int main()
{
    atomic_example();
    spinlock_example();
    lock_free_test();
    bit_fields_example();
    std::cout << sizeof( my_data ) << "\n";

    return 0;
}
