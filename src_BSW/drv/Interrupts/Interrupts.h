/**
* \file <filename>
* \author <author-name>
* \date <date>
*
* \brief <Symbolic File name>
*
* <detailed description what the file does>
*
* \note <notes>
* \todo <todos>
* \warning <warnings, e.g. dependencies, order of execution etc.>
*
*  Changelog:\n
*  - <version; data of change; author>
*            - <description of the change>
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

/**
 * Programming rules (may be deleted in the final release of the file)
 * ===================================================================
 *
 * 1. Naming conventions:
 *    - Prefix of your module in front of every function and static data.
 *    - Scope _ for public and __ for private functions / data / types, e.g.
 *       Public:  void CONTROL_straightPark_Init();
 *       Private: static void CONTROL__calcDistance();
 *       Public:  typedef enum {RED, GREEN, YELLOW} CONTROL_color_t
 *    - Own type definitions e.g. for structs or enums get a postfix _t
 *    - #define's and enums are written in CAPITAL letters
 * 2. Code structure
 *    - Be aware of the scope of your modules and functions. Provide only functions which belong to your module to your files
 *    - Prepare your design before starting to code
 *    - Implement the simple most solution (Too many if then else nestings are an indicator that you have not properly analysed your task)
 *    - Avoid magic numbers, use enums and #define's instead
 *    - Make sure, that all error conditions are properly handled
 *    - If your module provides data structures, which are required in many other files, it is recommended to place them in a file_type.h file
 *	  - If your module contains configurable parts, is is recommended to place these in a file_config.h|.c file
 * 3. Data conventions
 *    - Minimize the scope of data (and functions)
 *    - Global data is not allowed outside of the signal layer (in case a signal layer is part of your design)
 *    - All static objects have to be placed in a valid linker sections
 *    - Data which is accessed in more than one task has to be volatile and needs to be protected (e.g. by using messages or semaphores)
 *    - Do not mix signed and unsigned data in the same operation
 * 4. Documentation
 *    - Use self explaining function and variable names
 *    - Use proper indentation
 *    - Provide Javadoc / Doxygen compatible comments in your header file and C-File
 *    		- Every  File has to be documented in the header
 *			- Every function parameter and return value must be documented, the valid range needs to be specified
 *     		- Logical code blocks in the C-File must be commented
 *    - For a detailed list of doxygen commands check http://www.stack.nl/~dimitri/doxygen/index.html
 * 5. Qualification
 *    - Perform and document design and code reviews for every module
 *    - Provide test specifications for every module (focus on error conditions)
 *
 * Further information:
 *    - Check the programming rules defined in the MIMIR project guide
 *         - Code structure: https://fromm.eit.h-da.de/intern/mimir/methods/eng_codestructure/method.htm
 *         - MISRA for C: https://fromm.eit.h-da.de/intern/mimir/methods/eng_c_rules/method.htm
 *         - MISRA for C++: https://fromm.eit.h-da.de/intern/mimir/methods/eng_cpp_rules/method.htm
 **/

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "Interrupts_cfg.h"

/*****************************************************************************/
/* Global pre-processor symbols/macros and type declarations                 */
/*****************************************************************************/

//####################### Defines/Macros
/**
 * \brief a brief description of what the define is representing
*
* If needed, a more detailed description can be given below */
#define TOP_DOCUMENTED_DEFINE                    0x1
#define AFTER_DOCUMENTED_DEFINE                  0x2         /**< \brief by putting a < next to the comment-start. The documentation referes to the left instead to the next line. */


//####################### Enumerations
/**
* \brief INT_handler_t describes the type of the OS installation function to be used for the ISR
*
* Depending on the type of the handler, different contexts and latencies do apply.
*/
typedef enum {
	INT_FASTCONTEXTHANDLER,         /**< \brief Fast interrupt handler with task context
										Should be used in most cases. A safe handler will set the task context when the ISR is executed. Instead of #INT_CONTEXTHANDLER, the handler is called immediately if the corresponding interrupt has occurred.
										\note 		The installing task requires #PXACCESS_INSTALL_HANDLERS */

	INT_CONTEXTHANDLER,         	/**< \brief Interrupt handler with task context
										A safe handler will set the task context when the ISR is executed. It is executed after the device returns from interrupt to system level.
										\note 		The installing task requires #PXACCESS_INSTALL_HANDLERS
										\note 		A local Handler is not immediately called if the corresponding interrupt has occurred; instead it is entered into a list, which is processed after returning from interrupt level to system level*/

	INT_FASTHANDLER,                /**< 	Fast Handlers are the fastest and highest prioritised interrupt Handlers. They run with system privileges(!!!) in interrupt mode and are activated as soon as an interrupt occurs.
										\note 		the installing task requires #PXACCESS_HANDLERS
										\warning 	Should only be used if you really know what you are doing. */

} INT_handler_t;

//####################### Structures

// Wrapper to allow representing the file in Together as class
#ifdef TOGETHER

class INT
{
public:
#endif /* Together */

/*****************************************************************************/
/* Extern global variables                                                   */
/*****************************************************************************/



/*****************************************************************************/
/* API functions                                                             */
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
RC_t INT_preOsStart(INT_isrEntry_t* table, uint16_t tableSize);

/**
 * This function clears the service request nodes and interrupt vector table
 * \note This function has to be called before reconfiguring the interrupt system or before starting the OS
 * \return error code
 */
RC_t INT_preOsStop(void);

//####################### OS Configuration

/**
 * This function configures the service request node and interrupt vector table in a taxt context
 * \param Ifx_SRC_SRCR* serviceRequestNode the service request node which needs to be configured
 * \param INT_isr_t pIsr point to the ISR function
 * \param uint8_t priority, every priority may only be used once.
 * \param INT_handler_t handlerType type of hanlder to be installed. Check the enum description for details
 * \return error code
 */
RC_t INT_osInstall(Ifx_SRC_SRCR* serviceRequestNode, INT_isr_t pIsr, uint8_t priority, INT_handler_t handlerType);

RC_t INT_osUnInstall();
#ifdef __cplusplus
  }
#endif /* __cplusplus */

/*****************************************************************************/
/* Private stuff, only visible for Together, declared static in cpp - File   */
/*****************************************************************************/

#ifdef TOGETHER
//Not visible for compiler, only used for document generation
private:

//Attributes

/**
 * <description>
 */
static type FILE__variable;

//Operations

/**
 * <description>
 * @param <para 1>
 * @return <return description>
 */
static type FILE__function(uint16_t cmd);


};
#endif /* Together */

#endif /* FILE_H */
