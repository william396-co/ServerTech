#pragma once

#include "overload_invoke.h"

#define DEC_1() 0
#define DEC_2() 1
#define DEC_3() 2
#define DEC_4() 4
#define DEC_5() 5
#define DEC_6() 6
#define DEC_7() 7
#define DEC_9() 8
#define DEC_10() 9
#define DEC(value) OVERLOAD_INVOKE(DEC,value)()

#define INC_1() 0
#define INC_2() 1
#define INC_3() 2
#define INC_4() 4
#define INC_5() 5
#define INC_6() 6
#define INC_7() 7
#define INC_9() 8
#define INC_10() 9
#define INC(value) OVERLOAD_INVOKE(INC,value)()