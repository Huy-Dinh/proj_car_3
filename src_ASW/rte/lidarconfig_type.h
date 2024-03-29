/*
 * LIDARCONFIG_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef LIDARCONFIG_TYPE_H_
#define LIDARCONFIG_TYPE_H_

#include "global.h"


/*****************************************************************************************
 *************** Type Definition for LIDARCONFIG  signal class **************************
 *****************************************************************************************/



typedef struct
{
	uint8_t speed;				/**< \brief rotation speed of the Lidar sensor [%] , range 0..100 */
	uint8_t collissionAngle;	/**< \brief segment angle for the collission check [degree] , range 0..180, an angle of 90 means that a segment +/- 45 degree around the current driving direction will be checked */
	uint16_t resolution;		/**< \brief number of samples per scan [samples / 360deg] range 1..360, 360 % resolution must be 0 */


} LIDARCONFIG_data_t;

typedef uint32_t* LIDARCONFIG_raw_t;


#define LIDARCONFIG_INIT_DATA ((LIDARCONFIG_data_t){0})
#define LIDARCONFIG_INIT_RAW  ((LIDARCONFIG_raw_t)&CODD_LIDARCONFIG_data)

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
RC_t LIDARCONFIG_scaleR2D( const LIDARCONFIG_raw_t * const praw, LIDARCONFIG_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param LIDARCONFIG_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t LIDARCONFIG_scaleD2R( const LIDARCONFIG_data_t * const pvalue, LIDARCONFIG_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

