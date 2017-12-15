/*************************************
 * Task_C1_Control.h
 * Author: 			Fromm
 * Description: 	Control Logic Task
 * Task creation: 	
 */


#ifndef _TASK_C1_CONTROL_H_
#define _TASK_C1_CONTROL_H_

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

#define TASK_C1_CONTROL_NAMESERVERID        ((PxNameId_t){2,1,1,0})
#define TASK_C1_CONTROL_PRIO                 29




/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

extern PxTask_t Task_C1_Control_getTaskId();
 
extern PxTask_t Task_C1_Control_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C1_CONTROL_H_
