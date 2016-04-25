#include "sys/timer.h"
#include "sys/clock.h"

int timer_expired(struct timer *t)
{
	clock_time_t diff = clock_time() - t->start + 1;
	return t->interval < diff;
}

void timer_set(struct timer *t, clock_time_t interval)
{
	t->start = clock_time();
	t->interval = interval;
}

