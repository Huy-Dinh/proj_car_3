/*
 * CanOpen.h
 *
 *  Created on: 01.05.2016
 *      Author: Fromm
 */

#ifndef CANOPEN_H_
#define CANOPEN_H_

#include "global.h"
#include "signal_global.h"

#include "engine_type.h"
#include "lidarscan_type.h"
#include "enginedecoder_type.h"
#include "enginestate_type.h"
#include "lidarconfig_type.h"
#include "targetspeed_type.h"
#include "CANopen_Client.h"
#ifdef  __cplusplus
extern "C"
{
#endif


/**********************************************************************************
/** Engine (Faulhaber) Functions
 **********************************************************************************/
extern CODD_ENGINE_t CODD_ENGINE_data; //required for rawdata signal initialisation
extern CODD_ENGINEDECODER_t CODD_ENGINEDECODER_currentPosition_data;

RC_t CANOPEN_write_engineSpeedToFaulhaber( const ENGINE_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_engineStateFromFaulhaber( ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index);
RC_t CANOPEN_write_engineStateToFaulhaber( const ENGINESTATE_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_DecoderFromFaulhaber( ENGINEDECODER_raw_t * const  canbuffer, const SIG_IDX_TYPE index);


/**********************************************************************************
/** Lidar Functions
 **********************************************************************************/
extern uint32_t CODD_LIDAR_targetSpeed;		//Requested driving speed, required for rawdata signal initialisation
extern uint32_t CODD_LIDARCONFIG_data;		//Current configuration, required for rawdata signal initialisation

RC_t CANOPEN_write_ConfigToLidar( const LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index);
RC_t CANOPEN_read_ConfigFromLidar( LIDARCONFIG_raw_t * const canbuffer, const SIG_IDX_TYPE index);


RC_t CANOPEN_write_targetSpeedToLidar( const TARGETSPEED_raw_t * const canbuffer, const SIG_IDX_TYPE index);

RC_t CANOPEN_read_ScanFromLidar( LIDARSCAN_raw_t * const canbuffer, const SIG_IDX_TYPE index);

/**********************************************************************************
/** SmartPower Functions
 **********************************************************************************/
extern uint32_t CODD_SMARTPOWER_powerState;

/**
 * Request the power domain state from SmartPower Board
 */
RC_t CANOPEN_read_PowerStateFromSmartPower(uint32_t * const canbuffer, const SIG_IDX_TYPE index);


#ifdef  __cplusplus
}
#endif

#endif /* CANOPEN_H_ */

