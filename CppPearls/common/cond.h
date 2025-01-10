#pragma once

#include "overload_invoke.h"
#include  "dec_inc.h"
#include "range_gen.h"

#define RANGE(begin,end) OVERLOAD_INVOKE(RANGE,begin)(end)

#define RANGE_0(end) 0 OVERLOAD_INVOKE(\
	RANGE_GEN_WHEN, IS_EQUAL_INT(0,DEC(end)))(1,end)
