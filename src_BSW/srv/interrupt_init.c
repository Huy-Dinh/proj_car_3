/**
 * \file	interrupt_init.c
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	25.04.2018
 * \version	0.1
 *
 * \copyright Copyright �2018
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

#include "interrupt_init.h"
#include "IfxCpu_reg.h"
#include "IfxCpu_regdef.h"


//TODO: table for every core? [DERIVATE_NUM_CORES]
IRQ_hdl_t Cdisptab[MAX_INTRS]={
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0}
};


//This function is called whenever an Interrupt occurs
void commonDispatcher()
{
	Ifx_CPU_ICR IcrValue = (Ifx_CPU_ICR) __MFCR(CPU_ICR);
	if (Cdisptab[IcrValue.B.CCPN].irq_handler != NULL)
	{
		Cdisptab[IcrValue.B.CCPN].irq_handler(Cdisptab[IcrValue.B.CCPN].hnd_arg);
	}
}

// Interrupt Table Entry, the BIV points to this after startup
asm (".section .interrupttable_init_in, \"ax\", @progbits");	// Create an input section
asm("enable");													//enable interrupts
asm("svlcx");													//save lower context
asm("calla commonDispatcher");									//call common dispatcher function
asm("rslcx");													//restore lower context
asm("rfe");														//return from exception (leave ISR)
asm (".text");													//close input section

/* The time will come when we will need this function....
 *
 *
 * but this is not today
RC_t _install_int_hdl (uint32_t intno, void (*handler) (int), int arg){
  if(intno>=MAX_INTRS)
	  return RC_ERROR_BAD_PARAM;

  Cdisptab[intno].irq_handler=handler;
  Cdisptab[intno].hnd_arg=arg;

  return RC_SUCCESS;

}
*/
