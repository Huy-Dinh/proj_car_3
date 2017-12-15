/*
 * interrupt_init.h
 *
 *  Created on: 18.07.2016
 *      Author: Thomas
 */

#ifndef INTERRUPT_INIT_H_
#define INTERRUPT_INIT_H_

#include "global.h"

typedef struct
{
  void (*irq_handler) (int);
  int hnd_arg;
} IRQ_hdl_t;

#define MAX_INTRS 256

extern IRQ_hdl_t Cdisptab[MAX_INTRS];



RC_t _install_int_hdl (uint32_t intno, void (*handler) (int), int arg);




#endif /* INTERRUPT_INIT_H_ */
