/*
 * InterruptRouter_cfg.h
 *
 *  Created on: 05-Jun-2016
 *      Author: Gunaseelan
 */

#ifndef INTERRUPTS_CFG_H_
#define INTERRUPTS_CFG_H_

#include "pxdef.h"
#include "IfxSrc_reg.h"
#include "IfxSrc_regdef.h"
#include "global.h""

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

  //Todo: Cleanup structire and comments according to template

  /*
   * ISR function pointer
   */
  typedef void (*INT_isr_t)(PxArg_t);
  /*
   * structure definition of Interrupt configuration table
   */
  typedef struct{
  	volatile Ifx_SRC_SRCR* 	SRC;			// SRC register
  	INT_isr_t				pIsr;			// ISR function pointer
  	CpuId_t					Core;			// Type Of service
  	uint8_t					Priority;		//	Priority of the interrupt
  }INT_isrEntry_t;

extern const INT_isrEntry_t INT_preOsTable[];

#ifdef __cplusplus
  }
#endif /* __cplusplus */
#endif /* SRC_BSW_DRV_CONFIG_INTERRUPTROUTER_INTERRUPTROUTER_CFG_H_ */
