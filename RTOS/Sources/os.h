#ifndef _OS_H_ 
#define _OS_H_

#include "port.h"
// tipo de funcao tarefa com parametro de ponteiro
typedef void(*task_t)(void);

struct _tcb
{
	cpu_t *stk;
	cpu_t ready;
	long long timeout;
};

typedef struct _tcb tcb_t;

extern volatile tcb_t tcb[5];
extern volatile cpu_t ct;
extern volatile cpu_t it;
extern volatile long long os_time;

void InstallTask(task_t task,cpu_t *stk, int stk_size);
void os_start(void);
cpu_t *scheduler(void);
void os_delay(long long time);

cpu_t os_inc_and_compare(void);

void idle_task(void);

#endif