/*************************************
 * Task_C1_Init.h
 * Author: 			Fromm
 * Description: 	Initialisation Task of Core 1
 * Task creation: 	
 */

#ifndef TASK_C1_INIT_CREATE_1_H_
#define TASK_C1_INIT_CREATE_1_H_


#include "pxdef.h"

/*******************************************************************************************************
 * Core 1 specific defines
 *******************************************************************************************************/

#define NUM_OF_PXOBJS_CORE1  			50
#define PXROS_NAMESIZE_CORE1			20


#define SYSMEMSIZE_CORE1				12000
#define TASKMEMSIZE_CORE1				2000

#define TASK_C1_INIT_STACKSIZE_CORE1		200
#define TASK_C1_INIT_INTSTACKSIZE_CORE1	32

#define MINPRIO_CORE1        			31

/*******************************************************************************************************
 * Events
 ******************************************************************************************************/




/*******************************************************************************************************
 * Core 1 InitTask specification record
 *******************************************************************************************************/
extern const PxTaskSpec_T InitTaskSpec_CORE1;


/*******************************************************************************************************
 * Core 1 init task
 *******************************************************************************************************/
extern void Task_C1_Init_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);


#endif /* TASK_C1_INIT_CREATE_1_H_ */
