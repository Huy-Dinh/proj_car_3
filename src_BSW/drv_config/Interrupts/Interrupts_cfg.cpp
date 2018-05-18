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
extern void Sample_Timer_Isr(PxArg_t  inputArg);
extern void TX_UART_RX_Isr(PxArg_t  inputArg);
extern void TX_UART_TX_Isr(PxArg_t  inputArg);
extern void RX_UART_RX_Isr(PxArg_t  inputArg);
extern void RX_UART_TX_Isr(PxArg_t  inputArg);
extern void UART_Err_Isr(PxArg_t  inputArg);

 /*
  * 			SRC register
  * 				|				ISR function
  * 				|					|			   Core
  * 				|					|				|
  * 				|					|				|
  * 				|					|				|
  */
const INT_isrEntry_t INT_preOsTable[] = {
		{&SRC_GPT120T2, 		&Sample_Timer_Isr,	cpu0 }, // Timer priority 1
		{&SRC_ASCLIN2RX,		&TX_UART_RX_Isr,	cpu0 },	// TX UART's RX priority 2
		{&SRC_ASCLIN2TX,		&TX_UART_TX_Isr,	cpu0 },	// TX UART's TX riority 3
		{&SRC_ASCLIN3RX,		&RX_UART_RX_Isr,	cpu0 },	// RX UART's RX riority 4
		{&SRC_ASCLIN3TX,		&RX_UART_TX_Isr, 	cpu0 }	// TX UART's TX riority 5
};

const uint16_t INT_preOsTableSize = sizeof(INT_preOsTable) / sizeof(INT_isrEntry_t);
// Priority will be defined by the order of the entries
// Higher
//Note: Systick currently runs on prio 33 ==> see system configuration.c

#ifdef __cplusplus
  }
#endif /* __cplusplus */
