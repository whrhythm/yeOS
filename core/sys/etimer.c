#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/process.h"
#include "cc.h"

#include <stdio.h> // for printf();

static void add_timer(struct etimer *et);


static struct etimer *timerlist;
PROCESS(etimer_process, "Etimer Event");

PROCESS_THREAD(etimer_process, ev, data)
{
	struct etimer *t, *u;
	PROCESS_BEGIN();

	while (1) {
	PROCESS_YIELD();

	struct process *p;
	if (ev == PROCESS_EVENT_EXITED && timerlist != NULL) {
		p = (struct process *)data;
		if (timerlist->p == p) {
			p = PROCESS_NONE;
			timerlist = timerlist->next;
			continue;
		}
		for (t = timerlist; t->next != NULL; t = t->next) {
			if (t->next->p == p) {
				p = PROCESS_NONE;
				t->next = t->next->next;
			}
		}
	}	
	
angin:
	for (t = timerlist; t != NULL; t = t->next) {
		//判断是否超时
		if (timer_expired(&t->timer)) {
			//如果超时，就post;post的方式就是在 timerlist 链表中进行遍历
			//post函数的作用就是将该process添加到未处理的 列表(events)中，这个events链表是给do_event()调用的
			if (process_post(t->p, PROCESS_EVENT_TIMER, t) == PROCESS_ERR_OK) {
				//从 timerlist 中去掉已经成功 post 的事件
				t->p = PROCESS_NONE;
				if (t == timerlist) {
					timerlist = t->next;
				} else {
					for (u = timerlist; u->next != t; u = u->next);
					u->next = t->next;
				//update_time(t);
				}
				goto angin;
			} else {

			}
		}
	}
}

	PROCESS_END();
	return 0;
}

/* 添加timerlist在这里，删除此链表节点发生在删除process的时候删除 */
static void add_timer(struct etimer *et)
{
	struct etimer *t;
	for (t = timerlist; t != NULL && t != et; t = t->next);
	
	if (t == et)
		return;
	if (timerlist == NULL) {
		timerlist = et;
		timerlist->next = NULL;
	} else {
		et->next = timerlist;
		timerlist = et;
	}
	et->p = process_current;
}

void etimer_set(struct etimer *et, clock_time_t interval)
{
	timer_set(&et->timer, interval);
	add_timer(et);
}

void etimer_request_poll(void)
{
	process_poll(&etimer_process);
}

