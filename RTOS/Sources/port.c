#include "derivative.h" /* include peripheral declarations */
#include "os.h"

cpu_t *stk_tmp; // var.global para receber stk

cpu_t *PrepareStack(void *task, cpu_t *stk, int stk_size){
	// corrigir endereco da pilha, 
	stk = (cpu_t *) ( (int)stk + stk_size - sizeof(cpu_t) ); // vai pra posicao do ultimo byte da pilha, (-1) pra nao sair pra fora da pilha
	
	// primeira op para preparar a pilha
	// copiar o PC (& 0xFF é uma operacao lógica para retirar a parte baixa LSB(8 bits) dos 16 de PC, ou seja, PC LOW)
	// -- faz com que ande na pilha de baixo pra cima (vai pra posicao aonde está PC HIGH para a proxima copia)
	
	*stk-- = (cpu_t)((int)task & 0xFF);
	
	// >> 8 (shift de 8 bits pra copiar a parte mais significativa do dado MSB: PC HIGH)
	*stk-- = (cpu_t)((int)task >> 8);
	
	// o restante é 0, 0 e 0x60
	*stk-- = 0;
	*stk-- = 0;
	*stk-- = 0x60;
	*stk = 0; // apontador da pilha apontando para a ultima pos. da pilha
	
	return stk;
}

interrupt void SwitchContext(void){
	SAVE_SP();
	tcb[ct].stk = stk_tmp;
	
	stk_tmp = scheduler();
	RESTORE_SP();
}

interrupt void TickTimer(void){ // interrupção de tempo, controla o tempo do sistema
	TPM1SC_TOF = 0; // limpa a flag da interrupção de timer overflow
	//os_time++;
	if (os_inc_and_compare()){
		SAVE_SP();
		tcb[ct].stk = stk_tmp;
		stk_tmp = scheduler();
		RESTORE_SP();
	}
}

void init_os_timer(void){
	// nesse micro temos um timer 1; reg. TPM1SC - registrador de estado
	TPM1SC = 0x00; 		// zera reg. para parar o timer
	TPM1MOD = 19999;	// configura o modo dele para 19999
	// clock do processador de 19.922 MHz ~ 20 MHz
	// contar de 0 a 19999 (20k ciclos) vamos ter
	// 20 MHz / 20 k = 1000
	// 1/1000 = 1e-3s ou 1 milisegundo
	TPM1SC = 0x48; 		//liga a interrupção e passa a contar
}