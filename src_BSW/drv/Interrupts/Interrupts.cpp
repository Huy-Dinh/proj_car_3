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
#include "SRC.h"
#include "system.h"

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

//Todo: Define Section
static INT__preOsConfig_t INT_preOsConfig = {0, 0};


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

//####################### PreOs / Bare Metal Interrupt Configuration
/**
 * This function configures the service request nodes and interrupt vector table based on the given configuration parameter
 * \param <Format: copy of the parameter type and name - description>
 * \return error code
 */
RC_t INT_preOsStart(INT_isrEntry_t* table, uint16_t tableSize)
{
	uint8_t i = 0;
	//Iterate through the table
	for (i = 0; i < tableSize; i++)
	{
		//Configure the IRN ==< please use the registers from src.h.old
		SRC_init(table[i].SRC, table[i].Core, i+1);
		SRC_enable(table[i].SRC);
	}
	//Set pointer and size to internal configuration
	INT_preOsConfig.table = table;
	INT_preOsConfig.tableSize = tableSize;

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
	for (i = 0; i < INT_preOsConfig.tableSize; i++)
	{
		SRC_deinit(INT_preOsConfig.table[i].SRC);
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
RC_t INT_osInstall(Ifx_SRC_SRCR* serviceRequestNode, INT_isr_t pIsr, uint8_t priority, INT_handler_t handlerType)
{
	//Check if the SRN already has been configured

	//CHeck if priority already has been used

	//Configure the IRN ==< please use the registers from src.h.old

	//Depending on the handler type, call the corresponding PXROS function

}
#ifdef __cplusplus
  }
#endif /* __cplusplus */

