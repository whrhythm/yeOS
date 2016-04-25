#include "sys/autostart.h"
#include <stdio.h>

void autostart_start(struct process * const processes[])
{
	int i;
	for (i = 0; processes[i] != NULL; ++i) {
		printf("autostart_start process-name:%s ### \n", processes[i]->name);
		process_start(processes[i], NULL);
	}
}
