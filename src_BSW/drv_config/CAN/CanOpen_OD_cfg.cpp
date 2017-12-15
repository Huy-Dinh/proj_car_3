/*
 * CanOpen.cpp
 *
 *  Created on: 01.05.2016
 *      Author: Fromm
 */

#include "CanOpen_OD_cfg.h"
#include "CANopen_Client.h"

#include "enginedecoder_type.h"
#include "enginestate_type.h"
#include "engine_type.h"

#include "det.h"

extern "C" {

/**********************************************************************************************************************************************
/** Node Id's,....
 **********************************************************************************************************************************************/

/*
#define NODE_ID_FL 					1
#define NODE_ID_FR 					2
#define NODE_ID_RR 					3
#define NODE_ID_RL 					4
*/


//Modified identifiers due to car kinematics
//Node ID for Aurix is set in CANopen_config.h
#define NODE_ID_FL 					3				/**< \brief Node ID for front left engine */
#define NODE_ID_FR 					4				/**< \brief Node ID for front right engine */
#define NODE_ID_RR 					1				/**< \brief Node ID for rear right engine */
#define NODE_ID_RL 					2				/**< \brief Node ID for rear left engine */
#define NODE_ID_LIDAR 				5				/**< \brief Node ID for Lidar Board */
#define NODE_ID_SMARTPOWER 			7				/**< \brief Node ID for SmartPower Board */

#define INDEX_ENGINE_SETSPEED		0x60FF
#define INDEX_ENGINE_GETDECODER		0x6064
#define INDEX_ENGINE_GETSTATE		0x6041
#define INDEX_ENGINE_SETSTATE		0x6040

#define INDEX_LIDAR_SETCONFIG 		0x133D
#define INDEX_LIDAR_SETTARGETSPEED 	0x133C

#define INDEX_SMARTPOWER_POWSTATE        0x1100                  /**< \brief CANopen OD Index for converter enable PIN*/
#define INDEX_SMARTPOWER_CW_IDX          0x1101                  /**< \brief CANopen OD Index for converter warnings*/
#define INDEX_SMARTPOWER_CF_IDX          0x1102                  /**< \brief CANopen OD Index for converter faults*/
#define INDEX_SMARTPOWER_CV_IDX          0x1103                  /**< \brief CANopen OD Index for the converter voltage, converter addressing via s-index*/
#define INDEX_SMARTPOWER_CC_IDX          0x1104                  /**< \brief CANopen OD Index for the converter current, converter addressing via s-index*/
#define INDEX_SMARTPOWER_SC_IDX          0x1105                  /**< \brief CANopen OD Index for the converter state, converter addressing via s-index*/


#define INDEX_SMARTPOWER_OVWT_IDX        0x1110                  /**< \brief CANopen OD Index for converter OverVoltage warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_UVWT_IDX        0x1111                  /**< \brief CANopen OD Index for converter UnderVoltage warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OCWT_IDX        0x1112                  /**< \brief CANopen OD Index for converter OverCurrent warning threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OVFT_IDX        0x1113                  /**< \brief CANopen OD Index for converter OverVoltage fault threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_UVFT_IDX        0x1114                  /**< \brief CANopen OD Index for converter UnderVoltage fault threshold, converter addressing via s-index*/
#define INDEX_SMARTPOWER_OCFT_IDX        0x1115                  /**< \brief CANopen OD Index for converter OverCurrent fault threshold, converter addressing via s-index*/

#define INDEX_SMARTPOWER_NODE_IDX        0x1120                  /**< \brief CANopen OD Index for Node-ID*/

#define INDEX_SMARTPOWER_EMSWD_IDX       0x1200                  /**< \brief CANopen OD Index for Watchdog EMS control. Requires password*/
#define INDEX_SMARTPOWER_EMS2_IDX        0x1201                  /**< \brief CANopen OD Index for EMS 2 control. Requires password*/
#define INDEX_SMARTPOWER_EMS2T_IDX       0x1202                  /**< \brief CANopen OD Index for EMS 2 reboot delay*/

#define INDEX_SMARTPOWER_PMCMD_IDX       0x1300                  /**< \brief CANopen OD Index for Power Management Commands*/

#define INDEX_SMARTPOWER_NMTSHM_IDX      0x1400                  /**< \brief CANopen OD Index for System Health monitoring NMT state, Subindex NodeId, State is defined as enum in CANOpen.h */


#define TIMEOUT_ENGINESDO 			50
#define TIMEOUT_LIDARSDO 			1000
#define TIMEOUT_SMARTPOWER_SDO		50

/**********************************************************************************************************************************************
/** Data dictionary objects
 **********************************************************************************************************************************************/

/**
 * Add all data dictionary objects into this section
 * Naming Convention:
 * 	Type: CODD_<your signal><_optionalDescription>_t
 * 	Name: CODD_<your signal><_optionalDescription>_data
 * 	The type is declared in the corresponding <signal>_type.h file
 * 	The raw data of the signal is a pointer to this data structure
 * 	All CanOpen Objects are sored in the section .com.canopen.datadictionary
 */


#pragma section ".com.canopen.datadictionary.safety"


//Engine NMT, state and speed objects
CODD_ENGINESTATE_t CODD_ENGINESTATE_data = ENGINESTATE_INIT_CODD_ENGINESTATE ;
//CODD_ENGINESTATE_nmtStateAllEngines_t CODD_ENGINESTATE_nmtStateAllEngines_data = ENGINESTATE_INIT_CODD_NMT;
CODD_ENGINE_t CODD_ENGINE_data = ENGINE_INIT_CODD;


//Lidar data object
uint32_t CODD_LIDAR_targetSpeed = 0;		//Requested driving speed
uint32_t CODD_LIDARCONFIG_data = 0;			//Current configuration

//Decoder data objects
CODD_ENGINEDECODER_t CODD_ENGINEDECODER_currentPosition_data = ENGINEDECODER_INIT_CODD;
CODD_ENGINEDECODER_t CODD_ENGINEDECODER_previousPosition_data = ENGINEDECODER_INIT_CODD;

uint32_t CODD_SMARTPOWER_powerState = 0;													/**< \brief Bitpattern holding the state of the individual power domains */


#pragma section



/**********************************************************************************************************************************************
/** Engine (Faulhaber) Functions
 **********************************************************************************************************************************************/


RC_t CANOPEN_write_engineSpeedToFaulhaber(const ENGINE_raw_t * const canbuffer,
		const SIG_IDX_TYPE index) {

	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;


	//Engine front left
	status_FL = CANopen_setSDO(0, &((*canbuffer)->wheelspeedFL), sizeof(sint32_t), NODE_ID_FL, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FL_SDO_SETSPEED,status_FL);
	}

	//Engine front right
	status_FR = CANopen_setSDO(0, &((*canbuffer)->wheelspeedFR), sizeof(sint32_t), NODE_ID_FR, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FR_SDO_SETSPEED,status_FR);
	}

	//Engine rear right
	status_RR = CANopen_setSDO(0, &((*canbuffer)->wheelspeedRR), sizeof(sint32_t), NODE_ID_RR, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RR_SDO_SETSPEED,status_RR);
	}

	//Engine rear left
	status_RL = CANopen_setSDO(0, &((*canbuffer)->wheelspeedRL), sizeof(sint32_t), NODE_ID_RL, INDEX_ENGINE_SETSPEED, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RL_SDO_SETSPEED,status_RL);
	}


	//Return logical AND result of all four engines
	return result;

}



