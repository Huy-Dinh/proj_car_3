/*
 * InterruptRouter.cpp
 *
 *  Created on: 05-Jun-2016
 *      Author: Gunaseelan
 */

#include "InterruptRouter.h"
#include "InterruptRouter_cfg.h"
#include "SRC.h"
#include "interrupt_init.h"


//maximum interrupts that can be configured 256 *3 = 768
#define MAX_INT_NO		768



/*=============================================================================
 * ****************** Static function*********************************
 * ============================================================================
 */
/*
 * Function to get the size of the config table
 * @param	None
 * @return	size of the config table
 */
uint16_t InterruptRouter__GetSize()
{
	uint16_t loop = 0;

	while(loop < MAX_INT_NO)
	{
		if(0 == IntConfigTbl[loop].SRC)
		{
			return loop;
		}
		loop++;
	}
	return loop;
}
/*
 * Function to get the index of the config table where the ISR function is configured
 * @param	FuncPrt		ISR function pointer
 * @param	index		hold the index of the table
 * @return	bool		TRUE: Valid entry is found in the table and index is updated in out param
 * 						FALSE: Entry not found
 */
boolean_t InterruptRouter__GetIndex(ISRFunc FuncPrt, uint16_t* index)
{
	uint16_t loop,size;
	if(FuncPrt != NULL)
	{
		//get the size of the table
		size = InterruptRouter__GetSize();
		if(size < MAX_INT_NO)
		{
			//loop through the table
			for(loop = 0; loop < size; loop++)
			{
				//check if given ISR is configured
				if(FuncPrt == IntConfigTbl[loop].FunctionPtr)
				{
					//update the index
					*index = loop;
					return true;
				}
			}
		}
		return false;
	}
	//valid entry not found
	return false;
}
/*
 * Function to get the index of the config table where the SRC register is configured
 * @param	SRC			Address of the SRC reg
 * @param	index		hold the index of the table
 * @return	bool		TRUE: Valid entry is found in the table and index is updated in out param
 * 						FALSE: Entry not found
 */
boolean_t InterruptRouter__GetIndex(volatile Ifx_SRC_SRCR* SRC, uint16_t* index)
{
	uint16_t loop,size;
	if(SRC != NULL)
	{
		//get the size of the table
		size = InterruptRouter__GetSize();
		if(size < MAX_INT_NO)
		{
			//loop through the table
			for(loop = 0; loop < size; loop++)
			{
				//check if given SRC register is configured
				if(SRC == IntConfigTbl[loop].SRC)
				{
					//update the index
					*index = loop;
					return true;
				}
			}
		}
		return false;
	}
	return false;
}
/*=============================================================================
 * ****************** Global API functions*********************************
 * ============================================================================
 */


extern "C"
{

/*
 * Function to validate Interrupt router table configuration
 * @param	Node
 * @return	bool	TRUE: 	configuration is valid
 * 					FALSE:	Configuration is invalid
 */

RC_t InterruptRouter_CfgValidation(void)
{
	uint16_t loop1,loop2,size;
	//uint8_t core;
	size = InterruptRouter__GetSize();
	//check for valid size
	if(size < MAX_INT_NO)
	{
		// loop throuth the table to find duplicate entry
		for(loop1 = 0; loop1 < size;loop1++)
		{
			for(loop2 = loop1+1; loop2 < size;loop2++)
			{
				//check if priority and ccore ID matches
				if((IntConfigTbl[loop1].Priority == IntConfigTbl[loop2].Priority)
						&& (IntConfigTbl[loop1].Core == IntConfigTbl[loop2].Core))
				{
					//duplicate entry found return false
					return RC_ERROR_BAD_DATA;
				}
			}
		}
		return RC_SUCCESS;
	}
	else
	{
		return RC_ERROR_BAD_DATA;
	}
}

} //extern "C"

/*
 * Function to configure the SRC registers
 * @param	SRC		address of the SRC register to be configured
 * @return	bool	TRUE: SRC register configured successfully
 * 					FALSE: SRC register not configured properly and could be configured
 */
