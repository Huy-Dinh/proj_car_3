/**
 * \file 	clock.h
 * \author 	Thomas Barth	- Hochschule Darmstadt thomas.barth@h-da.de
 * \date 	13.12.2016
 * \version	0.2
 * \file	clock.h
 *
 * \brief Clock configuration parameters
 *
 *	Changelog:\n
 *	- 0.2 13.12.16 Barth
 *				- Removed backward compatibility macros SYSTEM_GetExtClock, SYSTEM_GetSysClock, SYSTEM_GetStmClock, SYSTEM_GetCpuClock. Use #Clock_get_Frequency instead.
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


#ifndef CLOCK_H_
#define CLOCK_H_

#include "global.h"

#define CLOCK_UNKNOWN	0	/**< \brief This value is returned by the clock-driver if a clock frequency could not be determined */

/**
 * \brief List of available Frequencies.
 *
 * This enumeration is a list of all available frequencies on the device.
 * \warning The frequencies for the CPUs must be consecutive by number!
 */
typedef enum {
	fOSC,					/**< fOSC is the external frequency fed to the PLL. */
	fOSCref,				/**< fOSCref is the frequency fed to the OSC Watchdog */
	fREF,					/**< fREF is the frequency that is fed to the VCO after it had been divided by the P-Divider. */
	fVCO,					/**< fVCO is the frequency after the VCO, that will go to K2 and K3 and also works as a feedback (via the N-Divider) for the PLL. */
	fK1,					/**< fK1 is fOSC divided by K1 */
	fK2,					/**< fK2 is fVCO divided by K2 */
	fK3,					/**< fK3 is fVCO divided by K3 */
	fSource,				/**< fSource is either the SCU_CCUCON0.B.CLKSEL=0-> Backup-CLock (fBack) or the SCU_CCUCON0.B.CLKSEL=1-> PLL(fPLL) that is fed to the CCU (Clock Control Unit)  */
	fPLL,					/**< fPLL is the main clock from which most clocks are derived in normal operation. */
	fPLLERAY,				/**< fPLLERAY is the clock generated by the ERAY PLL*/
	fBACK,					/**< fBACK the frequency of the Backup-Clock. Becomes active if fPLL is faulty.  */
	fBASE,					/**< fBASE is the base frequency of the PLL if no external oscilator is connected */
	fBAUD1,					/**< fBAUD1 Clock for I2C and PSI5(S) */
	fBAUD2,					/**< fBAUD2 Clock for QSPI */
	fSRI,					/**< fSRI Clock for SRI OCDS, MSC, HSCT/HSSL, LMU, XBAR, CPUs, SFI, EBU, DMA, PMU */
	fSPB,					/**< fBAUD2 Clock for SPB, OCDS, SPB Domain (ADCs,SCU,BCU, Ports, SENT, FCE, GPT, CCU, MSC, PSI4, I2C, PSO5(S), SMU), ERAY, CAN, STM, GTM, HSM, ETH, CPUs, SFI, EBU, DMA  */
	fFSI2,					/**< fFSI2 Clock for PMU */
	fFSI,					/**< fFSI Clock for PMU */
	fCAN,					/**< fCAN Clock for CAN */
	fERAY,					/**< fERAY Clock for ERAY */
	fSTM,					/**< fSTM Clock for STM */
	fETH,					/**< fETH Clock for ETH */
	fGTM,					/**< fGTM Clock for GTM */
	fASCLINF,				/**< fASCLINF Clock for ASCLIN-F */
	fASCLINS,				/**< fASCLINS Clock for ASCLIN-S */
	fMAX,					/**< fMAX Max clock */
	fEBU,					/**< fEBU Clock for EBU */
	fBBB,					/**< fBBB Clock for BBB */
	fCPU0,					/**< fCPU0 Clock for CPU0 */
	fCPU1,					/**< fCPU1 Clock for CPU1 */
	fCPU2					/**< fCPU2 Clock for CPU2 */
}Clock_frequency_t;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** \brief perform the clock initialization based on #clock_cfg.h */
RC_t Clock_Init();

/** \brief Get a frequency that is current set inside of the clock-tree
 *
 *  \param 	sub_clk The clock that is requested, see #Clock_frequency_t for a list of available clocks.
 *  \return The requested Frequency in Hz. #CLOCK_UNKNOWN if the request was not possible
 */
unsigned long Clock_get_Frequency(Clock_frequency_t sub_clk);

/** \brief Get the Frequency of the CPU that is calling that function
 *
 *  \return the Frequency of the calling CPU in Hz
 */
unsigned long Clock_GetCpuClock(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLOCK_H_ */
