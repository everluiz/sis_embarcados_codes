#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "os.h"

#ifdef __cplusplus
 extern "C"
#endif
void MCU_init(void); /* Device initialization function declaration */

cpu_t stk1[128]; // criando vetor (prof. nao quer malloc, nao se usa ¯\_(¨/)_/¯ )
cpu_t stk2[128];
cpu_t stk3[128];

//criando primeira tarefa
void task1(void){
	int i=0;
	for(;;){
		i++;
		os_delay(10);
	}
}
void task2(void){
	int i=0;
	for(;;){
		i++;
		os_delay(20);
	}
}
void task3(void){
	int i=0;
	for(;;){
		i++;
		os_delay(30);
	}
}

void main(void) {
  MCU_init(); /* call Device Initialization */

  /* include your code here */
	InstallTask(task1, stk1, 128); // instala tarefa 1 no sistema, prepara a pilha
	InstallTask(task1, stk2, 128);
	InstallTask(task1, stk3, 128);
	
	// macro para dispachar a tarefa
	// é macro por que dispachar uma tarefa é um codigo em assembly, e não queremos que esse codigo mexa na pilha do processador
	// e depende do processador que estamos utilizando, dispatcher é definido no port
	//stk_tmp = tcb[0].stk;
	//dispatcher(); 

	os_start(); //starta o sistema
	
  for(;;) {
    /* __RESET_WATCHDOG(); by default, COP is disabled with device init. When enabling, also reset the watchdog. */
  } /* loop forever */
  /* please make sure that you never leave main */
}
