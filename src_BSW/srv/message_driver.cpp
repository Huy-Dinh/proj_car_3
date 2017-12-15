/**
 * \file 	message_driver.cpp
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	18.10.2016
 *
 * \brief DEFAULT SOURCE FILE TEMPLATE
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

#include "global.h"
#include "pxdef.h"
#include "det.h"

#include "remote_type.h"
#include "remote_signal.h"

#include "remotemsg_type.h"
#include "remotemsg_signal.h"

#include "message_driver.h"

#include "Task_C0_Communication.h"



RC_t MSG_write_remoteToCom(const REMOTE_data_t * const msg, const SIG_IDX_TYPE index)
{
	//Message metadata, used to identify the type
	PxMsgMetadata_t msgmetadata = MSGMETADATACONTENT_REMOTE;

	//Get the handler to the mailbox
	//Should be optimized
	PxMbx_t Task_C0_Communication_Mbx = PxTaskGetMbx(Task_C0_Communication_getTaskId());

	//Create message
	//Needs to be done inside the loop, because handle is returned to the OS, not the task
	PxMsg_t pxmsg = PxMsgRequest(sizeof(REMOTE_data_t), PXMcTaskdefault, PXOpoolTaskdefault);


	//Send message
	if (PxMsgIdIsValid(pxmsg))
	{
		//Set the metadata of the message
		PxMsgSetMetadata(pxmsg, msgmetadata);

		//Get pointer to data area of the message
		REMOTE_data_t* dataptr =  (REMOTE_data_t*) PxMsgGetData(pxmsg);

		//Copy data into the message
		*dataptr = *msg;

		pxmsg = PxMsgSend(pxmsg, Task_C0_Communication_Mbx);
	}
	else
	{
		DET_stop(AUTOCORE, MSG_ERROR, index);
	}
	return RC_SUCCESS;
}
