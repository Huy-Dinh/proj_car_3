/*
 * ENGINEDECODER_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef ENGINEDECODER_TYPE_H_
#define ENGINEDECODER_TYPE_H_

#include "global.h"




 /*****************************************************************************************
 *************** Type Definition for ENGINEDECODER  signal class **************************
 *****************************************************************************************/
//Data dictionary for the engine encoder position storage
typedef struct
{
	sint32_t engineFL_Pos;
	sint32_t engineFR_Pos;
	sint32_t engineRL_Pos;
	sint32_t engineRR_Pos;
}CODD_ENGINEDECODER_t;

typedef CODD_ENGINEDECODER_t* ENGINEDECODER_raw_t;

typedef struct
{
	float64_t delta_displacementFL; //displacement of the FL in centimeters
	float64_t delta_displacementFR; //displacement of the FR in centimeters
	float64_t delta_displacementRL; //displacement of the RL in centimeters
	float64_t delta_displacementRR; //displacement of the RR in centimeters
} ENGINEDECODER_data_t;


#define ENGINEDECODER_INIT_DATA ((ENGINEDECODER_data_t){0,0,0,0})
#define ENGINEDECODER_INIT_RAW  ((ENGINEDECODER_raw_t)&CODD_ENGINEDECODER_currentPosition_data)
#define ENGINEDECODER_INIT_CODD ((CODD_ENGINEDECODER_t){0,0,0,0})

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
RC_t ENGINEDECODER_scaleR2D( const ENGINEDECODER_raw_t * const praw, ENGINEDECODER_data_t * const pvalue);

/**
 * Scaler to translate engine control speed into CanOpen protocol
 * @param const ENGINE_data_t * const pvalue ==> INPUT: Pointer to the signal application data object
 * @param ENGINEDECODER_raw_t * const praw  ==> Output: Pointer to the signal rawdata object
  */
RC_t ENGINEDECODER_scaleD2R( const ENGINEDECODER_data_t * const pvalue, ENGINEDECODER_raw_t * const praw);

#ifdef  __cplusplus
}
#endif

#endif /* SWC_ENGINE_TYPE_H_ */

