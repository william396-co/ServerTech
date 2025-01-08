#pragma once

#include <cstdio>

#if defined(__cplusplus) && __cplusplus >= 202002L

#define GET_VARARGS(_0, _1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) GET_VARARGS(\
 "ignored" __VA_OPT__(,) __VA_ARGS__, 4, 3, 2, 1, 0)

#else

#define EXPAND(x) x
#define _TRIGGER_PARENTHESIS(...),

#define _COMMA_CHECK(_0, _1, _2, _3, _4, _5, _6, _7, \
 _8, _9, _10, R, ...) R
#define HAS_COMMA(...) EXPAND(_COMMA_CHECK(\
 __VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0) )

#define _IS_EMPTY_CASE_0001 ,
#define _IS_EMPTY_CASE(_0, _1, _2, _3) \
 _IS_EMPTY_CASE_##_0 ## _1 ## _2 ## _3
#define _IS_EMPTY_IMPL(_0, _1, _2, _3) \
 HAS_COMMA(_IS_EMPTY_CASE(_0, _1, _2, _3))
#define IS_EMPTY(...) \
 _IS_EMPTY_IMPL( \
 EXPAND( HAS_COMMA(__VA_ARGS__) ), \
 EXPAND( HAS_COMMA(_TRIGGER_PARENTHESIS	, __VA_ARGS__)), \
	 EXPAND(HAS_COMMA(__VA_ARGS__())), \
	 EXPAND(HAS_COMMA(_TRIGGER_PARENTHESIS, __VA_ARGS__())) \
)

#define _COUNT_VARARGS_0_IMPL(\
 _0, _1, _2, _3, _4, N, ...) N
#define _COUNT_VARARGS_0(...) EXPAND(\
 _COUNT_VARARGS_0_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1) )
#define _COUNT_VARARGS_1() 0
#define COUNT_VARARGS_0(...) EXPAND(\
 _COUNT_VARARGS_0(__VA_ARGS__) )
#define COUNT_VARARGS_1(...) _COUNT_VARARGS_1()
#define OVERLOAD_INVOKE(call, version) call ## version
#define OVERLOAD_HELPER(call, version) \
 OVERLOAD_INVOKE(call, version)
#define COUNT_VARARGS(...) EXPAND(\
 OVERLOAD_HELPER(COUNT_VARARGS_, \
 IS_EMPTY(__VA_ARGS__))(__VA_ARGS__) )

#endif


inline void macros_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";
	std::cout << "version:" << __cplusplus << "\n";

	printf("%d %d %d %d %d\n",
		COUNT_VARARGS(),
		COUNT_VARARGS(1),
		COUNT_VARARGS('a', 'b'),
		COUNT_VARARGS('a', 'b', 'c'),
		COUNT_VARARGS('a', 'b', 1, 2));
}


template<typename...Args>
void testArgsCountBySizeof(Args&&...args) {
	std::cout << __PRETTY_FUNCTION__ << " parameter count: " << sizeof...(args) << "\n";
}

inline void sizeof_test() {

	std::cout << __PRETTY_FUNCTION__ << "\n";

	testArgsCountBySizeof();
	testArgsCountBySizeof(1);
	testArgsCountBySizeof(1, 2);
	testArgsCountBySizeof(1, 2, 3);
	testArgsCountBySizeof(1, 2, 3, 4);

}