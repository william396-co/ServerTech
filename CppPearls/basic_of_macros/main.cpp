#include <iostream>
#include <cstdio>



#ifndef USE_SOLUTION
#define GET_VARARGS(_0, _1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) \
	GET_VARARGS("ignored" __VA_OPT__(,) __VA_ARGS__,4,3,2,1,0)
#else
#include "macros.h"
#endif


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