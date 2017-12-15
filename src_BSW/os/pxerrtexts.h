#ifndef ___errno__
#define	___errno__

static	char	const * const errtext[]={

#define	_PXWarning	0
#define	_PXLogError	1
#define	_PXError	2
#define	_PXFatal	3

	/* PXERR_NOERROR=	0 */		"no error occured",
	/* PXERR_TIMESLOTLIST_OVERFLOW=1 */		"*TIME*",

	/* PXERR_ABORTSTACK_OVERFLOW=2 */		"Abortstack overflow",
	/* PXERR_ABORT_ILLEGAL_CALL=3 */		"PxExpectAbort: corrupted call chain in task %d",
	/* PXERR_DELAY_ILLDELAY=4 */		"Invalid delay object `%d'",
	/* PXERR_DELAY_USED_BY_INTERRUPT=5 */		"Delay object %d currently in use by interrupt handler",
	/* PXERR_DLQU_ILLELEM=6 */		"Invalid doubly linked queue element `%p'",
	/* PXERR_DLQU_ILLQUEUE=7 */		"Invalid doubly linked queue `%p'",
	/* PXERR_EVENT_ZERO=8 */		"Event must not be zero",
	/* PXERR_GENID_NOT_INITED=9 */		"`PxGenidObtain' called without previous `PxGenidInit'",
	/* PXERR_INIT_ILLALIGN=10 */		"Invalid memory block or size alignment in initialization `%p' `%d'",
	/* PXERR_INIT_ILLMCTYPE=11 */		"Type `%d' for PXMcSystemdefault is different from PXMcVarsized and PXMcVarsizedAdjusted",
	/* PXERR_INIT_NOMEM=12 */		"Not enough memory for initialization",
	/* PXERR_ILL_NULLPOINTER_PARAMETER=13 */		"illegal nullpointer parameter in call",
	/* PXERR_INIT_SEGBOUNDARY=14 */		"Block `%p[%x]' crosses segment boundary",
	/* PXERR_INTERRUPT_ILLINTERRUPT=15 */		"Invalid interrupt object `%d'",
	/* PXERR_INTERRUPT_USED_BY_INTERRUPT=16 */		"Interrupt object %d currently in use by interrupt handler",
	/* PXERR_INTR_ILL=17 */		"Only the installing task may override its installed interrupt handlers",
	/* PXERR_MBX_HNDINSTALLED=18 */		"Mailbox handler already installed",
	/* PXERR_MBX_ILLMBX=19 */		"Invalid maibox `%d' in mailbox command",
	/* PXERR_MBX_ILLMODE=20 */		"Invalid mailbox handler mode `%d'",
	/* PXERR_MBX_LEFTMESSAGES=21 */		"Mailbox delete failed: messages left in mailbox",
	/* PXERR_MBX_PRIVMBXDELETE=22 */		"Mailbox delete failed: private mailbox",
	/* PXERR_MBX_TASKWAITS=23 */		"Mailbox delete failed: task(s) waiting at mailbox",
	/* PXERR_MC_ILLALIGN=24 */		"Invalid memory block or size alignment in memory insert `%p' `%d'",
	/* PXERR_MC_ILLMC=25 */		"Invalid memory class `%d' in memory class operation",
	/* PXERR_MC_ILLSIZE=26 */		"Insufficient block size `%d < %d'",
	/* PXERR_MC_ILLTYPE=27 */		"Invalid memory class type `%d'",
	/* PXERR_MC_INCONSISTENCY=28 */		"Inconsistency in memory class `%d': blk `%p'",
	/* PXERR_MC_NOMEM=29 */		"Insufficient memory to satisfy request",
	/* PXERR_MC_NOTEMPTY=30 */		"Memory class delete failed: not empty",
	/* PXERR_MC_SIZETOOBIG=31 */		"Fixed block size to small to satisfy request `%d < %d'",
	/* PXERR_MC_SEGBOUNDARY=32 */		"Block `%p[%x]' crosses segment boundary",
	/* PXERR_MC_DAMAGED_BLOCK=33 */		"Block %p in memory class %d has been damaged",
	/* PXERR_MC_USE_BUDDY_FOR_MSG_ONLY=34 */		"Use Buddy memory classes for messages only!",
	/* PXERR_MSG_ABORTED=35 */		"Message command aborted",
	/* PXERR_MSG_ILLMSG=36 */		"Invalid message `%d'",
	/* PXERR_MSG_ILLOWNER=37 */		"Invalid message owner `%d'",
	/* PXERR_MSG_ILLSIZE=38 */		"Message size would exceed buffer size",
	/* PXERR_MSG_ILLUSER=39 */		"Invalid user `%d' for message `%d'",
	/* PXERR_MSG_NOMSG=40 */		"Insufficient memory to satisfy message request",
	/* PXERR_MSG_NOT_IMPLEMENTED=41 */		"request not implemented",
	/* PXERR_MSG_RELMBX_INSTALLED=42 */		"release mailbox already installed",
	/* PXERR_MSG_NOT_CLOSED=43 */		"message was not closed before open",
	/* PXERR_MSG_ILL_NEW_DATA=44 */		"Illegal new message data pointer",
	/* PXERR_MSG_ILL_ALIGN=45 */		"Illegal aligned data pointer for PxMsgEnvelop",
	/* PXERR_MSG_ILL_SIZE=46 */		"Illegal data size for PxMsgEnvelop",
	/* PXERR_OBJ_ABORTED=47 */		"Object command aborted",
	/* PXERR_OBJ_ILLOBJ=48 */		"Invalid object `%d'",
	/* PXERR_OBJ_NOOBJ=49 */		"No object available to satisfy request",
	/* PXERR_OPOOL_ILLCAPACITY=50 */		"Insufficient capacity in source object pool",
	/* PXERR_OPOOL_ILLDELETE=51 */		"Object pool delete failed: no superior object pool",
	/* PXERR_OPOOL_ILLOPOOL=52 */		"Invalid object pool `%d'",
	/* PXERR_OPOOL_ILLSRC=53 */		"Source of a real object pool must be real",
	/* PXERR_OPOOL_ILLTYPE=54 */		"Invalid object pool type",
	/* PXERR_OPOOL_TASKWAITS=55 */		"Object pool delete failed: task(s) waiting at object pool",
	/* PXERR_PE_ILLPE=56 */		"Perodic: invalid reference to a periodic object",
	/* PXERR_PXHND_ILLCALL=57 */		"PXROS handler service called by task or interrupt handler",
	/* PXERR_SELF_ABORTED=58 */		"Request aborted",
	/* PXERR_TASK_RETURNS=59 */		"Task %d  returns without `PxExit'",
	/* PXERR_TASK_DYING_WHILE_IN_USE=60 */		"Dying task still in use",
	/* PXERR_TASK_DIESRV_INITIALIZED=61 */		"Exitserver already initialized",
	/* PXERR_TASK_DIESRV_NOT_INITED=62 */		"Exitserver not yet initialized",
	/* PXERR_TASK_ILLCALL=63 */		"Task service called by handler",
	/* PXERR_TASK_ILLPRIO=64 */		"Invalid priority `%d >= %d'",
	/* PXERR_TASK_ILLRDYFUN=65 */		"Invalid ready function detected",
	/* PXERR_TASK_ILLSTACKSPECTYPE=66 */		"Invalid stack type `%d'",
	/* PXERR_TASK_ILLTASK=67 */		"Invalid task `%d'",
	/* PXERR_TASK_SCHEDEXT_NOT_CONFIGURED=68 */		"Task extensions not configured in this PXROS version",
	/* PXERR_TASK_STACKOVERFLOW=69 */		"Stack overflow for task `%d'",
	/* PXERR_TASK_STACKUNKNOWN=70 */		"Stack begin could not be determined: specify stack size",
	/* PXERR_TASK_STKMEM=71 */		"Insufficient memory to allocate task stack",
	/* PXERR_TASK_TCBMEM=72 */		"Insufficient memory to allocate task control block",
	/* PXERR_TASK_ILLPRIV=73 */		"Illegal privilege for tasks",
	/* PXERR_TASK_STACK_SEGBOUNDARY=74 */		"Task stack `%p[%x]' crosses segment boundary",
	/* PXERR_TASK_STACK_ILLALIGN=75 */		"Invalid task stack alignment `%p[%x]'",
	/* PXERR_TO_ILLTO=76 */		"Timeout: invalid reference to a timeout object",
	/* PXERR_TRACE_ILLCTRL=77 */		"Invalid trace control",

	/* PXERR_NAME_UNDEFINED=78 */		"Nameserver: name not defined",
	/* PXERR_NAME_BUFOVERFLOW=79 */		"Nameserver: information buffer too small",
	/* PXERR_NAME_DEFINED=80 */		"Nameserver: name already defined -- use `PxNameRedefine'",
	/* PXERR_NAME_MEM=81 */		"Nameserver: memory shortage",
	/* PXERR_NAME_ILL_REQUEST=82 */		"Nameserver: illegal nameserver request",
	/* PXERR_NAME_NOT_INITIALIZED=83 */		"Nameserver: not yet initialized",

	/* PXERR_MSGREL_INITED=84 */		"MsgRelServer: already initialized",
	/* PXERR_MSGREL_NOT_INITED=85 */		"MsgRelServer: not yet initialized",

	/* PXERR_MEMORY_SHORTAGE=86 */		"Memory shortage",
	/* PXERR_OBJECT_SHORTAGE=87 */		"Object shortage",
	/* PXERR_RESOURCE_SHORTAGE=88 */		"Resource shortage",
	/* PXERR_CONNECTION_FAILED=89 */		"Connection failed",
	/* PXERR_COMMUNICATION_FAILED=90 */		"Communication failed",
	/* PXERR_REQUEST_FAILED=91 */		"Request failed",
	/* PXERR_REQUEST_ILLEGAL=92 */		"Request illegal",
	/* PXERR_REQUEST_INVALID=93 */		"Request invalid",
	/* PXERR_REQUEST_INVALID_PARAMETER=94 */		"Request with invalid parameter",
	/* PXERR_REQUEST_INVALID_CONTEXT=95 */		"Request in invalid context",
	/* PXERR_REQUEST_CONFIGURATION_LIMITATION=96 */		"Request exceeds some configuration limitation",
	/* PXERR_REQUEST_IMPLEMENTATION_RESTRICTION=97 */		"Request violates some implementation restriction",
	/* PXERR_REQUEST_TIMEOUT=98 */		"Request timed out",
	/* PXERR_REQUEST_ABORTED=99 */		"Request aborted",
	/* PXERR_REQUEST_CANCELED=100 */		"Request canceled",
	/* PXERR_SERVICE_NOT_CONFIGURED=101 */		"Service %d has not been configured into your PXROS",
	/* PXERR_ILLEGAL_SERVICE_CALLED=102 */		"Service %d is not defined in PXROS",
	/* PXERR_PROT_NOFREE_ENTRY=103 */		"no free protection entry",
	/* PXERR_PROT_ILL_HANDLE=104 */		"illegal protection handle %d",
	/* PXERR_PROT_ILL_REGION=105 */		"illegal protection region definition",
	/* PXERR_PROT_PERMISSION=106 */		"Tried to set a protection without permission",
	/* PXERR_ILLEGAL_ACCESS=107 */		"Access through parameter pointer %p in size %d is illegal",
	/* PXERR_ACCESS_RIGHT=108 */		"No access right for this service",
	/* PXERR_SYSJOBLIST_INCONSISTENCY=109 */		"Inconsistency in the system job list",
	/* PXERR_GLOBAL_SYSJOBLIST_INCONSISTENCY=110 */		"Inconsistency in the global system job list",
	/* PXERR_GLOBAL_MSG_RELMBX_NOT_INSTALLED=111 */		"release mailbox for global messages not installed",
	/* PXERR_GLOBAL_ILLEGAL_CORE=112 */		"access illegal core",
	/* PXERR_GLOBAL_OBJLIST_INCONSISTENCY=113 */		"Inconsistency between local and global object list initialization",
	0};
#endif
