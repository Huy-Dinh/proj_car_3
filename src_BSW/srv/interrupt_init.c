/*	cint.c -- C interface for TriCore interrupts.

	Copyright (C) 1998 HighTec EDV-Systeme GmbH.

	This file is part of GCC.

	GCC is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3, or (at your option)
	any later version.

	GCC is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	Under Section 7 of GPL version 3, you are granted additional
	permissions described in the GCC Runtime Library Exception, version
	3.1, as published by the Free Software Foundation.

	You should have received a copy of the GNU General Public License and
	a copy of the GCC Runtime Library Exception along with this program;
	see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
	<http://www.gnu.org/licenses/>.  */

#include "interrupt_init.h"
#include "IfxCpu_reg.h"
#include "IfxCpu_regdef.h"

//#pragma section ".sys.inttab"

IRQ_hdl_t Cdisptab[MAX_INTRS]={
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0},
		{NULL,0}
};


/*==========================================
 * The common dispatcher for interrupts
 *========================================*/
void commonDispatcher()
{
	Ifx_CPU_ICR IcrValue = (Ifx_CPU_ICR) __MFCR(CPU_ICR);
	if (Cdisptab[IcrValue.B.CCPN].irq_handler != NULL)
	{
		Cdisptab[IcrValue.B.CCPN].irq_handler(Cdisptab[IcrValue.B.CCPN].hnd_arg);
	}
}

//#pragma section

/* This is the default interrupt vector table, which consists of 32
   entries, each consisting of eight words (32 bytes).  The table
   must be put into the section ".inttab", must be aligned to a
   4 KB boundary, and the entry symbol (virtual interrupt 0)
   must be called "TriCore_int_table", as it is referenced below
   in order to program the BIV register.  Note: This is only a
   sample implementation which can handle up to 31 interrupts.
   If you need more interrupts, you need to expand the table in the
   obvious way and adjust the alignment as explained in the "TriCore
   uC-DSP Architecture Manual".  Interrupt 1 is special in that it
   might get used to activate services and functions of an operating
   system.  It is therefore defined in a file called int1.o in the
   libos.a archive, so that it can be easily redefined by a customized
   version of __interrupt_1.  */

#if defined(ERRATA_CPU13) || defined(ERRATA_DMI12)
# if defined(ERRATA_CPU13)
#  define DSYNC asm ("dsync");
# else
#  define DSYNC
# endif
# ifdef ERRATA_CPU9
#   undef DSYNC
#   define DSYNC asm ("\tdsync\n\tnop\n\tnop");
# endif
#else
# define DSYNC
#endif

asm ("						\n\
	.section .interrupttable_init_in, \"ax\", @progbits	\n\
	.globl TriCore_int_table		\n\
TriCore_int_table:				\n\
");
asm ("            \n\
.globl ___commonDispatcher      \n\
___commonDispatcher:          \n\
"); 
asm	("enable");
asm ("svlcx");
asm("calla commonDispatcher");
asm("rslcx");
asm("rfe");

asm (".text");
#if defined(ERRATA_CPU13) || defined(ERRATA_DMI12)
  asm ("__workaround2_for_cpu13_9:");
  asm ("ld.a %a14,[%a15+]");
  asm ("ld.w %d4,[%a15]");
  asm ("calli %a14");
  asm ("rslcx");
  asm ("rfe");
#endif


RC_t _install_int_hdl (uint32_t intno, void (*handler) (int), int arg){
  if(intno>=MAX_INTRS)
	  return RC_ERROR_BAD_PARAM;

  Cdisptab[intno].irq_handler=handler;
  Cdisptab[intno].hnd_arg=arg;

  return RC_SUCCESS;

}