RC_t CANOPEN_read_engineStateFromFaulhaber(ENGINESTATE_raw_t * const canbuffer,
		const SIG_IDX_TYPE index) {
	//read the NMT state from the global variable set from ISR for each engine
	//@PF guess this is not working at all
	//Hier muss das Heartbeat Protokoll eingesetzt werden




	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;


	//engine front left
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetEngineState)
	{
		status_FL = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineFL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FL, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FL_SDO_GETSTATE,status_FL);
		}
	}


	//engine front right
	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetEngineState)
	{
		status_FR = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineFR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FR, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FR_SDO_GETSTATE,status_FR);
		}
	}


	//engine rear right
	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetEngineState)
	{
		status_RR = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineRR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RR, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RR_SDO_GETSTATE,status_RR);
		}
	}

	//engine rear left
	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetEngineState)
	{
		status_RL = CANopen_getSDO(0, &((CODD_ENGINESTATE_data.enfineRL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RL, INDEX_ENGINE_GETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RL_SDO_GETSTATE,status_RL);
		}
	}

	//Return logical AND of all engine error codes
	return result;
}

RC_t CANOPEN_write_engineStateToFaulhaber(const ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index) {

	//set the NMT state for each engine
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineFL_rawState).rawNmtState, NODE_ID_FL);
	}

	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineFR_rawState).rawNmtState, NODE_ID_FR);
	}

	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineRR_rawState).rawNmtState, NODE_ID_RR);
	}

	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetNmt)
	{
		CANopen_sendNMTcmd(((*canbuffer)->enfineRL_rawState).rawNmtState, NODE_ID_RL);
	}



	/*
	if (((*canbuffer)->enfineFL_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineFR_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineRL_rawState).rawNmtState != 0x05
			|| ((*canbuffer)->enfineRR_rawState).rawNmtState != 0x05) {
		CANopen_sendNMTcmd(CANopen_NMT_CMD_START_NODE, 0x00);
	}
	*/


	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;

	//engine front left
	if (((*canbuffer)->enfineFL_rawState).command == CMD_GetSetEngineState)
	{
		status_FL = CANopen_setSDO(0, &(((*canbuffer)->enfineFL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FL, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FL_SDO_SETSTATE, status_FL);
		}
	}

	//engine front right
	if (((*canbuffer)->enfineFR_rawState).command == CMD_GetSetEngineState)
	{
		status_FR = CANopen_setSDO(0, &(((*canbuffer)->enfineFR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_FR, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_FR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_FR_SDO_SETSTATE, status_FR);
		}
	}

	//engine rear right
	if (((*canbuffer)->enfineRR_rawState).command == CMD_GetSetEngineState)
	{
		status_RR = CANopen_setSDO(0, &(((*canbuffer)->enfineRR_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RR, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RR != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RR_SDO_SETSTATE, status_RR);
		}
	}

	//engine rear left
	if (((*canbuffer)->enfineRL_rawState).command == CMD_GetSetEngineState)
	{
		status_RL = CANopen_setSDO(0, &(((*canbuffer)->enfineRL_rawState).rawDriveState), sizeof(uint16_t), NODE_ID_RL, INDEX_ENGINE_SETSTATE, 0, TIMEOUT_ENGINESDO, 0);

		//Error handling
		if (status_RL != CANOPEN_SDO_SUCCESS)
		{
			result = RC_ERROR_WRITE_FAILS;
			DET_stop(AUTOCORE, ENGINE_RL_SDO_SETSTATE, status_RL);
		}
	}

	//Return logical and of all error codes
	return result;
}

RC_t CANOPEN_read_DecoderFromFaulhaber(ENGINEDECODER_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status_FL;
	CANopen_SDO_transfer_RET_t status_FR;
	CANopen_SDO_transfer_RET_t status_RR;
	CANopen_SDO_transfer_RET_t status_RL;

	RC_t result = RC_SUCCESS;


	//Engine front left
	status_FL = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineFL_Pos), sizeof(sint32_t), NODE_ID_FL, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FL_SDO_GETSPEED, status_FL);
	}


	//Engine front right
	status_FR = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineFR_Pos), sizeof(sint32_t), NODE_ID_FR, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_FR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_FR_SDO_GETSPEED, status_FR);
	}

	//Engine rear right
	status_RR = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineRR_Pos), sizeof(sint32_t), NODE_ID_RR, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RR != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RR_SDO_GETSPEED, status_RR);
	}

	//Engine rear left
	status_RL = CANopen_getSDO(0, &(CODD_ENGINEDECODER_currentPosition_data.engineRL_Pos), sizeof(sint32_t), NODE_ID_RL, INDEX_ENGINE_GETDECODER, 0, TIMEOUT_ENGINESDO, 0);
	if (status_RL != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_WRITE_FAILS;
		DET_stop(AUTOCORE, ENGINE_RL_SDO_GETSPEED, status_RL);
	}

	//Return logical and of all error codes
	return result;
}


