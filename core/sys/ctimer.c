#include "sys/etimer.h"
#include "sys/process.h"

struct ctimer *ctimer_list;

PROCESS(ctimer_process, "Ctimer process");
PROCESS_THREAD(ctimer_process, ev, data)
{
	struct ctimer *c;
	PROCESS_BEGIN();
	
	while (1) {
	PROCESS_YIELD();

	}
	PROCESS_END();	
}

void ctimer_init(void)
{
	
}
