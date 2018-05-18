/*
 * InterruptRouer.h
 *
 *  Created on: 05-Jun-2016
 *      Author: Gunaseelan
 */

#ifndef SRC_BSW_DRV_INTERRUPTROUTER_INTERRUPTROUER_H_
#define SRC_BSW_DRV_INTERRUPTROUTER_INTERRUPTROUER_H_


#include "global.h"
#include "register.h"
#include "pxdef.h"

typedef uint8_t	Interrupt_Priortiy_t;

/*
 * ISR function pointer
 */
typedef void (*ISRFunc)(PxArg_t);

typedef void (*ISRFunc_preOS)(int);
/*
 * structure definition of Interrupt configuration table
 */
typedef struct{
	volatile Ifx_SRC_SRCR* 	SRC;			// SRC register
	ISRFunc					FunctionPtr;	// ISR function pointer
	CpuId_t					Core;			// Type Of service
	Interrupt_Priortiy_t	Priority;		//	Priority of the interrupt
}IntConfig_t;
/*
 * ==================== API FUnctions==========================
 */

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */


/*
 * Function to validate Interrupt router table configuration
 * This function will be called from the init task and therefore needs C linkage
 * @param	Node
 * @return	bool	TRUE: 	configuration is valid
 * 					FALSE:	Configuration is invalid
 */
RC_t InterruptRouter_CfgValidation(void);

/*
 * Function to Install ISR in C context
 * @param	FuncPrt		Address of the function pointer to be installed
 * @param	Arg			Parameter to the ISR function
 * @return  bool		TRUE: ISR installation sucessful
 * 						FALSE: ISR not installed
 */
boolean_t ISR_Install_C(ISRFunc FuncPrt, PxArg_t Arg);


/*
 * Function to Install an ISR before OS runtime
 * \param	SRC			Pointer to the SRC register of the interrupt source
 * \param	FuncPrt		Address of the function pointer to be installed
 * \param	Core		CPU on which the ISR is executed
 * \param	Priority	Interrupt priority
 * \return  boolean_t	#RC_SUCCESS ISR installation sucessful
 * 						#RC_ERROR ISR not installed
 */
RC_t ISR_Install_preOS(Ifx_SRC_SRCR* SRC, ISRFunc_preOS FuncPrt, CpuId_t Core,Interrupt_Priortiy_t Priority, int arg);


/*
 * Function to configure the SRC registers
 * @param	SRC		address of the SRC register to be configured
 * @return	bool	TRUE: SRC register configured successfully
 * 					FALSE: SRC register not configured properly and could be configured
 */
boolean_t InterruptRouter_CfgSRC(volatile Ifx_SRC_SRCR* SRC);
/*
 * Function to Install ISR
 * @param	FuncPrt		Address of the function pointer to be installed
 * @param	Arg			Parameter to the ISR function
 * @return  bool		TRUE: ISR installation sucessful
 * 						FALSE: ISR not installed
 */

#ifdef __cplusplus
  }
#endif /* __cplusplus */

boolean_t InterruptRouter_InstallISR(ISRFunc FuncPrt, PxArg_t Arg);


#endif /* SRC_BSW_DRV_INTERRUPTROUTER_INTERRUPTROUER_H_ */
