/*
*
*	fsm_switch_case.h
*
*/
#ifndef FSM_SWITCH_CASE_H_
#define FSM_SWITCH_CASE_H_

// biblioteca de numeros inteiros do c (tipo uint8_t)
#include "stdint.h"

// constantes usadas na comunicacao
#define STX		0x02
#define ETX		0x03

void fsm_switch_case_processa(uint8_t *data, uint8_t qtd);
void fsm_switch_case_teste(void);

#endif // FSM_SWITCH_CASE_H_