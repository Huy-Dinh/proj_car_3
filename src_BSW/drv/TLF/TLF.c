/**
 * \file 	TLF.c
 * \author 	Arellano Zea, Luis Alberto		Hochschule Darmstadt - luis.a.zea@stud.h-da.de
 * \author 	Thomas Barth 					Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	27.10.2017
 *
 * \brief TLF DRIVER
 *
 * \copyright Copyright ©2017
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

#include "TLF.h"
#include "QSPI.h"

#include "TLF_regdef.h"



//HACK: helper function. Needs to be replaced with QSPI driver
static void __send_SPI_msg(unsigned short data){

	QSPI2_DATAENTRY0.U = (unsigned int)data;
	/* wait until transfer is complete */
	while (!QSPI2_STATUS.B.TXF)
		;
	/* clear TX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 9;
	/* wait for receive is finished */
	while (!QSPI2_STATUS.B.RXF)
		;
	/* clear RX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 10;
	/* read and discard value */
	(void)QSPI2_RXEXIT.U;
}




//---------------[local types]

/** \brief  SPI exchange container type*/
typedef struct
{
	QSPI_channel_t 			mChannel_hdl;					/**< \brief SPI channel handle*/
	TLF_SPI_exchange_t		mTxBuffer;						/**< \brief TX Buffer*/
	TLF_SPI_exchange_t		mRxBuffer;						/**< \brief RX Buffer*/
	RC_t					mResult;						/**< \brief Result buffer*/
}TLF_exchange_container_t;


//---------------[locally used global vars]

/** \brief SPI exchange container instance*/
static TLF_exchange_container_t gTLF;

/** \brief SPI exchange parameters*/
static QSPI_exchange_param_t	TLF_exchange_param;

/** \brief Spinlock for SPI busy waiting*/
static volatile boolean_t bResource  = FALSE;

//---------------[local functions]

/** \brief	TLF SPI Spinlock waiting
 *
 * While we wait for the SPI exchange to be complete, the inner spinlock keeps the CPU busy
 * \param apRes	The SPI driver will provide a pointer to the user data. In our case the user data is the spinlock
 */
static void TLF_wait_exchange_cplt(volatile boolean_t *apRes){
	// Busy Wait
	while (TRUE != *apRes);
}

/** \brief	TLF SPI Callback
 *
 * This function is called by the SPI driver if the SPI exchange finished (which doesn't mean that it was successful)
 * \param apRes	The SPI driver will provide a pointer to the user data. In our case the user data is the spinlock
 */
static void TLF_SPI_cb(QSPI_exchangeResult_t aResult, void *apUserData)
{
	if (QSPI_exchangeResult_SUCCESS == aResult)
		gTLF.mResult = RC_SUCCESS;
	else
		gTLF.mResult = RC_ERROR;

	//release spinlock
	if (NULL != apUserData)
		*(volatile boolean_t *)apUserData=TRUE;
}

/** \brief	TLF SPI Byte swap
 *
 * The communication with the TLF is 16 bit wide but the SPI driver currently is only capable of sending 8bit frames.
 * We send 2x8 bits but this swaps the byte order. Hence we need to swap the bytes.
 * This is a workaround and has to be fixed with a better SPI driver
 * \param p_payload	Pointer to the bytes, that shall we swaped.
 */
static void __TLF_SPI_byte_swap(TLF_SPI_exchange_t* p_payload){
	uint8_t __SPI_scramble_helper;
	__SPI_scramble_helper=p_payload->U8[0];
	p_payload->U8[0]=p_payload->U8[1];
	p_payload->U8[1]=__SPI_scramble_helper;
}

/** \brief Read a register from the TLF
 *
 * \param reg_offset	Register to be read
 * \param p_result		Pointer where the read register will be stored
 * \return	#RC_ERROR_BAD_PARAM		If there was no valid pointer to the register storage
 * 			#RC_ERROR_READ_FAILS	SPI exchange failed
 * 			#RC_SUCCESS				If register was read
 * 			others					\see #QSPI_channel_exchange
 */
