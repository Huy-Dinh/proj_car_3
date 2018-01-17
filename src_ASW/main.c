/**
 * \author	Peter Fromm 	Hochschule Darmstadt
 * \date 	12.12.2016
 * \version	0.1
 *
 *
 * \copyright Copyright �2016
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

#include "global.h"

#include "system.h"
#include "PORT.h"
#include "UART.h"
#include "CANopen.h"
#include "CFG_check.h"
#include "QSPI.h"
#include "TLF.h"

#include "det.h"
#include "sync.h"
#include "task_init.h"

#include "rte_time.h"
#include "det_time.h"

#include "InterruptRouter.h"
#include "register.h"
#include "IfxSrc_reg.h"
#include "IfxAsclin_reg.h"

uint8_t test = 0xFF;
uint64_t countingTimer;

#define BUFFER_SIZE 500

typedef enum
{
	TEST_RUN,
	TEST_FINISHED,
	TEST_STOPPED
} testStatus_t;

typedef enum
{
	TEST_FAILED_TIME,
	TEST_FAILED_CORRUPT,
	TEST_PASS
} testResult_t;

volatile testStatus_t testStatus;

unsigned char sendBuffer[BUFFER_SIZE];
unsigned char receiveBuffer[BUFFER_SIZE];

volatile unsigned int sendIndex;
volatile unsigned int receiveIndex;

void resetTimer()
{
	countingTimer = 0;
}

uint64_t getTimer()
{
	return countingTimer;
}


void TX_UART_RX_Isr(int inputChannel)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.RFLC = 1;
	// Read the byte anyway
	UART_ReadData(inputChannel, &test);
}

void TX_UART_TX_Isr(int inputChannel)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.TFLC = 1;
}


void RX_UART_RX_Isr(int inputChannel)
{
	// Echoing back the received byte
	MODULE_ASCLIN3.FLAGSCLEAR.B.RFLC = 1;
	UART_ReadData(inputChannel, &(receiveBuffer[receiveIndex]));
	resetTimer();
	if (receiveBuffer[receiveIndex] != sendBuffer[receiveIndex])
	{
		testStatus = TEST_STOPPED;
	}
	else if (receiveIndex < (BUFFER_SIZE - 1))
	{
		++receiveIndex;
	}
	else
	{
		testStatus = TEST_FINISHED;
		receiveIndex = 0;
	}
}

void RX_UART_TX_Isr(int inputChannel)
{
	MODULE_ASCLIN3.FLAGSCLEAR.B.TFLC = 1;
}

void Sample_Timer_Isr(int inputChannel)
{
	GPT12_Reload(GPT12_T2);
	countingTimer++;
	if ((testStatus == TEST_RUN) && (sendIndex <= (BUFFER_SIZE - 1)))
	{
		if (UART_WriteData(uart4, sendBuffer[sendIndex]) == RC_SUCCESS)
		{
			++sendIndex;
		}
	}
	else
	{
		//Do nothing
	}
}

void UART_Err_Isr(int inputChannel)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.FEC = 1;
	MODULE_ASCLIN2.FLAGSCLEAR.B.RFUC = 1;
}


testResult_t runOneTest()
{
	resetTest();
	testStatus = TEST_RUN;
	while (getTimer() < 100000)
	{
		if (testStatus == TEST_FINISHED)
		{
			testStatus = TEST_STOPPED;
			return TEST_PASS;
		}
		else if (testStatus == TEST_STOPPED)
		{
			testStatus = TEST_STOPPED;
			return TEST_FAILED_CORRUPT;
		}
	}
	testStatus = TEST_STOPPED;
	return TEST_FAILED_TIME;
}

void runSeveralTest(uint32_t numberOfTests, uint32_t* numberOfTimeFails, uint32_t* numberOfCorruptions)
{
	uint32_t i = 0;
	*numberOfCorruptions = 0;
	*numberOfTimeFails = 0;
	testResult_t singleTestResult;
	for (i = 0; i < numberOfTests; i++)
	{
		singleTestResult = runOneTest();
		if (singleTestResult == TEST_FAILED_CORRUPT)
		{
			++(*numberOfCorruptions);
		}
		else if (singleTestResult == TEST_FAILED_TIME)
		{
			++(*numberOfTimeFails);
		}
	}
}

void resetTest()
{
	testStatus = TEST_STOPPED;
	resetTimer();
	while (getTimer() < 1000);
	resetTimer();
	sendIndex = 0;
	receiveIndex = 0;
	memset(receiveBuffer, 0, BUFFER_SIZE);
}

void fillSendBuffer()
{
	unsigned int i = 0;
	for (i = 0; i < (BUFFER_SIZE - 1); i++)
	{
		sendBuffer[i] = i/255;
	}
}

int main(){

	// Storage for the result of PxInit
	PxError_t 	PxInit_ret = PXERR_NOERROR;

	//storage for the current CPU-ID
	CpuId_t 	CoreID = SYSTEM_GetCoreId();

	// Core 0 performs driver initialization
	if(CoreID == cpu0){

		//GPIO Init
		PORT_Init();

		RTE_TIME_TimerInit();
		DET_TIME_init();

		//check integrity of system configuration
		if(CFG_check()!=RC_SUCCESS)
			DET_stop(AUTOCORE,CFG_CHECK, 0);

		//system Init (PLL setup, DTS init and TLF shutdown)
		if(SYSTEM_Init()!=RC_SUCCESS)
			DET_stop(AUTOCORE,SYSTEM_INIT, 0);

		//SPI Init
		if( RC_SUCCESS != QSPI_module_init(QSPI0,NULL))
			DET_stop(AUTOCORE, QSPI_MODULE_INIT, 0);

		if( RC_SUCCESS != QSPI_module_init(QSPI2,NULL))
			DET_stop(AUTOCORE, QSPI_MODULE_INIT, 0);

		UART_init();

		ISR_Install_preOS(&SRC_ASCLIN2RX, TX_UART_RX_Isr, cpu0, 32, uart4);
		ISR_Install_preOS(&SRC_ASCLIN2TX, TX_UART_TX_Isr, cpu0, 33, uart4);

		ISR_Install_preOS(&SRC_ASCLIN3RX, RX_UART_RX_Isr, cpu0, 34, uart6);
		ISR_Install_preOS(&SRC_ASCLIN3TX, RX_UART_TX_Isr, cpu0, 35, uart6);

		ISR_Install_preOS(&SRC_GPT120T2, Sample_Timer_Isr, cpu0, 31, 0);
		GPT12_StartStop(GPT12_T2,Start);
		//ISR_Install_preOS(&SRC_ASCLIN2ERR, UART_Err_Isr, cpu0, 33, 10);
		SYSTEM_EnableInterrupts();
		//CANopen_Init which will initialize CAN
		//if(CANopen_Init()!=RC_SUCCESS)
		//	DET_stop(AUTOCORE,CAN_INIT, 0);

		//Initialize core synchronization
		SYNC_Init();
		_nop();

	}



	//TLF DEVELOPMENT
	//For now, we stop here, no OS and no cores are started
	//The TFT Backlight is turned off. Port_cfg.cpp Port 20 Pin 13
	//test01();
//	while(1){
//		//test01();
//		test02();
//	}


	//Initialize PxROS
	switch(CoreID)
	{
		case cpu0:
			//Initialize PXROS on core0
			//PxInit_ret = PxInit(InitSpecsArray,3);
			break;

		case cpu1:
			//Initialize PXROS on core1
			//PxInit_ret = PxInit(InitSpecsArray,3);
			break;

		case cpu2:
			//Initialize PXROS on core2
			//PxInit_ret = PxInit(InitSpecsArray,3);
			break;
	}

	//in case PxROS was not initialized, the return value will be processed by DET
	if (PxInit_ret != PXERR_NOERROR)
		DET_stop(CoreID,PXBOOTSYNC1,PxInit_ret);

	//this line should be unreachable, but you never know...
	while(1)
		asm("NOP");

	//as the return type of main is int, we have to return something. The result is not evaluated.
	return 1;

}
