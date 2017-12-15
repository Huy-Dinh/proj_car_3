/*
 * CARPOSITION_type.h
 *
 *  Created on: 01.05.2016
 *      Author: Autogenerated by H-DA RTE Generator, (c) Prof. Fromm, 2016
 */



#ifndef CARPOSITION_TYPE_H_
#define CARPOSITION_TYPE_H_

#include "global.h"

/*****************************************************************************************
 *************** Type Definition for CARPOSITION signal                     **************************
 *****************************************************************************************/



typedef struct
{
	float64_t xPos;		//x position in centimeter
	float64_t yPos;		//y position in centimeter
	float64_t theta;	//orientation angle with respect to initial heading in degree
} CARPOSITION_data_t;


#define CARPOSITION_INIT_DATA ((CARPOSITION_data_t){0,0,0}) //to be replace with your own data



#endif /* SWC_ENGINE_TYPE_H_ */

