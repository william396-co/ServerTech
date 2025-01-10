#include <iostream>
#include <cstdio>

template<typename...Args>
void testArgsCountBySizeof(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << sizeof...(args) << "\n";

}


#define USE_SOLUTION

#ifndef USE_SOLUTION
#define GET_VARARGS(_0, _1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) \
	GET_VARARGS("ignored" __VA_OPT__(,) __VA_ARGS__,4,3,2,1,0)
#else
#include "../common/count_varages.h"

#endif

void macros_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";
	std::cout << "version:" << __cplusplus << "\n";

	printf("%d %d %d %d %d\n",
		COUNT_VARARGS(),
		COUNT_VARARGS(1),
		COUNT_VARARGS('a', 'b'),
		COUNT_VARARGS('a', 'b', 'c'),
		COUNT_VARARGS('a', 'b', 1, 2));

}

void sizeof_test() {

	std::cout << __PRETTY_FUNCTION__ << "\n";

	testArgsCountBySizeof();
	testArgsCountBySizeof(1);
	testArgsCountBySizeof(1, 2);
	testArgsCountBySizeof(1, 2, 3);
	testArgsCountBySizeof(1, 2, 3, 4);
}

int main() {

#ifdef USE_SOLUTION
	macros_test();
	sizeof_test();

#else

	printf("zero parameters:%d\n", COUNT_VARARGS());
	printf("one parameters:%d\n", COUNT_VARARGS(1));
	printf("two parameters:%d\n", COUNT_VARARGS(1, 2));
	printf("three parameters:%d\n", COUNT_VARARGS(1, 2, 3));
	printf("four parameters:%d\n", COUNT_VARARGS(1, 2, 3, 4));
#endif 
	system("Pause");


	return 0;
}