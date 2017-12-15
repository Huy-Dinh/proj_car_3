/*
 * TARGETSPEED_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef TARGETSPEED_TYPE_H_
#define TARGETSPEED_TYPE_H_

#include "global.h"

/*****************************************************************************************
 *************** Type Definition for TARGETSPEED  signal class **************************
 *****************************************************************************************/



typedef struct
{
	sint8_t vx;
	sint8_t vy;
	sint8_t vphi;
} TARGETSPEED_data_t;

typedef uint32_t* TARGETSPEED_raw_t;

#define TARGETSPEED_INIT_DATA ((TARGETSPEED_data_t){0,0,0})
#define TARGETSPEED_INIT_RAW  ((TARGETSPEED_raw_t)&CODD_LIDAR_targetSpeed)

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
RC_t TARGETSPEED_scaleR2D( const TARGETSPEED_raw_t * const praw, TARGETSPEED_data_t * const pvalue);

/**
 * Scaler to translate target speed into message
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param TARGETSPEED_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t TARGETSPEED_scaleD2R( const TARGETSPEED_data_t * const pvalue, TARGETSPEED_raw_t * const praw);

/**
 * Scaler to translate targetspeed into CanOpen Signal for LIDAR
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param TARGETSPEED_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t TARGETSPEED_scaleD2R_Lidar( const TARGETSPEED_data_t * const pvalue, TARGETSPEED_raw_t * const praw);


#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

