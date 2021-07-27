/*
#ifndef:
	a primeira vez que for incluido em um .c esse header nao vai estar definido, entra no if e define 
	uma vez definido, quando for incluir o .h denovo, ele nao define denovo. resolve problemas com compiladores
funcao do .h:
	colocar definições, tipos e prototipos de funcoes 
*/
#ifndef _PORT_H_ 
#define _PORT_H_

// tipo do tam. do barramento do sistema
typedef unsigned char cpu_t;

extern cpu_t *stk_tmp; // informa que esta variavel existe mas é externa

cpu_t *PrepareStack(void *task, cpu_t *stk, int stk_size);

void init_os_timer(void);
		
		// macro dispatcher():
		// 		instrucao LDHX carrega 16 bits da mem. para o reg. H:X
		// 		reg. H:X é quem consegue sobreescrever a pilha nesse cpu
		//
		//		inst. TXS - Transfer Index Register to Stack Pointer (SP <- (H:X)- $0001);
		//		carrega o conteudo do HX-1 para o apontador da pilha
		//
		//		inst. PULH - pull H (index register high) from stack
		//		desempilha a parte alta HIGH do registrador Index por software
		//      e faz o endereço aumentar: (SP <- SP + $0001);
		//
		//		RTI - Return from Interrupt
		//	 	desempilha: CCR,A,X,PCH,PCL para a stack 
#define dispatcher()			\
		asm("LDHX stk_tmp");	\
		asm("TXS");				\
		asm("PULH");			\
		asm("RTI")

#define yield() asm("SWI")

#define SAVE_SP()				\
		asm("TSX");				\
		asm("STHX stk_tmp")
		
#define RESTORE_SP()			\
		asm("LDHX stk_tmp");	\
		asm("TXS")

	
#endif