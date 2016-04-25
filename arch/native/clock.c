#include <time.h>
#include <sys/time.h>
#include "contiki-conf.h"

clock_time_t clock_time(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	//毫秒级别
	return tv.tv_sec*1000 + tv.tv_usec/10000;	
}
