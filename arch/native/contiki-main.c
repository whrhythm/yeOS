#include <unistd.h> 
#include <stdio.h>
#include "sys/process.h"
#include "sys/etimer.h"
#include "sys/autostart.h"
#include "contiki.h"

int main(int argc, char **argv)
{
	
	process_init();

	//主要的时间调度模块
	process_start(&etimer_process, NULL);

	autostart_start(autostart_processes);
	//debug
	//return 0;
	// 核心进程	
	while (1) {
		process_run();
		usleep(1000); //1ms
		//调用etimer进程,指导process_run()哪个process
		etimer_request_poll();
	}
	return 0;
}
