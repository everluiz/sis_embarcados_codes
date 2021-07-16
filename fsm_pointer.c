/*
*
*	fsm_pointer.c
*
*/
#include <stdio.h>
#include "string.h"
#include "fsm_pointer.h"
#define MAX_BUFFER 512   
#define FSM_PONTEIRO 1
#if FSM_PONTEIRO == 1

/* numero maximo de bytes do buffer de dados */


/* possiveis estados da maquina de estados de comunicacao */
typedef enum{
	ST_STX = 0, ST_QTD, ST_DATA, ST_CHK, ST_ETX
}States;

// define ponteiro de função
typedef void (*Action)(uint8_t data);

struct StateMachine{
	States state;
	uint8_t buffer[MAX_BUFFER];
	uint8_t chkBuffer;
	uint16_t indBuffer;
	uint16_t qtdBuffer;
	Action action[5];
}sm;

// trata dados recebidos
void handlePackage(uint8_t *data, uint16_t qtd){
	uint16_t i;
	printf("Imprimindo dados recebidos...\n");
	for (int i = 0; i < qtd; i++)
	{
		printf("Data[%d] = %d\n",i, data[i]);
	}

}
void stSTX(uint8_t data){
	if (data == STX)
	{
		sm.indBuffer = sm.qtdBuffer = 0;
		sm.chkBuffer = 0;
		sm.state = ST_QTD;
	}
}
void stQTD(uint8_t data){
	sm.qtdBuffer = data;
	sm.state = ST_DATA;
}
void stDATA(uint8_t data){
	sm.buffer[sm.indBuffer++] = data;
	sm.chkBuffer ^= data;
	if (--sm.qtdBuffer == 0)
	{
		sm.state = ST_CHK;
	}
}
void stCHK(uint8_t data){
	if (data == sm.chkBuffer)
	{
		sm.state = ST_ETX;
	}else{
		sm.state = ST_STX;
	}
}
void stETX(uint8_t data){
	if (data == ETX)
	{
		//handlePackage(sm.buffer, sm.indBuffer);
	}
	sm.state = ST_STX;
}

void handleRx(uint8_t *data, uint16_t qtd){
	uint16_t i;
	for (int i = 0; i < qtd; i++)
	{
		/* executa a funcao de sm.action equivalente ao estado atual sm.state
		*  com parametro passado em data[i]
		*/
		sm.action[sm.state](data[i]);
	}
}
void initFSM(void){
	sm.state = ST_STX;
	sm.buffer[0] = 0;
	sm.chkBuffer = 0;
	sm.indBuffer = 0;
	sm.qtdBuffer = 0;
	sm.action[ST_STX] = stSTX;
	sm.action[ST_QTD] = stQTD;
	sm.action[ST_DATA] = stDATA;
	sm.action[ST_CHK] = stCHK;
	sm.action[ST_ETX]  = stETX;
}
void fsm_ponteiro_teste(void){
	uint8_t data1[] = {STX, 5, 11, 22, 33, 44};
	uint8_t data2[] = {55, 39, ETX};
	uint8_t resultado_esperado[] = {11, 22, 33, 44, 55};

	//struct StateMachine sm;
	initFSM();

	handleRx(data1, sizeof(data1));
	handleRx(data2, sizeof(data2));

	if (memcmp(resultado_esperado,sm.buffer,sizeof(resultado_esperado)) == 0)
	{
		printf("teste: PASSOU\n");
	}else{
		printf("teste: FALHOU\n");
	}
}
#endif