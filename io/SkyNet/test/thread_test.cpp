#include <functional>
#include <iostream>
#include <string>

#include "ThreadPool.h"

void print(int a, double b, const char* c, std::string d) {
    std::cout << "[" << a << "]\t" << b << "\t" << c << "\t" << d << "\n";
}

void test(int i) { std::cout << "help: [" << i << "]\n"; }

int main() {
    ThreadPool* pool = new ThreadPool();

    for (int i = 0; i != 30; ++i) {
        Task task =
            std::bind(print, i, 3.14, "hello", std::string("what's up"));
        pool->add(task);

        task = std::bind(test, i);
        pool->add(task);
    }

    delete pool;
    return 0;
}
