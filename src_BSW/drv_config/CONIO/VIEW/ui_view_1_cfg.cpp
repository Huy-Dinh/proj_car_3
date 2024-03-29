/**
 * \file	ui_view_1_cfg.cpp
 * \author	Rahul Raj 	Hochschule Darmstadt - rahul.r.rajan@stud.h-da.de
 * \date	14.05.2017
 * \version	0.1
 *
 * \brief UI view1 configuration
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

//-------------------------------------------------------------------- [Includes]

#include "ui_view_1_cfg.h"

#include "VIEW/ui_view_cfg.h"

#include "swc_ui.h"
#include "BASIC/conio_button.h"

#include <cstring>
#include <cstdlib>

//-------------------------------------------------------------------- [Defines]

#define UI_NORMAL_STATE_KEY_STR 		"NS :"
#define UI_SAFETY_STATE_KEY_STR 		"SS :"
#define UI_SAFETY_RUNNABLE_STR			"RUN:"
#define UI_JOYSTICK_KEY_STR 			"JS :"
#define UI_TARGET_SPEED_KEY_STR 		"TS :"
#define UI_ENGINE_KEY_STR 				"ENG:"

#define UI_NORMAL_STATE_VAL_WIDTH		(16) //7 + 2'()' + 2 + 1

#define UI_SAFETY_STATE_VAL_WIDTH		(16) //7 + 2'()' + 2 + 1

//Must be aligned with string size in rte
#define UI_SAFETY_RUNNABLE_VAL_WIDTH	(20) //7 + 2'()' + 2 + 1


#define UI_JOYSTICK_LINE				(6)		/** Line position on the display */
#define UI_JOYSTICK_X_WIDTH				(4)		/** -127 to 128 **/
#define UI_JOYSTICK_Y_WIDTH				(4)		/** -127 to 128 **/
#define UI_JOYSTICK_RZ_WIDTH			(4)		/** -127 to 128 **/
#define UI_JOYSTICK_AGE_WIDTH			(5)		/** 0 to 65535 **/
#define UI_JOYSTICK_ESTOP_WIDTH			(4)		/** Y/N - only 1 required,  4 for beauty**/
#define UI_JOYSTICK_GSTOP_WIDTH			(4)		/** Y/N - only 1 required,  4 for beauty**/

#define UI_TARGET_SPEED_LINE			(5)		/** Line position on the display */
#define UI_TARGET_SPEED_X_WIDTH			(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_Y_WIDTH			(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_PHI_WIDTH		(4)		/** -127 to 128 **/
#define UI_TARGET_SPEED_AGE_WIDTH		(5)		/** 0 to 65535 **/

#define UI_ENGINE_VAL_WIDTH				(36)
#define UI_ENGINE_VAL_HEIGHT			(CONIO_TEXT_Y_INC)

#define UI_PAGE1_NEXTBUTTON_WIDTH		(72)
#define UI_PAGE1_NEXTBUTTON_HEIGHT		(CONIO_TEXT_Y_INC)

#define UI_PAGE1_TFA					(0)
#define UI_PAGE1_BFA					(24)

//-------------------------------------------------------------------- [Global data]

#pragma section ".data.ui"

/** \brief CAR state UI data */
UI_carState_data_t		gUICarState;
/** \brief Joystick UI data */
UI_joystick_data_t 		gUIJoystick;
/** \brief Target Speed UI data */
UI_targetSpeed_data_t	gUITargetSpd;
/** \brief Engine UI data */
UI_engine_data_t		gUIEngine;
/** \brief Safety Runnable Name data */
UI_carSafetyRunnable_data_t gUISafetyRunnable;


#pragma section

//-------------------------------------------------------------------- [Global Configuration]

#pragma section ".rodata"

#if ((CONIO_MODE == CONIO_REV_LANDSCAPE) || (CONIO_MODE == CONIO_LANDSCAPE))

