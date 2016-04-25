#include <stdio.h>
#include "sys/autostart.h"
#include "sys/etimer.h"
#include "sys/process.h"

PROCESS(hello_world_1, "Hello world 1s");
PROCESS(hello_world_2, "Hello world 2s");
AUTOSTART_PROCESS(&hello_world_1, &hello_world_2);

PROCESS_THREAD(hello_world_1, ev, data)
{
	static struct etimer et_1;
	static int i = 0;
	PROCESS_BEGIN();
	while (1) {
		etimer_set(&et_1, 1000);
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
		++i;
		if (i > 4)
			break;
		printf("------------- hello world 1 second---------------\n");
	}
	PROCESS_END();
	return 0;
}

PROCESS_THREAD(hello_world_2, ev, data)
{
	static struct etimer et_2;
	PROCESS_BEGIN();
	while (1) {
		etimer_set(&et_2, 2000);
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
		printf("------------- hello world 2 second---------------\n");
	}
	PROCESS_END();
	return 0;
}