static RC_t TLF_read_reg(TLF_reg_t reg_offset, uint8_t* p_result){

	RC_t retCode = RC_ERROR;

	if(p_result==NULL)
		return RC_ERROR_BAD_PARAM;

	//prepare exchange container
	gTLF.mTxBuffer.B.rw=TLF_READ;
	gTLF.mTxBuffer.B.address=reg_offset;
	//TODO: Parity bit calculation

	//SPI transmission scrambles the byte order of the exchange format
	__TLF_SPI_byte_swap(&gTLF.mTxBuffer);

	//trigger exchange
	if ((retCode = QSPI_channel_exchange(&gTLF.mChannel_hdl, QSPI_exchange_readwrite, &TLF_exchange_param))!=RC_SUCCESS)
		return retCode;

	//make sure TX byte order is correct again
	__TLF_SPI_byte_swap(&gTLF.mTxBuffer);

	//we wait for the exchange to be completed
	TLF_wait_exchange_cplt(&bResource);

	//check SPI result
	if(gTLF.mResult!=RC_SUCCESS)
		return RC_ERROR_READ_FAILS;

	//SPI transmission scrambles the byte order of the exchange format
	__TLF_SPI_byte_swap(&gTLF.mRxBuffer);

	//safe result
	*p_result=gTLF.mRxBuffer.B.data;

	//return
	return RC_SUCCESS;
}

//---------------[global functions]

RC_t	TLF_init(){

	//result buffer
	RC_t retCode = RC_ERROR;

	//container for TLF read operations
	uint8_t	__TLF_RX_byte;

	//INit QSPI2 channel 1
	if ((retCode = QSPI_channel_init(QSPI2,QSPI_CH1,&gTLF.mChannel_hdl))!=RC_SUCCESS)
		return retCode;

	//TLF SPI exchange parameters
	TLF_exchange_param.mfOnExchange 	= &TLF_SPI_cb;				//callback which is called, once the exchange is completed
	TLF_exchange_param.mpUserData 		= (void *)&bResource;		//user Data (in our case the spinlock)
	TLF_exchange_param.mpReadPtr 		= &gTLF.mRxBuffer;			//RX data pointer
	TLF_exchange_param.mpWritePtr		= &gTLF.mTxBuffer;			//TX data pointer

	//TODO: we are limited to 8 bits here, which messes up the byte order and is a bit slower. We have to check why we are limited, so we can move to 16 bit.
	TLF_exchange_param.mnDataWidth 		= 8;
	TLF_exchange_param.mnLength 		= 2;

	//debugging hook
	asm("nop");

	//Test Case, read DEVCFG
	if((retCode=TLF_read_reg(TLF_DEVCFG0,&__TLF_RX_byte))!=RC_SUCCESS)
		return retCode;

	//we create a pointer, which will ease the access to the read register
	volatile TLF_DEVCFG0_t* p_TLF_DEVCFG0=&__TLF_RX_byte;

	//PLS is not able to visualize the pointer above, hence we buffer the bitfields. This is for debugging only!
	volatile 	uint8_t test[4];
	test[0]=p_TLF_DEVCFG0->B.TRDEL;
	test[1]=p_TLF_DEVCFG0->B.WKTIMCYC;
	test[2]=p_TLF_DEVCFG0->B.WKTIMEN;
	test[3]=p_TLF_DEVCFG0->B.nu;

	//debugging hook
	asm("nop");


	return RC_SUCCESS;
}

