#include <iostream>
#include <cstdio>

#include "macros.h"

template<typename...Args>
void testArgsCountBySizeof(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << sizeof...(args) << "\n";
}


int main() {

	std::cout << "version:" << __cplusplus << "\n";

	testArgsCountBySizeof();
	testArgsCountBySizeof(1);
	testArgsCountBySizeof(1, 2);
	testArgsCountBySizeof(1, 2, 3);
	testArgsCountBySizeof(1, 2, 3, 4);

	printf("%d %d %d %d %d\n",
		COUNT_VARARGS(),
		COUNT_VARARGS(1),
		COUNT_VARARGS('a', 'b'),
		COUNT_VARARGS('a', 'b', 'c'),
		COUNT_VARARGS('a', 'b', 1, 2));


	return 0;
}