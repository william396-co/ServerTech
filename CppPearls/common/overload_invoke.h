#pragma once

//////////////////////////////////////////////////////////
// OVERLOAD_INVOKE
//////////////////////////////////////////////////////////
#define CONCAT_HELPER(a, b) a ## b
#define CONCAT(a, b) CONCAT_HELPER(a, b)

#define OVERLOAD_INVOKE_1(call, v1) \
	CONCAT(CONCAT(call, _), v1)
#define OVERLOAD_INVOKE_2(call, v1, v2) \
	CONCAT( CONCAT(OVERLOAD_INVOKE_1(call, v1), _), v2 )
#define OVERLOAD_INVOKE_3(call, v1, v2, v3) \
	CONCAT( CONCAT(OVERLOAD_INVOKE_2(call, v1, v2), _), v3)
#define OVERLOAD_INVOKE_4(call, v1, v2, v3, v4) \
	CONCAT( CONCAT(OVERLOAD_INVOKE_3(call, v1, v2, v3), _), v4)
#define OVERLOAD_INVOKE_5(call, v1, v2, v3, v4, v5) \
	CONCAT( CONCAT(OVERLOAD_INVOKE_4(call, v1, v2, v3, v4), _), v5)

#define OVERLOAD_INVOKE(call, ...) \
	CONCAT( OVERLOAD_INVOKE_, \
	COUNT_VARARGS(__VA_ARGS__) )(call, __VA_ARGS__)