void test01(){

	int i;
		uint32_t tlf_version_response=0;


		// command sequence for disabling TLF A-Step
		const unsigned short wdtdiscmd_a_step[10] =
		{
			0x8756, 0x87de, 0x86ad, 0x8625, 0x8d27,
			0x8811, 0x87be, 0x8668, 0x877d, 0x8795
		};

		// command sequence for disabling TLF B-Step
		const unsigned short wdtdiscmd_b_step[10] =
		{
			0x8756, 0x87de, 0x86ad, 0x8625, 0x8d27,
			0x8A01, 0x87be, 0x8668, 0x877d, 0x8795
		};


//	    /* check that this disabling has not been already done (e.g. by the debugger) */
//	    if (QSPI2_GLOBALCON.B.EN)
//	    {
//	        /* don't do it again */
//	        return;
//	    }



		/* initialise QSPI2 interface */
		unlock_wdtcon();                /* remove ENDINIT protection */
		QSPI2_CLC.U = 0x8;              /* activate module, disable sleep mode */
		(void)QSPI2_CLC.U;              /* read back to get effective */
		P15_PDR0.U = 0x00000000;        /* fast speed (all pins) */
		P14_PDR0.U = 0x00000000;        /* fast speed (all pins) */
		QSPI2_PISEL.U = 1;              /* MRIS=1 ==> use MRST2B pin */
		lock_wdtcon();                  /* re-enable ENDINIT protection */

		/* configure port pins */
		P14_IOCR0.B.PC2 = 0x13;         /* SLSO21 */
		P15_IOCR0.B.PC3 = 0x13;         /* SCLK2 */
		P15_IOCR4.B.PC6 = 0x13;         /* MTSR2 */
		P15_IOCR4.B.PC7 = 0x02;         /* MRST2B */

		/* program QSPI2 parameters */
		QSPI2_GLOBALCON.U = 0x00003C04; /* EXPECT=15,SI=0, TQ=4 */
		QSPI2_GLOBALCON1.U = 0x14000000;/* RXFM=1,TXFM=1 (Single Move Mode for RX/TX) */
		QSPI2_SSOC.U = 0x00020000;      /* enable SLSO21, low active */
		QSPI2_ECON1.U = 0x501;          /* Q=1,A=0,B=1,C=1 */

		do
		{
			QSPI2_FLAGSCLEAR.U = 0xFFF; /* PT2F,PT1F,RXF,TXF,ERRORFLAGS */
		} while (QSPI2_STATUS.U & 0xFFF);

		/* prepare data transfer format */
		QSPI2_BACONENTRY.U = 0x17A10001;    /* CS=1,DL=15,MSB=1,TRAIL=1,LAST=1 */

		QSPI2_GLOBALCON.B.EN = 1;       /* ... and enable the module */

		//Check TLF Version

		__send_SPI_msg(0x6801);


		QSPI2_DATAENTRY0.U = 0x3e01;
		/* wait until transfer is complete */
		while (!QSPI2_STATUS.B.TXF)	;

		/* clear TX flag */
		QSPI2_FLAGSCLEAR.U = 1 << 9;

		/* wait for receive is finished */
		while (!QSPI2_STATUS.B.RXF)	;

		/* clear RX flag */
		QSPI2_FLAGSCLEAR.U = 1 << 10;

		/* read and discard value */
		tlf_version_response=QSPI2_RXEXIT.B.E;


		/* transfer all data */
		for (i = 0; i < 10; ++i)
		{
			if(tlf_version_response==0x8008)
				__send_SPI_msg((unsigned int)wdtdiscmd_b_step[i]);
			else
				__send_SPI_msg((unsigned int)wdtdiscmd_a_step[i]);

			SYSTEM_DelayUs(10);
		}

		SYSTEM_DelayUs(60);

		//B/C Step
		if(tlf_version_response==0x8008){
			__send_SPI_msg(0xabd5);
			__send_SPI_msg(0xac2b);
		}
		//A-Step
		else{

			__send_SPI_msg(0x9dd5);
		}


}

void test02(){
	__send_SPI_msg((unsigned short)0x8123);
	__send_SPI_msg((unsigned short)0x8456);
}





