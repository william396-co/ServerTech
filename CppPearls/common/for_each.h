#pragma once 
#include "../common/overload_invoke.h"
#include "../common/count_varages.h"

//////////////////////////////////////////////////////////
// FOR_EACH
//////////////////////////////////////////////////////////

#define _FOR_EACH_0(call, ...)
#define _FOR_EACH_1(call,x) call(x)
#define _FOR_EACH_2(call,x,...) \
	call(x), _FOR_EACH_1(call, __VA_ARGS__ )
#define _FOR_EACH_3(call, x, ...) \
	 call(x) _FOR_EACH_2(call, __VA_ARGS__)
#define _FOR_EACH_4(call, x, ...) \
	 call(x) _FOR_EACH_3(call, __VA_ARGS__)
#define _FOR_EACH_5(call, x, ...) \
	call(x) _FOR_EACH_4(call, __VA_ARGS__)

#define FOR_EACH(call, ...) \
	OVERLOAD_INVOKE(_FOR_EACH, \
	COUNT_VARARGS(__VA_ARGS__))(call, __VA_ARGS__)