/**
 * \file	ITouchObserver.h
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief Touch Observer interface definition
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
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

#ifndef SRC_ASW_UI_ITOUCHOBSERVER_H_
#define SRC_ASW_UI_ITOUCHOBSERVER_H_

//-------------------------------------------------------------------- [Includes]

#include "IObserver.h"

namespace ui {

//-------------------------------------------------------------------- [Types]

/** \brief Touch Notification interface type*/
typedef INotification<CONIO_PIXEL_t> CTouchNotification;

/** \brief Touch Observer type*/
class ITouchObserver: public IObserver<CONIO_PIXEL_t>
{
public:
	/**
	 * \brief void notify(CTouchNotification& notification)
	 *
	 * touch notification interface to touch observer
	 *
	 * \param	notification : touch notification
	 * \return void
	 *
	 */
	virtual void notify(CTouchNotification& notification) = 0;
};

} /* namespace ui */

#endif /* SRC_ASW_UI_ITOUCHOBSERVER_H_ */
