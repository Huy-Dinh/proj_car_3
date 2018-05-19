/**
 * \author	Peter Fromm 	Hochschule Darmstadt
 * \date 	12.12.2016
 * \version	0.1
 *
 *
 * \copyright Copyright �201 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
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

#include "Interrupts.h"
#include "register.h"
#include "IfxSrc_reg.h"
#include "IfxAsclin_reg.h"
//#include "interrupt_init.h"
#include "IfxCpu_reg.h"
#include "IfxCpu_regdef.h"
#include <machine/intrinsics.h>

volatile uint64_t RxIsrCount = 0;
volatile uint64_t TxIsrCount = 0;
volatile uint64_t TmIsrCount = 0;

/*==========================================
 * UART Test functions, variables and typedefs
 *========================================*/
volatile uint32_t mismatchedPackets;
volatile uint32_t timedOutPackets;

#define BUFFER_SIZE 500
typedef enum
{
	TEST_RUN, TEST_RECV_PASSED, TEST_STOPPED
} testStatus_t;

typedef enum
{
	TEST_FAILED_TIME, TEST_FAILED_CORRUPT, TEST_PASS
} testResult_t;

volatile testStatus_t testStatus;

unsigned char sendBuffer[BUFFER_SIZE];
unsigned char receiveBuffer[BUFFER_SIZE];

volatile unsigned int sendIndex;
volatile unsigned int receiveIndex;
volatile int failedAt = 0;

int memcompare(char * a, char * b, int size);
testResult_t runOneTest();
void runSeveralTest(uint32_t numberOfTests, uint32_t* numberOfTimeFails, uint32_t* numberOfMismatch);
void resetTest();
void fillSendBuffer();

/*==========================================
 * Timer functions, ISR and counter
 *========================================*/
volatile uint64_t timerCounter;
void resetTimer();
uint64_t getTimer();
void Sample_Timer_Isr(PxArg_t inputArg);

/*==========================================
 * UART ISRs
 *========================================*/
void TX_UART_RX_Isr(PxArg_t  inputArg);
void TX_UART_TX_Isr(PxArg_t  inputArg);
void RX_UART_RX_Isr(PxArg_t  inputArg);
void RX_UART_TX_Isr(PxArg_t  inputArg);
void UART_Err_Isr(PxArg_t  inputArg);


int main()
{
	volatile uint32_t numberOfTests = 1000;

	// Storage for the result of PxInit
	PxError_t PxInit_ret = PXERR_NOERROR;

	//storage for the current CPU-ID
	CpuId_t CoreID = SYSTEM_GetCoreId();

	// Core 0 performs driver initialization
	if (CoreID == cpu0)
	{

		//GPIO Init
		PORT_Init();

		RTE_TIME_TimerInit();
		DET_TIME_init();

		//check integrity of system configuration
		if (CFG_check() != RC_SUCCESS)
			DET_stop(AUTOCORE, CFG_CHECK, 0);

		//system Init (PLL setup, DTS init and TLF shutdown)
		if (SYSTEM_Init() != RC_SUCCESS)
			DET_stop(AUTOCORE, SYSTEM_INIT, 0);

		//SPI Init
		if (RC_SUCCESS != QSPI_module_init(QSPI0, NULL))
			DET_stop(AUTOCORE, QSPI_MODULE_INIT, 0);

		if (RC_SUCCESS != QSPI_module_init(QSPI2, NULL))
			DET_stop(AUTOCORE, QSPI_MODULE_INIT, 0);

		UART_init();
		INT_preOsStart(INT_preOsTable, INT_preOsTableSize);

		GPT12_StartStop(GPT12_T2, Stop);
		fillSendBuffer();
		runSeveralTest(numberOfTests, &timedOutPackets, &mismatchedPackets);
		//Initialize core synchronization
		SYNC_Init();
		_nop();

	}


	//Initialize PxROS
	switch (CoreID)
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
		DET_stop(CoreID, PXBOOTSYNC1, PxInit_ret);

	//this line should be unreachable, but you never know...
	while (1)
		asm("NOP");

	//as the return type of main is int, we have to return something. The result is not evaluated.
	return 1;

}

