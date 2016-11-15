/*****************************************************************************/
/*  $Header:: /VXIPnP/Eagle/hp663x2/hp663x2.c 4     5/22/00 5:19p Rickc    $ */
/*  Copyright 1998-2000 Agilent Technologies, Inc.                           */
/*---------------------------------------------------------------------------*/
/*  P&P Driver for HP663x2, Dynamic Measurement dc Sources                                     */
/*  Driver Version: B.00.00                                                  */
/*---------------------------------------------------------------------------*/
/*  This driver is compatible with the following VXIplug&play standards:     */
/*    WIN32 System Framework revision 4.0                                    */
/*    VISA revision 1.0 (does not work with VTL)                             */
/*****************************************************************************/

#define HP663X2_REV_CODE    "B.00.00"                /* Driver Revision */

/*****************************************************************************/
/* Exactly one of the following must be defined for the program to           */
/*   compile correctly.                                                      */
/*                                                                           */
/* #define __hpux          (for compilation for HP-UX)                       */
/* #define WIN32           (for compilation for 32-bit Windows)              */
/*                                                                           */
/* In addition, if WIN32 is defined, _CVI_ may also be defined to indicate   */
/*   that the program is being compiled under LabWindows CVI 4.0 or          */
/*   greater.                                                                */
/*****************************************************************************/

/*****************************************************************************/
/* The following defines are intended to provide OS compatibility among      */
/*   Windows 32-bit and HP-UX C compilers.                                   */
/*****************************************************************************/

#ifdef __hpux
#define _fcvt    fcvt
#endif

#define _huge

/*****************************************************************************/

#include <stdlib.h>		/* prototype for malloc() */
#include <string.h>		/* prototype for strcpy() */
#include <stdio.h>		/* prototype for sprintf() */
#include "visa.h"
#include "hp663x2.h"

#ifdef __hpux
#define _INCLUDE_HPUX_SOURCE
#define _INCLUDE_XOPEN_SOURCE_EXTENDED
#include <sys/time.h>
#else
#include <windows.h>
#endif

#define HP663X2_ERR_MSG_LENGTH 256  /* size of error message buffer */

/*****************************************************************************/
/*  The strings below are used to do the instrument identification in the    */
/*    init routine.  The string matches the first part of the instrument's   */
/*    response to it's *IDN? command.                                        */
/*****************************************************************************/

#define HP_66111A				"66111A"
#define HP_66309B				"66309B"
#define HP_66309D				"66309D"
#define HP_66311A				"66311A"
#define HP_66311B				"66311B"
#define HP_66311D				"66311D"
#define	HP_66312A				"66312A"
#define	HP_66319B				"66319B"
#define	HP_66319D				"66319D"
#define	HP_66321B				"66321B"
#define	HP_66321D				"66321D"
#define	HP_66332A				"66332A"


struct	hp663x2_model
{
    ViString name;
    ViBoolean advMeas;      /* DC only or not */
    ViBoolean arrayMeas;
    ViBoolean senseProt;
    ViBoolean dvm;
    ViBoolean output2;
    ViBoolean outRes;		/* Has output resistance programming */
};

const static ViString szHP = "HEWLETT-PACKARD";
const static ViString szAgilent = "Agilent Technologies";

const static struct hp663x2_model hp663x2_model_table[] =
{
    //		    AdvMeas   Array     SenseProt DVM       Dual Output, Resist
    {0,         VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE},
    {HP_66111A, VI_FALSE, VI_FALSE, VI_TRUE,  VI_FALSE, VI_FALSE, VI_FALSE},
    {HP_66309B, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_TRUE,  VI_FALSE},
    {HP_66309D, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE},
    {HP_66311A, VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE},
    {HP_66311B, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE, VI_FALSE},
    {HP_66311D, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE},
    {HP_66319B, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_TRUE,  VI_TRUE},
    {HP_66319D, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE},
    {HP_66321B, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE, VI_TRUE},
    {HP_66321D, VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_TRUE,  VI_FALSE, VI_TRUE},
    {HP_66312A, VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE},
    {HP_66332A, VI_TRUE,  VI_TRUE,  VI_FALSE, VI_FALSE, VI_FALSE, VI_FALSE}

};

const static ViInt16 hp663x2_model_table_size = sizeof(hp663x2_model_table)/sizeof(struct hp663x2_model);

/*****************************************************************************/
/*  VISA Globals data structure.                                             */
/*  It is desireable to store global variables in VISA rather than the       */
/*    driver.  This avoids having any state variables in the driver.  This   */
/*    structure defines the driver's global data.  Memory is allocated to    */
/*    this structure in the init function, which passes the pointer to VISA, */
/*    and every function which follows retrieves a pointer to it.            */
/*****************************************************************************/
struct hp663x2_globals
{
    ViSession			defRMSession;
    char 				address[256];
    ViInt16				model;

    ViStatus	errNumber;
    char 		errFuncName[40];
    char		errMessage[160];

    ViBoolean	errQueryDetect;
    ViUInt16    interfaceType;

    /**************************/
    /*  Instrument Specific   */
    /**************************/
    ViReal64	currMin;	/* minimum output current */
    ViReal64	currMax;	/* maximum output current */
    ViReal64	curr2Min;	/* minimum output current for second output */
    ViReal64	curr2Max;	/* maximum output current for second output */
    ViReal64	dvmMin;		/* minimum trigger voltage for DVM */
    ViReal64	dvmMax;		/* maximum trigger voltage for DVM */
    ViReal64	ovpMin;		/* minimum over-voltage level */
    ViReal64	ovpMax;		/* maximum over-voltage level */
    ViReal64	tintMin;	/* minimum sample time */
    ViReal64	tintMax;	/* maximum sample time */
    ViReal64	voltMin;	/* minimum output voltage */
    ViReal64	voltMax;	/* maximum output voltage */
    ViReal64	volt2Min;	/* minimum output voltage for second output */
    ViReal64	volt2Max;	/* maximum output voltage for second output */
    ViReal64	resMin;		/* minimum ouput resistance for main output */
    ViReal64	resMax;		/* maximum ouput resistance for main output */


    /* Setting this to VI_TRUE,
     * will prevent I/O from occuring in an SRQ
     * This is needed, because VTL 3.0 doesn't provide
     * an atomic write/read operations.  Don't want
     * to do I/O if the driver is in the middle of
     * a read from the instrument.
     */
    ViBoolean                       blockSrqIO;
    ViInt32                         countSrqIO;


    /******* these variables are for enabling concurrency *****/

    /* this is the time when the instrument will be ready */
    ViUInt32			hwReadyClock;	/* hw settled */
    ViUInt32			cmdReadyClock;	/* cmd processed */

    /* the maximum amt. of time a command takes */
    ViUInt32			cmdTime;	/* command processing */
    ViUInt32			settleTime; /* hw settle */
    ViUInt32			clrTime;	/* reset */
    ViUInt32			testTime;	/* self-test time */
    ViUInt32			measTime;	/* measurement timeout value */

    /* if an error occurred, indicate if err or fault need to be queried */
    ViBoolean 			error_occurred;
    ViBoolean			fault_occurred;
    ViBoolean			pon_occurred;

};  /* end globals */
/******************************************************************************/


#if (defined _WIN32) || (defined __WIN32__)
#define HP663X2_DECLARE_TIME_LOCALS 	SYSTEMTIME st;
#define HP663X2_GET_TIME		GetLocalTime(&st);
#define HP663X2_TIME_FIELDS	st.wMonth, st.wDay, st.wHour, \
					st.wMinute, st.wSecond,	st.wMilliseconds
#else				/* not win32 */
#ifdef _CVI_  		/* LabWindows for Win 3.1 */

#include <utility.h>
#else  /* not win32, not labWindows */
#include <time.h>		/* standard time functions */
#ifdef __hpux

#define HP663X2_DECLARE_TIME_LOCALS 	struct timeval tv; \
							struct timezone tz; \
							struct tm *tmp;
#define HP663X2_GET_TIME		gettimeofday(&tv, &tz); \
							tmp = localtime((time_t*)&tv.tv_sec);
#define HP663X2_TIME_FIELDS	tmp->tm_mon+1, tmp->tm_mday, tmp->tm_hour, \
							tmp->tm_min, tmp->tm_sec, tv.tv_usec/1000
#else				/* not unix, win32, or labwindows,so use ANSI time function */
#define HP663X2_DECLARE_TIME_LOCALS 	struct tm *tmp; time_t seconds;
#define HP663X2_GET_TIME		time(&seconds); \
							tmp = localtime(&seconds);
#define HP663X2_TIME_FIELDS	tmp->tm_mon+1, tmp->tm_mday, tmp->tm_hour, \
							tmp->tm_min, tmp->tm_sec, 0
#endif	/* ifdef __hpux  */
#endif	/* ifdef _CVI_ */
#endif  /* ifdef _WIN32 */


/* constants passed to the waitForCmdDone function */
#define HP663X2_WAIT_CMD_ONLY		0
#define HP663X2_WAIT_HW_SETTLE		1
#define HP663X2_WAIT_RESET_INST		2
#define HP663X2_WAIT_SELF_TEST		3

#define VIREAL64_LSB	1.11022303e-16	/* ~ 1/pow(2,53) */

/*===============================================================
 *
 *  All messages are stored in this area to aid in localization
 *
 *===============================================================
 */

#define HP663X2_MSG_VI_OPEN_ERR \
	"instrumentHandle was zero.  Was the hp663x2_init() successful?"

#define HP663X2_MSG_CONDITION \
	"condition"
/* hp663x2_statCond_Q() */

#define HP663X2_MSG_EVENT \
	"event"
/* hp663x2_statEvent_Q() */

#define HP663X2_MSG_EVENT_HDLR_INSTALLED \
	"event handler is already installed for event happening"
/* hp663x2_statEvent_Q() */

#define HP663X2_MSG_EVENT_HDLR_INST2 \
	"Only 1 handler can be installed at a time."
/* hp663x2_statEvent_Q() */

#define HP663X2_MSG_INVALID_HAPPENING \
	"is not a valid happening."
/* hp663x2_statCond_Q() */
/* hp663x2_statEven_Q() */
/* hp663x2_statEvenHdlr() */
/* hp663x2_statEvenHdlr_Q() */

#define HP663X2_MSG_NOT_QUERIABLE \
	"is not queriable."
/* hp663x2_statCond_Q() */
/* hp663x2_statEven_Q() */


#define HP663X2_MSG_IN_FUNCTION	\
	"in function"
/* hp663x2_error_message() */

#define HP663X2_MSG_INVALID_STATUS \
  	"Parameter 2 is invalid" \
	" in function hp663x2_error_message()."
/* hp663x2_error_message() */

#define HP663X2_MSG_INVALID_STATUS_VALUE \
	"is not a valid viStatus value."
/* hp663x2_error_message() */

#define  HP663X2_MSG_INVALID_VI \
  	"Parameter 1 is invalid" \
	" in function hp663x2_error_message()" \
	".  Using an inactive ViSession may cause this error."	\
	"  Was the instrument driver closed prematurely?"
/* hp663x2_message_query() */

#define HP663X2_MSG_NO_ERRORS \
	"No Errors"
/* hp663x2_error_message() */

#define HP663X2_MSG_SELF_TEST_FAILED \
	"Self test failed."
/* hp663x2_self_test() */

#define HP663X2_MSG_SELF_TEST_PASSED \
	"Self test passed."
/* hp663x2_self_test() */

/* the following messages are used by the functions to check parameters */

#define HP663X2_MSG_BOOLEAN		"Expected 0 or 1; Got %d"
#define HP663X2_MSG_REAL		"Expected %lg to %lg; Got %lg"
#define HP663X2_MSG_INT			"Expected %hd to %hd; Got %hd"
#define HP663X2_MSG_LONG		"Expected %ld to %ld; Got %ld"
#define HP663X2_MSG_LOOKUP		"Error converting string response to integer"
#define HP663X2_MSG_NO_MATCH	"Could not match string %s"

/*
 * static error message
 */

#define VI_ERROR_PARAMETER1_MSG			"Parameter 1 is invalid"
#define VI_ERROR_PARAMETER2_MSG			"Parameter 2 is invalid"
#define VI_ERROR_PARAMETER3_MSG			"Parameter 3 is invalid"
#define VI_ERROR_PARAMETER4_MSG			"Parameter 4 is invalid"
#define VI_ERROR_PARAMETER5_MSG			"Parameter 5 is invalid"
#define VI_ERROR_PARAMETER6_MSG			"Parameter 6 is invalid"
#define VI_ERROR_PARAMETER7_MSG			"Parameter 7 is invalid"
#define VI_ERROR_PARAMETER8_MSG			"Parameter 8 is invalid"
#define VI_ERROR_FAIL_ID_QUERY_MSG		"Instrument IDN does not match."
#define INSTR_ERROR_INV_SESSION_MSG		"ViSession (parmeter 1) was not created by this driver"
#define INSTR_ERROR_NULL_PTR_MSG		"NULL pointer detected"
#define INSTR_ERROR_RESET_FAILED_MSG	"reset failed"
#define INSTR_ERROR_UNEXPECTED_MSG 		"An unexpected error occurred"
#define INSTR_ERROR_DETECTED_MSG		"Instrument Error Detected, call hp663x2_error_query()."
#define INSTR_ERROR_LOOKUP_MSG   		"String not found in table"
#define	INSTR_ERROR_MEAS_TMO_MSG		"A measurement timeout occured"
#define	INSTR_ERROR_PORT_CONFIG_MSG		"The digital port is not configured for the request operation"
#define	INSTR_ERROR_GET_LIMIT_FAILED_MSG	"Instrument failed to return a limit value during init"
#define	INSTR_ERROR_NSUP_FUNC_MSG		"Function call is unsupported by this model"
#define INSTR_ERROR_NSUP_PARAM_MSG		"Function argument is unsupported by this model"

#define	CRD_RI_MODE_OFF			"OFF"
#define	CRD_RI_MODE_LATCHING	"LATC"
#define	CRD_RI_MODE_LIVE		"LIVE"

#define	CRD_DFI_SRC_PREV		""
#define	CRD_DFI_SRC_OFF			"OFF"
#define	CRD_DFI_SRC_QUES		"QUES"
#define	CRD_DFI_SRC_OPER		"OPER"
#define	CRD_DFI_SRC_ESB			"ESB"
#define	CRD_DFI_SRC_RQS			"RQS"

#define	CRD_CURR_DET_DC			"DC"
#define	CRD_CURR_DET_ACDC		"ACDC"

#define	CRD_WINDOW_RECT			"RECT"
#define	CRD_WINDOW_HANN			"HANN"

const	static	ViString	hp663x2_srd_VOLTAGE = "\"VOLT\"";
const	static	ViString	hp663x2_srd_CURRENT = "\"CURR\"";

const	static	ViString	hp663x2_curr_det_table[]=
{
    CRD_CURR_DET_ACDC,
    CRD_CURR_DET_DC,
    NULL
};

const	static	ViString	hp663x2_ri_mode_table[]=
{
    CRD_RI_MODE_OFF,
    CRD_RI_MODE_LATCHING,
    CRD_RI_MODE_LIVE,
    NULL
};

const	static	ViString	hp663x2_dfi_src_table[]=
{
    CRD_DFI_SRC_PREV,
    CRD_DFI_SRC_QUES,
    CRD_DFI_SRC_OPER,
    CRD_DFI_SRC_ESB,
    CRD_DFI_SRC_RQS,
    CRD_DFI_SRC_OFF,
    NULL
};

const	static	ViString	hp663x2_window_type_table[]=
{
    CRD_WINDOW_RECT,
    CRD_WINDOW_HANN,
    NULL
};

/*****************************************************************************/
/*  Error table structure.                                                   */
/*    The first element of the table is the error number, the second is the  */
/*    error message.  Error numbers in the "VISA Generic" section of the     */
/*    table are defined in the VISA header files.  Error numbers in the      */
/*    "Instrument Specific" section are defined in the driver header file.   */
/*    All of the error messages are defined above.                           */
/*****************************************************************************/
struct instrErrStruct
{
    ViStatus errStatus;
    ViString errMessage;
};

const static struct instrErrStruct instrErrMsgTable[] =
{
    { VI_ERROR_PARAMETER1,	VI_ERROR_PARAMETER1_MSG },
    { VI_ERROR_PARAMETER2,	VI_ERROR_PARAMETER2_MSG },
    { VI_ERROR_PARAMETER3,	VI_ERROR_PARAMETER3_MSG },
    { VI_ERROR_PARAMETER4,	VI_ERROR_PARAMETER4_MSG },
    { VI_ERROR_PARAMETER5,	VI_ERROR_PARAMETER5_MSG },
    { VI_ERROR_PARAMETER6,	VI_ERROR_PARAMETER6_MSG },
    { VI_ERROR_PARAMETER7,	VI_ERROR_PARAMETER7_MSG },
    { VI_ERROR_PARAMETER8,	VI_ERROR_PARAMETER8_MSG },
    { VI_ERROR_FAIL_ID_QUERY,	VI_ERROR_FAIL_ID_QUERY_MSG },

    { HP663X2_INSTR_ERROR_INV_SESSION,	INSTR_ERROR_INV_SESSION_MSG },
    { HP663X2_INSTR_ERROR_NULL_PTR,	INSTR_ERROR_NULL_PTR_MSG },
    { HP663X2_INSTR_ERROR_RESET_FAILED,INSTR_ERROR_RESET_FAILED_MSG },
    { HP663X2_INSTR_ERROR_UNEXPECTED,	INSTR_ERROR_UNEXPECTED_MSG },
    { HP663X2_INSTR_ERROR_DETECTED,	INSTR_ERROR_DETECTED_MSG },
    { HP663X2_INSTR_ERROR_LOOKUP,	INSTR_ERROR_LOOKUP_MSG },
    { HP663X2_INSTR_ERROR_MEAS_TMO,	INSTR_ERROR_MEAS_TMO_MSG },
    { HP663X2_INSTR_ERROR_PORT_CONFIG, INSTR_ERROR_PORT_CONFIG_MSG },
    { HP663X2_INSTR_ERROR_GET_LIMIT,	INSTR_ERROR_GET_LIMIT_FAILED_MSG },
    { HP663X2_INSTR_ERROR_NSUP_FUNC, INSTR_ERROR_NSUP_FUNC_MSG },
    { HP663X2_INSTR_ERROR_NSUP_PARAM, INSTR_ERROR_NSUP_PARAM_MSG }
};

/*****************************************************************************/
/*****************************************************************************/
/********************                                     ********************/
/**********                                                         **********/
/****                   SUPPORTING MACROS AND FUNCTIONS                   ****/
/**                                                                         **/
/**-------------------------------------------------------------------------**/
/**     These macros and functions are not exposed to the user, but         **/
/**     are used by other functions in the driver.                          **/
/****                                                                     ****/
/**********                                                         **********/
/********************                                     ********************/
/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
/*  MACRO GET_GLOBALS                                                        */
/*****************************************************************************/
/*  Returns a pointer to the VISA globals storage.  This storage was         */
/*    allocated in init, but it is turned over to VISA to avoid holding      */
/*    state variables in the driver.                                         */
/*  Defines context dependent errors                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession            instrumentHandle                                  */
/*      The instrument's VISA address.                                       */
/*    struct hp663x2_globals  *thisPtr                                        */
/*      A pointer to the VISA globals storage                                */
/*    funcname                                                               */
/*      The name of the function to be stored with the context dependent     */
/*      error information.                                                   */
/*****************************************************************************/

#define GET_GLOBALS(instrumentHandle,thisPtr)                                \
{                                                                            \
    errStatus = viGetAttribute(instrumentHandle, VI_ATTR_USER_DATA,          \
                                                      (ViAddr) &thisPtr);    \
    if (errStatus < VI_SUCCESS)                                              \
        return statusUpdate(instrumentHandle, NULL, errStatus);              \
}

/*****************************************************************************/
/*  MACRO DEBUG_CHK_THIS                                                     */
/*****************************************************************************/
/* don't check the debug pointer all the time!                               */
/*****************************************************************************/
#ifdef DEBUG
#define DEBUG_CHK_THIS(instrumentHandle, thisPtr)						\
{																		\
	ViSession defRM;													\
	/* check for NULL user data */										\
	if( thisPtr==NULL )													\
	{																	\
 		return updateStatus(vi, NULL, HP663X2_INSTR_ERROR_INV_SESSION );\
	}																	\
	/* This should never fail */										\
	errStatus = viGetAttribute(instrumentHandle,						\
		VI_ATTR_RM_SESSION, &defRM);									\
	if( VI_SUCCESS > errStatus )										\
	{																	\
 		return updateStatus( vi, NULL, HP663X2_INSTR_ERROR_UNEXPECTED );\
	}																	\
	if( defRM != thisPtr->defRMSession )								\
	{																	\
 		return updateStatus( vi, NULL, HP663X2_INSTR_ERROR_INV_SESSION );	\
	}																	\
}
#else
#define DEBUG_CHK_THIS(instrumentHandle, thisPtr)
#endif

