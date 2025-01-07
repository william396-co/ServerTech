#include <iostream>

#include "macros.h"
#include "../win/basic_of_macros/macros.h"

template<typename...Args>
void testArgsCountBySizeof(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << sizeof...(args) << "\n";
}

template<typename ...Args>
void testArgCountByMacros(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << COUNT_VARARGS(args) << "\n";
}



int main() {

	printf("version:%ld\n", __cplusplus);

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