void resetTimer()
{
	timerCounter = 0;
}
uint64_t getTimer()
{
	return timerCounter;
}
void Sample_Timer_Isr(PxArg_t inputArg)
{
	GPT12_Reload(GPT12_T2);
	++TmIsrCount;
	timerCounter++;
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

void TX_UART_RX_Isr(PxArg_t inputArg)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.RFLC = 1;
	uint8_t dummyReadByte;
	UART_ReadData(uart4, &dummyReadByte);
}
void TX_UART_TX_Isr(PxArg_t inputArg)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.TFLC = 1;
	++TxIsrCount;
}
void RX_UART_RX_Isr(PxArg_t inputArg)
{
	MODULE_ASCLIN3.FLAGSCLEAR.B.RFLC = 1;
	++RxIsrCount;
	if (UART_ReadData(uart6, &(receiveBuffer[receiveIndex])) == RC_SUCCESS)
	{
		resetTimer();
		if (receiveBuffer[receiveIndex] != sendBuffer[receiveIndex])
		{
			testStatus = TEST_STOPPED;
		}
		else
		if (receiveIndex < (BUFFER_SIZE - 1))
		{
			++receiveIndex;
		}
		else
		{
			testStatus = TEST_RECV_PASSED;
			receiveIndex = 0;
		}
	}
}
void RX_UART_TX_Isr(PxArg_t inputArg)
{
	MODULE_ASCLIN3.FLAGSCLEAR.B.TFLC = 1;
}
void UART_Err_Isr(PxArg_t inputArg)
{
	MODULE_ASCLIN2.FLAGSCLEAR.B.FEC = 1;
	MODULE_ASCLIN2.FLAGSCLEAR.B.RFUC = 1;
}

int memcompare(char * a, char * b, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		if (a[i] != b[i])
			return i;
	}
	return -1;
}
testResult_t runOneTest()
{
	resetTest();
	testStatus = TEST_RUN;
	GPT12_StartStop(GPT12_T2, Start);
	while (getTimer() < 20000)
	{
		if (testStatus == TEST_RECV_PASSED)
		{
			testStatus = TEST_STOPPED;
			failedAt = memcompare(receiveBuffer, sendBuffer, BUFFER_SIZE);
			GPT12_StartStop(GPT12_T2, Stop);
			return TEST_PASS;
		}
		else if (testStatus == TEST_STOPPED)
		{
			testStatus = TEST_STOPPED;
			GPT12_StartStop(GPT12_T2, Stop);
			return TEST_FAILED_CORRUPT;
		}
	}
	testStatus = TEST_STOPPED;
	GPT12_StartStop(GPT12_T2, Stop);
	return TEST_FAILED_TIME;
}
void runSeveralTest(uint32_t numberOfTests, uint32_t* numberOfTimeFails,
		uint32_t* numberOfMismatch)
{
	uint32_t i = 0;
	*numberOfMismatch = 0;
	*numberOfTimeFails = 0;
	testResult_t singleTestResult;
	for (i = 0; i < numberOfTests; i++)
	{
		singleTestResult = runOneTest();
		if (singleTestResult == TEST_FAILED_CORRUPT)
		{
			++(*numberOfMismatch);
		}
		else if (singleTestResult == TEST_FAILED_TIME)
		{
			++(*numberOfTimeFails);
		}
	}
	GPT12_StartStop(GPT12_T2, Stop);
}
void resetTest()
{
	testStatus = TEST_STOPPED;
	GPT12_StartStop(GPT12_T2, Stop);
	resetTimer();
	ASCLIN2_RXFIFOCON.B.FLUSH = 1;
	ASCLIN3_RXFIFOCON.B.FLUSH = 1;
	ASCLIN2_TXFIFOCON.B.FLUSH = 1;
	sendIndex = 0;
	receiveIndex = 0;
	memset(receiveBuffer, 0, BUFFER_SIZE);
}
void fillSendBuffer()
{
	unsigned int i = 0;
	for (i = 0; i < (BUFFER_SIZE - 1); i++)
	{
		sendBuffer[i] = i % 255;
	}
}

