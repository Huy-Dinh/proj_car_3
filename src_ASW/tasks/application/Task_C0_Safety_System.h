/*************************************
 * Task_C0_Safety_System.h
 * Author: 			Fromm
 * Description: 	Safety Input Monitor
 * Task creation: 	
 */


#ifndef _TASK_C0_SAFETY_SYSTEM_H_
#define _TASK_C0_SAFETY_SYSTEM_H_

#include "pxdef.h"

/**
 * The C API of the task.....
 */

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


/*****************************************************************************************************
 * Global defines, NameserverId and Events
 *****************************************************************************************************/

#define TASK_C0_SAFETY_SYSTEM_NAMESERVERID        ((PxNameId_t){2,0,5,0})

#define TASK_C0_SAFETY_SYSTEM_PRIO                 19





/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

extern PxTask_t Task_C0_Safety_System_getTaskId();
 
extern PxTask_t Task_C0_Safety_System_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C0_SAFETY_SYSTEM_H_