/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession              instrumentHandle (in)                           */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViStatus               s (in)                                          */
/*      A driver error return code.                                          */
/*****************************************************************************/
static ViStatus statusUpdate(ViSession  instrumentHandle,
                             struct     hp663x2_globals *thisPtr,
                             ViStatus   s )
{
    ViStatus  errStatus;
    ViInt16   eventQ;

    if (thisPtr)
        thisPtr->errNumber = s;

    /*************************************************************/
    /*  If error query detect is set on and the instrument       */
    /*    has not timed out, then ...                            */
    /*  Read the status byte into eventQ and check the error     */
    /*    bytes.                                                 */
    /*  Potential status byte errors are 0x04 (Query Error),     */
    /*    0x08 (Device Dependent Error), 0x10 (Execution Error), */
    /*    and 0x20 (Command Error).                              */
    /*************************************************************/

    if (thisPtr && thisPtr->errQueryDetect && s != VI_ERROR_TMO)
    {
        if ((errStatus = viQueryf(instrumentHandle, "*ESR?\n", "%hd%*t",
                                  &eventQ)) < VI_SUCCESS)
            return VI_ERROR_SYSTEM_ERROR;

        if( (0x04  | 0x08  | 0x10  | 0x20) & eventQ )
            return HP663X2_INSTR_ERROR_DETECTED;
    }

    return s;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_BOOLEAN                                                        */
/*****************************************************************************/
/*  Ref chk_boolean and statusUpdate for info.                               */
/*****************************************************************************/

#define CHK_BOOLEAN(chk_val, err) \
if (chk_boolean(thisPtr, chk_val)) \
    return statusUpdate(instrumentHandle, thisPtr, err);

/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViBoolean              chk_val (in)                                    */
/*      Check this value to make sure it is a legal Boolean.                 */
/*****************************************************************************/
static ViBoolean chk_boolean(struct     hp663x2_globals *thisPtr,
                             ViBoolean  chk_val)
{
    ViChar message[HP663X2_ERR_MSG_LENGTH];

    if ( (chk_val != VI_TRUE) && (chk_val != VI_FALSE) )
    {
        sprintf(message, HP663X2_MSG_BOOLEAN, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_REAL_RANGE                                                     */
/*****************************************************************************/
/*  Ref chk_real_range and statusUpdate for info.                            */
/*****************************************************************************/

#define CHK_REAL_RANGE(chk_val, min, max, err)         \
if (chk_real_range(thisPtr, chk_val, min, max))   \
    return statusUpdate(instrumentHandle, thisPtr, err);


/*****************************************************************************/
/*  Tests to see if a ViReal64 is in range.                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViReal64             chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViReal64             min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViReal64             max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/
static ViBoolean chk_real_range(struct    hp663x2_globals *thisPtr,
                                ViReal64  chk_val,
                                ViReal64  min,
                                ViReal64  max)
{
    ViChar message[HP663X2_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, HP663X2_MSG_REAL, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_INT_RANGE                                                      */
/*****************************************************************************/
/*  Ref chk_int_range and statusUpdate for info.                             */
/*****************************************************************************/

#define CHK_INT_RANGE(chk_val, min, max, err)       \
if (chk_int_range(thisPtr, chk_val, min, max) )      \
    return statusUpdate(instrumentHandle, thisPtr, err);


/*****************************************************************************/
/*  Tests to see if a ViInt16 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt16              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt16              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/
static ViBoolean chk_int_range(struct   hp663x2_globals *thisPtr,
                               ViInt16  chk_val,
                               ViInt16  min,
                               ViInt16  max)
{
    ViChar message[HP663X2_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, HP663X2_MSG_INT, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}        /* ----- end of function ----- */

/*****************************************************************************/
/*  MACRO CHK_LONG_RANGE                                                     */
/*****************************************************************************/
/*  Ref chk_long_range and statusUpdate for info.                            */
/*****************************************************************************/

#define CHK_LONG_RANGE(chk_val, min, max, err)      \
if (chk_long_range(thisPtr, chk_val, min, max))     \
    return statusUpdate(instrumentHandle, thisPtr, err);

/*****************************************************************************/
/*  Tests to see if a ViInt32 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt32              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt32              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt32              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/
static ViBoolean chk_long_range( struct  hp663x2_globals *thisPtr,
                                 ViInt32 chk_val,
                                 ViInt32 min,
                                 ViInt32 max)
{
    ViChar message[HP663X2_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, HP663X2_MSG_LONG, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_ENUM                                                           */
/*****************************************************************************/
/*  Ref chk_enum and statusUpdate for info.                                  */
/*****************************************************************************/

#define CHK_ENUM( chk_val, limit, err )							\
	if (chk_enum( thisPtr, chk_val, limit) )					\
		return statusUpdate(instrumentHandle, thisPtr, err);

/*****************************************************************************/
/* Chk_enum searches for a string in an array of strings.  It is used by     */
/* the CHK_ENUM macro                                                        */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                    */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt16              limit (in)                                        */
/*      The upper limit of the enumeration values.                           */
/*****************************************************************************/
static ViBoolean chk_enum (struct hp663x2_globals *thisPtr,
                           ViInt16 chk_val,
                           ViInt16 limit)
{
    ViChar message[HP663X2_ERR_MSG_LENGTH];

    if ( ( chk_val < 0 ) || (chk_val > limit) )
    {
        sprintf(message, HP663X2_MSG_INT, 0, limit, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;
}        /* ----- end of function ----- */

/*****************************************************************************/
/*  MACRO CHK_ADV_MEAS                                                       */
/*****************************************************************************/
/*  Ref chk_adv_meas and statusUpdate for info.                              */
/*****************************************************************************/

#define CHK_ADV_MEAS(chk_val)															\
	if (chk_adv_meas(thisPtr, chk_val))													\
		return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_PARAM);

/*****************************************************************************/
/* chk_adv_meas determines whether or not a measure/fetch parameter is		 */
/* supported for this model.                                                 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct hp663x2_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*****************************************************************************/

static ViBoolean chk_adv_meas(struct hp663x2_globals *thisPtr,
                              ViInt16 chk_val)
{
    if (!hp663x2_model_table[thisPtr->model].advMeas)
    {
        switch (chk_val)
        {
        case HP663X2_MEAS_DC:
            return VI_FALSE;
            break;
        case HP663X2_MEAS_ACDC:
        case HP663X2_MEAS_HIGH:
        case HP663X2_MEAS_LOW:
        case HP663X2_MEAS_MAX:
        case HP663X2_MEAS_MIN:
            return VI_TRUE;
            break;
        default:
            return VI_FALSE;
        }
    }

    return VI_FALSE;
}

/*----------------------------------------------------------------------------
 *  This function is only called from hp663x2_init.
 *
 *  PARAMETERS
 *    ViSession      vi (in)
 *      Instrument Handle returned by viOpen.
 *    ViSession      defRM (in)
 *      Resource Manager handle from viOpen.
 *    ViStatus       errStatus (in)
 *      A driver error return code.
 *----------------------------------------------------------------------------
 */
static ViStatus _VI_FUNC initError(ViPSession  vi,
                                   ViSession  defRM,
                                   ViStatus   errStatus)
{
    viClose(*vi);
    viClose(defRM);
    vi=VI_NULL;
    return errStatus;
}        /* ----- end of function ----- */


/*----------------------------------------------------------------------------
	This function parses the input idString for the HP manufacuter
	name and a model ID.  The ID is compared to a table of supported
	model for this driver.  If a supported model is found,
	the table index is returned, and the VI_SUCCESS function status is returned.
	Else HP663X2_INSTR_ERROR_LOOKUP is returned.
 *----------------------------------------------------------------------------
 */
static ViStatus findModel (struct	hp663x2_globals *thisPtr,
                           ViPChar	pIdString, /* ID string read from instrument */
                           ViPInt16 index) /* result index */
{
    ViInt16 i;
    ViChar	szMan[256], szModel[256];

    *index = 0;

    if (strlen(pIdString)>256)
        return HP663X2_INSTR_ERROR_LOOKUP;

    sscanf(pIdString, "%[^,], %[^,],", szMan, szModel);

    if (strcmp(szMan, szHP) != 0 && strcmp(szMan, szAgilent) != 0)
        return HP663X2_INSTR_ERROR_LOOKUP;

    for (i = 1; i < hp663x2_model_table_size; i++)
    {
        if (!strcmp (hp663x2_model_table[i].name, szModel))
        {
            *index = i;
            return VI_SUCCESS;
        }
    }

    /* if we got here, we did not find it */
    return HP663X2_INSTR_ERROR_LOOKUP;
}

/*----------------------------------------------------------------------------
     This function searches an array of strings for a specific string and
     returns its index.  If successful, a VI_SUCCESS is returned,
     else HP663X2_INSTR_ERROR_LOOKUP is returned.
 *----------------------------------------------------------------------------
 */
static ViStatus findIndex (struct hp663x2_globals *thisPtr,
                           const char * const array_of_strings[],
                           /*last entry in array must be 0 */
                           const char *string, /* string read from instrument */
                           ViPInt16 index) /* result index */
{
    ViInt16 i;
    ViInt16 my_len;
    ViChar	search_str[20];

    strcpy(search_str, string);

    /* get rid of newline if present in string */
    /* needed because %s includes newline in some VTL's */
    my_len = strlen(search_str);
    if (search_str[my_len - 1] == '\n')
        search_str[my_len - 1] = '\0';

    for (i = 0; array_of_strings[i]; i++)
    {
        if (!strcmp (array_of_strings[i], search_str))
        {
            *index = i;
            return VI_SUCCESS;
        }
    }

    /* if we got here, we did not find it */
    return HP663X2_INSTR_ERROR_LOOKUP;
}

/*-----------------------------------------------------------------------------
 * hp663x2_getMilliSeconds()
 *-----------------------------------------------------------------------------
 *	Parameters:
 *		None
 *  Return Value:
 *		ViUInt32: A number of milliseconds, taken from the system clock
 *
 *  What it does:
 *		This function can be used as a system-independent clock
 *		function.  It calls system-dependent functions to get the
 *		number of milliseconds in the system clock.  By calling
 *		this function twice, you can find the number of milliseconds
 *		that have occurred between the calls.  Note: in the Windows
 *		3.1/LabWindows environment, this number will go up to
 *		86,400,000 and then flip back to 0.  When doing relative
 *		time measurements, be sure to take this into consideration.
 *		In the Win 32 environment, the value will flip back to 0 every
 *		49.7 days approximately.
 *-----------------------------------------------------------------------------
 */
ViUInt32 _VI_FUNC hp663x2_getMilliSeconds(void)
{

#ifdef __hpux

    /* this finds the number of milliseconds since Jan. 1, 1970 */

    struct timeval tv;
    struct timezone tz;
    ViUInt32 retval, msec;

    gettimeofday(&tv, &tz);


    /* get milliseconds */
    msec = ((ViUInt32)(tv.tv_usec)) / ((ViUInt32)1000);

    /* add on the number of seconds */
    /* since gettimeofday() finds the # of seconds since 1970, */
    /* subtract a fixed # from this amount with the knowledge that
    it's now at least 1996.  Otherwise multiplying by 1000 will cause overflow. */

    retval = msec + ((ViUInt32)((tv.tv_sec) - 788400000) * (ViUInt32)1000);

    return retval;

#else		/* not unix */

#if (defined _WIN32) || (defined __WIN32__)

    /* this finds the number of milliseconds since windows was started */

    ViUInt32 retval;

    retval = (ViUInt32)GetTickCount();
    return retval;

#else /* not unix, not win32 */

#ifdef _CVI_

    ViReal64 t;
    ViUInt32 retval;

    /* gets # of seconds in system clock */
    t = Timer();
    t = t * 1000;  /* convert to milliseconds */
    retval = (ViUInt32) t;

    return retval;

#endif /* labwindows */
#endif /* win32 */
#endif /* unix */
}


/*-----------------------------------------------------------------------------
 * setupWaitTime()
 *-----------------------------------------------------------------------------
 *  Parameters:
 *	ViSession vi: the instrument session
 *	ViInt32 waitwhat: did we just send a plain command, a command
 *		that requires hardware settling, or a reset command?
 *	struct hp663x2_globals *thisPtr : specific inst. information
 *		- assume it's already been initialized with
 *		  GET_GLOBALS().
 *  Return Value:
 *	ViStatus : status of the operation
 *
 *  What it does:
 *	used for concurrency enabling: this function should be called
 *	every time a write is done to the instrument - the time at which
 *	the write was performed is stored in a global variable
 * -----------------------------------------------------------------------------
 */
static ViStatus setupWaitTime(ViSession instrumentHandle,
                              ViInt32 waitwhat,
                              struct hp663x2_globals *thisPtr)
{
    if(!thisPtr)
        return HP663X2_INSTR_ERROR_NULL_PTR;

    switch (waitwhat)
    {
    case HP663X2_WAIT_CMD_ONLY:
        thisPtr -> cmdReadyClock = hp663x2_getMilliSeconds() + (thisPtr->cmdTime);
        break;
    case HP663X2_WAIT_HW_SETTLE:
        thisPtr -> cmdReadyClock = hp663x2_getMilliSeconds() + (thisPtr->cmdTime);
        thisPtr -> hwReadyClock = hp663x2_getMilliSeconds() +
                                  thisPtr->cmdTime + thisPtr->settleTime;
        break;
    case HP663X2_WAIT_RESET_INST:
        thisPtr -> cmdReadyClock = hp663x2_getMilliSeconds() + (thisPtr->cmdTime);
        thisPtr -> hwReadyClock = hp663x2_getMilliSeconds() +
                                  thisPtr->cmdTime + thisPtr->clrTime;
        break;
    case HP663X2_WAIT_SELF_TEST:
        thisPtr -> cmdReadyClock = hp663x2_getMilliSeconds() +
                                   (thisPtr->cmdTime);
        thisPtr -> hwReadyClock = hp663x2_getMilliSeconds() +
                                  thisPtr->cmdTime + thisPtr->testTime;
        break;
    default:
        return VI_ERROR_PARAMETER2;
    }

    return VI_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * waitForCmdDone()
 *-----------------------------------------------------------------------------
 *	Parameters:
 *		ViSession instrumentHandle: the instrument session
 *		struct hp663x2_globals *thisPtr : specific inst. information
 *			- assume it's already been initialized with
 *				GET_GLOBALS().
 *	Return Value:
 *		ViStatus : status of the operation
 *
 *	What it does:
 *		used for concurrency enabling: this function should be called
 *		before communicating with the instrument to make sure the
 *		previous command has completed.  It waits until a specific
 *		amount of time has passed since the last command was sent
 *		and then returns.
 *-----------------------------------------------------------------------------
 */
static ViStatus waitForCmdDone(ViSession instrumentHandle, ViInt16 whatfor, struct hp663x2_globals *thisPtr)
{
    ViUInt32 t;

    if(!thisPtr)
        return HP663X2_INSTR_ERROR_NULL_PTR;


    /* only waiting for a command to process, so this is a short time */
    if(whatfor == HP663X2_WAIT_CMD_ONLY)
    {
        t = hp663x2_getMilliSeconds();
        /* two checks - the counter might be close to rolling over, so
        we have to test for this - rare, but possible. */
        while((t < thisPtr->cmdReadyClock)
                || ((t + thisPtr->cmdTime) < (thisPtr->cmdReadyClock + thisPtr->cmdTime)))
            t = hp663x2_getMilliSeconds();
    }
    /* waiting for the hardware to settle takes longer */
    else if(whatfor == HP663X2_WAIT_HW_SETTLE)
    {
        t = hp663x2_getMilliSeconds();
        while((t < thisPtr->hwReadyClock) ||
                ((t + thisPtr->testTime) < (thisPtr->hwReadyClock + thisPtr->testTime)))
            t = hp663x2_getMilliSeconds();
    }

    return VI_SUCCESS;
}


/*----------------------------------------------------------------------------
 * measure_cleanup
 *----------------------------------------------------------------------------
 *
 * This routine is only called by hp663x2_measure when a VISA call returns an error.
 * It tries to put VISA back into a good state where it won't be looking for SRQ,
 * It tries to put the instrument into a good state where it parse new messages,
 * stop any measurements in progress, and not generate SRQ,
 *
 * Returns nothing
 *
 * input:
 *	instrumentHandle			- session
 */
static void measure_cleanup(ViSession instrumentHandle)
{
    ViUInt32	timeOut;

    /* disable SRQ from causing VISA events */
    viDisableEvent(instrumentHandle, VI_EVENT_SERVICE_REQ, VI_QUEUE);

    /* reset the parser */
    viClear(instrumentHandle);

    /* save original bus timeout value */
    viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, &timeOut );

    /* Allow 20 seconds for device clear to complete */
    viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, 20000);

    /* clear SRE, nothing to cause SRQ */
    viPrintf(instrumentHandle,"*SRE 0\n");

    /* restore user bus timeout */
    viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, timeOut);
}

/*
 *----------------------------------------------------------------------------
 *	setupForSrq()
 *----------------------------------------------------------------------------
 *	Parameters:
 *		ViSession instrumentHandle: the instrument session
 *		ViInt16 whatfor: The serial poll result mask.
 *		struct hp663x2_globals *thisPtr : specific inst. information
 *			- assume it's already been initialized with
 *			GET_GLOBALS().
 *
 *	Return Value:
 *		ViStatus : status of the operation
 *
 *	What it does:
 *		used for concurrency enabling: this function should be called
 *		before communicating with the instrument to make sure the
 *		previous command has completed. It sets up the instrument to SRQ.
 *-----------------------------------------------------------------------------
 */
static ViStatus setupForSrq(ViSession instrumentHandle, ViInt16 whatfor, struct hp663x2_globals *thisPtr)
{
    ViStatus errStatus = 0;

    if(!thisPtr)
        return HP663X2_INSTR_ERROR_NULL_PTR;

    errStatus = viPrintf(instrumentHandle,"*SRE %d\n", whatfor);

    if (errStatus < VI_SUCCESS)
        return errStatus;

    errStatus=viEnableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE,VI_NULL);

    return VI_SUCCESS;
}

/*
 * -----------------------------------------------------------------------------
 *	waitForSrq()
 * -----------------------------------------------------------------------------
 *	Parameters:
 *		ViSession instrumentHandle:
 *			the instrument session
 *		ViInt16 whatfor:
 *			The serial poll result mask.
 *		struct hp663x2_globals *thisPtr :
 *			specific inst. information
 *			- assume it's already been initialized with GET_GLOBALS().
 *		ViUInt32 waitTime:
 *			Wait time for SRQ to set.  A VI_ERROR_TMO error is generated if
 *			the wait time expires before SRQ is set.
 *
 *	Return Value:
 *		ViStatus : status of the operation
 *
 *	What it does:
 *		used for concurrency enabling: this function should be called
 *		after communicating with the instrument to make sure the
 *		previous command has completed.  It waits until an SRQ is received,
 *		cleans up, and then returns.
 *
 * -----------------------------------------------------------------------------
 */
static ViStatus waitForSrq(ViSession	instrumentHandle,
                           ViInt16		whatfor,
                           ViUInt32		waitTime)
{
    ViStatus	errStatus = 0;
    ViEventType	eventType;
    ViEvent		context;
    ViInt16		spollResult;

    errStatus = viReadSTB(instrumentHandle, &spollResult);
    if (errStatus < VI_SUCCESS)
        return errStatus;

    while((spollResult & whatfor ) != whatfor)
    {
        errStatus=viWaitOnEvent(instrumentHandle, VI_EVENT_SERVICE_REQ, waitTime, &eventType, &context);

        if (errStatus < VI_SUCCESS)
        {
            /* Since the 663x2 should be ready by now, check the status byte one last time */
            if (errStatus==VI_ERROR_TMO)
            {
                viReadSTB(instrumentHandle,&spollResult);
                if((spollResult & whatfor ) == whatfor)
                {
                    viClose(context);
                    errStatus = viDisableEvent(instrumentHandle, VI_EVENT_SERVICE_REQ, VI_QUEUE);
                    return errStatus;
                }
                else
                {
                    /* set error status to indicate a measurement timeout */
                    errStatus = HP663X2_INSTR_ERROR_MEAS_TMO;
                }
            }
            viClose(context);
            return errStatus;
        }

        /* Always close the context to avoid a memory leak. If the viWaitOnEvent was*/
        /*  unsuccessful, there is no need to close the context as it is not valid. */
        errStatus = viClose(context);
        if (errStatus < VI_SUCCESS)
            return errStatus;

        /* poll STB again */
        errStatus = viReadSTB(instrumentHandle, &spollResult);
        if (errStatus < VI_SUCCESS)
            return errStatus;
    }

    errStatus = viDisableEvent(instrumentHandle, VI_EVENT_SERVICE_REQ, VI_QUEUE);

    return errStatus;
}

/*
 *----------------------------------------------------------------------------
 *	hp663x2_getErrMsgFromNum()
 *----------------------------------------------------------------------------
 *	Parameters:
 *		ViInt32 errnum: IN - the numeric code of the error
 *		ViString errmsg: OUT - the message associated with that code
 *
 *  Return Value:
 *		ViStatus: status of operation
 *
 *  What it does:
 *		gets the error message associated with that errnum for the
 *		ERR? query.
 *----------------------------------------------------------------------------
 */
static ViStatus _VI_FUNC hp663x2_getErrMsgFromNum(ViInt32 errnum, ViChar _VI_FAR error_message[])
{
    /* this is from the table in the manual */
    switch(errnum)
    {
    case 0:
        sprintf(error_message, "No Errors");
        break;

    case -100:
        sprintf(error_message, "Command error");
        break;

    case -101:
        sprintf(error_message, "Invalid character");
        break;

    case -102:
        sprintf(error_message, "Syntax error");
        break;

    case -103:
        sprintf(error_message, "Invalid separator");
        break;

    case -104:
        sprintf(error_message, "Data type error");
        break;

    case -105:
        sprintf(error_message, "GET not allowed");
        break;

    case -108:
        sprintf(error_message, "Parameter not allowed");
        break;

    case -109:
        sprintf(error_message, "Missing parameter");
        break;

    case -112:
        sprintf(error_message, "Program mnemonic too long");
        break;

    case -113:
        sprintf(error_message, "Undefined header");
        break;

    case -121:
        sprintf(error_message, "Invalid character in number");
        break;

    case -123:
        sprintf(error_message, "Numeric Overflow");
        break;

    case -124:
        sprintf(error_message, "Too many digits");
        break;

    case -128:
        sprintf(error_message, "Numeric data not allowed");
        break;

    case -131:
        sprintf(error_message, "Invalid suffix");
        break;

    case -138:
        sprintf(error_message, "Suffix not allowed");
        break;

    case -141:
        sprintf(error_message, "Invalid character data");
        break;

    case -144:
        sprintf(error_message, "Character data too long");
        break;

    case -148:
        sprintf(error_message, "Character data not allowed");
        break;

    case -150:
        sprintf(error_message, "String data error");
        break;

    case -151:
        sprintf(error_message, "Invalid string data");
        break;

    case -158:
        sprintf(error_message, "String data not allowed");
        break;

    case -160:
        sprintf(error_message, "Block data error");
        break;

    case -161:
        sprintf(error_message, "Invalid block data");
        break;

    case -168:
        sprintf(error_message, "Block data not allowed");
        break;

    case -170:
        sprintf(error_message, "Expression error");
        break;

    case -171:
        sprintf(error_message, "Invalid expression");
        break;

    case -178:
        sprintf(error_message, "Expression data not allowed");
        break;

    case -200:
        sprintf(error_message, "Execution error");
        break;

    case -222:
        sprintf(error_message, "Data out of range");
        break;

    case -223:
        sprintf(error_message, "Too much data");
        break;

    case -224:
        sprintf(error_message, "Illegal parameter value");
        break;

    case -225:
        sprintf(error_message, "Out of memory");
        break;

    case -241:
        sprintf(error_message, "Hardware missing");
        break;

    case -270:
        sprintf(error_message, "Macro error");
        break;

    case -272:
        sprintf(error_message, "Macro execution error");
        break;

    case -273:
        sprintf(error_message, "Illegal macro label");
        break;

    case -276:
        sprintf(error_message, "Macro recursion error");
        break;

    case -277:
        sprintf(error_message, "Macro redefinition not allowed");
        break;

    case -310:
        sprintf(error_message, "System error");
        break;

    case -350:
        sprintf(error_message, "Too many errors");
        break;

    case -400:
        sprintf(error_message, "Query error");
        break;

    case -410:
        sprintf(error_message, "Query INTERRUPTED");
        break;

    case -420:
        sprintf(error_message, "Query UNTERMINATED");
        break;

    case -430:
        sprintf(error_message, "Query DEADLOCKED");
        break;

    case -440:
        sprintf(error_message, "Query UNTEMINATED");
        break;

    case HP663X2_INSTR_ERROR_NVRAM_RD0_CS:
        sprintf(error_message, "Non-volatile RAM RD0 section checksum failed");
        break;

    case HP663X2_INSTR_ERROR_NVRAM_CONFIG_CS:
        sprintf(error_message, "Non-volatile RAM CONFIG section checksum failed");
        break;

    case HP663X2_INSTR_ERROR_NVRAM_CAL_CS:
        sprintf(error_message, "Non-volatile RAM CAL section checksum failed");
        break;

    case HP663X2_INSTR_ERROR_NVRAM_STATE_CS:
        sprintf(error_message, "Non-volatile RAM STATE section checksum failed");
        break;

    case HP663X2_INSTR_ERROR_NVRAM_RST_CS:
        sprintf(error_message, "Non-volatile RST section checksum failed");
        break;

    case HP663X2_INSTR_ERROR_RAM_SELFTEST:
        sprintf(error_message, "RAM selftest");
        break;

    case HP663X2_INSTR_ERROR_DAC_SELFTEST1:
        sprintf(error_message, "VDAC/IDAC selftest 1");
        break;

    case HP663X2_INSTR_ERROR_DAC_SELFTEST2:
        sprintf(error_message, "VDAC/IDAC selftest 2");
        break;

    case HP663X2_INSTR_ERROR_DAC_SELFTEST3:
        sprintf(error_message, "VDAC/IDAC selftest 3");
        break;

    case HP663X2_INSTR_ERROR_DAC_SELFTEST4:
        sprintf(error_message, "VDAC/IDAC selftest 4");
        break;

    case HP663X2_INSTR_ERROR_OVDAC_SELFTEST:
        sprintf(error_message, "OVDAC selftest");
        break;

    case HP663X2_INSTR_ERROR_DIGIO_SELFTEST:
        sprintf(error_message, "Digital I/O selftest error");
        break;

    case HP663X2_INSTR_ERROR_INGUARD_RXBUF_OVR:
        sprintf(error_message, "Ingrd receiver buffer overrun");
        break;

    case HP663X2_INSTR_ERROR_RS232_FRAMING:
        sprintf(error_message, "RS-232 receiver framing error");
        break;

    case HP663X2_INSTR_ERROR_RS232_PARITY:
        sprintf(error_message, "RS-232 receiver parity error");
        break;

    case HP663X2_INSTR_ERROR_RS232_RX_OVR:
        sprintf(error_message, "RS-232 receiver overrun error");
        break;

    case HP663X2_INSTR_ERROR_FP_UART_OVR:
        sprintf(error_message, "Front panel uart overrun");
        break;

    case HP663X2_INSTR_ERROR_FP_UART_FRAMING:
        sprintf(error_message, "Front panel uart framing");
        break;

    case HP663X2_INSTR_ERROR_FP_UART_PARITY:
        sprintf(error_message, "Front panel uart parity");
        break;

    case HP663X2_INSTR_ERROR_FP_RXBUF_OVR:
        sprintf(error_message, "Front panel buffer overrun");
        break;

    case HP663X2_INSTR_ERROR_FP_TIMEOUT:
        sprintf(error_message, "Front panel timeout");
        break;

    case HP663X2_INSTR_ERROR_CAL_SWITCH:
        sprintf(error_message, "CAL switch prevents calibration");
        break;

    case HP663X2_INSTR_ERROR_CAL_PASSWORD:
        sprintf(error_message, "CAL password is incorrect");
        break;

    case HP663X2_INSTR_ERROR_CAL_DISABLED:
        sprintf(error_message, "CAL not enabled");
        break;

    case HP663X2_INSTR_ERROR_CAL_RB_CONST:
        sprintf(error_message, "Computed readback cal constants are incorrect");
        break;

    case HP663X2_INSTR_ERROR_CAL_PROG_CONST:
        sprintf(error_message, "Computed programming cal constants are incorrect");
        break;

    case HP663X2_INSTR_ERROR_CAL_CMD_SEQUENCE:
        sprintf(error_message, "Incorrect sequence of calibration commands");
        break;

    case HP663X2_INSTR_ERROR_CVCC_STATUS:
        sprintf(error_message, "CV or CC status is incorrect for this command");
        break;

    case HP663X2_INSTR_ERROR_ALC_NOT_NORMAL:
        sprintf(error_message, "Output mode switch mus be in NORMAL position");
        break;

    case HP663X2_INSTR_ERROR_TOO_MANY_SWE_POINTS:
        sprintf(error_message, "Too many sweep points");
        break;

    case HP663X2_INSTR_ERROR_RS232_CMD_ONLY:
        sprintf(error_message, "Command only applies to RS-232 interface");
        break;

    case HP663X2_INSTR_ERROR_INCOMPATIBLE_FETCH:
        sprintf(error_message, "CURRent or VOLTage fetch incompatible with last acquisition");
        break;

    case HP663X2_INSTR_ERROR_MEAS_OVERRANGE:
        sprintf(error_message, "Measurement overrange");
        break;

    default:
        sprintf(error_message, "Unknown Error");
        break;
    }


    return VI_SUCCESS;
}

/*****************************************************************************/
/********************                                     ********************/
/**********                                                         **********/
/****                          DRIVER FUNCTIONS                           ****/
/**                                                                         **/
/**-------------------------------------------------------------------------**/
/**     The functions which are exposed to the driver user start here.      **/
/**     They are in the following order:                                    **/
/**        VPP required functions.                                          **/
/**        HP required utility functions                                    **/
/**        HP passthrough functions                                         **/
/**        Instrument specific functions                                    **/
/****                                                                     ****/
/**********                                                         **********/
/********************                                     ********************/
/*****************************************************************************/

/*----------------------------------------------------------------------------
 * hp663x2_init
 *----------------------------------------------------------------------------
 *   Parameter Name                                       Type    Direction
 *  ------------------------------------------------------------------------
 * | resourceName                                        ViRsrc      IN
 * |   ---------------------------------------------------------------------
 * |  | The Instrument Description.
 * |  |
 * |  | Examples: GPIB0::5::INSTR
 *  ------------------------------------------------------------------------
 * | idQuery                                             ViBoolean   IN
 * |   ---------------------------------------------------------------------
 * |  | if( VI_TRUE) Perform In-System Verification.
 * |  | if(VI_FALSE) Do not perform In-System Verification
 *  ------------------------------------------------------------------------
 * | resetDevice                                         ViBoolean   IN
 * |   ---------------------------------------------------------------------
 * |  | IF( VI_TRUE) Perform Reset Operation.
 * |  | if(VI_FALSE) Do not perform Reset operation
 *  ------------------------------------------------------------------------
 * | instrumentHandle                                    ViPSession  OUT
 * |   ---------------------------------------------------------------------
 * |  | Instrument Handle. This is VI_NULL if an error occurred
 * |  | during the init.
 *----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_init (ViRsrc resourceName,
                                ViBoolean idQuery,
                                ViBoolean resetDevice,
                                ViPSession instrumentHandle)
{
    struct		hp663x2_globals *thisPtr;
    /* added some variables */
    ViStatus	errStatus;
    ViSession	defRM;
    char		idn_buf[HP663X2_CMDSTRING_MAX];
    ViReal64	tempMin, tempMax;

    *instrumentHandle = VI_NULL;

    /*************************************************************/
    /* Find the Default Resource Manager.  Potential errors are  */
    /*   VI_ERROR_SYSTEM_ERROR, VI_ERROR_ALLOC                   */
    /*************************************************************/

    if ((errStatus = viOpenDefaultRM(&defRM)) < VI_SUCCESS)
        return errStatus;

    /*************************************************************/
    /* Open a session to the instrument.  Potential errors are   */
    /*   VI_ERROR_NSUP_OPER, VI_ERROR_INV_RSRC_NAME,             */
    /*   VI_ERROR_INV_ACC_MODE, VI_ERROR_RSRC_NFOUND,            */
    /*   VI_ERROR_ALLOC                                          */
    /*************************************************************/

    if ((errStatus = viOpen(defRM, resourceName,
                            VI_NULL, VI_NULL, instrumentHandle)) < VI_SUCCESS)
    {
        viClose(defRM);
        *instrumentHandle=VI_NULL;
        return errStatus;
    }

    /*************************************************************/
    /* Allocate global memory for the session.  Inititalize the  */
    /*   memory.  Note that viSetAttribute should never fail,    */
    /*   as all of it's input conditions are satisfied.          */
    /*************************************************************/

    if ((thisPtr = (struct hp663x2_globals *)malloc(sizeof(struct hp663x2_globals))) == 0)
    {
        viClose(*instrumentHandle);
        viClose(defRM);
        *instrumentHandle=VI_NULL;
        return VI_ERROR_ALLOC;
    }

    if ((errStatus = viSetAttribute(*instrumentHandle, VI_ATTR_USER_DATA,
                                    (ViAttrState)thisPtr)) < VI_SUCCESS)
    {
        viClose(*instrumentHandle);
        viClose(defRM);
        *instrumentHandle=VI_NULL;
        return errStatus;
    }

#if 0
    /* set read/write attributes */
    errStatus = viSetAttribute(*instrumentHandle, VI_ATTR_RD_BUF_OPER_MODE, VI_FLUSH_ON_ACCESS);
    if( VI_SUCCESS > errStatus)
    {
        viClose( *instrumentHandle);
        viClose( defRM);	/* also closes instrumentHandle session */
        *instrumentHandle = VI_NULL;
        return statusUpdate(*instrumentHandle, NULL, errStatus);
    }
#endif

    /*************************************************************/
    /* Set initial values of variables in global memory.         */
    /*************************************************************/

    /* common variables */
    thisPtr->errNumber = VI_SUCCESS;
    thisPtr->errMessage[0] = 0;
    thisPtr->errQueryDetect = VI_FALSE;
    if ((errStatus = viGetAttribute(*instrumentHandle, VI_ATTR_INTF_TYPE,
                                    &thisPtr->interfaceType)) < VI_SUCCESS)
        return initError(instrumentHandle, defRM, errStatus);

    /* instrument specific variables */
    thisPtr->defRMSession = defRM;
    thisPtr->errFuncName[0] = 0;
    strcpy(thisPtr->address, resourceName);
    thisPtr->blockSrqIO = VI_FALSE;
    thisPtr->countSrqIO = 0;
    thisPtr->cmdTime = HP663X2_CLOCK_W_DISPLAY_ON;
    thisPtr->settleTime = HP663X2_CLOCK_HW_SETTLE;
    thisPtr->clrTime = HP663X2_CLOCK_RESET_INST;
    thisPtr->testTime = HP663X2_CLOCK_SELF_TEST;
    thisPtr->measTime = HP663X2_DEFAULT_MEAS_TIMEOUT ;

    /* ready to accept commands now */
    thisPtr->cmdReadyClock = hp663x2_getMilliSeconds();
    thisPtr->hwReadyClock = hp663x2_getMilliSeconds();

    /* no errors have occurred */
    thisPtr->fault_occurred = VI_FALSE;
    thisPtr->error_occurred = VI_FALSE;
    thisPtr->pon_occurred = VI_FALSE;

    /*************************************************************/
    /* Validate the instrument's identity.                       */
    /*************************************************************/

    if (thisPtr->interfaceType == VI_INTF_GPIB)
    {
        errStatus = viClear(*instrumentHandle);
        if (errStatus < VI_SUCCESS)
            return initError(instrumentHandle, defRM, errStatus);
    }

    errStatus = viQueryf(*instrumentHandle, "*IDN?\n", "%t", idn_buf);
    if (errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, errStatus);

    errStatus = findModel(thisPtr, idn_buf, &thisPtr->model);
    if (idQuery && errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, VI_ERROR_FAIL_ID_QUERY);

    /*************************************************************/
    /* Reset the instrument if so instructed.                    */
    /*   GPIB reset includes a 488.2 device clear.               */
    /*************************************************************/

    if (resetDevice)
    {
        errStatus = hp663x2_reset(*instrumentHandle);
        if (errStatus <  VI_SUCCESS)
            return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_RESET_FAILED);
    }

    /*************************************************************/
    /* initialize maximum ratings of instrument                  */
    /*************************************************************/

    /* get min/max programmable voltage values from instrument */
    errStatus = viQueryf(*instrumentHandle, "VOLT? MIN;VOLT? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
    if (errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
    thisPtr->voltMin = tempMin - tempMin*VIREAL64_LSB;
    thisPtr->voltMax = tempMax + tempMax*VIREAL64_LSB;

    /* get min/max voltage protection values from instrument */
    errStatus = viQueryf(*instrumentHandle, "VOLT:PROT? MIN;PROT? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
    if (errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
    thisPtr->ovpMin = tempMin - tempMin*VIREAL64_LSB;
    thisPtr->ovpMax = tempMax + tempMax*VIREAL64_LSB;

    /* get min/max programmable current values from instrument */
    errStatus = viQueryf(*instrumentHandle, "CURR? MIN;CURR? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
    if (errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
    thisPtr->currMin = tempMin - tempMin*VIREAL64_LSB;
    thisPtr->currMax = tempMax + tempMax*VIREAL64_LSB;

    /* get max sample time from instrument */
    errStatus = viQueryf(*instrumentHandle, "SENS:SWE:TINT? MIN;TINT? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
    if (errStatus < VI_SUCCESS)
        return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
    thisPtr->tintMin = tempMin - tempMin*VIREAL64_LSB;
    thisPtr->tintMax = tempMax + tempMax*VIREAL64_LSB;

    if (hp663x2_model_table[thisPtr->model].output2)
    {
        /* get second output's max programmable voltage */
        errStatus = viQueryf(*instrumentHandle, "VOLT2? MIN;VOLT2? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
        if (errStatus < VI_SUCCESS)
            return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
        thisPtr->volt2Min = tempMin - tempMin*VIREAL64_LSB;
        thisPtr->volt2Max = tempMax + tempMax*VIREAL64_LSB;

        /* get second output's max programmable current */
        errStatus = viQueryf(*instrumentHandle, "CURR2? MIN;CURR2? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
        if (errStatus < VI_SUCCESS)
            return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
        thisPtr->curr2Min = tempMin - tempMin*VIREAL64_LSB;
        thisPtr->curr2Max = tempMax + tempMax*VIREAL64_LSB;
    }

    if (hp663x2_model_table[thisPtr->model].dvm)
    {
        /* get DVM's max trigger level */
        errStatus = viQueryf(*instrumentHandle, "TRIG:ACQ:LEV:DVM? MIN;DVM? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
        if (errStatus < VI_SUCCESS)
            return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
        thisPtr->dvmMin = tempMin - tempMin*VIREAL64_LSB;
        thisPtr->dvmMax = tempMax + tempMax*VIREAL64_LSB;
    }

    if (hp663x2_model_table[thisPtr->model].outRes)
    {
        errStatus = viQueryf(*instrumentHandle, "RES? MIN;RES? MAX\n", "%lf;%lf\n", &tempMin, &tempMax);
        if (errStatus < VI_SUCCESS)
            return initError(instrumentHandle, defRM, HP663X2_INSTR_ERROR_GET_LIMIT);
        thisPtr->resMin = tempMin + tempMin*VIREAL64_LSB;
        thisPtr->resMax = tempMax + tempMax*VIREAL64_LSB;

    }

    return statusUpdate(*instrumentHandle, thisPtr, VI_SUCCESS);
}

/*----------------------------------------------------------------------------
 * 63x2_close
 *----------------------------------------------------------------------------
 *   Parameter Name                                       Type    Direction
 *  ------------------------------------------------------------------------
 * | instrumentHandle                                    ViSession   IN
 * |   ---------------------------------------------------------------------
 * |  | Instrument Handle returned from hp663x2_init()
 *
 *----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_close(ViSession instrumentHandle)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus=0;
    ViSession defRM;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    /* retrieve Resource Management session */
    defRM = thisPtr->defRMSession;

    /* free memory */
    if( thisPtr)
    {
        /* make sure there is something to free */
        free( thisPtr);
    }

    /* close the instrumentHandle and RM sessions */
    return viClose( defRM);
} /*  _close */



/*----------------------------------------------------------------------------
 * hp663x2_reset
 *----------------------------------------------------------------------------
 *    Parameter Name                                       Type    Direction
 *   ------------------------------------------------------------------------
 *  | instrumentHandle                                   ViSession   IN
 *  |   ---------------------------------------------------------------------
 *  |  | Instrument Handle returned from hp663x2_init()
 *
 *----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_reset(ViSession instrumentHandle)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus=0;

    GET_GLOBALS(instrumentHandle, thisPtr)

    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    measure_cleanup(instrumentHandle);

    errStatus = viPrintf(instrumentHandle, "*RST;*CLS\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}/* _reset */



/*----------------------------------------------------------------------------
 * hp663x2_self_test
 *----------------------------------------------------------------------------
 *   Parameter Name                                       Type    Direction
 *  ------------------------------------------------------------------------
 * | instrumentHandle                                   ViSession   IN
 * |   ---------------------------------------------------------------------
 * |  | Instrument Handle returned from hp663x2_init()
 *  ------------------------------------------------------------------------
 * | test_result                                        ViPInt16    OUT
 * |   ---------------------------------------------------------------------
 * |  | Numeric result from self-test operation
 * |  |
 * |  | 0 = no error ( test passed)
 * |  | anything else = failure
 *  ------------------------------------------------------------------------
 * | test_message                                       ViChar _VI_FAR []OUT
 * |   ---------------------------------------------------------------------
 * |  | Self-test status message.  This is limited to 256 characters.
 *
 *----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_self_test (ViSession instrumentHandle,
                                     ViPInt16 testResult,
                                     ViChar _VI_FAR testMessage[])
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus=0;

    /* initialize output parameters */
    *testResult = -1;
    testMessage[0] = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    thisPtr->blockSrqIO = VI_TRUE;

    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    errStatus = viPrintf(instrumentHandle, "*TST?\n");
    if( VI_SUCCESS > errStatus)
    {

        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_SELF_TEST, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    /* read test result */
    /* work around for NI-VISA driver */
    viFlush(instrumentHandle, VI_READ_BUF_DISCARD);

    errStatus = viScanf(instrumentHandle, "%hd", testResult);
    if( VI_SUCCESS > errStatus)
    {
        *testResult = -1;
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }


    errStatus = viPrintf(instrumentHandle,  "SYST:ERR?\n");
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_SELF_TEST, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    /* work around for NI-VISA driver */
    viFlush(instrumentHandle, VI_READ_BUF_DISCARD);

    errStatus = viScanf(instrumentHandle, "%t", testMessage);
    if( VI_SUCCESS > errStatus)
    {
        *testResult = -1;
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}



/*----------------------------------------------------------------------------
 * hp663x2_error_query
 *----------------------------------------------------------------------------
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                    ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | error_number                                        ViPInt32    OUT
  |   ---------------------------------------------------------------------
  |  | Instruments error code
   ------------------------------------------------------------------------
  | error_message                                       ViChar _VI_FAR []OUT
  |   ---------------------------------------------------------------------
  |  | Instrument's error message.  This is limited to 256 characters.
 *----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_error_query (ViSession instrumentHandle,
                                       ViPInt32 errorCode,
                                       ViChar _VI_FAR errorMessage[])
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus=0;

    /* initialize output parameters */
    *errorCode = -1;
    errorMessage[0] = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    errStatus = viQueryf(instrumentHandle, "SYST:ERR?\n", "%ld,%t", errorCode, errorMessage);

    if( VI_SUCCESS > errStatus)
    {
        *errorCode = -1;
        errorMessage[0] = 0;
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    hp663x2_getErrMsgFromNum(*errorCode, errorMessage);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}


/*----------------------------------------------------------------------------
 * hp663x2_error_message
 *----------------------------------------------------------------------------
 *  PARAMETERS
 *    ViSession   instrumentHandle (in)
 *      Instrument Handle returned from hp663x2_init()
 *      May be VI_NULL for this function
 *    ViStatus    statusCode (in)
 *      The error return value from an instrument driver function
 *    ViPString   message[] (out)
 *      Error message string.  This is limited to 256 characters.
 *-----------------------------------------------------------------------------
*/
ViStatus _VI_FUNC hp663x2_error_message (ViSession instrumentHandle,
        ViStatus statusCode,
        ViChar _VI_FAR statusMessage[])
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus=0;
    ViInt32 idx;

    /* initialize output parameters */
    statusMessage[0] = 0;

    thisPtr = NULL;

    /* try to find a thisPtr */
    if( VI_NULL != instrumentHandle)
    {
        GET_GLOBALS(instrumentHandle, thisPtr)
        DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    }

    if( VI_SUCCESS == statusCode)
    {
        sprintf( statusMessage, HP663X2_MSG_NO_ERRORS);
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
    }

    /*************************************************************/
    /* Search the error message table to see if the error is in  */
    /*   that table.  If it is, copy the corresponding error     */
    /*   message to the output error message.  If context        */
    /*   dependent error messages are being used, append the     */
    /*   context dependent information to the message.           */
    /* If the error is found in the table, exit the routine      */
    /*   successfully.                                           */
    /*************************************************************/
    for(idx=0;
            idx < (sizeof instrErrMsgTable / sizeof( struct instrErrStruct));
            idx++)
    {
        /* check for a matching error number */
        if( instrErrMsgTable[idx].errStatus == statusCode)
        {
            if( (thisPtr) && (thisPtr->errNumber == statusCode))
            {
                sprintf( statusMessage,
                         "%s - %s",
                         instrErrMsgTable[idx].errMessage,
                         thisPtr->errMessage);
            }
            else
            {
                strcpy(statusMessage, instrErrMsgTable[idx].errMessage);
            }

            return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
        }
    }

    /*************************************************************/
    /* Check to see if the error is a VTL/VISA error number      */
    /*   using viStatusDesc, which will also return the error    */
    /*   message.  If context dependent error messages are       */
    /*   used, append the context dependent information to the   */
    /*   message.                                                */
    /* If the error is found in the table, exit the routine      */
    /*   successfully.                                           */
    /*************************************************************/
    errStatus = viStatusDesc(instrumentHandle, statusCode, statusMessage);
    if( VI_SUCCESS == errStatus)
    {
        if( (thisPtr) && (thisPtr->errNumber == statusCode))
        {
            strcat( statusMessage, ";  ");
            strcat( statusMessage, thisPtr->errMessage);
        }

        /* VTL found an error message, so return success */
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
    }

    /*************************************************************/
    /*   At this point vi is either INVALID or VI_NULL           */
    /*************************************************************/

    /* if we have a VI_NULL, then we need to return a error message */
    if( VI_NULL == instrumentHandle)
    {
        strcpy(statusMessage, HP663X2_MSG_VI_OPEN_ERR);
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
    }

    /* user passed in an invalid status */
    sprintf( statusMessage,
             HP663X2_MSG_INVALID_STATUS
             "  %ld"
             HP663X2_MSG_INVALID_STATUS_VALUE,
             (long)statusCode );

    return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
}


/****************************************************************************
hp663x2_revision_query
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession      IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | instrumentDriverRevision                                ViChar _VI_FAR []      OUT
  |   ---------------------------------------------------------------------
  |  | Instrument driver revision.  This is limited to 256 characters.
   ------------------------------------------------------------------------
  | firmwareRevision                                 ViChar _VI_FAR []      OUT
  |   ---------------------------------------------------------------------
  |  | Instrument firmware revision.  This is limited to 256 characters.

*****************************************************************************/

ViStatus _VI_FUNC hp663x2_revision_query (ViSession instrumentHandle,
        ViChar _VI_FAR instrumentDriverRevision[],
        ViChar _VI_FAR firmwareRevision[])
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus=0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];		/* temp hold for instr rev string */

    /* initialize output parameters */
    instrumentDriverRevision[0] = 0;
    firmwareRevision[0] = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    sprintf( instrumentDriverRevision, "%s", HP663X2_REV_CODE);

    if(( errStatus = viPrintf(instrumentHandle, "*IDN?\n")) < VI_SUCCESS)
    {
        instrumentDriverRevision[0] = 0;
        firmwareRevision[0] = 0;

        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    /* work around for NI-VISA driver */
    viFlush(instrumentHandle, VI_READ_BUF_DISCARD);

    if(( errStatus = viScanf(instrumentHandle, "%s", szString)) < VI_SUCCESS)
    {
        instrumentDriverRevision[0] = 0;
        firmwareRevision[0] = 0;

        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    strcpy(firmwareRevision, szString);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}


/****************************************************************************
hp663x2_timeOut
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | timeOut                                           ViInt32     IN
  |   ---------------------------------------------------------------------
  |  | This value sets the I/O timeout for all functions in
  |  | the driver. It is specified in milliseconds.

*****************************************************************************/
/* ----------------------------------------------------------------------- */
/* Purpose:  Changes the timeout value of the instrument.  Input is in     */
/*           milliseconds.                                                 */
/* ----------------------------------------------------------------------- */
ViStatus _VI_FUNC hp663x2_timeOut (ViSession instrumentHandle, ViInt32 timeOut)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;


    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_LONG_RANGE( timeOut, 1, 2147483647, VI_ERROR_PARAMETER2 );

    errStatus = viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, timeOut);
    if ( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}


/****************************************************************************
hp663x2_timeOut_Q
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | pTimeOut                                          ViPInt32    OUT
  |   ---------------------------------------------------------------------
  |  | This is the minimum timeout period that the driver
  |  | can be set to. It is specified in milliseconds.

*****************************************************************************/
/* ----------------------------------------------------------------------- */
/* Purpose:  Returns the current setting of the timeout value of the       */
/*           instrument in milliseconds.                                   */
/* ----------------------------------------------------------------------- */
ViStatus _VI_FUNC hp663x2_timeOut_Q (ViSession instrumentHandle, ViPInt32 timeOut)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, timeOut );
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}



/****************************************************************************
hp663x2_errorQueryDetect
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | errorQueryDetect                                  ViBoolean   IN
  |   ---------------------------------------------------------------------
  |  | Boolean which enables (VI_TRUE) or disables (VI_FALSE)
  |  | automatic instrument error querying.

*****************************************************************************/
ViStatus _VI_FUNC hp663x2_errorQueryDetect (ViSession instrumentHandle, ViBoolean errDetect)
/*same for both driver types */
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_BOOLEAN( errDetect, VI_ERROR_PARAMETER2 );

    thisPtr->errQueryDetect = errDetect;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/****************************************************************************
hp663x2_errorQueryDetect_Q
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()
   ------------------------------------------------------------------------
  | firmwareRevision                                        ViPBoolean  OUT
  |   ---------------------------------------------------------------------
  |  | Boolean indicating if automatic instrument error
  |  | querying is performed.

*****************************************************************************/
ViStatus _VI_FUNC hp663x2_errorQueryDetect_Q (ViSession instrumentHandle,
        ViPBoolean errorQueryDetect)
/* same for both types of driver */
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    *errorQueryDetect = thisPtr->errQueryDetect;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}




/****************************************************************************
hp663x2_dcl
*****************************************************************************
    Parameter Name                                       Type    Direction
   ------------------------------------------------------------------------
  | instrumentHandle                                                ViSession   IN
  |   ---------------------------------------------------------------------
  |  | Instrument Handle returned from hp663x2_init()

*****************************************************************************/
ViStatus _VI_FUNC hp663x2_dcl (ViSession instrumentHandle)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viClear(instrumentHandle);
    if( VI_SUCCESS > errStatus)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_abort
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Causes the instrument to cancel any trigger actions in
 *           progress.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_abort(ViSession instrumentHandle)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viPrintf(instrumentHandle,"ABOR\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_outputVoltCurr
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the current limit and voltage, and enables
 *           the output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * IN
 *            The voltage in volts.
 *
 *      MAX = HP663X2_VOLT_MAX   20.475
 *      MIN = HP663X2_VOLT_MIN   0.0
 *
 * PARAM 3 : ViReal64 curr
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   2.0475
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_outputVoltCurr(ViSession instrumentHandle,
        ViReal64 voltLev,
        ViReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_REAL_RANGE(voltLev,
                   thisPtr->voltMin,
                   thisPtr->voltMax,
                   VI_ERROR_PARAMETER2);

    CHK_REAL_RANGE(currLev,
                   thisPtr->currMin,
                   thisPtr->currMax,
                   VI_ERROR_PARAMETER3);

    /* set the levels */
    sprintf(szString, "VOLT %lG;CURR %lG;OUTP 1", voltLev, currLev);
    errStatus = viPrintf(instrumentHandle,"%s\n", szString);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* delay for HW settling */
    errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_outputVoltCurr2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the second output's current limit and voltage,
 *            and enables the output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * IN
 *            The voltage in volts.
 *
 *      MAX = HP663X2_VOLT2_MAX   MAX
 *      MIN = HP663X2_VOLT2_MIN   0.0
 *
 * PARAM 3 : ViReal64 curr
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR2_MAX   MAX
 *      MIN = HP663X2_CURR2_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_outputVoltCurr2(ViSession instrumentHandle,
        ViReal64 voltLev,
        ViReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_REAL_RANGE(voltLev,
                   thisPtr->volt2Min,
                   thisPtr->volt2Max,
                   VI_ERROR_PARAMETER2);

    CHK_REAL_RANGE(currLev,
                   thisPtr->curr2Min,
                   thisPtr->curr2Max,
                   VI_ERROR_PARAMETER3);

    /* set the levels */
    sprintf(szString, "VOLT2 %lG;CURR2 %lG;OUTP 1", voltLev, currLev);
    errStatus = viPrintf(instrumentHandle,"%s\n", szString);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* delay for HW settling */
    errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_CMD_ONLY, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setMeasure
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * IN
 *            The voltage in volts.
 *
 *      MAX = HP663X2_VOLT_MAX   20.475
 *      MIN = HP663X2_VOLT_MIN   0.0
 *
 * PARAM 3 : ViReal64 curr
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   2.0475
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setMeasure (ViSession instrumentHandle,
                                      ViInt16	outputParam,
                                      ViReal64	outputLev,
                                      ViInt16	settlingTime,
                                      ViPReal64 voltQLev,
                                      ViPReal64 currQLev,
                                      ViPInt16	outputStatus)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViUInt32	saveSettleTime;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(outputParam,
                  HP663X2_VOLTAGE,
                  HP663X2_CURRENT,
                  VI_ERROR_PARAMETER2);

    if (outputParam==HP663X2_VOLTAGE)
    {
        CHK_REAL_RANGE(outputLev,
                       thisPtr->voltMin,
                       thisPtr->voltMax,
                       VI_ERROR_PARAMETER3);
    }
    else
    {
        CHK_REAL_RANGE(outputLev,
                       thisPtr->currMin,
                       thisPtr->currMax,
                       VI_ERROR_PARAMETER3);
    }

    CHK_INT_RANGE(settlingTime,
                  HP663X2_HW_SETTLING_MIN,
                  HP663X2_HW_SETTLING_MAX,
                  VI_ERROR_PARAMETER4);

    if (outputParam==HP663X2_VOLTAGE)
        errStatus = viPrintf(instrumentHandle, "VOLT %lG\n", outputLev);
    else
        errStatus = viPrintf(instrumentHandle, "CURR %lG\n", outputLev);

    if (settlingTime)
    {
        /* save old time out value */
        saveSettleTime=thisPtr->settleTime;
        /* setup new delay */
        thisPtr->settleTime=settlingTime;

        errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        if( VI_SUCCESS > errStatus)
        {
            thisPtr->settleTime=saveSettleTime;
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
        }
        errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        thisPtr->settleTime=saveSettleTime;

        if( VI_SUCCESS > errStatus)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    /* do measurements and get status */
    errStatus = hp663x2_measureVolt(instrumentHandle, HP663X2_MEAS_DC, voltQLev);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = hp663x2_measureCurr(instrumentHandle, HP663X2_MEAS_DC, currQLev);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = hp663x2_readOutputStatus(instrumentHandle, outputStatus);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setMeasure2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * IN
 *            The voltage in volts.
 *
 *      MAX = HP663X2_VOLT_MAX   20.475
 *      MIN = HP663X2_VOLT_MIN   0.0
 *
 * PARAM 3 : ViReal64 curr
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   2.0475
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setMeasure2 (ViSession instrumentHandle,
                                       ViInt16	outputParam,
                                       ViReal64	outputLev,
                                       ViInt16	settlingTime,
                                       ViPReal64 voltQLev,
                                       ViPReal64 currQLev,
                                       ViPInt16	outputStatus)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViUInt32	saveSettleTime;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_INT_RANGE(outputParam,
                  HP663X2_VOLTAGE,
                  HP663X2_CURRENT,
                  VI_ERROR_PARAMETER2);

    if (outputParam==HP663X2_VOLTAGE)
    {
        CHK_REAL_RANGE(outputLev,
                       thisPtr->volt2Min,
                       thisPtr->volt2Max,
                       VI_ERROR_PARAMETER3);
    }
    else
    {
        CHK_REAL_RANGE(outputLev,
                       thisPtr->curr2Min,
                       thisPtr->curr2Max,
                       VI_ERROR_PARAMETER3);
    }

    CHK_INT_RANGE(settlingTime,
                  HP663X2_HW_SETTLING_MIN,
                  HP663X2_HW_SETTLING_MAX,
                  VI_ERROR_PARAMETER4);

    if (outputParam==HP663X2_VOLTAGE)
        errStatus = viPrintf(instrumentHandle, "VOLT2 %lG\n", outputLev);
    else
        errStatus = viPrintf(instrumentHandle, "CURR2 %lG\n", outputLev);

    if (settlingTime)
    {
        /* save old time out value */
        saveSettleTime=thisPtr->settleTime;
        /* setup new delay */
        thisPtr->settleTime=settlingTime;

        errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        if( VI_SUCCESS > errStatus)
        {
            thisPtr->settleTime=saveSettleTime;
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
        }
        errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        thisPtr->settleTime=saveSettleTime;

        if( VI_SUCCESS > errStatus)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    /* do measurements and get status */
    errStatus = hp663x2_measureVolt2(instrumentHandle, HP663X2_MEAS_DC, voltQLev);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = hp663x2_measureCurr2(instrumentHandle, HP663X2_MEAS_DC, currQLev);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = hp663x2_readOutputStatus2(instrumentHandle, outputStatus);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_doDelay
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16	milliSeconds
 * IN
 *            The delay time in milliseconds.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_doDelay(ViSession	instrumentHandle,
                                  ViInt16	milliseconds)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViUInt32	saveSettleTime;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(milliseconds,
                  HP663X2_HW_SETTLING_MIN,
                  HP663X2_HW_SETTLING_MAX,
                  VI_ERROR_PARAMETER4);

    if (milliseconds)
    {
        /* save old time out value */
        saveSettleTime=thisPtr->settleTime;
        /* setup new delay */
        thisPtr->settleTime=milliseconds;

        errStatus = setupWaitTime(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        if( VI_SUCCESS > errStatus)
        {
            thisPtr->settleTime=saveSettleTime;
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
        }
        errStatus = waitForCmdDone(instrumentHandle, HP663X2_WAIT_HW_SETTLE, thisPtr);
        thisPtr->settleTime=saveSettleTime;

        if( VI_SUCCESS > errStatus)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_readOutputStatus
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the power source for the output status.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * OUT
 *            The present output status as follow:
 *
 *			  HP663X2_OUTP_OFF		0	Power source output is in the off state.
 *			  HP663X2_OUTP_CV       1   Power source is in Constant Voltage mode.
 *			  HP663X2_OUTP_CC       2   Power source is in Constant Current mode.
 *                                      Either the CC+ or CC- bit is set.
 *            HP663X2_OUTP_UNREG    3   Power source is unregulated.
 *			  HP663X2_OUTP_PROT     4   Protection has tripped (OV, OCP, OT, or RI bit is set).
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_readOutputStatus (ViSession instrumentHandle,
        ViPInt16 outputStatus)

{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "OUTP?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* done if output is Off */
    if (i==HP663X2_OUTP_STATUS_OFF)
    {
        *outputStatus=i;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    errStatus = viQueryf(instrumentHandle, "STAT:QUES:COND?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* check protection conditions */
    if ( i & (HP663X2_QUES_OV | HP663X2_QUES_OCP | HP663X2_QUES_OT | HP663X2_QUES_RI))
    {
        *outputStatus=HP663X2_OUTP_STATUS_PROT;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    /* check unregulated conditions */
    if ( i & HP663X2_QUES_UNREG)
    {
        *outputStatus=HP663X2_OUTP_STATUS_UNREG;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    errStatus = viQueryf(instrumentHandle, "STAT:OPER:COND?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* check CC+, CC- conditions */
    if ( i & (HP663X2_OPER_CC_POS | HP663X2_OPER_CC_NEG))
    {
        *outputStatus=HP663X2_OUTP_STATUS_CC;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    /* check unregulated conditions */
    if ( i & HP663X2_OPER_CV)
    {
        *outputStatus=HP663X2_OUTP_STATUS_CV;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    *outputStatus=HP663X2_OUTP_STATUS_UNKNOWN;
    return statusUpdate(instrumentHandle, thisPtr, errStatus );  /* unknown status ??? */
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_readOutputStatus2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for the second output's
 *            status.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 volt
 * OUT
 *            The present output status as follow:
 *
 *			  HP663X2_OUTP_OFF		0	Power source output is in the off state.
 *			  HP663X2_OUTP_CV       1   Power source is in Constant Voltage mode.
 *			  HP663X2_OUTP_CC       2   Power source is in Constant Current mode.
 *                                      Either the CC+ or CC- bit is set.
 *            HP663X2_OUTP_UNREG    3   Power source is unregulated.
 *			  HP663X2_OUTP_PROT     4   Protection has tripped (OV, OCP, OT, or RI bit is set).
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_readOutputStatus2 (ViSession instrumentHandle,
        ViPInt16 outputStatus)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    errStatus = viQueryf(instrumentHandle, "OUTP?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* done if output is Off */
    if (i==HP663X2_OUTP_STATUS_OFF)
    {
        *outputStatus=i;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    errStatus = viQueryf(instrumentHandle, "STAT:QUES:COND?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* check protection conditions */
    if ( i & (HP663X2_QUES_OCP2 | HP663X2_QUES_OT | HP663X2_QUES_RI))
    {
        *outputStatus=HP663X2_OUTP_STATUS_PROT;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    /* check unregulated conditions */
    if ( i & HP663X2_QUES_UNREG2)
    {
        *outputStatus=HP663X2_OUTP_STATUS_UNREG;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    errStatus = viQueryf(instrumentHandle, "STAT:OPER:COND?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* check CC conditions */
    if ( i & HP663X2_OPER_CC2)
    {
        *outputStatus=HP663X2_OUTP_STATUS_CC;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    /* check unregulated conditions */
    if ( i & HP663X2_OPER_CV2)
    {
        *outputStatus=HP663X2_OUTP_STATUS_CV;
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
    }

    *outputStatus=HP663X2_OUTP_STATUS_UNKNOWN;
    return statusUpdate(instrumentHandle, thisPtr, errStatus );  /* unknown status ??? */
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_rippleRiDfi
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_rippleRiDfi (ViSession instrumentHandle)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViUInt16	i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    /* get Questionable Enable register */
    errStatus = viQueryf(instrumentHandle, "STAT:QUES:ENAB?\n", "%hd%*t",	&i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* mask in RI condition */
    i |= HP663X2_QUES_RI;
    errStatus = viPrintf(instrumentHandle, "STAT:QUES:ENAB %hd\n", i);
    if (errStatus < VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* configure RI/DFI port */

    /* configure digital port for RI/DFI function */
    errStatus = viPrintf(instrumentHandle, "DIG:FUNC RIDF\n");
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* set RI mode */
    errStatus = viPrintf(instrumentHandle, "OUTP:RI:MODE LATC\n");
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* set DFI state and source */
    errStatus = viPrintf(instrumentHandle, "OUTP:DFI:SOUR QUES;STAT 1\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );  /* unknown status ??? */
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setCurr
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the immediate or trigggered current level
 *            of the power source.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 currParam
 * IN
 *            The current parameter select.
 *
 *      HP663X2_CURR        0  the immediate current level
 *      HP663X2_CURR_TRIG   1  the triggered current level
 *
 * PARAM 3 : 64 curr
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   2.0475
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setCurr (ViSession instrumentHandle,
                                   ViInt16 currParam,
                                   ViReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(currParam,
                  HP663X2_CURR_IMM,
                  HP663X2_CURR_TRIG,
                  VI_ERROR_PARAMETER2);


    if (currParam==HP663X2_CURR_IMM)
    {
        CHK_REAL_RANGE(currLev,
                       thisPtr->currMin,
                       thisPtr->currMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "CURR %lG", currLev);
    }
    else
    {
        CHK_REAL_RANGE(currLev,
                       thisPtr->currMin,
                       thisPtr->currMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "CURR:TRIG %lG", currLev);
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getCurr
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for the triggered current
 *           setting.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPReal64 curr
 * OUT
 *            The current in amps.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getCurr (ViSession instrumentHandle,
                                   ViInt16 currQParam,
                                   ViPReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (currQParam)
    {
    case HP663X2_CURR_Q:
        sprintf(szString, "CURR?\n");
        break;
    case HP663X2_CURR_Q_TRIG:
        sprintf(szString, "CURR:TRIG?\n");
        break;
    case HP663X2_CURR_Q_MAX:
        sprintf(szString, "CURR? MAX\n");
        break;
    case HP663X2_CURR_Q_MIN:
        sprintf(szString, "CURR? MIN\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus=viQueryf(instrumentHandle, szString, "%lf", currLev);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setDisplay
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function enables or disables the display.  Disabling
 *            the display makes remote commands complete faster.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean dispState
 * IN
 *            VI_TRUE: enable the display.
 *            VI_FALSE: disable the display.
 *
 * PARAM 3 : ViInt16 dispMode
 * IN
 *            NORMAL 0 display normal instrument function.
 *            TEXT   1 display user message.
 *
 * PARAM 4 : ViString messageText
 * IN
 *            a user message less than 22 characters long.  If a null string is
 *            passed, the message text command is not set.  To clear the current
 *            message text, at least one blank character must be sent.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setDisplay (ViSession instrumentHandle,
                                      ViBoolean dispState,
                                      ViInt16	dispMode,
                                      ViChar	_VI_FAR messageText[])
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];
    ViChar		szFmt[HP663X2_CMDSTRING_MAX];
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_BOOLEAN(dispState, VI_ERROR_PARAMETER2);
    CHK_INT_RANGE(dispMode,
                  HP663X2_DISP_NORMAL,
                  HP663X2_DISP_TEXT,
                  VI_ERROR_PARAMETER3);

    if (dispState==VI_FALSE)
        sprintf(szString, "DISP 0");
    else
    {
        if (dispMode==HP663X2_DISP_NORMAL)
            sprintf(szString, "DISP:STAT 1;MODE NORM");
        else
        {
            i=strlen(messageText);
            if (i)
            {
                /* send the message text only if the string is not null */
                if (i>14) i=14;
                sprintf(szFmt, "%s%ds\"", "%s\"%.", i);
                sprintf(szString, szFmt, "DISP:STAT 1;MODE TEXT;TEXT ", messageText);
            }
            else
                sprintf(szString, "DISP:STAT 1;MODE TEXT");
        }
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getDisplay
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function enables or disables the display.  Disabling
 *            the display makes remote commands complete faster.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean dispState
 * IN
 *            VI_TRUE: enable the display.
 *            VI_FALSE: disable the display.
 *
 * PARAM 3 : ViInt16 dispMode
 * IN
 *            NORMAL 0 display normal instrument function.
 *            TEXT   1 display user message.
 *
 * PARAM 4 : ViString messageText
 * IN
 *            a user message less than 22 characters long.  If a null string is
 *            passed, the message text command is not set.  To clear the current
 *            message text, at least one blank character must be sent.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getDisplay (ViSession		instrumentHandle,
                                      ViPBoolean	dispState,
                                      ViPInt16		dispMode,
                                      ViChar		_VI_FAR messageText[])
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    messageText[0]=0;
    errStatus = viQueryf(instrumentHandle, "DISP:STAT?;MODE?\n","%hd;%s", dispState, szString);

    if (strcmp(szString, "NORM")==0)
        *dispMode=HP663X2_DISP_NORMAL;
    else
        *dispMode=HP663X2_DISP_TEXT;

    errStatus = viQueryf(instrumentHandle, "DISP:TEXT?\n","%t", szString);
    if (errStatus != VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    i=strlen(szString);
    if (i>3)
    {
        i -= 3;
        strncpy(messageText, &szString[1], i);
        messageText[i]=0;
    }
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_arm
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function initializes a trigger sequence of a subsystem.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 sequence
 * IN
 *
 *      Macro Name                 Value  Description
 *      -----------------------------------------------------------
 *      hp663x2_SEQ_ACQUIRE            0  Acquire
 *      hp663x2_SEQ_TRANSIENT          1  Transient
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_arm (ViSession instrumentHandle, ViInt16 trigSystem)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (trigSystem)
    {
    case HP663X2_ARM_OUTPUT_ONCE:
        sprintf(szString, "INIT:NAME TRAN");
        break;
    case HP663X2_ARM_OUTPUT_CONTINUOUS:
        sprintf(szString, "INIT:CONT:SEQ1 1");
        break;
    case HP663X2_ARM_OUTPUT_CONTINUOUS_OFF:
        sprintf(szString, "INIT:CONT:SEQ1 0");
        break;
    case HP663X2_ARM_ACQUISITION:
        sprintf(szString, "INIT:NAME ACQ");
        break;
    case HP663X2_ARM_VOLT_ACQUISITION:
        sprintf(szString, "SENS:FUNC 'VOLT';:INIT:NAME ACQ");
        break;
    case HP663X2_ARM_CURR_ACQUISITION:
        sprintf(szString, "SENS:FUNC 'CURR';:INIT:NAME ACQ");
        break;
    case HP663X2_ARM_DVM_ACQUISITION:
        if (!hp663x2_model_table[thisPtr->model].dvm)
            return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_PARAM);
        sprintf(szString, "SENS:FUNC 'DVM';:INIT:NAME ACQ");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus measure
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function preforms scalar measurements or fetches.
 *
 *-----------------------------------------------------------------------------
 */
static ViStatus _VI_FUNC measure(ViSession	instrumentHandle,
                                 struct		hp663x2_globals *thisPtr,
                                 ViUInt32	timeOut,
                                 ViPString	szMeasCmd,
                                 ViPReal64	resp)
{
    ViStatus	errStatus = 0;

    errStatus=setupForSrq(instrumentHandle, HP663X2_STB_MAV, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    errStatus = viPrintf(instrumentHandle, szMeasCmd);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    errStatus = waitForSrq(instrumentHandle, HP663X2_SRE_MAV, timeOut);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    /* work around for NI-VISA driver */
    viFlush(instrumentHandle, VI_READ_BUF_DISCARD);

    errStatus = viScanf(instrumentHandle, "%lf%*t", resp);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        viFlush(instrumentHandle, VI_READ_BUF_DISCARD);
        return errStatus;
    }

    errStatus = viPrintf(instrumentHandle, "*SRE 0\n");

    return errStatus;
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureVolt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for a voltage measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name              Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC           0    dc voltage
 *      HP663X2_MEAS_ACDC         1    ac+dc rms voltage
 *      HP663X2_MEAS_HIGH         2    high voltage
 *      HP663X2_MEAS_LOW          3    low voltage
 *      HP663X2_MEAS_MAX          4    maximum voltage
 *      HP663X2_MEAS_MIN          5    minimum voltage
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureVolt(ViSession instrumentHandle,
                                      ViInt16 measurement,
                                      ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    CHK_ADV_MEAS(measurement);

    switch (measurement)
    {
    case HP663X2_MEAS_DC:
        pszString="MEAS:VOLT?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="MEAS:VOLT:ACDC?\n";
        break;
    case HP663X2_MEAS_HIGH:
        pszString="MEAS:VOLT:HIGH?\n";
        break;
    case HP663X2_MEAS_LOW:
        pszString="MEAS:VOLT:LOW?\n";
        break;
    case HP663X2_MEAS_MAX:
        pszString="MEAS:VOLT:MAX?\n";
        break;
    case HP663X2_MEAS_MIN:
        pszString="MEAS:VOLT:MIN?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);
    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureCurr
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for a current measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name                Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC             0    dc current
 *      HP663X2_MEAS_ACDC           1    ac+dc rms current
 *      HP663X2_MEAS_HIGH           2    high current
 *      HP663X2_MEAS_LOW            3    low current
 *      HP663X2_MEAS_MAX            4    maximum current
 *      HP663X2_MEAS_MIN            5    minimum current
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureCurr(ViSession instrumentHandle,
                                      ViInt16 measParam,
                                      ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    CHK_ADV_MEAS(measParam);

    switch (measParam)
    {
    case HP663X2_MEAS_DC:
        pszString="MEAS:CURR?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="MEAS:CURR:ACDC?\n";
        break;
    case HP663X2_MEAS_HIGH:
        pszString="MEAS:CURR:HIGH?\n";
        break;
    case HP663X2_MEAS_LOW:
        pszString="MEAS:CURR:LOW?\n";
        break;
    case HP663X2_MEAS_MAX:
        pszString="MEAS:CURR:MAX?\n";
        break;
    case HP663X2_MEAS_MIN:
        pszString="MEAS:CURR:MIN?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);
    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_fetchVolt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for a voltage measurement from
 *				the previous voltage measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name              Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC           0    dc voltage
 *      HP663X2_MEAS_ACDC         1    ac+dc rms voltage
 *      HP663X2_MEAS_HIGH         2    high voltage
 *      HP663X2_MEAS_LOW          3    low voltage
 *      HP663X2_MEAS_MAX          4    maximum voltage
 *      HP663X2_MEAS_MIN          5    minimum voltage
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_fetchVolt(ViSession instrumentHandle,
                                    ViInt16 measParam,
                                    ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    CHK_ADV_MEAS(measParam);

    switch (measParam)
    {
    case HP663X2_MEAS_DC:
        pszString="FETC:VOLT?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="FETC:VOLT:ACDC?\n";
        break;
    case HP663X2_MEAS_HIGH:
        pszString="FETC:VOLT:HIGH?\n";
        break;
    case HP663X2_MEAS_LOW:
        pszString="FETC:VOLT:LOW?\n";
        break;
    case HP663X2_MEAS_MAX:
        pszString="FETC:VOLT:MAX?\n";
        break;
    case HP663X2_MEAS_MIN:
        pszString="FETC:VOLT:MIN?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);
    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_fetchCurr
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for a current measurement
 *            from a previous current measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name                Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC             0    dc current
 *      HP663X2_MEAS_ACDC           1    ac+dc rms current
 *      HP663X2_MEAS_HIGH           2    high current
 *      HP663X2_MEAS_LOW            3    low current
 *      HP663X2_MEAS_MAX            4    maximum current
 *      HP663X2_MEAS_MIN            5    minimum current
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_fetchCurr(ViSession instrumentHandle,
                                    ViInt16 measParam,
                                    ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );
    CHK_ADV_MEAS(measParam);

    switch (measParam)
    {
    case HP663X2_MEAS_DC:
        pszString="FETC:CURR?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="FETC:CURR:ACDC?\n";
        break;
    case HP663X2_MEAS_HIGH:
        pszString="FETC:CURR:HIGH?\n";
        break;
    case HP663X2_MEAS_LOW:
        pszString="FETC:CURR:LOW?\n";
        break;
    case HP663X2_MEAS_MAX:
        pszString="FETC:CURR:MAX?\n";
        break;
    case HP663X2_MEAS_MIN:
        pszString="FETC:CURR:MIN?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);
    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus measureArray
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function performs array measurements.
 *
 *-----------------------------------------------------------------------------
 */
static ViStatus measureArray(ViSession	instrumentHandle,
                             struct		hp663x2_globals *thisPtr,
                             ViReal64	_VI_FAR waveformArray [],
                             ViString	szMeasCmd,
                             ViInt32	numReadings,
                             ViInt32	timeOut)
{
    ViStatus	errStatus=0;

    errStatus=setupForSrq( instrumentHandle, HP663X2_SRE_MAV, thisPtr);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    errStatus = viPrintf(instrumentHandle, szMeasCmd);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    errStatus=waitForSrq(instrumentHandle, HP663X2_SRE_MAV, timeOut);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        return errStatus;
    }

    /* work around for NI-VISA driver */
    viFlush(instrumentHandle, VI_READ_BUF_DISCARD);

    errStatus = viScanf(instrumentHandle, "%,#lf%*t", &numReadings, waveformArray);
    if (errStatus < VI_SUCCESS)
    {
        measure_cleanup(instrumentHandle);
        viFlush(instrumentHandle, VI_READ_BUF_DISCARD);
        return errStatus;
    }

    errStatus = viPrintf(instrumentHandle, "*SRE 0\n");

    return errStatus;
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureVoltArray
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries for the voltage waveform measured.
 *
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 numReadings
 * IN
 *            The number of points for the measurement sweep.
 *
 *      MAX = HP663X2_POINTS_MAX   4096
 *      MIN = HP663X2_POINTS_MIN   0
 *
 * PARAM 3 : ViReal64 interval
 * IN
 *            The sampling interval for the measurement sweep.
 *
 *      MAX = HP663X2_INTERVAL_MAX   0.0003900
 *      MIN = HP663X2_INTERVAL_MIN   0.0000156
 *
 * PARAM 4 : ViInt32 timeOut
 * IN
 *            The value (in milli-seconds) to set the time out value to for
 *            the duration of this function. When the function completes, the
 *            original time out value is restored.
 *
 *      MAX = HP663X2_TIMEOUT_MAX   65535 (65.535 seconds)
 *      MIN = HP663X2_TIMEOUT_MIN   0
 *
 * PARAM 5 : ViReal64 _VI_FAR readings[]
 * OUT
 *            The array of readings in volts.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureVoltArray (ViSession instrumentHandle,
        ViInt32 numReadings,
        ViReal64 _VI_FAR waveformArray [])

{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    if (!hp663x2_model_table[thisPtr->model].advMeas)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_LONG_RANGE(numReadings,
                   HP663X2_SWEEP_SIZE_MIN,
                   HP663X2_SWEEP_SIZE_MAX,
                   VI_ERROR_PARAMETER2);

    errStatus = measureArray(instrumentHandle,
                             thisPtr,
                             waveformArray,
                             "MEAS:ARR:VOLT?\n",
                             numReadings,
                             thisPtr->measTime);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureCurrArray
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries for the current waveform measured.
 *
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 numReadings
 * IN
 *            The number of points for the measurement sweep.
 *
 *      MAX = HP663X2_POINTS_MAX   4096
 *      MIN = HP663X2_POINTS_MIN   0
 *
 * PARAM 3 : ViReal64 interval
 * IN
 *            The sampling interval for the measurement sweep.
 *
 *      MAX = HP663X2_INTERVAL_MAX   0.0003900
 *      MIN = HP663X2_INTERVAL_MIN   0.0000156
 *
 * PARAM 4 : ViInt32 timeOut
 * IN
 *            The value (in milli-seconds) to set the time out value to for
 *            the duration of this function. When the function completes, the
 *            original time out value is restored.
 *
 *      MAX = HP663X2_TIMEOUT_MAX   65535 (65.535 seconds)
 *      MIN = HP663X2_TIMEOUT_MIN   0
 *
 * PARAM 5 : ViReal64 _VI_FAR readings[]
 * OUT
 *            The array of readings in amperes.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureCurrArray (ViSession instrumentHandle,
        ViInt32 numReadings,
        ViReal64 _VI_FAR waveformArray [])

{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );
    if (!hp663x2_model_table[thisPtr->model].advMeas)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_LONG_RANGE(numReadings,
                   HP663X2_SWEEP_SIZE_MIN,
                   HP663X2_SWEEP_SIZE_MAX,
                   VI_ERROR_PARAMETER2);

    errStatus = measureArray(instrumentHandle,
                             thisPtr,
                             waveformArray,
                             "MEAS:ARR:CURR?\n",
                             numReadings,
                             thisPtr->measTime);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_fetchVoltArray
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries for the voltage waveform measured.
 *
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 numReadings
 * OUT
 *            Number of readings in the array.
 *
 * PARAM 3 : ViReal64 _VI_FAR readings[]
 * OUT
 *            The array of readings in volts.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_fetchVoltArray (ViSession instrumentHandle,
        ViInt32 numReadings,
        ViReal64 _VI_FAR waveformArray [])
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );
    if (!hp663x2_model_table[thisPtr->model].advMeas)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_LONG_RANGE(numReadings,
                   HP663X2_SWEEP_SIZE_MIN,
                   HP663X2_SWEEP_SIZE_MAX,
                   VI_ERROR_PARAMETER2);

    errStatus = measureArray(instrumentHandle,
                             thisPtr,
                             waveformArray,
                             "FETC:ARR:VOLT?\n",
                             numReadings,
                             thisPtr->measTime);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_fetchCurrArray
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries for the current waveform measured.
 *
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 numReadings
 * OUT
 *            Number of readings in the array.
 *
 * PARAM 3 : ViReal64 _VI_FAR readings[]
 * OUT
 *            The array of readings in volts.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_fetchCurrArray (ViSession instrumentHandle,
        ViInt32 numReadings,
        ViReal64 _VI_FAR waveformArray [])
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);
    if (!hp663x2_model_table[thisPtr->model].advMeas)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_LONG_RANGE(numReadings,
                   HP663X2_SWEEP_SIZE_MIN,
                   HP663X2_SWEEP_SIZE_MAX,
                   VI_ERROR_PARAMETER2);

    errStatus = measureArray(instrumentHandle,
                             thisPtr,
                             waveformArray,
                             "FETC:ARR:CURR?\n",
                             numReadings,
                             thisPtr->measTime);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_clearOutpProt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function resets overvoltage and overcurrent protection
 *           after one has been tripped.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_clearOutpProt(ViSession instrumentHandle)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viPrintf(instrumentHandle, "OUTP:PROT:CLE\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setOutpRelay
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function opens or closes the output relay contacts.
 *			  It is only valid for units with Option 760, otherwise an
 *			  error will occur. The relay is controlled independently
 *			  of the output state.  If the  instrument is supplying power
 *			  to a load, that power will appear at the relay contacts
 *			  during switching.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean relayState
 * IN
 *			  VI_TRUE: closes relay contacts
 *			  VI_FALSE: opens relay contacts
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setOutpRelay (ViSession instrumentHandle,
                                        ViBoolean relayState)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (relayState)
        errStatus = viPrintf(instrumentHandle, "OUTP:REL 1\n");
    else
        errStatus = viPrintf(instrumentHandle, "OUTP:REL 0\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getOutpRelay
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the state of the output relay contacts.
 *			  It is only valid for units with Option 760, otherwise an error
 *			  will occur. The relay state is independently of the output state.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPBoolean relayState
 * OUT
 *			  VI_TRUE: closes relay contacts
 *			  VI_FALSE: opens relay contacts
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getOutpRelay (ViSession instrumentHandle,
                                        ViPBoolean relayState)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "OUTP:REL?\n", "%ld%*t", relayState);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setOutpRelayPolarity
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean relayState
 * IN
 *			  HP663X2_RELAY_POLARITY_REVERSE: relay contacts reversed
 *			  HP663X2_RELAY_POLARITY_NORMAL: relay contacts normal
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setOutpRelayPolarity (ViSession instrumentHandle,
        ViInt16 relayPolarity)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(relayPolarity,
                  HP663X2_RELAY_POLARITY_NORMAL,
                  HP663X2_RELAY_POLARITY_REVERSE,
                  VI_ERROR_PARAMETER2);

    if (relayPolarity==HP663X2_RELAY_POLARITY_NORMAL)
        errStatus = viPrintf(instrumentHandle, "OUTP:REL:POL NORM\n");
    else
        errStatus = viPrintf(instrumentHandle, "OUTP:REL:POL REV\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getOutpRelayPolarity
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the state of the output relay contacts.
 *			  It is only valid for units with Option 760, otherwise an error
 *			  will occur. The relay state is independently of the output state.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPBoolean relayPolarity
 * OUT
 *			  HP663X2_RELAY_POLARITY_REVERSE: relay contacts reversed
 *			  HP663X2_RELAY_POLARITY_NORMAL: relay contacts normal
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getOutpRelayPolarity (ViSession instrumentHandle,
        ViPInt16 relayPolarity)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];
    ViInt16		iTemp=sizeof(szString);

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "OUTP:REL:POL?\n", "%#t", &iTemp, &szString);

    if (errStatus==VI_SUCCESS)
    {
        if (strcmp(szString, "NORM")==0)
            *relayPolarity=HP663X2_RELAY_POLARITY_NORMAL;
        else
            *relayPolarity=HP663X2_RELAY_POLARITY_REVERSE;
    }
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setRiDfi
 *-----------------------------------------------------------------------------
 *
 * PURPOSE : This function configures the digital control port for Remote
 *           Inhibit/Discrete Fault Interrupt (RI/DFI) operation and sets
 *           the RI/DFI parameters to the specified conditions.
 *           The parameters are saved in non-volatile memory.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 riMode
 * IN
 *			 This parameter selects the operation mode of the
 *           Remote Inhibit protection feature.
 *
 *			 Macro Name           Value  Description
 *			 ---------------------------------------
 *			 HP663X2_RI_OFF			0    off
 *			 HP663X2_RI_LATCHING	1    Latching
 *			 HP663X2_RI_LIVE		2	 Live
 *
 * PARAM 3 : ViBoolean dfiState
 * IN
 *			  Specify if the discrete fault indicator signal is enabled
 *            at the digital control port.
 *
 *			  VI_TRUE: DFI is enabled.
 *			  VI_FALSE: DFI is disabled.
 *
 * PARAM 4 : ViInt16 dfiSourceBit
 * IN
 *
 *			  Specify the source of the DFI signal.
 *
 *			  The internal sources that drive the discrete fault
 *			  indicator signal are:
 *
 *			  MACRO						  Value  Description
 *			  -----------------------------------------------------------------
 *			  HP663X2_DFI_SRC_OFF			0    None, DFI off
 *			  HP663X2_DFI_SRC_QUES			1	 Questionable event summary bit
 *			  HP663X2_DFI_SRC_OPER			2	 Operation Event summary bit
 *			  HP663X2_DFI_SRC_ESB			3	 Standard Event summary bit
 *			  HP663X2_DFI_SRC_RQS			4	 Request for Service bit
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setRiDfi (ViSession instrumentHandle,
                                    ViInt16 riMode,
                                    ViBoolean dfiState,
                                    ViInt16 dfiSourceBit)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViInt16		i;
    ViPString	pszRiMode, pszDfiSourceBit;
    ViChar		szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (riMode)
    {
    case HP663X2_RI_OFF:
        pszRiMode=hp663x2_ri_mode_table[HP663X2_RI_OFF];
        break;
    case HP663X2_RI_LATCHING:
        pszRiMode=hp663x2_ri_mode_table[HP663X2_RI_LATCHING];
        break;
    case HP663X2_RI_LIVE:
        pszRiMode=hp663x2_ri_mode_table[HP663X2_RI_LIVE];
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    CHK_BOOLEAN(dfiState, VI_ERROR_PARAMETER3);
    i = dfiState==VI_TRUE? 1:0;

    switch (dfiSourceBit)
    {
    case HP663X2_DFI_SRC_PREV:
        break;
    case HP663X2_DFI_SRC_QUES:
        pszDfiSourceBit=hp663x2_dfi_src_table[HP663X2_DFI_SRC_QUES];
        break;
    case HP663X2_DFI_SRC_OPER:
        pszDfiSourceBit=hp663x2_dfi_src_table[HP663X2_DFI_SRC_OPER];
        break;
    case HP663X2_DFI_SRC_ESB:
        pszDfiSourceBit=hp663x2_dfi_src_table[HP663X2_DFI_SRC_ESB];
        break;
    case HP663X2_DFI_SRC_RQS:
        pszDfiSourceBit=hp663x2_dfi_src_table[HP663X2_DFI_SRC_RQS];
        break;
    case HP663X2_DFI_SRC_OFF:
        pszDfiSourceBit=hp663x2_dfi_src_table[HP663X2_DFI_SRC_OFF];
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
        break;
    }

    /* configure digital port for RI/DFI function */
    errStatus = viPrintf(instrumentHandle, "DIG:FUNC RIDF\n");
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* set RI mode */
    sprintf(szString, "OUTP:RI:MODE %s\n", pszRiMode);
    errStatus = viPrintf(instrumentHandle, szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    /* set DFI state and source */
    sprintf(szString, "OUTP:DFI:STAT %d\n", i);
    if (dfiState==VI_TRUE)
    {
        if (!(dfiSourceBit==HP663X2_DFI_SRC_PREV))
            sprintf(szString, "OUTP:DFI:SOUR %s;STAT %d\n", pszDfiSourceBit, i);
    }

    errStatus = viPrintf(instrumentHandle, szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getRiDfi
 *-----------------------------------------------------------------------------
 *
 * PURPOSE : This function queries the RI/DFI settings of the power source.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 ponState
 * OUT
 *			 Remote inhibit operating mode.
 *
 *			 Macro Name           Value  Description
 *			 ---------------------------------------
 *			 HP663X2_RI_OFF			0    off
 *			 HP663X2_RI_LATCHING	1    Latching
 *			 HP663X2_RI_LIVE		2	 Live
 *
 * PARAM 3 : ViPBoolean dfiState
 * OUT
 *			  Discrete fault indicator operation state.
 *
 *			  VI_TRUE: DFI is enabled.
 *			  VI_FALSE: DFI is disabled.
 *
 * PARAM 4 : ViPInt16 dfiSourceBit
 * OUT
 *
 *			  Source of discrete faults.
 *
 *			  MACRO						  Value  Description
 *			  -----------------------------------------------------------------
 *			  HP663X2_DFI_SRC_OFF			0    None, DFI off
 *			  HP663X2_DFI_SRC_QUES			1	 Questionable event summary bit
 *			  HP663X2_DFI_SRC_OPER			2	 Operation Event summary bit
 *			  HP663X2_DFI_SRC_ESB			3	 Standard Event summary bit
 *			  HP663X2_DFI_SRC_RQS			4	 Request for Service bit
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getRiDfi (ViSession instrumentHandle,
                                    ViPInt16 riMode,
                                    ViPBoolean dfiState,
                                    ViPInt16 dfiSourceBit)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViInt16		i;
    ViChar		szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    i=sizeof(szString);
    errStatus = viQueryf(instrumentHandle, "DIG:FUNC?\n", "%#s", &i, szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    if (strcmp(szString, "RIDF"))
    {
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_PORT_CONFIG ); /* wrong mode */
    }

    i=sizeof(szString);
    errStatus=viQueryf(instrumentHandle, "OUTP:RI:MODE?\n", "%#s", &i, szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = findIndex(thisPtr, hp663x2_ri_mode_table, szString, &i);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }
    *riMode=i;

    if ((errStatus = viQueryf(instrumentHandle, "OUTP:DFI?\n","%hd",	dfiState)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    i=sizeof(szString);
    errStatus=viQueryf(instrumentHandle, "OUTP:DFI:SOUR?\n", "%#s", &i, szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = findIndex(thisPtr, hp663x2_dfi_src_table, szString, &i);
    if (errStatus==VI_SUCCESS)
    {
        *dfiSourceBit=i;
    }

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
* FUNC    : ViStatus _VI_FUNC hp663x2_setPonState
*-----------------------------------------------------------------------------
*
* PURPOSE : This function selects the power-on state of the instrument.
*			 This information is saved in non-volatile memory.
*
* PARAM 1 : ViSession instrumentHandle
* IN
*            The handle to the instrument.
*
* PARAM 2 : ViInt16 ponState
* IN
*			 This parameter selects the power-on state of the power source.
*
*			 Macro Name               Value  Description
*			 -------------------------------------------
*			 HP663X2_PON_STATE_RST      0    *RST  state
*			 HP663X2_PON_STATE_RCL0     1    *RCL0 State
*
* RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
*           condition. To determine error message, pass the return value to
*           routine "hp663x2_error_message".
*
*-----------------------------------------------------------------------------
*/
ViStatus _VI_FUNC hp663x2_setPonState (ViSession instrumentHandle,
                                       ViInt16 ponState)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(ponState,
                  HP663X2_PON_STATE_RST,
                  HP663X2_PON_STATE_RCL0,
                  VI_ERROR_PARAMETER2);

    if (ponState==HP663X2_PON_STATE_RST)
        errStatus=viPrintf(instrumentHandle, "OUTP:PON:STAT RST\n");
    else
        errStatus=viPrintf(instrumentHandle, "OUTP:PON:STAT RCL0\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getPonState
 *-----------------------------------------------------------------------------
 *
 * PURPOSE : This function queries the power-on state setting of the instrument.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 ponState
 * OUT
 *			 The power-on state of the power source.
 *
 *			 Macro Name               Value  Description
 *			 -------------------------------------------
 *			 HP663X2_PON_STATE_RST      0    *RST  state
 *			 HP663X2_PON_STATE_RCL0     1    *RCL0 State
 *
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getPonState (ViSession instrumentHandle,
                                       ViPInt16 ponState)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus=viQueryf(instrumentHandle, "OUTP:PON:STAT?\n", "%s", &szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    if (strcmp(szString, "RST")==0)
        *ponState = HP663X2_PON_STATE_RST;
    else
        *ponState = HP663X2_PON_STATE_RCL0;

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setOutpTrigSource
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the trigger source of the output subsystem.
 *			  The HP663X2 power source output subsystem supports BUS trigger only.
 *			  It is also the default setting.  Therefore, it is un-necessary to
 *			  program the output trigger source.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 outpTrigSource
 * IN
 *			  The only valid value is HP663X2_TRIG_SRC_BUS (0).
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setOutpTrigSource (ViSession instrumentHandle,
        ViInt16 outpTrigSource)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(outpTrigSource,
                  HP663X2_TRIG_SRC_BUS,
                  HP663X2_TRIG_SRC_BUS,
                  VI_ERROR_PARAMETER2);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getOutpTrigSource
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the trigger source setting of the output subsystem.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPBoolean relayPolarity
 * OUT
 *			  The HP663X2 power source output subsystem supports BUS trigger only.
 *			  Therefore, HP663X2_TRIG_SRC_BUS is always returned.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getOutpTrigSource (ViSession instrumentHandle,
        ViPInt16 outpTrigSource)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    *outpTrigSource=HP663X2_TRIG_SRC_BUS;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setOcpParams
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This command sets turns overcurrent protection on or off and
 *			  and optionally sets the protection delay between the programming
 *			  of an output change that produces a Constant Current
 *            condition and the recording of that condition by the Operation
 *            Status Condition register. The delay prevents the momentary
 *            changes in dc source status that can occur during reprogramming
 *            from being registered as events by the status subsystem. Since
 *            the delay applies to Constant Current status, it also delays the
 *            OverCurrent Protection feature. The OverVoltage Protection
 *            feature is not affected by this delay.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean ocpEnable
 * IN
 *			  VI_TRUE: enables the over-current protection feature
 *			  VI_FALSE: disables the over-current protection feature,
 *						the ccDelay parameter is ignored
 *
 * PARAM 3 : ViReal64 ccDelay
 * IN
 *            The delay in seconds.
 *
 *      MAX = HP663X2_DLY_MAX   2147483.647
 *      MIN = HP663X2_DLY_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setOcpParams (ViSession instrumentHandle,
                                        ViBoolean ocpEnable,
                                        ViReal64 ccDelay)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (ocpEnable)
    {
        CHK_REAL_RANGE(ccDelay,
                       HP663X2_OCP_DELAY_MIN,
                       HP663X2_OCP_DELAY_MAX,
                       VI_ERROR_PARAMETER2);
        errStatus = viPrintf(instrumentHandle, "%s%1G\n", "CURR:PROT:STAT 1;:OUTP:PROT:DEL ", ccDelay);
    }
    else
        errStatus = viPrintf(instrumentHandle, "CURR:PROT:STAT 0\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getOcpParam
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This command queries the time between the programming of an
 *           output change that produces a Constant Voltage or Constant
 *           Current condition and the recording of that condition by the
 *           Operation Status Condition register.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPReal64 delay
 * OUT
 *            The delay in seconds.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getOcpParams (ViSession instrumentHandle,
                                        ViPBoolean ocpEnable,
                                        ViPReal64 ccDelay)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if ((errStatus = viQueryf(instrumentHandle,
                              "CURR:PROT:STAT?\n",
                              "%hd%*t",
                              ocpEnable)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    errStatus = viQueryf(instrumentHandle,
                         "OUTP:PROT:DEL?\n",
                         "%lf%*t",
                         ccDelay);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );

}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setDigio
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function configures the power supply digital control port to
 *			  function as a digital I/O port and outputs the digOutData parameter
 *            to the port.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean digOutData
 * IN
 *			  I/O port value. Only the least 3 significant bits are used.
 *			  Therefore the output value can range from 0 to 7.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setDigio (ViSession instrumentHandle,
                                    ViInt16 digIoData)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(digIoData,
                  HP663X2_DIG_OUT_MIN,
                  HP663X2_DIG_OUT_MAX,
                  VI_ERROR_PARAMETER2);

    errStatus = viPrintf(instrumentHandle, "DIG:FUNC DIG;DATA %d\n", digIoData);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getDigio
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function reads the digital control port when the port is
 *			  configured for Digital I/O operation. Configuring of the port
 *			  is done with the hp663x2_setDigio function.  The query returns
 *			  the last programmed value in bits 0 and 1 and the value read
 *			  at pin 3 in bit 2.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean digInData
 * OUT
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getDigio (ViSession instrumentHandle,
                                    ViPInt16 digIoData)

{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViInt16		i;
    ViChar		szString[HP663X2_CMDSTRING_MAX];


    GET_GLOBALS(instrumentHandle, thisPtr)

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    i=sizeof(szString);
    errStatus = viQueryf(instrumentHandle, "DIG:FUNC?\n", "%#s", &i, &szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    if (strcmp(szString, "DIG"))
    {
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_PORT_CONFIG );
    }

    errStatus = viQueryf(instrumentHandle, "DIG:DATA?\n", "%hd",	digIoData);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getVoltAlcBandwidth
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the setting of the output mode switch.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean alcBandwidth
 * OUT
 *            Output mode switch setting.  The returned value is 15,000 if the
 *			  switch is set to Normal and 60,000 if the switch is set to Fast
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getVoltAlcBandwidth (ViSession instrumentHandle,
        ViPInt32 alcBandwidth)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus=viQueryf(instrumentHandle, "VOLT:ALC:BAND?\n", "%ld", alcBandwidth);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}
/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setOutpState
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function enables or disables the power source output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViBoolean outputState
 * IN
 *            VI_TRUE: enable output.
 *            VI_FALSE: disable output.
 *
 * PARAM 3 : ViBoolean switchRelay
 * IN
 *            VI_TRUE: switch Option 760 output relay.
 *            VI_FALSE: do not switch Option 760 output relay.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setOutpState (ViSession instrumentHandle,
                                        ViBoolean outputState,
                                        ViBoolean switchRelay)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_BOOLEAN(outputState, VI_ERROR_PARAMETER2);
    CHK_BOOLEAN(switchRelay, VI_ERROR_PARAMETER3);

    if (outputState)
        strcpy(szString, "OUTP 1");
    else
        strcpy(szString, "OUTP 0");

    if (!switchRelay)
        strcat(szString, ",NOR");

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getOutpState
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Queries the present output state of the power source output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 outputState
 * OUT
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getOutpState (ViSession instrumentHandle,
                                        ViPBoolean outputState)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle,
                         "OUTP?\n",
                         "%hd%*t",
                         outputState);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getAcqSenseFunc
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries for the measurement sensor setting.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 senseFunction
 * OUT
 *
 *      Macro Name           Value  Description
 *      ---------------------------------------
 *      hp663x2_CURRENT        0    Current
 *      hp663x2_VOLTAGE        1    Voltage
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */

ViStatus _VI_FUNC hp663x2_getAcqSenseFunc(ViSession instrumentHandle,
        ViPInt16 senseFunction)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "SENS:FUNC?\n", "%s", szString);
    if (errStatus < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    if (strcmp(szString, hp663x2_srd_VOLTAGE)==0)
        *senseFunction = HP663X2_VOLTAGE;
    else if (strcmp(szString, hp663x2_srd_CURRENT)==0)
        *senseFunction = HP663X2_CURRENT;
    else
        *senseFunction = HP663X2_DVM;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setCurrSenseParams
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the current sensing range and detector parameters.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 currSenseRange
 * IN
 *
 *			  The dc current measurement range.
 *
 * PARAM 3 : ViInt16 currSenseDetector
 * IN
 *
 *			  The type of detector used for output current measurements.
 *			  The current detectors available in the power source are:
 *
 *			  Macro						Value	Description
 *			  --------------------------------------------------
 *			  HP663X2_CURR_DET_ACDC		  0		AC+DC
 *			  HP663X2_CURR_DET_DC		  1		DC
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setCurrSenseParams (ViSession instrumentHandle,
        ViReal64 currSenseRange,
        ViInt16 currSenseDetector)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_REAL_RANGE(currSenseRange,
                   thisPtr->currMin,
                   thisPtr->currMax,
                   VI_ERROR_PARAMETER2);

    CHK_INT_RANGE(currSenseDetector,
                  HP663X2_CURR_DET_ACDC,
                  HP663X2_CURR_DET_DC,
                  VI_ERROR_PARAMETER3);
    if (!hp663x2_model_table[thisPtr->model].advMeas && currSenseDetector == HP663X2_CURR_DET_ACDC)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_PARAM);

    pszString = hp663x2_curr_det_table[currSenseDetector];

    errStatus = viPrintf(instrumentHandle, "SENS:CURR:RANG %lG;DET %s\n",  currSenseRange, pszString);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getCurrSenseParams
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the current sensing range and detector
 *			  settings.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 currSenseRange
 * OUT
 *
 *			  The dc current measurement range.
 *
 * PARAM 3 : ViPInt16 currSenseDetector
 * OUT
 *
 *			  The type of detector used for output current measurements.
 *			  The current detectors available in the power source are:
 *
 *			  Macro						Value	Description
 *			  --------------------------------------------------
 *			  HP663X2_CURR_DET_ACDC		  0		AC+DC
 *			  HP663X2_CURR_DET_DC		  1		DC
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getCurrSenseParams (ViSession instrumentHandle,
        ViPReal64 currSenseRange,
        ViPInt16 currSenseDetector)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    i=sizeof(szString);
    szString[0]=0;
    errStatus = viQueryf(instrumentHandle, "SENS:CURR:RANG?;DET?\n",  "%lf;%#s",
                         currSenseRange, &i, &szString);

    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    errStatus = findIndex(thisPtr, hp663x2_curr_det_table, szString, &i);
    if (errStatus==VI_SUCCESS)
    {
        *currSenseDetector = i;
    }

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setWindowType
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the windowing function that is used in
 *			  output measurement calculations.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 currSenseRange
 * IN
 *			 The window function type.  The following functions are available:
 *
 *			  Macro						Value	Description
 *			  --------------------------------------------------------
 *			  HP663X2_SENSE_WIND_RECT	  0		The rectangular window
 *			  HP663X2_SENSE_WIND_HANN	  1		The Hanning window
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setWindowType (ViSession instrumentHandle,
        ViInt16 windowType)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_INT_RANGE(windowType,
                  HP663X2_SENSE_WIND_RECT,
                  HP663X2_SENSE_WIND_HANN,
                  VI_ERROR_PARAMETER3);

    if (windowType==HP663X2_SENSE_WIND_RECT)
        errStatus = viPrintf(instrumentHandle, "SENS:WIND RECT\n");
    else
        errStatus = viPrintf(instrumentHandle, "SENS:WIND HANN\n");

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getWindowType
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the windowing function that is used in
 *			  output measurement calculations.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPInt16 currSenseRange
 * OUT
 *			 The window function type.  The following functions are available:
 *
 *			  Macro						Value	Description
 *			  --------------------------------------------------------
 *			  HP663X2_SENSE_WIND_RECT	  0		The rectangular window
 *			  HP663X2_SENSE_WIND_HANN	  1		The Hanning window
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getWindowType (ViSession instrumentHandle,
        ViPInt16 windowType)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];
    ViInt16		i;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "SENS:WIND?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = findIndex(thisPtr, hp663x2_window_type_table, szString, &i);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }
    *windowType=i;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setSweepParams
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the measurement sweep parameters -
 *			  interval, size, and delay offset.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 sweepInterval
 * IN
 *            The time interval for measurement sweeps.
 *
 *      MAX = HP663X2_INTERVAL_MAX   0.0003900
 *      MIN = HP663X2_INTERVAL_MIN   0.0000156
 *
 * PARAM 3 : ViInt32 sweepSize
 * IN
 *            The number of points in an acquisition.
 *
 *      MAX = 4096
 *      MIN = 0
 *
 * PARAM 4 : ViInt32 sweepOffset
 * IN
 *            The time interval for measurement sweeps.
 *
 *      MAX = 2E9 (2,000,000,000)
 *      MIN = -4096
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setSweepParams (ViSession instrumentHandle,
        ViReal64 sweepInterval,
        ViInt32 sweepSize,
        ViInt32 sweepOffset)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViChar		szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_REAL_RANGE(sweepInterval,
                   thisPtr->tintMin,
                   thisPtr->tintMax,
                   VI_ERROR_PARAMETER2);

    CHK_LONG_RANGE(sweepSize,
                   HP663X2_SWEEP_SIZE_MIN,
                   HP663X2_SWEEP_SIZE_MAX,
                   VI_ERROR_PARAMETER3);

    CHK_LONG_RANGE(sweepOffset,
                   HP663X2_SWEEP_OFFSET_MIN,
                   HP663X2_SWEEP_OFFSET_MAX,
                   VI_ERROR_PARAMETER4);

    sprintf(szString, "SENS:SWE:TINT %lG;POIN %ld;OFFS:POIN %ld\n", sweepInterval, sweepSize, sweepOffset);

    errStatus = viPrintf(instrumentHandle, szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getSweepParams
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the measurement sweep parameters -
 *			  interval, size, and delay offset.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViReal64 sweepInterval
 * OUT
 *            The time interval between measurement.
 *
 * PARAM 3 : ViInt32 sweepSize
 * OUT
 *            The number of points in an acquisition.
 *
 * PARAM 4 : ViInt32 sweepOffset
 * OUT
 *            The data logging offset with the sweep.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getSweepParams (ViSession instrumentHandle,
        ViPReal64 sampleInterval,
        ViPInt32 sweepSize,
        ViPInt32 sweepOffset)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "SENS:SWE:TINT?;POIN?;OFFS:POIN?\n",
                         "%lf;%ld;%ld", sampleInterval, sweepSize, sweepOffset);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getMeasTimeout
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Set timeout value for all measurement functions.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 measTimeout
 * INT
 *            The measurement timeout value.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setMeasTimeout (ViSession instrumentHandle,
        ViInt32 measTimeout)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    CHK_LONG_RANGE (measTimeout,
                    HP663X2_TIMEOUT_MIN,
                    HP663X2_TIMEOUT_MAX,
                    VI_ERROR_PARAMETER2);

    thisPtr->measTime = measTimeout;

    return VI_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getMeasTimeout
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Query the measurement timeout setting.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 measTimeout
 * OUT
 *            The measurement timeout value.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getMeasTimeout (ViSession instrumentHandle,
        ViPInt32 measTimeout)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    *measTimeout = thisPtr->measTime;

    return VI_SUCCESS;
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_trigger
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Causes the instrument to trigger the selected trigger subsystem.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 subSystem
 * IN
 *            The select trigger subsystem.
 *			  0 - output/transient subsystem
 *            1 - acquisition subsystem
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_trigger (ViSession instrumentHandle, ViInt16 subSystem)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (subSystem)
    {
    case HP663X2_TRIG_OUTPUT:
        errStatus = viPrintf(instrumentHandle, "TRIG\n");
        break;
    case HP663X2_TRIG_ACQUISITION:
        errStatus = viPrintf(instrumentHandle, "TRIG:ACQ\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_trg
 *-----------------------------------------------------------------------------
 *
 * PURPOSE : This function sends an IEEE Group Execute Trigger
 *			  (GET or *TRG) to the instrument.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_trg (ViSession instrumentHandle)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS( instrumentHandle, thisPtr );

    errStatus = viPrintf(instrumentHandle,"*TRG\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setVoltAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the parameters of the voltage acquisition trigger
 *			  system.  The sense function is set to voltage acquisition. The
 *			  acquisition trigger system is not armed.  It must be armed with the
 *			  hp663x2_arm function before it can receive any acquisition trigger.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * IN
 *			 This parameter configures the acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigVoltCount
 * IN
 *			 This parameter sets up a successive number of triggers for
 *           measuring voltage data.
 *
 *			 MAX = HP663X2_COUNT_MAX   100
 *			 MIN = HP663X2_COUNT_MIN   0
 *
 * PARAM 4 : ViInt16 acqTrigVoltSlope
 * IN
 *			 This paramter sets the trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 hp663x2_SLOPE_EITHER          0    Either Slope
 *			 hp663x2_SLOPE_POS             1    Positive Slope
 *			 hp663x2_SLOPE_NEG             2    Negative Slope
 *
 *
 * PARAM 5 : ViReal64 acqTrigVoltLev
 * IN
 *			 MAX = HP663X2_VOLT_MAX   MAX
 *			 MIN = HP663X2_VOLT_MIN   0.0
 *
 *
 * PARAM 6 : ViReal64 acqTrigVoltHyst
 * IN
 *			 MAX = HP663X2_VOLT_MAX   MAX
 *			 MIN = HP663X2_VOLT_MIN   0.0
 *
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setVoltAcqTrig (ViSession instrumentHandle,
        ViInt16 acqTrigSource,
        ViInt16 acqTrigVoltCount,
        ViInt16 acqTrigVoltSlope,
        ViReal64 acqTrigVoltLev,
        ViReal64 acqTrigVoltHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar *pTrigSource, *pTrigSlope;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (acqTrigSource)
    {
    case HP663X2_TRIG_SRC_BUS:
        pTrigSource="BUS";
        break;
    case HP663X2_TRIG_SRC_INT:
        pTrigSource="INT";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    CHK_INT_RANGE(acqTrigVoltCount,
                  HP663X2_ACQ_COUNT_MIN,
                  HP663X2_ACQ_COUNT_MAX,
                  VI_ERROR_PARAMETER3);

    switch (acqTrigVoltSlope)
    {
    case HP663X2_TRIG_SLOPE_EITHER:
        pTrigSlope="EITH";
        break;
    case HP663X2_TRIG_SLOPE_POS:
        pTrigSlope="POS";
        break;
    case HP663X2_TRIG_SLOPE_NEG:
        pTrigSlope="NEG";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
        break;
    }

    CHK_REAL_RANGE(acqTrigVoltLev,
                   thisPtr->voltMin,
                   thisPtr->voltMax,
                   VI_ERROR_PARAMETER5);

    CHK_REAL_RANGE(acqTrigVoltHyst,
                   thisPtr->voltMin,
                   thisPtr->voltMax,
                   VI_ERROR_PARAMETER6);

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:SOUR %s;SLOP:VOLT %s\n",	pTrigSource, pTrigSlope);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:COUN:VOLT %hd\n", acqTrigVoltCount);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:LEV:VOLT %lG\n", acqTrigVoltLev);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:HYST:VOLT %lG\n", acqTrigVoltHyst);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "SENS:FUNC \"VOLT\"\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getVoltAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the parameters of the acquisition trigger
 *            system for measuring voltage data.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * OUT
 *			 The acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigVoltCount
 * OUT
 *			 The number of successive triggers for measuring voltage data.
 *
 * PARAM 4 : ViInt16 acqTrigVoltSlope
 * OUT
 *			 The trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 hp663x2_SLOPE_EITHER          0    Either Slope
 *			 hp663x2_SLOPE_POS             1    Positive Slope
 *			 hp663x2_SLOPE_NEG             2    Negative Slope
 *
 * PARAM 5 : ViReal64 acqTrigVoltLev
 * OUT
 *			 The trigger voltage level.
 *
 *			 MAX = HP663X2_VOLT_MAX   MAX
 *			 MIN = HP663X2_VOLT_MIN   0.0
 *
 * PARAM 6 : ViReal64 acqTrigVoltHyst
 * OUT
 *			 The trigger voltage hysteresis level.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getVoltAcqTrig (ViSession instrumentHandle,
        ViPInt16 acqTrigSource,
        ViPInt16 acqTrigVoltCount,
        ViPInt16 acqTrigVoltSlope,
        ViPReal64 acqTrigVoltLev,
        ViPReal64 acqTrigVoltHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SOUR?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "INT")==0)
        *acqTrigSource = HP663X2_TRIG_SRC_INT;
    else
        *acqTrigSource = HP663X2_TRIG_SRC_BUS;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SLOP:VOLT?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "EITH")==0)
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_EITHER;
    else if (strcmp(szString, "POS")==0)
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_POS;
    else
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_NEG;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:COUN:VOLT?\n", "%hd", acqTrigVoltCount);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:LEV:VOLT?\n", "%lf", acqTrigVoltLev);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:HYST:VOLT?\n", "%lf", acqTrigVoltHyst);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setCurrAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the parameters of the current acquisition trigger
 *			  system.  The sense function is set to current acquisition. The
 *			  acquisition trigger system is not armed.  It must be armed with the
 *			  hp663x2_arm function before it can receive any acquisition trigger.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * IN
 *			 This parameter configures the acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigCurrCount
 * IN
 *			 This parameter sets up a successive number of triggers for
 *           measuring current data.
 *
 *			 MAX = HP663X2_COUNT_MAX   100
 *			 MIN = HP663X2_COUNT_MIN   0
 *
 * PARAM 4 : ViInt16 acqTrigCurrSlope
 * IN
 *			 This paramter sets the trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 hp663x2_SLOPE_EITHER          0    Either Slope
 *			 hp663x2_SLOPE_POS             1    Positive Slope
 *			 hp663x2_SLOPE_NEG             2    Negative Slope
 *
 * PARAM 5 : ViReal64 acqTrigCurrLev
 * IN
 *			 This parameter sets the trigger current level.
 *
 *			 MAX = HP663X2_CURR_MAX   MAX
 *			 MIN = HP663X2_CURR_MIN   0.0
 *
 * PARAM 6 : ViReal64 acqTrigCurrHyst
 * IN
 *			 This parameter sets the trigger current hysteresis level.
 *
 *			 MAX = HP663X2_CURR_MAX   MAX
 *			 MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setCurrAcqTrig (ViSession instrumentHandle,
        ViInt16 acqTrigSource,
        ViInt16 acqTrigCurrCount,
        ViInt16 acqTrigCurrSlope,
        ViReal64 acqTrigCurrLev,
        ViReal64 acqTrigCurrHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar *pTrigSource, *pTrigSlope;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (acqTrigSource)
    {
    case HP663X2_TRIG_SRC_BUS:
        pTrigSource="BUS";
        break;
    case HP663X2_TRIG_SRC_INT:
        pTrigSource="INT";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    CHK_INT_RANGE(acqTrigCurrCount,
                  HP663X2_ACQ_COUNT_MIN,
                  HP663X2_ACQ_COUNT_MAX,
                  VI_ERROR_PARAMETER3);

    switch (acqTrigCurrSlope)
    {
    case HP663X2_TRIG_SLOPE_EITHER:
        pTrigSlope="EITH";
        break;
    case HP663X2_TRIG_SLOPE_POS:
        pTrigSlope="POS";
        break;
    case HP663X2_TRIG_SLOPE_NEG:
        pTrigSlope="NEG";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
        break;
    }

    CHK_REAL_RANGE(acqTrigCurrLev,
                   thisPtr->currMin,
                   thisPtr->currMax,
                   VI_ERROR_PARAMETER5);

    CHK_REAL_RANGE(acqTrigCurrHyst,
                   thisPtr->currMin,
                   thisPtr->currMax,
                   VI_ERROR_PARAMETER6);

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:SOUR %s;SLOP:CURR %s\n",	pTrigSource, pTrigSlope);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:COUN:CURR %hd\n", acqTrigCurrCount);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }


    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:LEV:CURR %lG\n", acqTrigCurrLev);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:HYST:CURR %lG\n", acqTrigCurrHyst);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "SENS:FUNC \"CURR\"\n");
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getCurrAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the parameters of the acquisition trigger
 *            system for measuring current data.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * OUT
 *			 The acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigCurrCount
 * OUT
 *			 The number of successive triggers for measuring current data.
 *
 * PARAM 4 : ViInt16 acqTrigCurrSlope
 * OUT
 *			 The trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 hp663x2_SLOPE_EITHER          0    Either Slope
 *			 hp663x2_SLOPE_POS             1    Positive Slope
 *			 hp663x2_SLOPE_NEG             2    Negative Slope
 *
 * PARAM 5 : ViReal64 acqTrigCurrLev
 * OUT
 *			 The trigger current level.
 *
 *			 MAX = HP663X2_CURR_MAX   MAX
 *			 MIN = HP663X2_CURR_MIN   0.0
 *
 * PARAM 6 : ViReal64 acqTrigCurrHyst
 * OUT
 *			 The trigger current hysteresis level.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getCurrAcqTrig (ViSession instrumentHandle,
        ViPInt16 acqTrigSource,
        ViPInt16 acqTrigCurrCount,
        ViPInt16 acqTrigCurrSlope,
        ViPReal64 acqTrigCurrLev,
        ViPReal64 acqTrigCurrHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SOUR?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "INT")==0)
        *acqTrigSource = HP663X2_TRIG_SRC_INT;
    else
        *acqTrigSource = HP663X2_TRIG_SRC_BUS;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SLOP:CURR?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "EITH")==0)
        *acqTrigCurrSlope = HP663X2_TRIG_SLOPE_EITHER;
    else if (strcmp(szString, "POS")==0)
        *acqTrigCurrSlope = HP663X2_TRIG_SLOPE_POS;
    else
        *acqTrigCurrSlope = HP663X2_TRIG_SLOPE_NEG;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:COUN:CURR?\n", "%hd", acqTrigCurrCount);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:LEV:CURR?\n", "%lf", acqTrigCurrLev);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:HYST:CURR?\n", "%lf", acqTrigCurrHyst);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setVolt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the immediate, trigggered, or protection
 *            DC voltage level of the power source.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 voltParam
 * IN
 *            The current parameter select.
 *
 *      HP663X2_VOLT        0  the immediate voltage level
 *      HP663X2_VOLT_TRIG   1  the triggered voltage level
 *      HP663X2_VOLT_PROT   2  the over-voltage protection level
 *
 * PARAM 3 : ViReal64 voltLev
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   2.0475
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setVolt (ViSession instrumentHandle,
                                   ViInt16 voltParam,
                                   ViReal64 voltLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (voltParam)
    {
    case HP663X2_VOLT_IMM:
        CHK_REAL_RANGE(voltLev,
                       thisPtr->voltMin,
                       thisPtr->voltMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "VOLT %lG", voltLev);
        break;

    case HP663X2_VOLT_TRIG:
        CHK_REAL_RANGE(voltLev,
                       thisPtr->voltMin,
                       thisPtr->voltMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "VOLT:TRIG %lG", voltLev);
        break;

    case HP663X2_VOLT_OVP:
        CHK_REAL_RANGE(voltLev,
                       thisPtr->ovpMin,
                       thisPtr->ovpMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "VOLT:PROT %lG", voltLev);
        break;

    case HP663X2_VOLT_OVP_MAX:
        sprintf(szString, "VOLT:PROT MAX");
        break;

    case HP663X2_VOLT_OVP_MIN:
        sprintf(szString, "VOLT:PROT MIN");
        break;

    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getVolt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for the selected voltage
 *           paramter setting.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 voltParam
 * OUT
 *            The voltage parameter selection.
 *
 * PARAM 3 : ViPReal64 voltLev
 * OUT
 *            The voltage level in volts.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getVolt (ViSession instrumentHandle,
                                   ViInt16 voltQParam,
                                   ViPReal64 voltLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (voltQParam)
    {
    case HP663X2_VOLT_Q:
        sprintf(szString, "VOLT?\n");
        break;
    case HP663X2_VOLT_Q_MAX:
        sprintf(szString, "VOLT? MAX\n");
        break;
    case HP663X2_VOLT_Q_MIN:
        sprintf(szString, "VOLT? MIN\n");
        break;
    case HP663X2_VOLT_Q_TRIG:
        sprintf(szString, "VOLT:TRIG?\n");
        break;
    case HP663X2_VOLT_Q_TRIG_MAX:
        sprintf(szString, "VOLT:TRIG? MAX\n");
        break;
    case HP663X2_VOLT_Q_TRIG_MIN:
        sprintf(szString, "VOLT:TRIG? MIN\n");
        break;
    case HP663X2_VOLT_Q_OVP:
        sprintf(szString, "VOLT:PROT?\n");
        break;
    case HP663X2_VOLT_Q_OVP_MAX:
        sprintf(szString, "VOLT:PROT? MAX\n");
        break;
    case HP663X2_VOLT_Q_OVP_MIN:
        sprintf(szString, "VOLT:PROT? MIN\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus=viQueryf(instrumentHandle, szString, "%lf", voltLev);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_opc_Q                                     */
/*---------------------------------------------------------------------------*/
/*  Sends an *OPC? command to the instrument and returns VI_TRUE when all    */
/*  pending operations are complete.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession   instrumentHandle (in)                                      */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViPBoolean  instrumentReady (out)                                      */
/*      Returns VI_TRUE when pending operations are complete.                */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_opc_Q(ViSession	instrumentHandle,
                                ViPBoolean  instrumentReady)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "*OPC?\n", "%hd%*t", instrumentReady);

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}        /* ----- end of function ----- */

/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_stdEventStatus_Q                                */
/*---------------------------------------------------------------------------*/
/*  This function queries the Standart Event Status register.				 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViPInt16   stdEventStatus (out)                                        */
/*      The integer returned from the instrument.                            */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_stdEvent_Q(ViSession  instrumentHandle,
                                     ViPInt16   stdEventStatus)

{
    struct   hp663x2_globals *thisPtr;
    ViStatus errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    errStatus = viQueryf(instrumentHandle, "*ESR?\n", "%hd%*t", stdEventStatus);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_setEnableReg                              */
/*---------------------------------------------------------------------------*/
/*  This function queries the Standart Event Status register.				 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViPInt16   enableMask (in)											 */
/*      The integer returned from the instrument.                            */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_setEnableReg (ViSession instrumentHandle,
                                        ViInt16 enableRegister,
                                        ViInt16 enableMask)
{
    struct   hp663x2_globals *thisPtr;
    ViStatus errStatus;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (enableRegister)
    {
    case HP663X2_REG_ESE:
        CHK_INT_RANGE(enableMask,
                      0,
                      255,
                      VI_ERROR_PARAMETER3);
        pszString = "*ESE";
        break;
    case HP663X2_REG_SRE:
        CHK_INT_RANGE(enableMask,
                      0,
                      255,
                      VI_ERROR_PARAMETER3);
        pszString = "*SRE";
        break;
    case HP663X2_REG_OPER:
        CHK_INT_RANGE(enableMask,
                      0,
                      32767,
                      VI_ERROR_PARAMETER3);
        pszString = "STAT:OPER:ENAB";
        break;
    case HP663X2_REG_QUES:
        CHK_INT_RANGE(enableMask,
                      0,
                      32767,
                      VI_ERROR_PARAMETER3);
        pszString = "STAT:QUES:ENAB";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus = viPrintf(instrumentHandle, "%s %d\n", pszString, enableMask);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_getEnableReg                              */
/*---------------------------------------------------------------------------*/
/*  This function queries the Standart Event Status register.				 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViInt16   enableRegister (in)                                          */
/*      The integer returned from the instrument.                            */
/*    ViPInt16   enableMask (out)                                        */
/*      The integer returned from the instrument.                            */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_getEnableReg (ViSession instrumentHandle,
                                        ViInt16 enableRegister,
                                        ViPInt16 enableMask)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;
    ViChar		*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    switch (enableRegister)
    {
    case HP663X2_REG_ESE:
        pszString = "*ESE";
        break;
    case HP663X2_REG_SRE:
        pszString = "*SRE";
        break;
    case HP663X2_REG_OPER:
        pszString = "STAT:OPER:ENAB";
        break;
    case HP663X2_REG_QUES:
        pszString = "STAT:QUES:ENAB";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus = viQueryf(instrumentHandle, "%s?\n", "%hd%*t", pszString, enableMask);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */

/***************************************************/
/*  HP standard status functions                   */
/***************************************************/

/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_readStatusByte_Q                          */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from hp663x2_init()                       */
/*    ViPInt16  statusByte (out)                                             */
/*      Returns the contents of the status byte                              */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_readStatusByte_Q(ViSession  instrumentHandle,
        ViPInt16   statusByte)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus = 0;
    ViUInt16	stb;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viReadSTB(instrumentHandle, &stb)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    *statusByte = (ViInt16)stb;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_operEvent_Q                               */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from hp663x2_init()                       */
/*    ViPInt32  operationEventRegister (out)                                 */
/*      Returns the contents of the operation event register                 */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_operEvent_Q(ViSession  instrumentHandle,
                                      ViPInt32   operationEventRegister)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viQueryf(instrumentHandle,"STAT:OPER:EVEN?\n", "%ld%*t",
                              operationEventRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_operCond_Q                                */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from hp663x2_init()                       */
/*    ViPInt32  operationConditionRegister (out)                             */
/*      Returns the contents of the operation condition register             */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_operCond_Q(ViSession instrumentHandle,
                                     ViPInt32 operationConditionRegister)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viQueryf(instrumentHandle, "STAT:OPER:COND?\n", "%ld%*t",
                              operationConditionRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_quesEvent_Q                               */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from hp663x2_init()                       */
/*    ViPInt32  questionableEventRegister (out)                              */
/*      Returns the contents of the questionable event register              */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_quesEvent_Q(ViSession instrumentHandle,
                                      ViPInt32  questionableEventRegister)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viQueryf(instrumentHandle, "STAT:QUES:EVEN?\n", "%ld%*t",
                              questionableEventRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_quesCond_Q                                */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from hp663x2_init()                        */
/*    ViPInt32  questionableConditionRegister (out)                          */
/*      Returns the contents of the questionable condition register          */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_quesCond_Q(ViSession instrumentHandle,
                                     ViPInt32  questionableConditionRegister)
{
    struct		hp663x2_globals *thisPtr;
    ViStatus	errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viQueryf(instrumentHandle, "STAT:QUES:COND?\n", "%ld%*t",
                              questionableConditionRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */

/***************************************************/
/*  HP standard command passthrough functions      */
/***************************************************/

/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_cmd                                       */
/*---------------------------------------------------------------------------*/
/*  Send a scpi command, it does not look for a response                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession     instrumentHandle (in)                                    */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViString      sendStringCommand (in)                                   */
/*      The SCPI command string to be sent to the instrument                 */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_cmd(ViSession instrumentHandle,
                              ViString  sendStringCommand)
{
    ViStatus errStatus;
    struct   hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    errStatus = viPrintf(instrumentHandle,"%s\n", sendStringCommand);

    return statusUpdate( instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_cmdString_Q                               */
/*---------------------------------------------------------------------------*/
/*  PARAMETERS                                                               */
/*    ViSession       instrumentHandle (in)                                  */
/*      Instrument handle returned from hp663x2_init()                       */
/*    ViString        queryStringCommand (in)                                */
/*      The SCPI command string to be sent to the instrument                 */
/*    ViInt32         stringSize (in)                                        */
/*      The size of the char array (result) passed to the function to hold   */
/*      the string returned by the instrument                                */
/*    ViChar _VI_FAR  stringResult[] (out)                                   */
/*      The string returned by the instrument                                */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_cmdString_Q (ViSession	instrumentHandle,
                                       ViString		queryStringCommand,
                                       ViInt32		stringSize,
                                       ViChar		_VI_FAR stringResult[])
{
    struct    hp663x2_globals *thisPtr;
    ViStatus  errStatus;
    int       sz;

    /* Command strings must have at least one non-null character */
    if(stringSize < 2)
        return statusUpdate(instrumentHandle,VI_NULL,VI_ERROR_PARAMETER2);

    GET_GLOBALS(instrumentHandle, thisPtr)

    sz = (int)stringSize;
    if ((errStatus = viQueryf(instrumentHandle, "%s\n", "%#t",
                              queryStringCommand, &sz, stringResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    stringResult[sz]='\0';

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */

/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_cmdInt                                    */
/*---------------------------------------------------------------------------*/
/*  Sends an instrument command which takes a single integer parameter.      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from hp663x2_init()                        */
/*    ViString   sendIntegerCommand (in)                                     */
/*      The instrument command string to be sent to the instrument.          */
/*    ViPInt32   sendInteger (in)                                            */
/*      The integer sent to the instrument at the end of the instrument      */
/*      command.  Can be ViInt16 or ViInt32.                                 */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_cmdInt(ViSession  instrumentHandle,
                                 ViString   sendIntegerCommand,
                                 ViInt32    sendInteger )
{
    ViStatus errStatus;
    struct hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viPrintf(instrumentHandle, "%s %ld\n", sendIntegerCommand,
                              sendInteger)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */


/*---------------------------------------------------------------------------*/
/* FUNC: ViStatus _VI_FUNC hp663x2_cmdInt16_Q                                */
/*---------------------------------------------------------------------------*/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an int16.  A non numeric instrument response returns zero in        */
/*    result.                                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from hp663x2_init()                        */
/*    ViString   queryI16Command (in)                                        */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPInt16   i16Result (out)                                             */
/*      The integer returned from the instrument.                            */
/*---------------------------------------------------------------------------*/
ViStatus _VI_FUNC hp663x2_cmdInt16_Q(ViSession  instrumentHandle,
                                     ViString   queryI16Command,
                                     ViPInt16   i16Result)

{
    ViStatus errStatus;
    struct   hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viQueryf(instrumentHandle, "%s\n", "%hd%*t",
                              queryI16Command, i16Result)) < VI_SUCCESS)
        return statusUpdate( instrumentHandle, thisPtr, errStatus );

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}        /* ----- end of function ----- */

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureCurr2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument's second output for a
 *            current measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name                Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC             0    dc current
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureCurr2(ViSession instrumentHandle,
                                       ViInt16 measParam,
                                       ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (measParam)
    {
    case HP663X2_MEAS_DC:
        pszString="MEAS:CURR2?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureVolt2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument's second output for a
 *            voltage measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name              Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC           0    dc voltage
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureVolt2(ViSession instrumentHandle,
                                       ViInt16 measurement,
                                       ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (measurement)
    {
    case HP663X2_MEAS_DC:
        pszString="MEAS:VOLT2?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_measureDvm
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument's DVM for a measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name              Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC           0    dc voltage
 *      HP663X2_MEAS_ACDC         1    ac+dc rms voltage
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_measureDvm(ViSession instrumentHandle,
                                     ViInt16 measurement,
                                     ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr);

    if (!hp663x2_model_table[thisPtr->model].dvm)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (measurement)
    {
    case HP663X2_MEAS_DC:
        pszString="MEAS:DVM?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="MEAS:DVM:ACDC?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_fetchDvm
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument's DVM for a voltage
 *            measurement from the previous voltage measurement.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 measParam
 * IN
 *            Specifies the type of measurement to be performed.
 *
 *      Macro Name              Value  Description
 *      -----------------------------------------------------------
 *      HP663X2_MEAS_DC           0    dc voltage
 *      HP663X2_MEAS_ACDC         1    ac+dc rms voltage
 *
 * PARAM 3 : ViPReal64 measResult
 * OUT
 *            Result of the measurement.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_fetchDvm(ViSession instrumentHandle,
                                   ViInt16 measParam,
                                   ViPReal64 measResult)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;
    ViChar	*pszString;

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].dvm)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (measParam)
    {
    case HP663X2_MEAS_DC:
        pszString="FETC:DVM?\n";
        break;
    case HP663X2_MEAS_ACDC:
        pszString="FETC:DVM:ACDC?\n";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    errStatus=measure(instrumentHandle, thisPtr, thisPtr->measTime, pszString, measResult);

    return statusUpdate(instrumentHandle, thisPtr, errStatus);
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setDisplayChannel
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets which output channel will be displayed on the
 *            front panel.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 dispChannel
 * IN
 *            The channel to display.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setDisplayChannel (ViSession instrumentHandle,
        ViInt32 dispChannel)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_LONG_RANGE(dispChannel, 1, 2, VI_ERROR_PARAMETER2);
    errStatus = viPrintf(instrumentHandle, "DISP:CHAN %ld\n", dispChannel);
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getDisplayChannel
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function gets which output channel is being displayed on the
 *            front panel.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 dispChannel
 * IN
 *            The channel being displayed.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getDisplayChannel (ViSession instrumentHandle,
        ViPInt32 dispChannel)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    errStatus = viQueryf(instrumentHandle, "DISP:CHAN?\n", "%ld\n", dispChannel);
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setSenseProt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function enables or disables the open sense lead detect feature.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 dispChannel
 * IN
 *            Whether or not to enable the open sense lead detect feature.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setSenseProt (ViSession instrumentHandle,
                                        ViBoolean senseProtState)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].senseProt)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_BOOLEAN(senseProtState, VI_ERROR_PARAMETER2);
    errStatus = viPrintf(instrumentHandle, "SENS:PROT:STAT %hd\n", senseProtState);
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getSenseProt
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function determines whether or not the open sense lead detect
 *            feature is enabled.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt32 dispChannel
 * IN
 *            Whether or not the open sense lead detect feature is enabled.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getSenseProt (ViSession instrumentHandle,
                                        ViPBoolean senseProtState)
{
    ViStatus errStatus = 0;
    struct hp663x2_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].senseProt)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    errStatus = viQueryf(instrumentHandle, "SENS:PROT:STAT?\n", "%hd\n", senseProtState);
    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setCurr2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the immediate or trigggered current level
 *            of the power source's second output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 currParam
 * IN
 *            The current parameter select.
 *
 *      HP663X2_CURR_IMM    0  the immediate current level
 *      HP663X2_CURR_TRIG   1  the triggered current level
 *
 * PARAM 3 : ViReal64 currLev
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   MAX
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setCurr2 (ViSession instrumentHandle,
                                    ViInt16 currParam,
                                    ViReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_INT_RANGE(currParam,
                  HP663X2_CURR2_IMM,
                  HP663X2_CURR2_TRIG,
                  VI_ERROR_PARAMETER2);


    if (currParam==HP663X2_CURR2_IMM)
    {
        CHK_REAL_RANGE(currLev,
                       thisPtr->curr2Min,
                       thisPtr->curr2Max,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "CURR2 %lG", currLev);
    }
    else
    {
        CHK_REAL_RANGE(currLev,
                       thisPtr->curr2Min,
                       thisPtr->curr2Max,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "CURR2:TRIG %lG", currLev);
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getCurr2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function gets the immediate or triggered current level
 *            of the instruments's second output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPReal64 curr
 * OUT
 *            The current in amps.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getCurr2(ViSession instrumentHandle,
                                   ViInt16 currQParam,
                                   ViPReal64 currLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (currQParam)
    {
    case HP663X2_CURR2_Q:
        sprintf(szString, "CURR2?\n");
        break;
    case HP663X2_CURR2_Q_MAX:
        sprintf(szString, "CURR2? MAX\n");
        break;
    case HP663X2_CURR2_Q_MIN:
        sprintf(szString, "CURR2? MIN\n");
        break;
    case HP663X2_CURR2_Q_TRIG:
        sprintf(szString, "CURR2:TRIG?\n");
        break;
    case HP663X2_CURR2_Q_TRIG_MAX:
        sprintf(szString, "CURR2:TRIG? MAX\n");
        break;
    case HP663X2_CURR2_Q_TRIG_MIN:
        sprintf(szString, "CURR2:TRIG? MIN\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus=viQueryf(instrumentHandle, szString, "%lf", currLev);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setVolt2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the immediate, or trigggered
 *            DC voltage level of the instrument's second output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 voltParam
 * IN
 *            The current parameter select.
 *
 *      HP663X2_VOLT2_IMM    0  the immediate voltage level
 *      HP663X2_VOLT2_TRIG   1  the triggered voltage level
 *
 * PARAM 3 : ViReal64 voltLev
 * IN
 *            The current in amps.
 *
 *      MAX = HP663X2_CURR_MAX   MAX
 *      MIN = HP663X2_CURR_MIN   0.0
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setVolt2(ViSession instrumentHandle,
                                   ViInt16 voltParam,
                                   ViReal64 voltLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (voltParam)
    {
    case HP663X2_VOLT2_IMM:
        CHK_REAL_RANGE(voltLev,
                       thisPtr->volt2Min,
                       thisPtr->volt2Max,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "VOLT2 %lG", voltLev);
        break;

    case HP663X2_VOLT2_TRIG:
        CHK_REAL_RANGE(voltLev,
                       thisPtr->volt2Min,
                       thisPtr->volt2Max,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "VOLT2:TRIG %lG", voltLev);
        break;

    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getVolt2
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the instrument for the selected voltage
 *           paramter setting.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 voltParam
 * OUT
 *            The voltage parameter selection.
 *
 * PARAM 3 : ViPReal64 voltLev
 * OUT
 *            The voltage level in volts.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getVolt2(ViSession instrumentHandle,
                                   ViInt16 voltQParam,
                                   ViPReal64 voltLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].output2)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (voltQParam)
    {
    case HP663X2_VOLT2_Q:
        sprintf(szString, "VOLT2?\n");
        break;
    case HP663X2_VOLT2_Q_MAX:
        sprintf(szString, "VOLT2? MAX\n");
        break;
    case HP663X2_VOLT2_Q_MIN:
        sprintf(szString, "VOLT2? MIN\n");
        break;
    case HP663X2_VOLT2_Q_TRIG:
        sprintf(szString, "VOLT2:TRIG?\n");
        break;
    case HP663X2_VOLT2_Q_TRIG_MAX:
        sprintf(szString, "VOLT2:TRIG? MAX\n");
        break;
    case HP663X2_VOLT2_Q_TRIG_MIN:
        sprintf(szString, "VOLT2:TRIG? MIN\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus=viQueryf(instrumentHandle, szString, "%lf", voltLev);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setDvmAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the parameters of the DVM acquisition trigger
 *			  system.  The sense function is set to DVM acquisition. The
 *			  acquisition trigger system is not armed.  It must be armed with the
 *			  hp663x2_arm function before it can receive any acquisition trigger.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * IN
 *			 This parameter configures the acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigVoltCount
 * IN
 *			 This parameter sets up a successive number of triggers for
 *           measuring voltage data.
 *
 *			 MAX = HP663X2_COUNT_MAX   100
 *			 MIN = HP663X2_COUNT_MIN   0
 *
 * PARAM 4 : ViInt16 acqTrigVoltSlope
 * IN
 *			 This paramter sets the trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 HP663X2_SLOPE_EITHER          0    Either Slope
 *			 HP663X2_SLOPE_POS             1    Positive Slope
 *			 HP663X2_SLOPE_NEG             2    Negative Slope
 *
 *
 * PARAM 5 : ViReal64 acqTrigVoltLev
 * IN
 *			 MAX = HP663X2_DVM_MAX   MAX
 *			 MIN = HP663X2_DVM_MIN   0.0
 *
 *
 * PARAM 6 : ViReal64 acqTrigVoltHyst
 * IN
 *			 MAX = HP663X2_DVM_MAX   MAX
 *			 MIN = HP663X2_DVM_MIN   0.0
 *
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setDvmAcqTrig (ViSession instrumentHandle,
        ViInt16 acqTrigSource,
        ViInt16 acqTrigVoltCount,
        ViInt16 acqTrigVoltSlope,
        ViReal64 acqTrigVoltLev,
        ViReal64 acqTrigVoltHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar *pTrigSource, *pTrigSlope;

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].dvm)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (acqTrigSource)
    {
    case HP663X2_TRIG_SRC_BUS:
        pTrigSource="BUS";
        break;
    case HP663X2_TRIG_SRC_INT:
        pTrigSource="INT";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
        break;
    }

    CHK_INT_RANGE(acqTrigVoltCount,
                  HP663X2_ACQ_COUNT_MIN,
                  HP663X2_ACQ_COUNT_MAX,
                  VI_ERROR_PARAMETER3);

    switch (acqTrigVoltSlope)
    {
    case HP663X2_TRIG_SLOPE_EITHER:
        pTrigSlope="EITH";
        break;
    case HP663X2_TRIG_SLOPE_POS:
        pTrigSlope="POS";
        break;
    case HP663X2_TRIG_SLOPE_NEG:
        pTrigSlope="NEG";
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
        break;
    }

    CHK_REAL_RANGE(acqTrigVoltLev,
                   thisPtr->dvmMin,
                   thisPtr->dvmMax,
                   VI_ERROR_PARAMETER5);

    CHK_REAL_RANGE(acqTrigVoltHyst,
                   thisPtr->dvmMin,
                   thisPtr->dvmMax,
                   VI_ERROR_PARAMETER6);

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:SOUR %s;SLOP:DVM %s\n",	pTrigSource, pTrigSlope);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:COUN:DVM %hd\n", acqTrigVoltCount);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:LEV:DVM %lG\n", acqTrigVoltLev);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "TRIG:ACQ:HYST:DVM %lG\n", acqTrigVoltHyst);
    if (errStatus<VI_SUCCESS)
    {
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    }

    errStatus = viPrintf(instrumentHandle, "SENS:FUNC \"DVM\"\n");

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getDvmAcqTrig
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function queries the parameters of the acquisition trigger
 *            system for measuring DVM data.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 acqTrigSource
 * OUT
 *			 The acquisition trigger source.
 *
 *			 Macro Name            Value  Description
 *			 ----------------------------------------
 *			 HP663X2_INT             0    Internal
 *			 HP663X2_BUS             1    Bus
 *
 * PARAM 3 : ViInt16 acqTrigVoltCount
 * OUT
 *			 The number of successive triggers for measuring voltage data.
 *
 * PARAM 4 : ViInt16 acqTrigVoltSlope
 * OUT
 *			 The trigger slope.
 *
 *			 Macro Name                  Value  Description
 *			 -------------------------------------------------
 *			 HP663X2_SLOPE_EITHER          0    Either Slope
 *			 HP663X2_SLOPE_POS             1    Positive Slope
 *			 HP663X2_SLOPE_NEG             2    Negative Slope
 *
 * PARAM 5 : ViReal64 acqTrigVoltLev
 * OUT
 *			 The trigger voltage level.
 *
 *			 MAX = HP663X2_DVM_MAX   MAX
 *			 MIN = HP663X2_DVM_MIN   0.0
 *
 * PARAM 6 : ViReal64 acqTrigVoltHyst
 * OUT
 *			 The trigger voltage hysteresis level.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getDvmAcqTrig (ViSession instrumentHandle,
        ViPInt16 acqTrigSource,
        ViPInt16 acqTrigVoltCount,
        ViPInt16 acqTrigVoltSlope,
        ViPReal64 acqTrigVoltLev,
        ViPReal64 acqTrigVoltHyst)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].dvm)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SOUR?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "INT")==0)
        *acqTrigSource = HP663X2_TRIG_SRC_INT;
    else
        *acqTrigSource = HP663X2_TRIG_SRC_BUS;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:SLOP:DVM?\n", "%s", szString);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );
    if (strcmp(szString, "EITH")==0)
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_EITHER;
    else if (strcmp(szString, "POS")==0)
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_POS;
    else
        *acqTrigVoltSlope = HP663X2_TRIG_SLOPE_NEG;

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:COUN:DVM?\n", "%hd", acqTrigVoltCount);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:LEV:DVM?\n", "%lf", acqTrigVoltLev);
    if (errStatus<VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    errStatus = viQueryf(instrumentHandle, "TRIG:ACQ:HYST:DVM?\n", "%lf", acqTrigVoltHyst);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}



/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_setRes
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function sets the immediate output resistance
 *            of the power source's output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViInt16 resParam
 * IN
 *            The resistance parameter select.
 *
 *      HP663X2_RES_IMM    0  the immediate current level
 *      HP663X2_RES_TRIG   1  the triggered current level
 *
 * PARAM 3 : ViReal64 resLev
 * IN
 *            The resistance in ohms
 *
 *      MAX = HP663X2_RES_MAX   MAX
 *      MIN = HP663X2_RES_MIN   MIN
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_setRes (ViSession instrumentHandle,
                                  ViInt16 resParam,
                                  ViReal64 resLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);
    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].outRes)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    CHK_INT_RANGE(resParam,
                  HP663X2_RES_IMM,
                  HP663X2_RES_TRIG,
                  VI_ERROR_PARAMETER2);


    if (resParam==HP663X2_RES_IMM)
    {
        CHK_REAL_RANGE(resLev,
                       thisPtr->resMin,
                       thisPtr->resMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "RES %lG", resLev);
    }
    else
    {
        CHK_REAL_RANGE(resLev,
                       thisPtr->resMin,
                       thisPtr->resMax,
                       VI_ERROR_PARAMETER3);
        sprintf(szString, "RES:TRIG %lG", resLev);
    }

    errStatus = viPrintf(instrumentHandle, "%s\n", szString);

    return statusUpdate(instrumentHandle, thisPtr, errStatus );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC hp663x2_getRes
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  This function gets the immediate or triggered resistance level
 *            of the instruments's main output.
 *
 * PARAM 1 : ViSession instrumentHandle
 * IN
 *            The handle to the instrument.
 *
 * PARAM 2 : ViPReal64 res
 * OUT
 *            The resistance in ohms.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "hp663x2_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC hp663x2_getRes(ViSession instrumentHandle,
                                 ViInt16 resQParam,
                                 ViPReal64 resLev)
{
    struct hp663x2_globals *thisPtr;
    ViStatus errStatus = 0;
    ViChar szString[HP663X2_CMDSTRING_MAX];

    GET_GLOBALS(instrumentHandle, thisPtr);

    DEBUG_CHK_THIS(instrumentHandle, thisPtr );

    if (!hp663x2_model_table[thisPtr->model].outRes)
        return statusUpdate(instrumentHandle, thisPtr, HP663X2_INSTR_ERROR_NSUP_FUNC);

    switch (resQParam)
    {
    case HP663X2_RES_Q:
        sprintf(szString, "RES?\n");
        break;
    case HP663X2_RES_Q_MAX:
        sprintf(szString, "RES? MAX\n");
        break;
    case HP663X2_RES_Q_MIN:
        sprintf(szString, "RES? MIN\n");
        break;
    case HP663X2_RES_Q_TRIG:
        sprintf(szString, "RES:TRIG?\n");
        break;
    case HP663X2_RES_Q_TRIG_MAX:
        sprintf(szString, "RES:TRIG? MAX\n");
        break;
    case HP663X2_RES_Q_TRIG_MIN:
        sprintf(szString, "RES:TRIG? MIN\n");
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2 );
        break;
    }

    errStatus=viQueryf(instrumentHandle, szString, "%lf", resLev);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );
}