/**********************************************************************************************************************************************
/** Lidar Functions
 **********************************************************************************************************************************************/


RC_t CANOPEN_write_ConfigToLidar( const LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status = CANopen_setSDO(0, &(CODD_LIDARCONFIG_data), sizeof(uint32_t), NODE_ID_LIDAR, INDEX_LIDAR_SETCONFIG, 0, TIMEOUT_LIDARSDO, 0);

	if (status != CANOPEN_SDO_SUCCESS)
	{
		DET_stop(AUTOCORE, LIDAR_SDO_SETCONFIG, status);
		return RC_ERROR_WRITE_FAILS;
	}

	return RC_SUCCESS;
}

RC_t CANOPEN_read_ConfigFromLidar( LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{
	return RC_SUCCESS;
}

RC_t CANOPEN_write_targetSpeedToLidar( const TARGETSPEED_raw_t * const canbuffer, const SIG_IDX_TYPE index)
{

	CANopen_SDO_transfer_RET_t status = CANopen_setSDO(0, &(CODD_LIDAR_targetSpeed), sizeof(uint32_t), NODE_ID_LIDAR, INDEX_LIDAR_SETTARGETSPEED, 0, TIMEOUT_LIDARSDO, 0);

	if (status != CANOPEN_SDO_SUCCESS)
	{
		DET_stop(AUTOCORE, LIDAR_SDO_SETTARGETSPEED, status);
		return RC_ERROR_WRITE_FAILS;
	}

	return RC_SUCCESS;

}

RC_t CANOPEN_read_ScanFromLidar(LIDARSCAN_raw_t * const canbuffer, const SIG_IDX_TYPE index) {

	//Request a data update, in a first step in a blocking mode

	//CANopen_getsetSDO(PointerToOurScanDataArea)

	return RC_SUCCESS;
}


/**********************************************************************************************************************************************
/** SmartPower Functions
 **********************************************************************************************************************************************/

RC_t CANOPEN_read_PowerStateFromSmartPower(uint32_t * const canbuffer, const SIG_IDX_TYPE index)
{
	RC_t result = RC_SUCCESS;


	CANopen_SDO_transfer_RET_t status = CANopen_getSDO(0, canbuffer, sizeof(uint32_t), NODE_ID_SMARTPOWER, INDEX_SMARTPOWER_POWSTATE, 0, TIMEOUT_SMARTPOWER_SDO, 0);
	if (status != CANOPEN_SDO_SUCCESS)
	{
		result = RC_ERROR_READ_FAILS;
		DET_stop(AUTOCORE, SMARTPOWER_SDO_GETPOWERSTATE, status);
	}

	return result;
}

} //extern c

