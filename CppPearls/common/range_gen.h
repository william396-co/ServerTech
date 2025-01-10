#pragma once

#include "overload_invoke.h"
#include "is_equal_int.h"

#define RANGE_GEN_WHEN_1(cur,end)
#define RANGE_GEN_WHEN_0(cur,end) \
	, OVERLOAD_INVOKE(RANGE,cur)(end)

#define RANGE(begin, end) OVERLOAD_INVOKE(RANGE, begin) (end)
#define RANGE_0(end) 0 OVERLOAD_INVOKE(RANGE_GEN_WHEN,\
	IS_EQUAL_INT(0,DEC(end)))(1,end)
#define RANGE_1(end) 1 OVERLOAD_INVOKE(RANGE_GEN_WHEN,\
	IS_EQUAL_INT(1,DEC(end)))(2,end)
#define RANGE_2(end) 2 OVERLOAD_INVOKE(RANGE_GEN_WHEN,\
	IS_EQUAL_INT(2,DEC(end)))(3,end)
#define RANGE_3(end) 3 OVERLOAD_INVOKE(RANGE_GEN_WHEN,\
	IS_EQUAL_INT(3,DEC(end)))(4,end)


