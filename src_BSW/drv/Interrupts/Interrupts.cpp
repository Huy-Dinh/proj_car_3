/**
* \file <filename>
* \author <author-name>
* \date <date>
*
* \brief <Symbolic File name>
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

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/

#include "Interrupts.h"
//#include "SRC.h"
#include "system.h"
#include "IfxCpu_reg.h"
#include "IfxCpu_regdef.h"
#include "global.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

typedef struct
{
	INT_isrEntry_t* table;
	uint16_t tableSize;
} INT__preOsConfig_t;

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

//Todo: Define Section
#pragma section ".drv.isr.data"
static INT__preOsConfig_t INT__preOsConfig = {0, 0};
static uint8_t INT__OSConfigBits[32] = {0};
#pragma section
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

static void INT__setConfigBit(uint8_t priority);
static void INT__clearConfigBit(uint8_t priority);
static uint8_t INT__configBitIsSet(uint8_t priority);

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


//####################### PreOs / Bare Metal Interrupt Configuration
/**
 * This function configures the service request nodes and interrupt vector table based on the given configuration parameter
 * \param <Format: copy of the parameter type and name - description>
 * \return error code
 */
RC_t INT_preOsStart(INT_isrEntry_t* table, uint16_t tableSize)
{
	uint8_t i = 0;
	SYSTEM_DisableInterrupts();
	//Iterate through the table
	for (i = 0; i < tableSize; i++)
	{
		//Configure the IRN ==< please use the registers from src.h.old
		table[i].SRC->B.SRPN = i + 1; //Set up the priority
		table[i].SRC->B.TOS = table[i].Core; //Select the core
		table[i].SRC->B.CLRR = 1; //Clear pending request
		table[i].SRC->B.SRE = 1; //Enable the SRN
	}
	//Set pointer and size to internal configuration
	INT__preOsConfig.table = table;
	INT__preOsConfig.tableSize = tableSize;

	//Enable global interrupts
	SYSTEM_EnableInterrupts();

	return RC_SUCCESS;
}

/**
 * This function clears the service request nodes and interrupt vector table
 * \note This function has to be called before reconfiguring the interrupt system or before starting the OS
 * \return error code
 */
RC_t INT_preOsStop(void)
{
	uint8_t i = 0;
	//Disable global interrupts
	SYSTEM_DisableInterrupts();
	//Reset all SRN configurations
	for (i = 0; i < INT__preOsConfig.tableSize; i++)
	{
		INT__preOsConfig.table[i].SRC->U = 0;
	}
	return RC_SUCCESS;
}

//####################### OS Configuration

/**
 * This function configures the service request node and interrupt vector table in a taxt context
 * \param Ifx_SRC_SRCR* serviceRequestNode the service request node which needs to be configured
 * \param INT_isr_t pIsr point to the ISR function
 * \param uint8_t priority, every priority may only be used once.
 * \param INT_handler_t handlerType type of hanlder to be installed. Check the enum description for details
 * \return error code
 */
RC_t INT_osInstall(Ifx_SRC_SRCR* serviceRequestNode, INT_isr_t pIsr, uint8_t priority, INT_handler_t handlerType, PxArg_t argument)
{
	//Check if the SRN already has been configured
	if (serviceRequestNode->B.SRE)
	{
		return RC_ERROR_BAD_PARAM;
	}
	//Check if priority already has been used
	if (INT__configBitIsSet(priority))
	{
		return RC_ERROR_BAD_PARAM;
	}

	//Configure the IRN
	serviceRequestNode->B.SRPN = priority; //Set up the priority
	serviceRequestNode->B.TOS = SYSTEM_GetCoreId(); //Select the core
	serviceRequestNode->B.CLRR = 1; //Clear pending request
	//Depending on the handler type, call the corresponding PXROS function
	switch (handlerType)
	{
		case INT_FASTCONTEXTHANDLER:
			if (PxIntInstallFastContextHandler(priority, pIsr, argument) != PXERR_NOERROR)
			{
				return RC_ERROR;
			}
			else
			{
				serviceRequestNode->B.SRE = 1; //Enable the SRN
				//Set the config bit so that we know an interrupt has been configured for this priority
				INT__setConfigBit(priority);
			}
			break;
		case INT_FASTHANDLER:
			if (PxIntInstallFastHandler(priority, pIsr, argument) != PXERR_NOERROR)
			{
				return RC_ERROR;
			}
			else
			{
				serviceRequestNode->B.SRE = 1; //Enable the SRN
				//Set the config bit so that we know an interrupt has been configured for this priority
				INT__setConfigBit(priority);
			}
			break;
		case INT_CONTEXTHANDLER:
			//TODO add handler call
			return RC_ERROR_BAD_PARAM;
			break;
		default:
			return RC_ERROR_BAD_PARAM;
			break;
	}
	return RC_SUCCESS;
}

/********************/
/* Local functions  */
/********************/
void INT__setConfigBit(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	INT__OSConfigBits[bytePosition] |= 1 << bitPosition;
}

void INT__clearConfigBit(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	INT__OSConfigBits[bytePosition] &= ~(1 << bitPosition);
}
uint8_t INT__configBitIsSet(uint8_t priority)
{
	uint8_t bitPosition = priority % 8;
	uint8_t bytePosition = priority / 8;
	if (INT__OSConfigBits[bytePosition] & (1 << bitPosition))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/********************/
/* Technical Stuffs */
/********************/

//This function is called whenever an Interrupt occurs
void INT_CommonDispatcher()
{
	Ifx_CPU_ICR IcrValue = {__MFCR(CPU_ICR)}; // Workaround for the C++ constructor problem
  	if ((IcrValue.B.CCPN <= INT__preOsConfig.tableSize) || (IcrValue.B.CCPN > 0))
  	{
  		if (INT__preOsConfig.table[IcrValue.B.CCPN - 1].pIsr != NULL)
  		{
  			INT__preOsConfig.table[IcrValue.B.CCPN - 1].pIsr(NULL);
  		}
  	}
}
// Interrupt Table Entry, the BIV points to this after startup
asm (".section .interrupttable_init_in, \"ax\", @progbits");	// Create an input section
asm("enable");													//enable interrupts
asm("svlcx");													//save lower context
asm("calla INT_CommonDispatcher");									//call common dispatcher function
asm("rslcx");													//restore lower context
asm("rfe");														//return from exception (leave ISR)
asm (".text");


#ifdef __cplusplus
}
#endif /* __cplusplus */

