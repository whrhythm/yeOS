#ifndef PT_H_
#define PT_H_
#include "sys/lc.h"

#define PT_WAITING 0
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3

struct pt {
	lc_t lc;
};

#define PT_THREAD(name_args) char name_args

#endif
