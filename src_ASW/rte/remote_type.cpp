/**
* \file remote_type.h
* \author Remote Team 2016
* \date 17.08.2016
*
* \brief remote_type
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

#include "remote_type.h"
#include "remote_signal.h"
#include "sint32_ir_signal.h"
#include "signal_bb.h"
#include "carconfig.h"

/******************************************************************************
 * Protocol RAWDATA defines
 ******************************************************************************/

// Protocol structure
// MessageId | SenderId | ReceiverId | Length | FeatureId | Data[] | CRC| EOP

//Length and position

//MID(1) + Sender(1) + Receiver(1) + Length (1) + EOP(2) = 6
//+ length (FeatureId + Data + CRC) = physical size of protocol
#define REMOTE_PROT_LENGTH_FRAME 	6
#define REMOTE_PROT_LENGTH_CRC	 	1
#define REMOTE_PROT_LENGTH_MAX REMOTE_PROT_LENGTH_FRAME + REMOTE_MAX_DATASIZE + REMOTE_ACK_LENGTH

#define REMOTE_PROT_POS_MID			0
#define REMOTE_PROT_POS_SENDER		1
#define REMOTE_PROT_POS_RECEIVER	2
#define REMOTE_PROT_POS_LENGTH 		3
#define REMOTE_PROT_POS_FID 		4
#define REMOTE_PROT_POS_DATA 		5

#define REMOTE_PROT_OFFSET_CRC 		3


