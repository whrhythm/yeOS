#ifndef CTIMER_H_
#define CTIMER_H_

#include "sys/etimer.h"

struct ctimer {
	struct ctimer *next;
	struct etimer etimer;
	struct process *p;
	void (*f)(void *);
	void *ptr;
};

#endif
