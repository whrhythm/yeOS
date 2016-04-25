#ifndef TIMER_H_
#define TIMER_H_

#include "contiki-conf.h"

struct timer {
	clock_time_t start;
	clock_time_t interval;
};

int timer_expired(struct timer *t); 
void timer_set(struct timer *t, clock_time_t interval);

#endif