boolean_t InterruptRouter_CfgSRC(volatile Ifx_SRC_SRCR* SRC)
{
	uint16_t index;

	if(InterruptRouter__GetIndex(SRC, &index))
	{
		 SRC_init(SRC ,IntConfigTbl[index].Core,IntConfigTbl[index].Priority);
		 SRC_enable(SRC);
		 return true;
	}
	return false;
}
/*
 * Function to Install ISR
 * @param	FuncPrt		Address of the function pointer to be installed
 * @param	Arg			Parameter to the ISR function
 * @return  bool		TRUE: ISR installation sucessful
 * 						FALSE: ISR not installed
 */
boolean_t InterruptRouter_InstallISR(ISRFunc FuncPrt, PxArg_t Arg)
{
	uint16_t index;
	//get the index of the table
	if(InterruptRouter__GetIndex(FuncPrt, &index))
	{
		//Install ISR
		//Hack: At the moment, the FastContextHandler does not iterate through the memory region array of the task
		//      This may lead to an unhandled instruction and will be fixed in the next release of PXROS
		//      Workaround: Execute the ISR in the system context and allow the system access to the required memory regions
		PxIntInstallFastContextHandler(IntConfigTbl[index].Priority , (PxIntHandler_t)FuncPrt, (PxArg_t)Arg);
		//PxIntInstallFastHandler(IntConfigTbl[index].Priority , (PxIntHandler_t)FuncPrt, (PxArg_t)Arg);
		return true;
	}
	return false;
}


#include TC_INCLUDE(TCPATH/IfxSrc_bf.h)
#include TC_INCLUDE(TCPATH/IfxSrc_reg.h)
#include TC_INCLUDE(TCPATH/IfxSrc_regdef.h)

///** \\brief  Service request register */
//typedef struct _Ifx_SRC_SRCR_Bits
//{
//    unsigned int SRPN:8;                    /**< \brief [7:0] Service Request Priority Number (rw) */
//    unsigned int reserved_8:2;              /**< \brief \internal Reserved */
//    unsigned int SRE:1;                     /**< \brief [10:10] Service Request Enable (rw) */
//    unsigned int TOS:2;                     /**< \brief [12:11] Type of Service Control (rw) */
//    unsigned int reserved_13:3;             /**< \brief \internal Reserved */
//    unsigned int ECC:6;                     /**< \brief [21:16] ECC (rwh) */
//    unsigned int reserved_22:2;             /**< \brief \internal Reserved */
//    unsigned int SRR:1;                     /**< \brief [24:24] Service Request Flag (rh) */
//    unsigned int CLRR:1;                    /**< \brief [25:25] Request Clear Bit (w) */
//    unsigned int SETR:1;                    /**< \brief [26:26] Request Set Bit (w) */
//    unsigned int IOV:1;                     /**< \brief [27:27] Interrupt Trigger Overflow Bit (rh) */
//    unsigned int IOVCLR:1;                  /**< \brief [28:28] Interrupt Trigger Overflow Clear Bit (w) */
//    unsigned int SWS:1;                     /**< \brief [29:29] SW Sticky Bit (rh) */
//    unsigned int SWSCLR:1;                  /**< \brief [30:30] SW Sticky Clear Bit (w) */
//    unsigned int reserved_31:1;             /**< \brief \internal Reserved */
//} Ifx_SRC_SRCR_Bits;
//
//typedef union
//{
//    /** \brief Unsigned access */
//    unsigned int U;
//    /** \brief Signed access */
//    signed int I;
//    /** \brief Bitfield access */
//    Ifx_SRC_SRCR_Bits B;
//} Ifx_SRC_SRCR;




RC_t ISR_Install_preOS(Ifx_SRC_SRCR* SRC, ISRFunc_preOS FuncPrt, CpuId_t Core,Interrupt_Priortiy_t Priority, int arg){

	//set handler
	Cdisptab[Priority].irq_handler=FuncPrt;
	Cdisptab[Priority].hnd_arg=arg;

	//set Interrupt Priority
	SRC->B.SRPN = Priority;

	//set TOS (CPU which will handle the IRQ)
	SRC->B.TOS  = Core;

	//clear pending requests
	SRC->B.CLRR=BIT_1;

	//Enable Interrupt
	SRC->B.SRE = BIT_1;


	return RC_SUCCESS;
}


extern "C" {
boolean_t ISR_Install_C(ISRFunc FuncPrt, PxArg_t Arg){
	return(InterruptRouter_InstallISR(FuncPrt, Arg));
}
}//extern C
