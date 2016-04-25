#ifndef AUTOSTART_H_
#define AUTOSTART_H_

#include "sys/process.h"
#include <stdio.h>


#define AUTOSTART_PROCESS(...) \
struct process *const autostart_processes[] = {__VA_ARGS__, NULL}

extern struct process *const autostart_processes[];

void autostart_start(struct process * const processes[]);

#endif
