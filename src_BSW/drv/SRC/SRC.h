/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: SRC.h
 * Desc: Interrupt configuration routines
 ******************************************************************************/

#ifndef SRC_H
#define SRC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "register.h"

#include "global.h"

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** Resets the overrun flag of the Service Request.
 * @param: src pointer to the Service Request Control register which the overrun flag should be cleared.
 * @return: None
 */
INLINE void SRC_clearOverrun(volatile Ifx_SRC_SRCR *src);

/** Resets a specific interrupt service by software.
 * @param: src pointer to the Service Request Control register which the request should be cleared.
 * @return: None
 */
INLINE void SRC_clearRequest(volatile Ifx_SRC_SRCR *src);

/** Gets the current overrun status.
 * @param: src pointer to the Service Request Control register for which the overrun status should be returned.
 * @return: current service request control overrun status.
 */
INLINE boolean_t SRC_isOverrun(volatile Ifx_SRC_SRCR *src);

/** Gets the current request status.
 * @param: src pointer to the Service Request Control register for which the request status should be returned.
 * @return: current service request control request status.
 */
INLINE boolean_t SRC_isRequested(volatile Ifx_SRC_SRCR *src);

/** Requests a specific interrupt service by software
 * @param: src pointer to the Service Request Control register which the interrupt has to be requested.
 * @return: None
 */
INLINE void SRC_setRequest(volatile Ifx_SRC_SRCR *src);

/** DeInitializes the service request control register.
 * @param: src pointer to the Service Request Control register which should be deinitialised.
 * @return: None
 */
INLINE void SRC_deinit(volatile Ifx_SRC_SRCR *src);

/** Disables a specific interrupt service request.
 * @param: src pointer to the Service Request Control register for which the interrupt has to be disabled.
 * @return: None
 */
INLINE void SRC_disable(volatile Ifx_SRC_SRCR *src);

/** Enables a specific interrupt service request.
 * @param: src pointer to the Service Request Control register for which the interrupt has to be enabled.
 * @return: None
 */
INLINE void SRC_enable(volatile Ifx_SRC_SRCR *src);

/** Initializes the service request control register.
 * @param: src pointer to the Service Request Control register which should be initialised.
 * @param: typOfService type of interrupt service provider.
 * @param: priority Interrupt priority.
 * @return: None
 */
INLINE void SRC_init(volatile Ifx_SRC_SRCR *src, CpuId_t typOfService, uint8_t priority);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

INLINE void SRC_clearOverrun(volatile Ifx_SRC_SRCR *src)
{
    src->B.IOVCLR = 1;
}


INLINE void SRC_clearRequest(volatile Ifx_SRC_SRCR *src)
{
    src->B.CLRR = 1;
}


INLINE boolean_t SRC_isOverrun(volatile Ifx_SRC_SRCR *src)
{
    return src->B.IOV ? TRUE : FALSE;
}


INLINE boolean_t SRC_isRequested(volatile Ifx_SRC_SRCR *src)
{
    return src->B.SRR ? TRUE : FALSE;
}


INLINE void SRC_setRequest(volatile Ifx_SRC_SRCR *src)
{
    src->B.SETR = 1;
}


INLINE void SRC_deinit(volatile Ifx_SRC_SRCR *src)
{
    src->U = 0;
}


INLINE void SRC_disable(volatile Ifx_SRC_SRCR *src)
{
    src->B.SRE = 0;
}


INLINE void SRC_enable(volatile Ifx_SRC_SRCR *src)
{
    src->B.SRE = 1;
}


INLINE void SRC_init(volatile Ifx_SRC_SRCR *src, CpuId_t typOfService, uint8_t priority)
{
    src->B.SRPN = priority;
    src->B.TOS  = typOfService;
    SRC_clearRequest(src);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRC_H */
