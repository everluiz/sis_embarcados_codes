#include "os.h"


volatile tcb_t tcb[5];
volatile cpu_t ct = 0; // current task
volatile cpu_t it = 0; // task index
volatile long long os_time = 0; // variavel contadora do sistema

cpu_t stk_idle[128];

// instalador de tarefa (end. da tarefa, ponteiro da pilha da tarefa, tamanho da pilha da tarefa)
// InstallTask vai mexer no TCB  
void InstallTask(task_t task, cpu_t *stk, int stk_size){
	// preparar pilha
	stk = PrepareStack(task, stk, stk_size);
	tcb[it].stk = stk;
	tcb[it].ready = 1;
	tcb[it].timeout = 0;
	it++;
	// stk é local, salva em uma var.global temporaria stk_tmp
	//stk_tmp = stk;
}

// estrategia de usar var. long long (2^64) para não dar overflow na variavel de contagem de tempo do delay
void os_delay(long long time)
{
	tcb[ct].timeout = os_time + time; //tempo para q a tarefa tenha q concorrer com o processador
	tcb[ct].ready = 0; // diz se a tarefa está pronta ou nao
	yield(); // desiste do processador (chama SwitchContext)
}

void idle_task(void){
	for(;;){
	}
}

cpu_t os_inc_and_compare(void){
	int i = 0;
	cpu_t ready = 0;
	
	os_time++;
	for (i = 0; i < it; i++){
		if (tcb[i].timeout == os_time){
			tcb[i].ready = 1;
			ready = 1;
		}
	}
	return ready;
}

cpu_t *scheduler(void){
	do{
		ct++;
		if (ct >= it){
			ct = 0;
		}
	}while(!tcb[ct].ready); // fica no laco até achar uma tarefa pronta
	return tcb[ct].stk;     // retorna o tcb da tarefa selecionada	
}

void os_start(void){
	InstallTask(idle_task, stk_idle, 128); // instala tarefa osciosa
	init_os_timer();
	stk_tmp = tcb[ct].stk;
	dispatcher();
}