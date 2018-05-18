/*
 * InterruptRouter_cfg.cpp
 *
 *  Created on: 05-Jun-2016
 *      Author: Gunaseelan
 */

//Todo: Cleanup comments


#include "Interrupts_cfg.h"
//#include "Task_C0_Communication.h"
#include "register.h"
#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

  /*
   * SRC registers
   * ================ UART & ASCLIN SRC register===========
   * ===================RX - REGISTERS====================
   * SRC_ASCLIN0RX		- UARTusb RX
   * SRC_ASCLIN1RX		- UART1RX or UART2RX or UART3RX
   * SRC_ASCLIN2RX		- UART4RX or UART5Rx
   * SRC_ASCLIN3RX		- UART6RX
   * ===================TX- REGISTERS=====================
   * SRC_ASCLIN0TX		- UARTusb TX
   * SRC_ASCLIN1TX		- UART1TX or UART2TX or UART3TX
   * SRC_ASCLIN2TX		- UART4TX or UART5Tx
   * SRC_ASCLIN3TX		- UART6TX
   * ===================ERROR REGISTERS====================
   * SRC_ASCLINERR		- UARTusb ERR
   * SRC_ASCLIN1ERR		- UART1ERR or UART2ERR or UART3ERR
   * SRC_ASCLIN2ERR		- UART4ERR or UART5ERR
   * SRC_ASCLIN3ERR		- UART6TERR
   * =======================================================
   * ===================== CAN SRC REGISTER=================
   * NOTE: CAN MODULE HAS 16 INTERRUPT REGISTERS AND 4 CAN NODES
   * 		FOR EACH NODE 4 INT REGISTERS IS ALLOCATED
   * ===================CAN NODE 0 SRC REGISTER===========================
   * SRC_CANINT0		CAN0 - TX
   * SRC_CANINT1		CANO - RX
   * SRC_CANINT2		CAN0 - ERR
   * SRC_CANINT3		CANO - RESERVED
   * ===================CAN NODE 1 SRC REGISTER===========================
   * SRC_CANINT4		CAN1 - TX
   * SRC_CANINT5		CAN1 - RX
   * SRC_CANINT6		CAN1 - ERR
   * SRC_CANINT7		CAN1 - RESERVED
   * ===================CAN NODE 2 SRC REGISTER===========================
   * SRC_CANINT8		CAN2 - TX
   * SRC_CANINT9		CAN2 - RX
   * SRC_CANINT10		CAN2 - ERR
   * SRC_CANINT11		CAN2 - RESERVED
  * ===================CAN NODE 3 SRC REGISTER===========================
   * SRC_CANINT12		CAN3 - TX
   * SRC_CANINT13		CAN3 - RX
   * SRC_CANINT14		CAN3 - ERR
   * SRC_CANINT15		CAN3 - RESERVED
   */
  /*
   * =========================RULES TO BE FOLLOWED======================
   * 1. DO NOT CONFIGURE MULTIPLE INTERRUPTS WITH SAME PRIORITY FOR SINGLE CORE
   * 2. CONFIGURE THE TABLE IN ASCENDING ORDER TO AVOID CONFUSIONS
   * 3. CONFIGURE THE TABLE IN ORDER OF CORES TO AVOID CONFUSION
   * 4. All ISR handlers must be declared with "extern "C" linkage"
   */
  extern void UART_ASCLIN0_RxISR(PxArg_t arg); 	//RteTiming command
  extern void RTE_TIMER_T3_ISR(PxArg_t arg); 	//RteTimerTick
  extern void DET_TIMER_T2_ISR(PxArg_t arg); 	//DetTimerTick

  extern void UART_ASCLIN1_RxISR(PxArg_t arg); 	//Remote command
  extern void UART_ASCLIN1_TxISR(PxArg_t arg); 	//Remote command

  extern void UART_ASCLIN2_RxISR(PxArg_t arg); 	//Lidar collission and Heartbeat

  extern void CAN_RxISR(PxArg_t arg);			//CAN bus
  extern void CAN_TxISR(PxArg_t arg);			//CAN bus

  extern void QSPI0_txISR(PxArg_t arg);
  extern void QSPI0_ptISR(PxArg_t arg);
  extern void QSPI0_errorISR(PxArg_t arg);

 /*
  * 			SRC register
  * 				|				ISR function
  * 				|					|			   Core
  * 				|					|				|
  * 				|					|				|
  * 				|					|				|
  */
const INT_isrEntry_t INT_preOsTable[] = {
		{&SRC_ASCLIN0RX, 		&UART_ASCLIN0_RxISR,	cpu0 }, // will be priority 1
		{&SRC_ASCLIN1RX,		&UART_ASCLIN1_RxISR,	cpu0 },	// priority 2
		{&SRC_ASCLIN1TX,		&UART_ASCLIN1_TxISR,	cpu0,		35},	// Remote / Zigbee
		{&SRC_ASCLIN2RX,		&UART_ASCLIN2_RxISR,	cpu0,		34},	// Collission Signal from LIDAR
		{&SRC_CANINT1,			&CAN_RxISR, 			cpu0,		40},	// CAN RX
		{&SRC_CANINT0,			&CAN_TxISR, 			cpu0,		39},	// CAN TX
		{&SRC_GPT120T3, 		&RTE_TIMER_T3_ISR, 		cpu0,	    60},	//Needs high priority due to exact time measurement
		{&SRC_GPT120T2,			&DET_TIMER_T2_ISR, 		cpu0,       59},
		{&SRC_QSPI0TX,          &QSPI0_txISR,           cpu2,       32},
		{&SRC_QSPI0PT,          &QSPI0_ptISR,           cpu2,       31},
		{&SRC_QSPI0ERR,         &QSPI0_errorISR,        cpu2,       30},
		{0,0,(CpuId_t)0,0}
};

const uint16_t INT_preOsTableSize = sizeof(INT_preOsTable) / sizeof(INT_isrEntry_t);
// Priority will be defined by the order of the entries
// Higher
//Note: Systick currently runs on prio 33 ==> see system configuration.c

#ifdef __cplusplus
  }
#endif /* __cplusplus */
