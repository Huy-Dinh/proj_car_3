/*
 * Task_Init.c
 *
 *  Created on: Dec 18, 2014
 *      Author: fromm
 */

#include "core.h"
#include "pxdef.h"

#include "symbols.h"

#include "Task_Init.h"




/*******************************************************************************************************
 * Magic defines to sum up some memory sizes and to allocate the resulting memory
 * Core specific sizes are defined in the corresponding initfile header
 *******************************************************************************************************/


#define _DEF_SYM(n,v) \
   __asm__ (".global       " #n );     \
   __asm__ (".set  " #n "," #v);   \
   __asm__ (".type " #n ",STT_OBJECT")

#define DEF_SYM(name,val) _DEF_SYM(name,val)

#define MULTI_CORE
extern void _start(void);


//#define PXROS_NAMESIZE		(PXROS_NAMESIZE_CORE0 + PXROS_NAMESIZE_CORE1 + PXROS_NAMESIZE_CORE2)
//DEF_SYM(__PXROS_NAMESIZE__, PXROS_NAMESIZE);

#define PXROS_NAMESIZE      0   /* we don't need additional object name storage */

DEF_SYM(__PXROS_NAMESIZE__, PXROS_NAMESIZE);

#define NUM_OF_PXOBJS	(NUM_OF_PXOBJS_CORE0 + NUM_OF_PXOBJS_CORE1 + NUM_OF_PXOBJS_CORE2)
DEF_SYM(__NUM_OF_PXOBJS__, NUM_OF_PXOBJS);

#define COREN_NUM_OF_PXOBJS 40

#define CORE0_NUM_OF_PXOBJS (COREN_NUM_OF_PXOBJS)
#define CORE1_NUM_OF_PXOBJS (COREN_NUM_OF_PXOBJS)
#define CORE2_NUM_OF_PXOBJS (COREN_NUM_OF_PXOBJS)

DEF_SYM(__NUM_OF_PXOBJS__CPU0_, CORE0_NUM_OF_PXOBJS);
DEF_SYM(__NUM_OF_PXOBJS__CPU1_, CORE1_NUM_OF_PXOBJS);
DEF_SYM(__NUM_OF_PXOBJS__CPU2_, CORE2_NUM_OF_PXOBJS);

/*******************************************************************************************************
 * Pragma sections
 *******************************************************************************************************/


// memory class for system objects
// Sysmem:
//	- TCB (400byte per Task)
// Taskmem:
//	- Message boxes (sender task)
// every core gets own system memory in the DSPR section of the core
#pragma section ".CPU0.pxros.sysmem" awB
PxAligned_t Sysmem_CORE0[(SYSMEMSIZE_CORE0 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

#pragma section ".CPU1.pxros.sysmem" awB
PxAligned_t Sysmem_CORE1[(SYSMEMSIZE_CORE1 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

#pragma section ".CPU2.pxros.sysmem" awB
PxAligned_t Sysmem_CORE2[(SYSMEMSIZE_CORE2 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

//Zugriff auf die lokalen SPeicher kann dem betroffenen Core einige Zyklen kosten
//Um eine Beeinflussung zu vermeiden, sollten alle Objekte, die zwischen Cores geteilt werden
//i.e. insbesondere Nachrichtenobjkte, im LMU abgelegt werden
#pragma section ".GLOBAL.pxros.taskmem" awB
PxAligned_t Taskmem_Core0[(TASKMEMSIZE_CORE0 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
PxAligned_t Taskmem_Core1[(TASKMEMSIZE_CORE1 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
PxAligned_t Taskmem_Core2[(TASKMEMSIZE_CORE2 + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section

// global object memory
// memory for global objects, initialised by core 0
// memory is currently allocated in Linkerfile
#pragma section ".GLOBAL.pxros.objmem" awB
//PxAligned_t GlobalObjmem[(GLOBALOBJMEMSIZE + sizeof(PxAligned_t) - 1)/sizeof(PxAligned_t)] __attribute__ ((aligned(8)));
#pragma section



/*******************************************************************************************************
 * System (Kernel) Protection Areas
 *******************************************************************************************************/
/* CPU0 Protection description */
const PxCodeProtectSet_T _cpu0_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{(PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{(PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END },

				//Range 2 - the interrupt table
				{(PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END },

				//Range 4 - not used
				{(PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED},
		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0}

};


const PxCodeProtectSet_T _cpu0_task_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END } ,

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0 },
};



const PxDataProtectSet_T _cpu0_sys_data_protection = {
		{
		 /* Range 0 read only data */
		 { (PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound },
		  // { (PxUInt_t)0x80000000, (PxUInt_t)PxTricSystemRodataUpperBound },

		 /* Range 1 global data used by all cores, the object list (read/write) */
		 { (PxUInt_t)GLOBAL_SYSTEM_RAM_BEGIN, (PxUInt_t)GLOBAL_SYSTEM_RAM_END },

		 /* Range 2 the CSA area of CPU0 (read/write) and system data including sysmem */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU1 */
		 //Achtung: Object 0 enthalt die Kerneldaten von PxROS!!!!!!! Ehemals PxTricSystemDataLowerBound...
		 { (PxUInt_t)PxGlobalMem_begin_CPU1_, (PxUInt_t)PxGlobalMem_end_CPU1_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU2_, (PxUInt_t)PxGlobalMem_end_CPU2_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 internal Scratch Pad Memory */
		 //{ (PxUInt_t)0x70000000, (PxUInt_t)0x90000000 },
		 { (PxUInt_t)DATA_0_START, (PxUInt_t)DATA_0_END },

		 /* 7 initialize to 0 */
		 { 0,0 }
		},
		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		},
		/* the DPWE 1..5,7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		}
};


	  /* CPU1 Protection description */
const PxCodeProtectSet_T _cpu1_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{ (PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END },

				//Range 2 - the interrupt table
				{ (PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END },

				//Range 4 - not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,
		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0,
	},
};


const PxCodeProtectSet_T _cpu1_task_code_protection = {


		{
				//The Code area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

				//Not used
				{ (PxUInt_t)0, (PxUInt_t)0 },

		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0 }
};


//const PxDataProtectSet_T _cpu1_sys_data_protection = {
//
//
//	//Range 0 - read only data (const)
//	(PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound,
//
//	//Range 1 global data used by all cores
//	(PxUInt_t)GLOBAL_SYSTEM_RAM_BEGIN, (PxUInt_t)GLOBAL_SYSTEM_RAM_END,
//
//	//Range 2 - the CSA area of CPU1
//	(PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END,
//
//	//Range 3 - system data of the kernel
//	(PxUInt_t)PxTricSystemDataLowerBound, (PxUInt_t)PxTricSystemDataUpperBound,
//
//	//Range 4 -  the SFR area
//	(PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END,
//
//	//Range 5 - core local scratch pad RAM
//	(PxUInt_t)DATA_1_START, (PxUInt_t)DATA_1_END,
//
//	//Range 6 - not used
//	(PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED,
//
//	//Range 7 - Reserved for PXROS (RW)
//	(PxUInt_t)0, (PxUInt_t)0,
//
//
//	.dpmr.dpre.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 1,.dp4 = 1,.dp5 = 1,.dp6 = 0,.dp7 = 1,
//	},
//
//	.dpmr.dpwe.bits = {.dp0 = 0,.dp1 = 1,.dp2 = 1,.dp3 = 1,.dp4 = 1,.dp5 = 1,.dp6 = 0,.dp7 = 1,
//	},
//};


const PxDataProtectSet_T _cpu1_sys_data_protection = {
		{
		 /* Range 0 read only data */
		 { (PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound },

		 /* Range 1 global data used by all cores, the object list (read/write) */
		 { (PxUInt_t)GLOBAL_SYSTEM_RAM_BEGIN, (PxUInt_t)GLOBAL_SYSTEM_RAM_END },

		 /* Range 2 the CSA area of CPU0 (read/write) and system data including sysmem */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU0 */
		 //Achtung: Object 0 enthalt die Kerneldaten von PxROS!!!!!!! Ehemals PxTricSystemDataLowerBound...
		 { (PxUInt_t)PxGlobalMem_begin_CPU0_, (PxUInt_t)PxGlobalMem_end_CPU0_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU2_, (PxUInt_t)PxGlobalMem_end_CPU2_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 internal Scratch Pad Memory */
		 { (PxUInt_t)DATA_1_START, (PxUInt_t)DATA_1_END },

		 /* 7 initialize to 0 */
		 { 0,0 }
		},


		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		},
		/* the DPWE 1..5,7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		}
};


	  /* CPU2 Protection description */
const PxCodeProtectSet_T _cpu2_sys_code_protection = {

		{
				//Range 0 - The complete flash area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END },

				//Range 1 - The trap table
				{ (PxUInt_t)__TRAP_TAB_BEGIN, (PxUInt_t)__TRAP_TAB_END} ,

				//Range 2 - the interrupt table
				{ (PxUInt_t)__INT_TAB_BEGIN, (PxUInt_t)__INT_TAB_END} ,

				//Range 4 - not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		},


	.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 1,.dp2 = 1,.dp3 = 0,
	},
};


const PxCodeProtectSet_T _cpu2_task_code_protection = {


		{
				//Core area
				{ (PxUInt_t)__TEXT_BEGIN, (PxUInt_t)__TEXT_END} ,

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED },

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED },

				//Not used
				{ (PxUInt_t)NOT_USED, (PxUInt_t)NOT_USED} ,

		},


		.cpmr.cpxe.bits = {.dp0 = 1,.dp1 = 0,.dp2 = 0,.dp3 = 0,
		},
};



const PxDataProtectSet_T _cpu2_sys_data_protection = {


		{
		 /* Range 0 read only data */
		 { (PxUInt_t)PxTricSystemRodataLowerBound, (PxUInt_t)PxTricSystemRodataUpperBound },

		 /* Range 1 global data used by all cores, the object list (read/write) */
		 { (PxUInt_t)GLOBAL_SYSTEM_RAM_BEGIN, (PxUInt_t)GLOBAL_SYSTEM_RAM_END },

		 /* Range 2 the CSA area of CPU0 (read/write) and system data including sysmem */
		 { (PxUInt_t)__CSA_BEGIN, (PxUInt_t)__CSA_END },

		 /* Range 3 Objectlist of CPU0 */
		 //Achtung: Object 0 enthalt die Kerneldaten von PxROS!!!!!!! Ehemals PxTricSystemDataLowerBound...
		 { (PxUInt_t)PxGlobalMem_begin_CPU0_, (PxUInt_t)PxGlobalMem_end_CPU0_ },

		 /* Range 4 Objectlist of CPU2 */
		 { (PxUInt_t)PxGlobalMem_begin_CPU1_, (PxUInt_t)PxGlobalMem_end_CPU1_ },

		 /* Range 5 the SFR area */
		 { (PxUInt_t)PERIPHERAL_START, (PxUInt_t)PERIPHERAL_END },

		 /* Range 6 internal Scratch Pad Memory */
		 { (PxUInt_t)DATA_2_START, (PxUInt_t)DATA_2_END },

		 /* 7 initialize to 0 */
		 { 0,0 }
		},


		/* the DPRE 0..7 readable */
		.dpmr.dpre.bits = {
				.dp0 = 1, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		},
		/* the DPWE 1..5,7 writable */
		.dpmr.dpwe.bits = {
				.dp0 = 0, .dp1 = 1, .dp2 = 1, .dp3 = 1,
				.dp4 = 1, .dp5 = 1, .dp6 = 1, .dp7 = 1
		}
};



/*******************************************************************************************************
 * Core spec created as array out of the three individual specs
 *******************************************************************************************************/

/* the PXROS System specification */
const PxInitSpec_T InitSpec[PX_NO_OF_CORES] =
{
    [0] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE0,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE0,

        .is_obj_number      = CORE0_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE0,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU0_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU0_,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core0,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE0,

        /* Core0 is the initial core and has not to be started */
        .is_core_start      = 0,

        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU0_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU0_,

        /* the protection definition */
        .is_sys_code = &_cpu0_sys_code_protection,
        .is_sys_data = &_cpu0_sys_data_protection,
        .is_task_code = &_cpu0_task_code_protection,
    },
#if PX_NO_OF_CORES > 1
    [1] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE1,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE1,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU1_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU1_,

        .is_obj_number      = CORE1_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE1,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core1,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE1,

        /* Core1 has to be started */
#ifdef MULTI_CORE
        .is_core_start      = (unsigned int)_start,
#else
        .is_core_start      = 0,
#endif
        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU1_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU1_,

        /* the protection definition */
        .is_sys_code = &_cpu1_sys_code_protection,
        .is_sys_data = &_cpu1_sys_data_protection,
        .is_task_code = &_cpu1_task_code_protection,
    },
    [2] =
    {
        .is_sysmc_type      = PXMcVarsizedAligned,
        .is_sysmc_size      = 8,
        .is_sysmc_blk       = Sysmem_CORE2,
        .is_sysmc_blksize   = SYSMEMSIZE_CORE2,

        .is_obj_number      = CORE2_NUM_OF_PXOBJS,
        .is_obj_namelength  = PXROS_NAMESIZE,
        .is_inittask        = &InitTaskSpec_CORE2,
        .is_monitoring      = PXMonitorMemory,
        .is_schedext        = 0,

        .is_objmc_type      = PXMcVarsizedAligned,
        .is_objlmc_size     = 8,
        .is_objmc_blk       = PxObjmem_CPU2_,
        .is_objmc_blksize   = (PxSize_t)PX_OBJMEMSIZE_CPU2_,

        .is_taskmc_type     = PXMcVarsizedAdjusted,
        .is_taskmc_size     = 8,
        .is_taskmc_blk      = Taskmem_Core2,
        .is_taskmc_blksize  = TASKMEMSIZE_CORE2,

        /* Core2 has to be started */
#ifdef MULTI_CORE
        .is_core_start      = (unsigned int)_start,
#else
        .is_core_start      = 0,
#endif
        /* the system stack */
        .is_system_stack = PXROS_SYSTEM_STACK_BEGIN_CPU2_,
        .is_system_stack_size = (PxUInt_t)PXROS_SYSTEM_STACK_SIZE_CPU2_,


        /* the protection definition */
        .is_sys_code = &_cpu2_sys_code_protection,
        .is_sys_data = &_cpu2_sys_data_protection,
        .is_task_code = &_cpu2_task_code_protection,
    }
#endif /* CORE_COUNT */
};

const PxInitSpecsArray_t InitSpecsArray =
{
    &InitSpec[0],
    &InitSpec[1],
    &InitSpec[2]
};


