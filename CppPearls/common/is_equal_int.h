#pragma once

#include "overload_invoke.h"

#define IS_EQUAL_INT_0_0() 1
#define IS_EQUAL_INT_1_1() 1
#define IS_EQUAL_INT_2_2() 1
#define IS_EQUAL_INT_3_3() 1
#define IS_EQUAL_INT_4_4() 1
#define IS_EQUAL_INT_5_5() 1
#define IS_EQUAL_INT_6_6() 1
#define IS_EQUAL_INT_7_7() 1
#define IS_EQUAL_INT_8_8() 1
#define IS_EQUAL_INT_9_9() 1


#define IS_EQUAL_INT_RESULT_2() 1
#define IS_EQUAL_INT_RESULT_1() 0
#define IS_EQUAL_INT_RESULT(...)\
	OVERLOAD_INVOKE(IS_EQUAL_INT_RESULT,\
	COUNT_VARARGS(__VA_ARGS__))()

#define IS_EQUAL_INT(a,b) \
	IS_EQUAL_INT_RESULT(\
	OVERLOAD_INVOKE(IS_EQUAL_INT, a,b)())
