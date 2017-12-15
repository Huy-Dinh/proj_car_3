/**
 * \file spinlock.h
 * \author Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 15.08.2016
 *
 * \brief Tricore atomic swap implementation for use in spinlocks
 *
 * On multicore controllers the user can not use simple while-loops to check if a semaphore is available or not.
 * The reason is that there are multiple instructions to read a semaphore and once it becomes free, multiple CPUs could claim it.
 * A solution to that problem are the SWAP instructions that work in an atomic way (in only one instruction).
 * The bus-arbitration permits multiple access to a single memory location at a time.
 *
 * \copyright Copyright ©2016
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

#ifndef SPINLOCK_H_
#define SPINLOCK_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include <stdint.h>

/*******************************************************************************
**                      Defines                                               **
*******************************************************************************/

#define SEMAPHORE_FREE	 		0x0
#define SEMAPHORE_BLOCKED 		0x1

//The NOPs in this macro are inserted to prevent a bus starve during spinlock
#define BUS_DELAY		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\
	__asm__("nop");		\


/*******************************************************************************
**                      Spinlock Macros                                       **
*******************************************************************************/

//Macro for SWAP.W Spinlock with bus starve prevention

#define SWAP_M(address) 								\
{ 														\
	uint32_t SWAP_M_result;								\
	do { 												\
			SWAP_M_result=SEMAPHORE_BLOCKED;			\
			__asm__ volatile ( 							\
			"swap.w [%1]0,%0" 							\
			: "+d" (SWAP_M_result) 						\
			: "a" (address) 							\
			: "memory"); 								\
		if(SWAP_M_result==SEMAPHORE_BLOCKED)			\
		{												\
			BUS_DELAY									\
		}												\
	} while (SWAP_M_result==SEMAPHORE_BLOCKED); 		\
	SWAP_M_result; 										\
}														\



#endif /* SPINLOCK_H_ */




