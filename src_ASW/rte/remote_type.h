/**
* \file remote_type.h
* \author Remote Team 2016
* \date 17.08.2016
*
* \brief remote_type
*
* This file contains the declarations and scaler functions for the remote data type.
* A byte stream protocol is translated into a remote command structure and vice versa.
* Communication is typically done through a uart or zigbee port.
*
* \note Preliminary release, qualification pending
*
* \todo Error handling of the parser needs to be checked
* \todo Buffers for copying are probably not required
* \todo Magic numbers for positions should be replaced by defines
* \todo Protocol structure should be displayed in the header - please extend structure below
* \todo Handling of sender/Receiver id not implemented yet
*        - the remote id will be communicated to the car through the connection process, it is not hardcoded.
*        - Protocols are only processed if the receiver id is correct.
*        - The car id will be sent to the car and stored there while the communication is active
*
*
* \warning The scaler functions are only called through the remote_signal functions.
*
*  Changelog:\n
*  - 1.0; 17.08.2016; P. Fromm
*            - Transfer of the file into the integration space
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



#ifndef REMOTE_TYPE_H_
#define REMOTE_TYPE_H_

#include "global.h"
#include "CRingBuffer.h"

/*****************************************************************************************
 *************** Type Definition for REMOTE  signal class **************************
 *****************************************************************************************/

//Rawdata is pointer to instantiated ringbuffer containing the received data from the UART
typedef CRingBuffer<uint8_t, 100> REMOTE_zigbeeBuffer_t;
typedef REMOTE_zigbeeBuffer_t* REMOTE_raw_t;

//The following defines are valid for the application data representation of a protocol
//

//The length of an ACK record, length is defined as featureId + dataLength (0) + CRC
#define REMOTE_ACK_LENGTH		2
#define REMOTE_MAX_DATASIZE 	20

//Message Identifiers
#define REMOTE_MID_CONNECT		'C'
#define REMOTE_MID_FEATURE		'F'
#define REMOTE_MID_JOYSTICK		'J'



#define REMOTE_BROADCAST_ID		'*'
#define REMOTE_DISCONNECT_ID	-1
//Id of the control unit is transmitted through protocol and stored in signal layer

// Feature Id's
//Controlling the connection status
#define REMOTE_FID_ACK_PRESENT 		'A'
#define REMOTE_FID_ACK_CONNECT 		'B'
#define REMOTE_FID_ACK_DISCONNECT 	'E'
#define REMOTE_FID_CONNECT			'C'
#define REMOTE_FID_DISCONNECT		'D'
#define REMOTE_FID_PRESENT			'P'
#define REMOTE_FID_STOP				'S'
#define REMOTE_FID_RESET			'R'
#define REMOTE_FID_INFO  			'I'

//Feature calls
#define REMOTE_FID_REQUEST_FEATURES 0xFF

typedef struct
{
	uint8_t msgID;								//Message identifier
	uint8_t length;								//Length of protocol, defined as featureId (1) + data(0...REMOTE_MAX_DATASIZE) + CRC(1)
	uint8_t featureID;							//Feature identifier
	char featureData[REMOTE_MAX_DATASIZE];		//Buffer for additional protocol payload
} REMOTE_data_t;

#define REMOTE_INIT_DATA (REMOTE_data_t){0, 0, 0, {0}}
#define REMOTE_TX_INIT_RAW  ((REMOTE_raw_t)&UARTPROT_remoteTxBuffer)
#define REMOTE_RX_INIT_RAW  ((REMOTE_raw_t)&UARTPROT_remoteRxBuffer)



/*****************************************************************************************
 *************** Type specific scaler functions ******************************************
 *****************************************************************************************/
#ifdef  __cplusplus
extern "C"
{
#endif

/**
 * Scaler to translate bytestream protocol into ENGINE application data type
 * @param const ENGINE_raw_t * const praw  ==> INPUT: Pointer to the signal raw data object
 * @param const ENGINE_data_t * const pvalue ==> OUTPUT: Pointer to the signal application data object
 */
RC_t REMOTE_scaleR2D( const REMOTE_raw_t * const praw, REMOTE_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param REMOTE_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t REMOTE_scaleD2R( const REMOTE_data_t * const pvalue, REMOTE_raw_t * const praw);


#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

