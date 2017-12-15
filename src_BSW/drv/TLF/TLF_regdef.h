/**
 * \file 	TLF_regdef.h
 * \author 	Arellano Zea, Luis Alberto		Hochschule Darmstadt - luis.a.zea@stud.h-da.de
 * \author 	Thomas Barth 					Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	30.10.2017
 *
 * \brief TLF register definition FILE
 * \TODO: distinguish between TLF Steps
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
#ifndef SRC_BSW_DRV_TLF_TLF_REGDEF_H_
#define SRC_BSW_DRV_TLF_TLF_REGDEF_H_

#include "global.h"



//---------------[TLF Registers]
/** \brief  Enumeration for TLF registers. The number defines the register offset */
typedef enum{
	TLF_DEVCFG0=0x00,									/**< \brief Device configuration 0 (DEVCFG0) Register*/
	TLF_DEVCFG2=0x02,									/**< \brief Device configuration 2 (DEVCFG2) Register*/
	TLF_PROTCFG=0x03,									/**< \brief Protection configuration (PROTCFG) Register*/

}TLF_reg_t;


//---------------[SPI]

/** \brief Defines if a read or write operation shall be performed on the TLF*/
typedef enum{
	TLF_READ=0b0,
	TLF_WRITE=0b1,
}TLF_CMD_BIT_t;


/** \brief  TLF SPI exchange format */
typedef union
{
    /** \brief Unsigned access */
    uint16_t U16;

    uint8_t	U8[2];

    /** \brief Bitfield access */
    struct{

    	/** \brief Parity bit
    	 *
    	 * The parity is calculated for the output data stream in case a read operation is performed. The data for the
    	 * calculation consists of 1’b1, status [5:0] and rd_data[7:0]. The parity bit is set to ‘1’ if the number of ‘1’ in the output
    	 * data stream is odd, i.e.XOR function between all 15 bits to send out */
        boolean_t 		parity:1;


    	/** \brief data field
    	 *
    	 * Used as payload container during write operations and as a dummy during read operations, in order to clock out the MISO bits*/
        uint8_t			data:8;


    	/** \brief address field
    	 *
    	 * Addresses the register of interest within the TLF. */
        uint8_t			address:6;


    	/** \brief command bit
    	 *
    	 * determines if this is a read or write operation. */
        TLF_CMD_BIT_t	rw:1;

    } B;
} TLF_SPI_exchange_t;

//---------------[Device Registers]

/** \brief  TLF Device configuration 0 (DEVCFG0) Register
 *
 * Offset 		0x00
 * reset value 	0x08
 */
typedef union
{
    /** \brief Unsigned access */
    uint8_t	U8;

    /** \brief Bitfield access */
    struct{

    	/** \brief Transition delay ([3:0] RW)
    	 *
    	 *	Transition delay into low power states (STANDBY, SLEEP). Defined as a step of 100 us
    	 *	0D , 100 us
    	 *	1D , 200 us
    	 *	2D , 300 us
    	 *	15D, 1600 us
    	 *
    	 *	Reset: 0x08
    	 */
        uint8_t 		TRDEL:4;

    	/** \internal unused*/
        uint8_t 		nu:2;

    	/** \brief Wake timer cycle period ([6] RW)
    	 *
    	 *  0D , 10 us
    	 *  1D , 10 ms
    	 *
    	 *  Reset: 0x0
    	 */
        uint8_t 		WKTIMCYC:1;

    	/** \brief Wake timer enable ([7] RW)
    	 *
    	 * 0D , Wake timer disabled
    	 * 1D , Wake timer enabled in SLEEP or STANDBY state
    	 *
    	 * Reset: 0x0
    	 */
        uint8_t 		WKTIMEN:1;
    } B;
} TLF_DEVCFG0_t;



#endif /* SRC_BSW_DRV_TLF_TLF_REGDEF_H_ */
