#include <iostream>


#include "../common/for_each.h"



void foo(int x) {
	std::cout << "x: " << x << "\n";
}

#define FOO(x) foo(x)

int main() {

	std::cout << __PRETTY_FUNCTION__ << "\n";

	FOR_EACH(Foo, 0, 1, 2)

		return 0;
}