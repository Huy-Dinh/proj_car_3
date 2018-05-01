/**
 * \file	interrupt_init.h
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	25.04.2018
 * \version	0.1
 *
 * \brief TODO
 *
 * TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
 *
 *	Changelog:
 *
 * \copyright Copyright ©2018
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

//TODO: doxygen docu

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



//RC_t _install_int_hdl (uint32_t intno, void (*handler) (int), int arg);




#endif /* INTERRUPT_INIT_H_ */
