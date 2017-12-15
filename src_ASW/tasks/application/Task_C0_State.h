/*************************************
 * Task_C0_State.h
 * Author: 			Fromm
 * Description: 	Central State Machine Task
 * Task creation: 	
 */


#ifndef _TASK_C0_STATE_H_
#define _TASK_C0_STATE_H_

#include "pxdef.h"

/**
 * The C API of the task.....
 */

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


/*****************************************************************************************************
 * Global defines, NameserverId and Priority
 *****************************************************************************************************/

#define TASK_C0_STATE_NAMESERVERID        ((PxNameId_t){2,0,4,0})
#define TASK_C0_STATE_PRIO                 20


/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

extern PxTask_t Task_C0_State_getTaskId();
 
extern PxTask_t Task_C0_State_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C0_STATE_H_