/** \brief Normal state UI config */
const UI_normalState_cfg_t gUINormStateCfg =
	{
		{{0, 10}, strlen(UI_NORMAL_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NORMAL_STATE_KEY_STR,
		{{(strlen(UI_NORMAL_STATE_KEY_STR) + 1), 10}, UI_NORMAL_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Safety state UI config */
const UI_safetyState_cfg_t gUISafeStateCfg =
	{
		{{0, 9}, strlen(UI_SAFETY_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_SAFETY_STATE_KEY_STR,
		{{(strlen(UI_SAFETY_STATE_KEY_STR) + 1), 9}, UI_SAFETY_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Safety runnable UI config */
const UI_safety_runnable_cfg_t gUISafetyRunnableCfg =
	{
		{{0, 8}, strlen(UI_SAFETY_RUNNABLE_STR), RGB565_BLACK, RGB565_WHITE},
		UI_SAFETY_RUNNABLE_STR,
		{{(strlen(UI_SAFETY_RUNNABLE_STR) + 1), 8}, UI_SAFETY_RUNNABLE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Joystick UI config */
const UI_joystick_cfg_t gUIJoystickCfg =
	{
		{{0, UI_JOYSTICK_LINE}, strlen(UI_JOYSTICK_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_JOYSTICK_KEY_STR,
//		 @PF {
//			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 8}, UI_JOYSTICK_X_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* x */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + 2), 8}, UI_JOYSTICK_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},									/* y */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + 3), 8}, UI_JOYSTICK_RZ_WIDTH, RGB565_BLACK, RGB565_WHITE},				/* rz */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 7}, UI_JOYSTICK_ESTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* estop */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + 2), 7}, UI_JOYSTICK_GSTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* gstop */
//			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + UI_JOYSTICK_GSTOP_WIDTH + 3), 7}, UI_JOYSTICK_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* age */
//		}
		{
			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), UI_JOYSTICK_LINE}, UI_JOYSTICK_X_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* x */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + 2), UI_JOYSTICK_LINE}, UI_JOYSTICK_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},									/* y */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + 3), UI_JOYSTICK_LINE}, UI_JOYSTICK_RZ_WIDTH, RGB565_BLACK, RGB565_WHITE},				/* rz */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + UI_JOYSTICK_RZ_WIDTH + 4), UI_JOYSTICK_LINE}, UI_JOYSTICK_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* age */
		}


	};


/** \brief target speed UI config */
const UI_targetSpeed_cfg_t gUITargetSpdCfg =
	{
		{{0, UI_TARGET_SPEED_LINE}, strlen(UI_TARGET_SPEED_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_TARGET_SPEED_KEY_STR,
		{
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + 1), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_X_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* x */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + 2), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* y */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + UI_TARGET_SPEED_Y_WIDTH + 3), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_PHI_WIDTH, RGB565_BLACK, RGB565_WHITE},	/* phi */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + UI_TARGET_SPEED_Y_WIDTH + UI_TARGET_SPEED_PHI_WIDTH + 4), UI_TARGET_SPEED_LINE}, UI_TARGET_SPEED_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* age */
		}
	};

/** \brief engine UI config */
const UI_engine_cfg_t gUIEngineCfg =
	{
		{{0, 4}, strlen(UI_ENGINE_KEY_STR), RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		UI_ENGINE_KEY_STR,						   							/** mpKey **/
		RGB565_WHITE,														/** ClrColor - WHITE **/
		RGB565_GREEN,														/** SetColor - GREEN **/
		{
			{
				/** mVal[0][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[0][1] **/
				{
					/** mVal[0][1] **/
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[1][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1)  * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[1][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (3 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[2][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[2][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[3][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[3][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (1 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

			},
		}
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI1NextPageButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE1_NEXTBUTTON_WIDTH), 0},
			UI_PAGE1_NEXTBUTTON_WIDTH,
			UI_PAGE1_NEXTBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};

#elif ((CONIO_MODE == CONIO_REV_PORTRAIT) || (CONIO_MODE == CONIO_PORTRAIT))

/** \brief Normal state UI config */
const UI_normalState_cfg_t gUINormStateCfg =
	{
		{{0, 14}, strlen(UI_NORMAL_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_NORMAL_STATE_KEY_STR,
		{{(strlen(UI_NORMAL_STATE_KEY_STR) + 1), 14}, UI_NORMAL_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Safety state UI config */
const UI_safetyState_cfg_t gUISafeStateCfg =
	{
		{{0, 13}, strlen(UI_SAFETY_STATE_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_SAFETY_STATE_KEY_STR,
		{{(strlen(UI_SAFETY_STATE_KEY_STR) + 1), 13}, UI_SAFETY_STATE_VAL_WIDTH, RGB565_BLACK, RGB565_WHITE}
	};

/** \brief Joystick UI config */
const UI_joystick_cfg_t gUIJoystickCfg =
	{
		{{0, 12}, strlen(UI_JOYSTICK_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_JOYSTICK_KEY_STR,
		{
			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 12}, UI_JOYSTICK_X_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* x */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + 2), 12}, UI_JOYSTICK_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},									/* y */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_X_WIDTH + UI_JOYSTICK_Y_WIDTH + 3), 12}, UI_JOYSTICK_RZ_WIDTH, RGB565_BLACK, RGB565_WHITE},				/* rz */
			{{(strlen(UI_JOYSTICK_KEY_STR) + 1), 11}, UI_JOYSTICK_ESTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* estop */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + 2), 11}, UI_JOYSTICK_GSTOP_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* gstop */
			{{(strlen(UI_JOYSTICK_KEY_STR) + UI_JOYSTICK_ESTOP_WIDTH + UI_JOYSTICK_GSTOP_WIDTH + 3), 11}, UI_JOYSTICK_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},													/* age */
		}
	};


/** \brief target speed UI config */
const UI_targetSpeed_cfg_t gUITargetSpdCfg =
	{
		{{0, 10}, strlen(UI_TARGET_SPEED_KEY_STR), RGB565_BLACK, RGB565_WHITE},
		UI_TARGET_SPEED_KEY_STR,
		{
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + 1), 10}, UI_TARGET_SPEED_X_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* x */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + 2), 10}, UI_TARGET_SPEED_Y_WIDTH, RGB565_BLACK, RGB565_WHITE},							/* y */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + UI_TARGET_SPEED_X_WIDTH + UI_TARGET_SPEED_Y_WIDTH + 3), 10}, UI_TARGET_SPEED_PHI_WIDTH, RGB565_BLACK, RGB565_WHITE},	/* phi */
			{{(strlen(UI_TARGET_SPEED_KEY_STR) + 1), 9}, UI_TARGET_SPEED_AGE_WIDTH, RGB565_BLACK, RGB565_WHITE},												/* age */
		}
	};

