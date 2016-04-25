#include <stdio.h>
#include "sys/process.h"
#include "sys/etimer.h"

#define DEBUG  0

struct process *process_list = NULL;
struct process *process_current = NULL;

static void exit_process(struct process *p, struct process *fromprocess);
static void call_process(struct process *p, process_event_t ev, process_data_t data);
static void do_poll(void);
static void do_event(void);

static volatile unsigned char poll_requested;
static process_event_t lastevent;

struct event_data {
	process_event_t ev;
	process_data_t data;
	struct process *p;
};

/* 未处理事件个数 - nevents
 * 待处理事件下标 - fevent
 */
static process_num_events_t nevents, fevent;
static struct event_data events[PROCESS_CONF_NUMEVENTS];


/* 定义process 的三种状态 */
#define PROCESS_STATE_NONE 0
#define PROCESS_STATE_RUNNING 1
#define PROCESS_STATE_CALLED 2

void process_init()
{
	printf("process init()\n");
	lastevent = PROCESS_EVENT_MAX;
	
	nevents = fevent = 0;
	process_list = process_current = NULL;
}

void process_exit(struct process *p)
{
	exit_process(p, &etimer_process);
}

static void rm_process_list(struct process *p)
{
	struct process *q;

	if (p != process_list) {
		for (q = process_list; q != NULL; q = q->next) {
			if (q->next == p) {
				q->next = p->next;
					return;
			}
		}
	}
	process_list = p->next;

}

static void exit_process(struct process *p, struct process *rootp)
{
	/* 从process_list链表中删除 */
	
	if (process_list == NULL) {
		//PR_ERR("Kernel panic!\n");
	}
	
	if (p->state == PROCESS_STATE_RUNNING) {
		// etimer 设计为永不退出
		call_process(rootp, PROCESS_EVENT_EXITED, p);
		rm_process_list(p);	
	}
	p->state = PROCESS_STATE_NONE;	
}

static void call_process(struct process *p, process_event_t ev, process_data_t data)
{
	int ret;

#if DEBUG
	//PR_INFO("### call_process : %s###\n", p->name);
#endif

	if ((p->state == PROCESS_STATE_RUNNING) &&
		p->thread != NULL) {
		p->state = PROCESS_STATE_CALLED;
		process_current = p;
		ret = p->thread(&p->pt, ev, data);
		if (ret == PT_ENDED) {
			exit_process(p, &etimer_process);
		} else {
			p->state = PROCESS_STATE_RUNNING;
		}
	}
}

void process_post_synch(struct process *p, process_event_t ev, process_data_t data)
{
	process_current = p;
	call_process(p, ev, data);	
}

void process_start(struct process *p, process_data_t data)
{
	struct process *q;

	/* 过滤掉正在running 的process */
	for (q = process_list; q != p && q!= NULL; q = q->next);
	
	if (q == p)
		return;
	
	/* put on the process list */
	/* 头部添加 */
	p->next = process_list;
	process_list = p;
	/* 更新他的状态 */
	p->state = PROCESS_STATE_RUNNING;
	
	process_post_synch(p, PROCESS_EVENT_INIT, data);
}

int process_post(struct process *p, process_event_t ev, process_data_t data)
{
	static process_num_events_t snum;

	if (nevents >= PROCESS_CONF_NUMEVENTS) {
		//PR_ERR();
		//FIX ME
		return PROCESS_ERR_FAULT;
	}	

	/* 将该事件添加到待处理的列表中， 并将待处理的个数加1 */	
	snum = (process_num_events_t)(fevent + nevents) % PROCESS_CONF_NUMEVENTS;
	events[snum].p = p;
	events[snum].ev = ev;
	events[snum].data = data;
	++nevents;
	
	return PROCESS_ERR_OK;	
}

/* 对待特殊的事件 */
static void do_poll(void)
{
	struct process *p;
	poll_requested = 0;
	
	/*Call the process that needs to be polled*/
	for (p = process_list; p != NULL; p = p->next) {
		if (p->needspoll) {
			p->state = PROCESS_STATE_RUNNING;
			poll_requested = 0;
			p->needspoll = 0;
			call_process(p, PROCESS_EVENT_POLL, NULL);
		}
	}	
}

/* 对待普通的事件 */
static void do_event(void)
{
	static process_event_t ev;
	static process_data_t data;
	static struct process *receive;

	/* 存在未处理事件 */
	if (nevents > 0) {
		ev = events[fevent].ev;
		data = events[fevent].data;
		receive = events[fevent].p;
		
		/* 跟新下标 */
		fevent = (fevent +1) % PROCESS_CONF_NUMEVENTS;
		--nevents;
		call_process(receive, ev, data);	
	}
}

int process_run(void)
{
	/* 
	 * 在这里可以对优先级进行判断
	 */
	if (poll_requested)
		do_poll();// 直接调用优先等级高的
	
	/* Process on event from the queue */
	do_event();
	
	/* 返回未处理事件数 + 是否有特殊事件发生 */
	return nevents + poll_requested;
}

void process_poll(struct process *p)
{
	poll_requested = 1;
	p->needspoll = 1;
}
