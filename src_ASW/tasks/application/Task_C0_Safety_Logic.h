/*************************************
 * Task_C0_Safety_Logic.h
 * Author: 			Fromm
 * Description: 	Safety Logic Monitor
 * Task creation: 	
 */


#ifndef _TASK_C0_SAFETY_LOGIC_H_
#define _TASK_C0_SAFETY_LOGIC_H_

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

#define TASK_C0_SAFETY_LOGIC_NAMESERVERID        ((PxNameId_t){2,0,2,0})
#define TASK_C0_SAFETY_LOGIC_PRIO                 25




/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

extern PxTask_t Task_C0_Safety_Logic_getTaskId();
 
extern PxTask_t Task_C0_Safety_Logic_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C0_SAFETY_LOGIC_H_
