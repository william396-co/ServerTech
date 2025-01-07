#include <cstdio>

#if defined(__cplusplus) &&__cplusplus >= 202002L
#define GET_VARAGS(_0,_1,_2,_3,_4,N,...) N
#define COUNT_VARAGS(...) GET_VARARGS(\
	"ignored" _VA_OPT(,) __VA_ARGS__,4,3,2,1,0)
#else

#endif