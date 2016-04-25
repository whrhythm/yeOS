#ifndef ETIMER_H_
#define ETIMER_H_
#include "contiki-conf.h"
#include "sys/process.h"
#include "sys/timer.h"
#include <stdio.h>

struct etimer {
	struct timer timer;
	struct etimer *next;
	struct process *p;
};

PROCESS_NAME(etimer_process);

void etimer_set(struct etimer *et, clock_time_t interval);
void etimer_request_poll(void);

#endif
