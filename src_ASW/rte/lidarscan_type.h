/*
 * LIDARSCAN_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator
 */



#ifndef LIDARSCAN_TYPE_H_
#define LIDARSCAN_TYPE_H_

#include "global.h"

/*****************************************************************************************
 *************** Type Definition for LIDARSCAN  signal class **************************
 *****************************************************************************************/


typedef /* PointerToSDOArea */ uint8_t LIDARSCAN_raw_t;


typedef struct
{
	uint8_t data;
} LIDARSCAN_data_t;

#define LIDARSCAN_INIT_DATA ((LIDARSCAN_data_t){0})
#define LIDARSCAN_INIT_RAW  ((LIDARSCAN_raw_t)0)

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
RC_t LIDARSCAN_scaleR2D( const LIDARSCAN_raw_t * const praw, LIDARSCAN_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param LIDARSCAN_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t LIDARSCAN_scaleD2R( const LIDARSCAN_data_t * const pvalue, LIDARSCAN_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

