#include <iostream>
#include <cstdio>

template<typename...Args>
void testArgsCountBySizeof(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << sizeof...(args) << "\n";
}

#define GET_VARAGES(_0,_1,_2,_3,_4,N,...) N
#ifndef USE_DOUBLE
#define COUNT_VARAGES(...) \
		GET_VARAGES("ignored", __VA_OPT_(,)__VA_ARGS__,4,3,2,1,0)
#else
#define COUNT_VARAGES(...) \
		GET_VARAGES("ignored", ##__VA_ARGS__,4,3,2,1,0)
#endif


template<typename ...Args>
void testArgCountByMacros(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << COUNT_VARAGES(args) << "\n";
}



int main() {

	printf("version:%ld\n", __cplusplus);

	testArgsCountBySizeof(1);
	testArgsCountBySizeof(1,2,3,4,5,6);


	int i = 0, j = 0;
	printf("zero arg:%d\n", COUNT_VARAGES());
	printf("one arg:%d\n", COUNT_VARAGES(1));
	printf("two arg:%d\n", COUNT_VARAGES(1,2));
	printf("three arg:%d\n", COUNT_VARAGES(1, "2", 3));
	printf("four arg:%d\n", COUNT_VARAGES(1, true, 3, "dd"));
	printf("five arg:%d\n", COUNT_VARAGES(1, true, j, 3, "dd"));
	printf("six arg:%d\n", COUNT_VARAGES(1, true, 4, i, 3, "dd"));


	return 0;
}