extern "C" {

/******************************************************************************
 * Static Declarations
 ******************************************************************************/
static uint8_t REMOTE__computeCRC(uint8_t* data, uint8_t len);




/******************************************************************************
 * Function Implementations
 ******************************************************************************/


RC_t REMOTE_scaleR2D(const REMOTE_raw_t * const praw,
		REMOTE_data_t * const pvalue) {

	boolean_t read_status = false;
	uint16_t prot_len = REMOTE_PROT_LENGTH_MAX;	//max size of the buffer, required for readFullProtocol

	uint8_t uart_prot_Rxbuff[REMOTE_PROT_LENGTH_MAX];

	uint8_t temp_calcCRC = 0;

	//prot_len contains real physical length of protocol after call
	read_status = (*praw)->readFullProtocol(uart_prot_Rxbuff, &prot_len);


	if (read_status == false)
	{
		REMOTE_set_status_invalid(&so_in_remote);
		return RC_ERROR_READ_FAILS;
	}

	if (prot_len > REMOTE_PROT_LENGTH_MAX)
	{
		//This seems to be a configuration error of the protocol definition.
		//Should not happen, as this error is handled in readFullProtocol
		DET_stop(AUTOCORE, APPL_WRONGPROTOCOLSIZE, prot_len);
	}

	prot_len = uart_prot_Rxbuff[REMOTE_PROT_POS_LENGTH];


	//Perform checks for correct protocol content

	//Check for correct sender / receiverId or broadcast
	if (!( (uart_prot_Rxbuff[REMOTE_PROT_POS_RECEIVER] == REMOTE_BROADCAST_ID) ||
		   ( (uart_prot_Rxbuff[REMOTE_PROT_POS_RECEIVER] == CONFIG_REMOTE_CAR_ID) && (uart_prot_Rxbuff[REMOTE_PROT_POS_SENDER] == SINT32_IR_get(&so_remoteId) )) ) )
	{
		REMOTE_set_status_invalid(&so_in_remote);
		return RC_ERROR_READ_FAILS;
	}

	//Check CRC
	temp_calcCRC = REMOTE__computeCRC(&(uart_prot_Rxbuff[REMOTE_PROT_POS_FID]), prot_len - 1);
	if (temp_calcCRC != uart_prot_Rxbuff[prot_len + REMOTE_PROT_OFFSET_CRC])
	{
		REMOTE_set_status_invalid(&so_in_remote);
		return RC_ERROR_READ_FAILS;
	}

	//Set sender id in case of broadcast
	if (uart_prot_Rxbuff[REMOTE_PROT_POS_RECEIVER] == REMOTE_BROADCAST_ID && SINT32_IR_get(&so_remoteId) == REMOTE_DISCONNECT_ID)
	{
		SINT32_IR_set(&so_remoteId, uart_prot_Rxbuff[REMOTE_PROT_POS_SENDER]);
	}

	//Everything ok
	pvalue->msgID 		= uart_prot_Rxbuff[REMOTE_PROT_POS_MID];
	pvalue->length 		= uart_prot_Rxbuff[REMOTE_PROT_POS_LENGTH];
	pvalue->featureID 	= uart_prot_Rxbuff[REMOTE_PROT_POS_FID];

	//Copy the data without FID and CRC
	for (int i = 0; i < (prot_len - REMOTE_ACK_LENGTH); i++)
	{
		pvalue->featureData[i] = uart_prot_Rxbuff[i + REMOTE_PROT_POS_DATA];
	}

	REMOTE_set_status(&so_in_remote, SIG_STS_VALID);


	return RC_SUCCESS;
}

RC_t REMOTE_scaleD2R(const REMOTE_data_t * const pvalue, REMOTE_raw_t * const praw) {
	//ID|SENDER_ID|RECEIVER_ID|LENGTH|FEATURE_ID|FEATURE_DATA|CRC|OD|OE


	// Get the total physical protocol length
	uint8_t prot_length = REMOTE_PROT_LENGTH_FRAME + pvalue->length ;

	if (prot_length > REMOTE_PROT_LENGTH_MAX)
	{
		//This seems to be a configuration error of the protocol definition.
		DET_stop(AUTOCORE, APPL_WRONGPROTOCOLSIZE, prot_length);
	}


	// Create buffer of maximum protocol length
	uint8_t uart_prot_Txbuf[REMOTE_PROT_LENGTH_MAX];



	//Store 1st 5 protocol parameters
	uart_prot_Txbuf[0] = pvalue->msgID;
	uart_prot_Txbuf[1] = CONFIG_REMOTE_CAR_ID;
	uart_prot_Txbuf[2] = SINT32_IR_get(&so_remoteId);
	uart_prot_Txbuf[3] = pvalue->length;
	uart_prot_Txbuf[4] = pvalue->featureID;

	//Store payload data in buffer
	uint8_t index = REMOTE_PROT_POS_DATA;	//Start with first payload position
	for (int i = 0; i < pvalue->length - REMOTE_ACK_LENGTH; i++) {
		uart_prot_Txbuf[index++] = pvalue->featureData[i];
	}

	//Compute CRC for protocol parameters + data
	//CRC is calculated without frame and CRC
	uart_prot_Txbuf[index++] = REMOTE__computeCRC(&(uart_prot_Txbuf[REMOTE_PROT_POS_FID]), prot_length - REMOTE_PROT_LENGTH_FRAME - REMOTE_PROT_LENGTH_CRC);

	//EOP
	uart_prot_Txbuf[index++] = 0x0D;
	uart_prot_Txbuf[index]   = 0x0E;

	//Write full protocol in ring buffer
	bool write_status = (*praw)->writeFullProtocol(uart_prot_Txbuf, prot_length);

	//ToDo: set the validity and invalidity of the so_out_remote signal. Find out the criterias to do so

	if (write_status == true) {
		return RC_SUCCESS;
	} else {
		return RC_ERROR_WRITE_FAILS;
	}

}

static uint8_t REMOTE__computeCRC(uint8_t* data, uint8_t len) {

	uint8_t tempcrc = 0xff;

	for (int itr = 0; itr < len; itr++) //Changed start and end of loop to exclude preamble and crc (written in previous code)
			{
		tempcrc = (uint8_t) (tempcrc ^ data[itr]);
	}

	return tempcrc;
}

} //extern  "C"
