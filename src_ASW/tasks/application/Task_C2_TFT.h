/*************************************
 * Task_C2_TFT.h
 * Author: 			P. Fromm et al
 * Description: 	Task for TFT Display and Touchscreen
 * Task creation: 	
 */


#ifndef _TASK_C2_TFT_H_
#define _TASK_C2_TFT_H_

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

#define TASK_C2_TFT_NAMESERVERID        ((PxNameId_t){2,2,1,0})
#define TASK_C2_TFT_PRIO                 25



/*****************************************************************************************************
 * API function for creating the task
 *****************************************************************************************************/

extern PxTask_t Task_C2_TFT_getTaskId();
 
extern PxTask_t Task_C2_TFT_Create(PxUChar_t prio, PxEvents_t actevents);

#ifdef __cplusplus
  }
#endif /* __cplusplus */


#endif // _TASK_C2_TFT_H_
