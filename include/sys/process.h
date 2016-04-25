#ifndef PROCESS_H_
#define PROCESS_H_

#include "sys/pt.h"

#ifndef PROCESS_CONF_NUMEVENTS
#define PROCESS_CONF_NUMEVENTS 32
#endif

#define PROCESS_EVENT_NONE 0x80
#define PROCESS_EVENT_INIT 0x81
#define PROCESS_EVENT_POLL 0x82
#define PROCESS_EVENT_EXIT 0x83
#define PROCESS_EVENT_SERVICE_REMOVED 0x84
#define PROCESS_EVENT_CONTINUE 0x85
#define PROCESS_EVENT_MSG 0x86
#define PROCESS_EVENT_EXITED 0x87
#define PROCESS_EVENT_TIMER 0x88
#define PROCESS_EVENT_COM 0x89
#define PROCESS_EVENT_MAX 0x8a

/* 返回值：成功 */
#define PROCESS_ERR_OK 0

/* 返回值：失败 */
#define PROCESS_ERR_FAULT -1
#define PROCESS_NONE NULL


#ifndef PROCESS_CONF_NUMEVENTS
#define PROCESS_CONF_NUMEVENTS 32
#endif



#define PROCESS_BEGIN() \
{char PT_YIELD_FLAG = 1; if (PT_YIELD_FLAG){;}switch (process_pt->lc) {case 0:

#define PROCESS_YIELD() \
do { \
PT_YIELD_FLAG = 0;\
process_pt->lc = __LINE__;\
case __LINE__:\
if (PT_YIELD_FLAG == 0)\
	return PT_YIELDED;\
}while(0)

#define PROCESS_END() \
} PT_YIELD_FLAG = 0; process_pt->lc = 0; return PT_ENDED; }

#define PROCESS_WAIT_EVENT_UNTIL(c) \
do { \
PT_YIELD_FLAG = 0; \
process_pt->lc = __LINE__; \
case __LINE__: \
if (PT_YIELD_FLAG == 0 || !c) \
	return PT_YIELDED; \
}while(0)


#define PROCESS(p, name) \
PROCESS_THREAD(p, ev, data); \
struct process p = {NULL, name, process_thread_##p}

#define PROCESS_THREAD(p, ev, data) \
static PT_THREAD(process_thread_##p(struct pt* process_pt, process_event_t ev, process_data_t data))

#define PROCESS_NAME(name) extern struct process name

typedef unsigned char process_event_t;
typedef void * process_data_t;
typedef unsigned char process_num_events_t;

extern struct process *process_current;

struct process {
	struct process *next;
	const char *name;
	char (*thread)(struct pt*, process_event_t, process_data_t);
	struct pt pt;
	unsigned char state, needspoll;
};

void process_init(void);

void process_start(struct process *p, process_data_t data);

void process_exit(struct process *p);


void process_post_synch(struct process *p, process_event_t ev, process_data_t data);

void process_poll(struct process *p);

int process_post(struct process *p, process_event_t ev, process_data_t data);

int process_run(void);

#endif