/** \brief engine UI config */
const UI_engine_cfg_t gUIEngineCfg =
	{
		{{0, 8}, strlen(UI_ENGINE_KEY_STR), RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		UI_ENGINE_KEY_STR,						   							/** mpKey **/
		RGB565_WHITE,														/** ClrColor - WHITE **/
		RGB565_GREEN,														/** SetColor - GREEN **/
		{
			{
				/** mVal[0][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (7 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[0][1] **/
				{
					/** mVal[0][1] **/
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (7 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[1][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1)  * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (7 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[1][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (7 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[2][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (0 * UI_ENGINE_VAL_WIDTH)), (5 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[2][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (1 * UI_ENGINE_VAL_WIDTH)), (5 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},
			},
			{
				/** mVal[3][0] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (3 * UI_ENGINE_VAL_WIDTH)), (5 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

				/** mVal[3][1] **/
				{
					{(((strlen(UI_ENGINE_KEY_STR) + 1) * CONIO_TEXT_X_INC) + (4 * UI_ENGINE_VAL_WIDTH)), (5 * CONIO_TEXT_Y_INC)},
					UI_ENGINE_VAL_WIDTH,
					UI_ENGINE_VAL_HEIGHT,
					RGB565_BLACK
				},

			},
		}
	};

/** \brief UI next button */
const CONIO_RECTBUTTON_cfg_t gUI1NextPageButton =
	{
		{
			{(CONIO_PAGE_MAX_WIDTH - UI_PAGE1_NEXTBUTTON_WIDTH), 0},
			UI_PAGE1_NEXTBUTTON_WIDTH,
			UI_PAGE1_NEXTBUTTON_HEIGHT,
			RGB565_BLACK
		},
		{{0, 0}, 0, RGB565_BLACK, RGB565_WHITE}, 	/** mKey **/
		NULL,
		RGB565_GREEN
	};

#endif


/** \brief View 1 output config */
const CONIO_PAGE_outputElement_t gUIView1OutCfg[] =
{
	/* Normal State Entry */
	{
		TRUE, &gUINormStateCfg, &gUICarState,
		&UI_CARSTATE__init_normalState, &UI_CARSTATE__read_normalState, &UI_CARSTATE__update_normalState
	},

	/* Safety State Entry */
	{
		TRUE, &gUISafeStateCfg, &gUICarState,
		&UI_CARSTATE__init_safetyState, &UI_CARSTATE__read_safetyState, &UI_CARSTATE__update_safetyState
	},

	/* Safety Runnable Entry */
	{
		TRUE, &gUISafetyRunnableCfg, &gUISafetyRunnable,
		&UI_CARSTATE__init_safetyRunnable, &UI_CARSTATE__read_safetyRunnable, &UI_CARSTATE__update_safetyRunnable
	},

	/* Joystick Entry */
	{
		TRUE, &gUIJoystickCfg, &gUIJoystick,
		&UI_CARSTATE__init_joystick, UI_CARSTATE__read_joystick, &UI_CARSTATE__update_joystick
	},

	/* target speed Entry */
	{
		TRUE, &gUITargetSpdCfg, &gUITargetSpd,
		&UI_CARSTATE__init_targetSpeed, &UI_CARSTATE__read_targetSpeed, &UI_CARSTATE__update_targetSpeed
	},

	/* Engine Entry */
	{
		TRUE, &gUIEngineCfg, &gUIEngine,
		&UI_CARSTATE__init_engine, &UI_CARSTATE__read_engine, &UI_CARSTATE__update_engine
	},

	/* Mandatory Empty Entry */
	{
		FALSE, NULL, NULL,
		NULL, NULL, NULL
	}
};

/** \brief View 1 input config */
const CONIO_PAGE_inputElement_t gUIView1InCfg[] =
{
	/* Next Page Button */
	{TRUE, &gUI1NextPageButton, &CONIO_RECTBUTTON_init, &CONIO_RECTBUTTON_isPressed, PAGE_ACTION_SWITCH_NEXT, NULL},
	/* Mandatory Empty Entry */
	{FALSE, NULL, NULL, NULL, PAGE_ACTION_NONE, NULL}
};

/** \brief View 1 config */
const CONIO_PAGE_cfg_t gUIView1 =
{
		UI_PAGE1_TFA, UI_PAGE1_BFA, &gUIView1OutCfg[0], &gUIView1InCfg[0], RGB565_WHITE
};

#pragma section
