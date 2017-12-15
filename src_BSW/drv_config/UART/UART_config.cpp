/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: UART_config.c
 * Desc: UART driver configuration for Infineon Tricore Aurix
 ******************************************************************************/

#include "UART_config.h"


uartConfig_t const UART_Config[UART_CHANNELS_NUM] =
{
/*		     BAUD RATE     Data length( 7 or 8)
 *				|			   |
 *				|			   | No. Of. Stop bits(1 or 2)
 *				|			   |	|
 *				|			   |	|
 *				|			   |	|
 *				|			   |	|	Parity type (Even or Odd)
 *				|			   |	|	(only if parity bit enabled)
 *				|			   |	|			|
 *				|			   |	|			|
 *				|			   |	|			|
 *				|			   |	|			|	Transmission FIFO Interrupt level (0-15)
 *				|			   |	|			|		 |
 *				|			   |	|			|		 |
 *				|			   |	|			|		 |
 *				|			   |	|			|		 |		  Receiver FIFO Interrupt level (1-16)
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		 		 |
 *				|			   |	|			|		 |		  		 |
*				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  	  	 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 *				|			   |	|			|		 |		  		 |
 */
/* UARTUSB*/{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1},
/* UART 1 */{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1},
/* UART 2 */{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1},
/* UART 3 */{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1},
/* UART 4 */{uart_115200	, __8, StopBit_1,  None, CsEnable, LowActive, TxInt_1	,   RxInt_1},
/* UART 5 */{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1},
/* UART 6 */{uart_115200	, __8, StopBit_1,  None, CsDisable, LowActive, TxInt_1	,   RxInt_1}
};


