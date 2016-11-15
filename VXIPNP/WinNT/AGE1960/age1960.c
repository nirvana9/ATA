/*****************************************************************************/
/*  age1960.c                                                                */
/*  Copyright (C) 1999 - 2002 Agilent Technologies                           */
/*---------------------------------------------------------------------------*/
/*  Driver for age1960, GSM Mobile Test for 8960 Series 10 E5515             */
/*  Driver Version: A.05.00                                                  */
/*---------------------------------------------------------------------------*/
/*  This driver is compatible with the following VXIplug&play standards:     */
/*    WIN32 System Framework revision 4.0                                    */
/*    VISA revision 1.1 (does not work with VTL)							 */
/*    E1960A firmware version A.09.xx or higher								 */
/*    E1985A firmware version A.03.xx or higher								 */
/*****************************************************************************/

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
#include <stdlib.h>           /* prototype for malloc()  */
#include <string.h>           /* prototype for strcpy()  */
#include <stdio.h>            /* prototype for sprintf() */
#include <math.h>             /* prototype for pow()     */

#include <locale.h>           /* prototype for setlocal for atof */

#ifdef __hpux
#define _INCLUDE_HPUX_SOURCE                    USUALLY NEEDED
#define _INCLUDE_XOPEN_SOURCE_EXTENDED          USUALLY NEEDED
#include <sys/time.h>
#else
#include <windows.h>
#include <time.h>
#endif

#include "visa.h"
#include "Age1960.h"

#define age1960_ERR_MSG_LENGTH 256  /* size of error message buffer     */
#define ZERO_DELAY           0      /* Number of milliseconds to delay. */
/*   See doDelay function.          */


/*****************************************************************************/
/*  The string below is used to do the instrument identification in the      */
/*    init routine.  The string matches the first part of the instrument's   */
/*    response to it's *IDN? command.                                        */
/*****************************************************************************/
#define age1960_IDN_STRING  "Agilent Technologies,8960 Series 10 E5515"

/*****************************************************************************/
/*  The string below is used to do the instrument TAP identification.		 */
/*****************************************************************************/
#define age1960_SUPPORTED_FORMAT_STRING  "GSM"

/*****************************************************************************/
/*  VISA Globals data structure.                                             */
/*  It is desireable to store global variables in VISA rather than the       */
/*    driver.  This avoids having any state variables in the driver.  This   */
/*    structure defines the driver's global data.  Memory is allocated to    */
/*    this structure in the init function, which passes the pointer to VISA, */
/*    and every function which follows retrieves a pointer to it.            */
/*****************************************************************************/

struct age1960_globals
{
    ViStatus     errNumber;

    char         errMessage[160];

    ViBoolean    errQueryDetect;

    ViInt32      dTime;

    ViUInt16     interfaceType;

    /*  Setting for trace diagnositcs.                                  */
    /*  traceLevel determines what trace is generated                   */
    /*  traceDest  determines where the trace information is sent       */
    /*  traceCallback is the routine called when the destination        */
    /*                 is TRAC_CALLBACK                                 */
    /*  traceHandle is void pointer passed back in the callback routine */
    char                         address[256];
#if defined _WIN32 || defined __WIN32__
    HANDLE                        hEventSource;
#endif

    ViInt16                        traceLevel;
    ViInt16                        traceDest;
    void (_VI_FUNCH _VI_PTR traceCallback)(ViAddr, const char *);
    ViAddr                        traceHandle;

    /**************************/
    /*  Instrument Specific   */
    /**************************/

    ViInt16        cell1Band;
    ViInt16        cell2Band;
    ViInt16        manualBand;
    ViInt32        interruptBitPower;
    ViInt16        interruptType;
    /* Should the driver use SRQs and MAV to keep from locking the bus */
    ViBoolean      useSRQ;
    /* Track the current setting for loopback so it can be reset when BER and */
    /* Fast BER are turned off from and Init or Read function. */
    ViInt16        loopback;
    /* Hold the index for TAPsuffix1 and TAPsuffix2 for the partition TA's */
    ViInt16        TAIndex;
    ViChar         TAModel[16];
    ViChar		  TArev[16];

};


/*****************************************************************************/
/*  Driver Message Strings.                                                  */
/*****************************************************************************/

#define age1960_MSG_VI_OPEN_ERR            "vi was zero.  Was the age1960_init() successful?"
#define age1960_MSG_CONDITION              "condition"
#define age1960_MSG_EVENT                  "event"
#define age1960_MSG_EVENT_HDLR_INSTALLED   "event handler is already installed for event happening"
#define age1960_MSG_EVENT_HDLR_INST2       "Only 1 handler can be installed at a time."
#define age1960_MSG_INVALID_HAPPENING      "is not a valid happening."
#define age1960_MSG_NOT_QUERIABLE          "is not queriable."
#define age1960_MSG_INVALID_STATUS         "Parameter 2 is invalid in function age1960_error_message()."
#define age1960_MSG_INVALID_STATUS_VALUE   "is not a valid viStatus value."
#define age1960_MSG_INVALID_VI             "Parameter 1 is invalid in function age1960_error_message()."  \
                                            "  Using an inactive ViSession may cause this error."\
                                            "  Was the instrument driver closed prematurely?"
#define age1960_MSG_NO_ERRORS              "No Errors"
#define age1960_MSG_SELF_TEST_FAILED       "Self test failed."
#define age1960_MSG_SELF_TEST_PASSED       "Self test passed."

#define age1960_MSG_BOOLEAN                "Expected 0 or 1; Got %hd"
#define age1960_MSG_REAL                   "Expected %lg to %lg; Got %lg"
#define age1960_MSG_INT                    "Expected %hd to %hd; Got %hd"
#define age1960_MSG_LONG                   "Expected %ld to %ld; Got %ld"
#define age1960_MSG_LOOKUP                 "Error converting string response to integer"
#define age1960_MSG_NO_MATCH               "Could not match string %s"

#define VI_ERROR_PARAMETER1_MSG             "Parameter 1 is invalid"
#define VI_ERROR_PARAMETER2_MSG             "Parameter 2 is invalid"
#define VI_ERROR_PARAMETER3_MSG             "Parameter 3 is invalid"
#define VI_ERROR_PARAMETER4_MSG             "Parameter 4 is invalid"
#define VI_ERROR_PARAMETER5_MSG             "Parameter 5 is invalid"
#define VI_ERROR_PARAMETER6_MSG             "Parameter 6 is invalid"
#define VI_ERROR_PARAMETER7_MSG             "Parameter 7 is invalid"
#define VI_ERROR_PARAMETER8_MSG             "Parameter 8 is invalid"
#define VI_ERROR_PARAMETER9_MSG             "Parameter 9 is invalid"
#define VI_ERROR_PARAMETER10_MSG            "Parameter 10 is invalid"
#define VI_ERROR_PARAMETER11_MSG            "Parameter 11 is invalid"
#define VI_ERROR_PARAMETER12_MSG            "Parameter 12 is invalid"
#define VI_ERROR_PARAMETER13_MSG            "Parameter 13 is invalid"
#define VI_ERROR_PARAMETER14_MSG            "Parameter 14 is invalid"
#define VI_ERROR_PARAMETER15_MSG            "Parameter 15 is invalid"
#define VI_ERROR_PARAMETER16_MSG            "Parameter 16 is invalid"
#define VI_ERROR_PARAMETER17_MSG            "Parameter 17 is invalid"
#define VI_ERROR_PARAMETER18_MSG            "Parameter 18 is invalid"
#define VI_ERROR_FAIL_ID_QUERY_MSG          "Instrument IDN does not match."

#define INSTR_ERROR_INV_SESSION_MSG         "ViSession (parameter 1) was not created by this driver"
#define INSTR_ERROR_NULL_PTR_MSG            "NULL pointer detected"
#define INSTR_ERROR_RESET_FAILED_MSG        "reset failed"
#define INSTR_ERROR_UNEXPECTED_MSG          "An unexpected error occurred"
#define INSTR_ERROR_DETECTED_MSG            "Instrument Error Detected, call age1960_error_query()."
#define INSTR_ERROR_LOOKUP_MSG              "String not found in table"
#define INSTR_ERROR_NO_LAST_COMMA           "no last comma found in IDN string"

#define INSTR_ERROR_ARRAY_TOO_SMALL         "The specified array is too small to hold the data"
#define INSTR_ERROR_CANNOT_ORIG             "8960A/E1960A is not in the correct state to originate call"
#define INSTR_ERROR_CALL_FAILED             "8960A/E1960A call processing activity failed"
#define INSTR_ERROR_CPTO                    "The call processing activity did not complete within given time"
#define age1960_MSG_INT_2                   "Expected %ld to %ld or %ld to %ld; Got %ld"
#define age1960_MSG_REAL_2                  "Expected %lg to %lg or %lg to %lg; Got %lg"
#define INSTR_ERROR_INITTO                  "The initiate done activity did not complete within given time"
#define age1960_MSG_DUPLICATE               "Duplicate entry for %ld"
#define INSTR_ERROR_ORFS_MEAS_COUNT         "Cannot have one and only one of the ORFS measurement counts = 0"
#define INSTR_ERROR_ARRAY_TOO_LARGE         "Not enough memory for the specified array size"
#define INSTR_ERROR_AUTORANGE_FAILED        "8960A/E1960A auto ranging failed"
#define INSTR_ERROR_CANNOT_CHNGBAND_NOW     "Requested TCH band is invalid in the 8960A/E1960A current state"
#define INSTR_ERROR_TOO_MANY_NONCONCURRENT  "Cannot initate BER, Fast BER, or DAUDIO at the same time"
#define INSTR_UNDEFINED_DATA                "Undefined data or no data returned"
#define INSTR_TA_NOT_AVAILABLE              "Requested TA and/or TA Revision for Agilent 8960 is not available"
#define INSTR_TA_NOT_SWITCHABLE             "The GSM TA Revision A.03.04 does not support TA Switching"
#define INSTR_TA_NOT_LICENSE				"Requested TA and/or TA Revision for Agilent 8960 is not licensed"
#define	INSTR_ERROR_NSUP_FUNC_MSG           "Function call is not supported by this model"
#define INSTR_ERROR_FUNC_NOT_SUPPORTED		"Function call not supported with current instrument setup"

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
    /**************************/
    /*  Template Messages     */
    /**************************/

    { VI_ERROR_FAIL_ID_QUERY,             VI_ERROR_FAIL_ID_QUERY_MSG },
    { VI_ERROR_PARAMETER1,                VI_ERROR_PARAMETER1_MSG },
    { VI_ERROR_PARAMETER2,                VI_ERROR_PARAMETER2_MSG },
    { VI_ERROR_PARAMETER3,                VI_ERROR_PARAMETER3_MSG },
    { VI_ERROR_PARAMETER4,                VI_ERROR_PARAMETER4_MSG },
    { VI_ERROR_PARAMETER5,                VI_ERROR_PARAMETER5_MSG },
    { VI_ERROR_PARAMETER6,                VI_ERROR_PARAMETER6_MSG },
    { VI_ERROR_PARAMETER7,                VI_ERROR_PARAMETER7_MSG },
    { VI_ERROR_PARAMETER8,                VI_ERROR_PARAMETER8_MSG },
    { age1960_INSTR_ERROR_PARAMETER9,    VI_ERROR_PARAMETER9_MSG },
    { age1960_INSTR_ERROR_PARAMETER10,   VI_ERROR_PARAMETER10_MSG },
    { age1960_INSTR_ERROR_PARAMETER11,   VI_ERROR_PARAMETER11_MSG },
    { age1960_INSTR_ERROR_PARAMETER12,   VI_ERROR_PARAMETER12_MSG },
    { age1960_INSTR_ERROR_PARAMETER13,   VI_ERROR_PARAMETER13_MSG },
    { age1960_INSTR_ERROR_PARAMETER14,   VI_ERROR_PARAMETER14_MSG },
    { age1960_INSTR_ERROR_PARAMETER15,   VI_ERROR_PARAMETER15_MSG },
    { age1960_INSTR_ERROR_PARAMETER16,   VI_ERROR_PARAMETER16_MSG },
    { age1960_INSTR_ERROR_PARAMETER17,   VI_ERROR_PARAMETER17_MSG },
    { age1960_INSTR_ERROR_PARAMETER18,   VI_ERROR_PARAMETER18_MSG },
    { age1960_INSTR_ERROR_INV_SESSION,   INSTR_ERROR_INV_SESSION_MSG },
    { age1960_INSTR_ERROR_NULL_PTR,      INSTR_ERROR_NULL_PTR_MSG },
    { age1960_INSTR_ERROR_RESET_FAILED,  INSTR_ERROR_RESET_FAILED_MSG },
    { age1960_INSTR_ERROR_UNEXPECTED,    INSTR_ERROR_UNEXPECTED_MSG },
    { age1960_INSTR_ERROR_DETECTED,      INSTR_ERROR_DETECTED_MSG },
    { age1960_INSTR_ERROR_LOOKUP,        INSTR_ERROR_LOOKUP_MSG },
    { age1960_INSTR_NO_LAST_COMMA,       INSTR_ERROR_NO_LAST_COMMA },

    /**************************/
    /*  Instrument Specific   */
    /**************************/

    { age1960_INSTR_ERROR_ARRAY_TOO_SMALL,  	INSTR_ERROR_ARRAY_TOO_SMALL },
    { age1960_INSTR_CANNOT_ORIG_NOW,        	INSTR_ERROR_CANNOT_ORIG },
    { age1960_INSTR_ERROR_CALL_FAILED,      	INSTR_ERROR_CALL_FAILED },
    { age1960_INSTR_ERROR_CPTO,             	INSTR_ERROR_CPTO},
    { age1960_INSTR_ERROR_INITTO,           	INSTR_ERROR_INITTO},
    { age1960_INSTR_ERROR_ORFS_MEAS_COUNT,  	INSTR_ERROR_ORFS_MEAS_COUNT},
    { age1960_INSTR_ERROR_ARRAY_TOO_LARGE,  	INSTR_ERROR_ARRAY_TOO_LARGE },
    { age1960_INSTR_ERROR_AUTORANGE_FAILED, 	INSTR_ERROR_AUTORANGE_FAILED },
    { age1960_INSTR_CANNOT_CHNGBAND_NOW,    	INSTR_ERROR_CANNOT_CHNGBAND_NOW },
    { age1960_INSTR_TOO_MANY_NONCONCURRENT, 	INSTR_ERROR_TOO_MANY_NONCONCURRENT },
    { age1960_INSTR_UNDEFINED_DATA,         	INSTR_UNDEFINED_DATA },
    { age1960_INSTR_TA_NOT_AVAILABLE,       	INSTR_TA_NOT_AVAILABLE },
    { age1960_INSTR_TA_NOT_SWITCHABLE,      	INSTR_TA_NOT_SWITCHABLE },
    { age1960_INSTR_TA_NOT_LICENSE,         	INSTR_TA_NOT_LICENSE },
    { age1960_INSTR_ERROR_NSUP_FUNC,        	INSTR_ERROR_NSUP_FUNC_MSG },
    { age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED, 	INSTR_ERROR_FUNC_NOT_SUPPORTED },

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
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession            instrumentHandle                                  */
/*      The instrument's VISA address.                                       */
/*    struct age1960_globals  *thisPtr                                       */
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
        return errStatus;                                                    \
}


/*****************************************************************************/
/*  DELAY Functions                                                          */
/*  setDelay (located prior to init function) sets the delay time for the    */
/*    instrument.                                                            */
/*  doDelay uses delay time to slow down the driver after a read request.    */
/*---------------------------------------------------------------------------*/
/*  In some SCPI instruments, the instrument cannot change its state to      */
/*    'processing a read request' fast enough to keep another request        */
/*    from getting through to the instrument.  By setting a short delay      */
/*    (less than 100 microseconds should be sufficient) after each read,     */
/*    the driver will wait a sufficient amount of time before attempting     */
/*    to send the next string to the instrument.                             */
/*  DO NOT document these functions in the fp file or the help file.  They   */
/*    are intended to address problems in specific instruments, problems     */
/*    that may only appear on faster CPU's.  They are designed so that the   */
/*    problem can be addressed quickly in the field once it is identified.   */
/*  If the problem is identified by driver developers, a 100 ms delay        */
/*    should be set in the age1960_init function.  The template version of   */
/*    init currently sets the delay to 0.                                    */
/*****************************************************************************/

/*****************************************************************************/
static void doDelay(
    ViInt32 delayTime)
/*****************************************************************************/
/*  Delays execution for a specified number of microseconds.                 */
/*  Platform   Code    Tick Resolution  Tick Increment  Call Overhead        */
/*   WIN32     32-bit  CPU dependent    CPU dependent   0-20 us              */
/*   HP-UX     32-bit  1 us             CPU dependent   0-20 us              */
/*  PARAMETERS                                                               */
/*    long           delayTime (in)                                          */
/*      The number of micro-seconds to delay before proceeding.              */
/*****************************************************************************/

{
    long   ticksToCount;

    /*************************************************************/
    /*  WIN32 Version                                            */
    /*  WIN32 increments depend on the hardware.  If high-       */
    /*    frequency counting isn't supported, revert to common   */
    /*    standard functions (same as WIN16).                    */
    /*  The call overhead to execute doDelay if high-frequency   */
    /*    is supported is about 25-30 micro-seconds.             */
    /*  The call overhead to execute doDelay if high-frequency   */
    /*    is -NOT- supported is ~10 milli-seconds on             */
    /*    Windows 95 & NT.                                       */
    /*************************************************************/
#ifdef WIN32

#ifdef _CVI_
#else
    __int64  ticksPerSecond;
    __int64  startTicks;
    __int64  nowTicks;
#endif
    DWORD    msTicks;

    if (delayTime == 0)
        return;

#ifdef _CVI_
#else
    if (QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond))
    {
        ticksToCount = (long)((ticksPerSecond/1E6)*delayTime) + 1;
        QueryPerformanceCounter((LARGE_INTEGER *)&nowTicks);
        startTicks = nowTicks;
        while (nowTicks - startTicks < (DWORD)ticksToCount)
            QueryPerformanceCounter((LARGE_INTEGER *)&nowTicks);
    }
    else
#endif
    {
        ticksToCount = (long)((1E3/1E6)*delayTime) + 1;
        msTicks = GetTickCount();
        while (GetTickCount() - msTicks < (DWORD)ticksToCount) ;
    }

#endif

    /*************************************************************/
    /*  HP-UX Version                                            */
    /*  HP-UX counts in 1 micro-second increments.               */
    /*  The call overhead to execute doDelay on HP-UX is about   */
    /*    15-20 micro-seconds.                                   */
    /*************************************************************/
#ifdef __hpux

    struct timeval t0, t1;

    if (delayTime == 0)
        return;

    /* printf("doDelay delaying %ld\n", ticks);  */
    gettimeofday(&t0, NULL);
    t0.tv_usec += delayTime;
    t0.tv_sec += t0.tv_usec / 1000000;
    t0.tv_usec = t0.tv_usec % 1000000;

    gettimeofday(&t1, NULL);
    while (t1.tv_sec < t0.tv_sec)
        gettimeofday(&t1, NULL);

    if (t1.tv_sec > t0.tv_sec) return; /* get out quick if past delay time */

    while (t1.tv_usec < t0.tv_usec)
        gettimeofday(&t1, NULL);

#endif

    return;

}        /* ----- end of function ----- */

/*****************************************************************************/
static ViStatus delayVPrintf(
    ViSession  instrumentHandle,
    ViString   printFmt,
    ... )
/*****************************************************************************/
/*  Same as viVPrintf, but with a delay added if necessary.                  */
/*                                                                           */
/*  PARAMETERS (same as viVPrintf)                                           */
/*    ViSession   instrumentHandle (in)                                      */
/*    ViString    printFmt (in)                                              */
/*    ...                                                                    */
/*****************************************************************************/

{
    ViStatus  errStatus;
    va_list   va;
    struct age1960_globals *thisPtr;

    if ((errStatus = viGetAttribute(instrumentHandle, VI_ATTR_USER_DATA,
                                    (ViAddr) &thisPtr)) < VI_SUCCESS)
        return errStatus;

    va_start(va, printFmt);
    errStatus = viVPrintf(instrumentHandle, printFmt, va);
    va_end(va);

    if (thisPtr->dTime > 0)
        doDelay(thisPtr->dTime);

    return errStatus;

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus delayVQueryf(
    ViSession  instrumentHandle,
    ViString   printFmt,
    ViString   scanFmt,
    ... )
/*****************************************************************************/
/*  Same as viVQueryf, but with a delay added if necessary.                  */
/*                                                                           */
/*  PARAMETERS (same as viVQueryf)                                           */
/*    ViSession   instrumentHandle (in)                                      */
/*    ViString    printFmt (in)                                              */
/*    ViString    scanFmt (in)                                               */
/*    ...                                                                    */
/*****************************************************************************/

{
    ViStatus  errStatus;
    va_list   va;
    struct age1960_globals *thisPtr;

    if ((errStatus = viGetAttribute(instrumentHandle, VI_ATTR_USER_DATA,
                                    (ViAddr) &thisPtr)) < VI_SUCCESS)
        return errStatus;

    va_start(va, scanFmt);
    errStatus = viVQueryf(instrumentHandle, printFmt, scanFmt, va);
    va_end(va);

    if (thisPtr->dTime > 0)
        doDelay(thisPtr->dTime);

    return errStatus;

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus statusUpdate(
    ViSession  instrumentHandle,
    struct     age1960_globals *thisPtr,
    ViStatus   s )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession              instrumentHandle (in)                           */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViStatus               s (in)                                          */
/*      A driver error return code.                                          */
/*****************************************************************************/

{
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

        if (( delayVQueryf(instrumentHandle, "*ESR?\n", "%hd%*t",
                           &eventQ)) < VI_SUCCESS)
            return VI_ERROR_SYSTEM_ERROR;


        if( (0x04  | 0x08  | 0x10  | 0x20) & eventQ )
            return age1960_INSTR_ERROR_DETECTED;


    }

    return s;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_BOOLEAN                                                        */
/*****************************************************************************/
/*  Ref chk_boolean for info.                                                */
/*****************************************************************************/

#define CHK_BOOLEAN(chk_val, err)                          \
if (chk_boolean(thisPtr, chk_val))                         \
   {                                                       \
    thisPtr->errNumber = err;                              \
    return err;                                            \
   }


/*****************************************************************************/
static ViBoolean chk_boolean(
    struct     age1960_globals *thisPtr,
    ViBoolean  chk_val)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViBoolean              chk_val (in)                                    */
/*      Check this value to make sure it is a legal Boolean.                 */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val != VI_TRUE) && (chk_val != VI_FALSE) )
    {
        sprintf(message, age1960_MSG_BOOLEAN, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_REAL_RANGE                                                     */
/*****************************************************************************/
/*  Ref chk_real_range for info.                                             */
/*****************************************************************************/

#define CHK_REAL_RANGE(chk_val, min, max, err)         \
if (chk_real_range(thisPtr, chk_val, min, max))        \
    {                                                  \
    thisPtr->errNumber = err;                          \
    return err;                                        \
   }


/*****************************************************************************/
static ViBoolean chk_real_range(
    struct    age1960_globals *thisPtr,
    ViReal64  chk_val,
    ViReal64  min,
    ViReal64  max)
/*****************************************************************************/
/*  Tests to see if a ViReal64 is in range.                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViReal64             chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViReal64             min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViReal64             max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, age1960_MSG_REAL, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_REAL_RANGE_2                                                   */
/*****************************************************************************/
/*  Ref chk_real_range for two ranges                                        */
/*****************************************************************************/

#define CHK_REAL_RANGE_2(chk_val, min_1, max_1,min_2,max_2, err)        \
if (chk_real_range_2(thisPtr, chk_val, min_1, max_1,min_2,max_2) )      \
    {                                                                   \
    thisPtr->errNumber = err;                                           \
    return err;                                                         \
    }


/*****************************************************************************/
static ViBoolean chk_real_range_2(
    struct   age1960_globals *thisPtr,
    ViReal64 chk_val,
    ViReal64 min_1,
    ViReal64 max_1,
    ViReal64 min_2,
    ViReal64 max_2)
/*****************************************************************************/
/*  Tests to see if a ViReal64 is in range.                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViReal64              chk_val (in)                                     */
/*      The value to be checked.                                             */
/*    ViReal64              min_1 (in)                                       */
/*      The bottom of the range 1.                                           */
/*    ViReal64              max_1 (in)                                       */
/*      The top of the range 1.                                              */
/*    ViReal64              min_2 (in)                                       */
/*      The bottom of the range 2.                                           */
/*    ViReal64              max_2 (in)                                       */
/*      The top of the range 2.                                              */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val < min_1) || (chk_val > max_1) &&
            (chk_val < min_2) || (chk_val > max_2) )
    {
        sprintf(message, age1960_MSG_REAL_2, min_1, max_1, min_2, max_2, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_INT_RANGE                                                      */
/*****************************************************************************/
/*  Ref chk_real_range for info.                                             */
/*****************************************************************************/

#define CHK_INT_RANGE(chk_val, min, max, err)        \
if (chk_int_range(thisPtr, chk_val, min, max) )      \
   {                                                 \
    thisPtr->errNumber = err;                        \
    return err;                                      \
   }


/*****************************************************************************/
static ViBoolean chk_int_range(
    struct   age1960_globals *thisPtr,
    ViInt16  chk_val,
    ViInt16  min,
    ViInt16  max)
/*****************************************************************************/
/*  Tests to see if a ViInt16 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt16              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt16              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, age1960_MSG_INT, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_LONG_RANGE_2                                                   */
/*****************************************************************************/
/*  Ref chk_long_range for two ranges                                        */
/*****************************************************************************/

#define CHK_LONG_RANGE_2(chk_val, min_1, max_1,min_2,max_2, err)       \
if (chk_int_range_2(thisPtr, chk_val, min_1, max_1,min_2,max_2) )      \
    {                                                                  \
    thisPtr->errNumber = err;                                          \
    return err;                                                        \
    }


/*****************************************************************************/
static ViBoolean chk_int_range_2(
    struct   age1960_globals *thisPtr,
    ViInt32  chk_val,
    ViInt32  min_1,
    ViInt32  max_1,
    ViInt32  min_2,
    ViInt32  max_2)
/*****************************************************************************/
/*  Tests to see if a ViInt32 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt32              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt32              min_1 (in)                                        */
/*      The bottom of the range 1.                                           */
/*    ViInt32              max_1 (in)                                        */
/*      The top of the range 1.                                              */
/*    ViInt32              min_2 (in)                                        */
/*      The bottom of the range 2.                                           */
/*    ViInt32              max_2 (in)                                        */
/*      The top of the range 2.                                              */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val < min_1) || (chk_val > max_1) &&
            (chk_val < min_2) || (chk_val > max_2) )
    {
        sprintf(message, age1960_MSG_INT_2, min_1, max_1, min_2, max_2, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  MACRO CHK_LONG_RANGE                                                     */
/*****************************************************************************/
/*  Ref chk_long_range for info.                                             */
/*****************************************************************************/

#define CHK_LONG_RANGE(chk_val, min, max, err)      \
if (chk_long_range(thisPtr, chk_val, min, max))     \
   {                                                \
    thisPtr->errNumber = err;                       \
    return err;                                     \
   }


/*****************************************************************************/
static ViBoolean chk_long_range(
    struct  age1960_globals *thisPtr,
    ViInt32 chk_val,
    ViInt32 min,
    ViInt32 max)
/*****************************************************************************/
/*  Tests to see if a ViInt32 is in range.                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt32              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt32              min (in)                                          */
/*      The bottom of the range.                                             */
/*    ViInt32              max (in)                                          */
/*      The top of the range.                                                */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( (chk_val < min) || (chk_val > max) )
    {
        sprintf(message, age1960_MSG_LONG, min, max, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */

/*****************************************************************************/
/*  MACRO CHK_DUPLICATE                                                      */
/*****************************************************************************/
/*  Ref chk_duplicate for info.                                              */
/*****************************************************************************/

#define CHK_DUPLICATE(chk_val_1, chk_val_2, err)      \
if (chk_duplicate(thisPtr, chk_val_1, chk_val_2))     \
   {                                                  \
    thisPtr->errNumber = err;                         \
    return err;                                       \
   }


/*****************************************************************************/
static ViBoolean chk_duplicate(
    struct  age1960_globals *thisPtr,
    ViInt32 chk_val_1,
    ViInt32 chk_val_2)
/*****************************************************************************/
/*  Tests to see if a ViInt32 chk_val_1 is not the same as ViInt32 chk_val_2.*/
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt32              chk_val_1 (in)                                    */
/*      The value 1 to be checked.                                           */
/*    ViInt32             chk_val_2 (in)                                     */
/*      The value 2 to be checked.                                           */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( chk_val_1 == chk_val_2 )
    {
        sprintf(message, age1960_MSG_DUPLICATE, chk_val_1);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */

/*****************************************************************************/
/*  MACRO CHK_ENUM                                                           */
/*****************************************************************************/
/*  Ref chk_enum for info.                                                   */
/*****************************************************************************/

#define CHK_ENUM( chk_val, limit, err )             \
if (chk_enum( thisPtr, chk_val, limit) )            \
   {                                                \
    thisPtr->errNumber = err;                       \
    return err;                                     \
   }


/*****************************************************************************/
static ViBoolean chk_enum (
    struct age1960_globals *thisPtr,
    ViInt16 chk_val,
    ViInt16 limit)
/*****************************************************************************/
/* Chk_enum searches for a string in an array of strings.  It is used by     */
/* the CHK_ENUM macro                                                        */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    struct age1960_globals *thisPtr (in)                                   */
/*      A pointer to the VISA globals for this VISA session                  */
/*    ViInt16              chk_val (in)                                      */
/*      The value to be checked.                                             */
/*    ViInt16              limit (in)                                        */
/*      The upper limit of the enumeration values.                           */
/*****************************************************************************/

{
    ViChar message[age1960_ERR_MSG_LENGTH];

    if ( ( chk_val < 0 ) || (chk_val > limit) )
    {
        sprintf(message, age1960_MSG_INT, 0, limit, chk_val);
        strcpy(thisPtr->errMessage, message);
        return VI_TRUE;
    }

    return VI_FALSE;

}        /* ----- end of function ----- */



/*****************************************************************************/
static ViStatus _VI_FUNC initError(
    ViPSession  vi,
    ViSession  defRM,
    ViStatus   errStatus)
/*****************************************************************************/
/*  This function is only called from age1960_init.                          */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViPSession      vi (in)												 */
/*      Instrument Handle returned by viOpen.                                */
/*    ViSession      defRM (in)                                              */
/*      Resource Manager handle from viOpen.                                 */
/*    ViStatus       errStatus (in)                                          */
/*      A driver error return code.                                          */
/*****************************************************************************/

{
    viClose(*vi);
    viClose(defRM);
    *vi=VI_NULL;
    return errStatus;

}        /* ----- end of function ----- */

/*-----------------------------------------------------------------------------
 * FUNC    : age1960_printTrace
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :   Print log to appropriate destination.
 *             Called by age1960trace and age1960_statusUpdate
 * PARAM 1 : thisPtr
 * IN
 *           state record pointer
 *
 * PARAM 2 : const char *s
 * IN
 *           Message to be printed
 *
 *-----------------------------------------------------------------------------
 */
static void _VI_FUNC age1960_printTrace(const struct age1960_globals *thisPtr,
                                        const char *s)
{
#if (defined _WIN32 || defined __WIN32__) && !defined _CVI_
    if (thisPtr->traceDest == age1960_TRAC_STDERR)
    {
        fprintf(stderr, "%s", s);
    }
    else if (thisPtr->traceDest == age1960_TRAC_EVENTLOG)
    {
        const char *array = s;

        ReportEvent(thisPtr->hEventSource,
                    EVENTLOG_INFORMATION_TYPE,
                    0, 0x40000001, NULL,
                    1, 0, &array, NULL);
    }
    else if (thisPtr->traceDest == age1960_TRAC_DEBUGSTRING)
    {
        OutputDebugString(s);
    }
    else if (thisPtr->traceDest == age1960_TRAC_CALLBACK)
    {
        /* call the trace callback function if it was set */
        if (thisPtr->traceCallback)
        {
            (thisPtr->traceCallback)(thisPtr->traceHandle, s);
        }
        else
        {
            OutputDebugString("age1960" "No callback function defined");
        }
    }
#else
    if (thisPtr->traceDest == age1960_TRAC_STDERR)
    {
        fputs(s, stderr);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Macro and FUNC    : age1960_TRACE,  age1960_trace
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :   Logs function calls if tracing is enabled
 * PARAM 1 : thisPtr
 * IN
 *           state record pointer
 *
 * PARAM 2 : const char *nameViBoolean name
 * IN
 *           name of function called
 *
 * PARAM 3 : const char *fmt
 * IN
 *           Format for printing the log message
 *
 * PARAM 4... : input parameters
 *
 *-----------------------------------------------------------------------------
 */
#define age1960_TRACE if (thisPtr->traceLevel==age1960_TRAC_ALL) age1960_trace

static void age1960_trace(const struct age1960_globals *thisPtr, const char *name,
                          const char *fmt, ...)
{

    SYSTEMTIME st;
    char *p;
    char logmsg[500];
    va_list ap;

    GetLocalTime(&st);
    sprintf(logmsg, "%02d%02d%02d%02d%02d.%03d, %s(%s",
            st.wMonth, st.wDay, st.wHour,st.wMinute, st.wSecond,st.wMilliseconds,
            name, thisPtr->address);
    p = logmsg + strlen(logmsg);
    va_start(ap, fmt);
    vsprintf(p, fmt, ap);
    va_end(ap);
    strcat(p, ")\n");
    age1960_printTrace(thisPtr, logmsg);
}


/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC age1960_diagTrac
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Enables diagnostic tracing of driver calls.  Tracing may be
 *            sent to stderr or to the windows event logdebug string or callback
 *            routine.  The trace may show all calls or only calls with errors.
 *
 *            The TS-5500 software automatically controls tracing in driver
 *           so messages appear in the Trace window. This routine should
 *           never be called in an action.
 *
 * PARAM 1 : ViSession vi
 * IN
 *            Instrument Handle returned from age1960_init().
 *
 * PARAM 2 : ViInt16 destination
 * IN
 *            Selects stderr or windows event log.
 *
 *      Macro Name                     Value  Description
 *      -----------------------------------------------------------
 *      age1960_TRAC_STDERR               0  Standard error
 *      age1960_TRAC_EVENTLOG             1  Windows Event Log
 *      age1960_TRAC_DEBUGSTRING          2  Windows Debug String
 *      age1960_TRAC_CALLBACK             3  Callback function
 *
 * PARAM 3 : ViInt16 level
 * IN
 *            Selects none, error, or all.
 *
 *      Macro Name               Value  Description
 *      -----------------------------------------------------------
 *      age1960_TRAC_NONE           0  None
 *      age1960_TRAC_ERROR          1  Trace parameter errors
 *      age1960_TRAC_ALL            2  Trace all function calls
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "age1960_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC age1960_diagTrac(
    ViSession vi,
    ViInt16 destination,
    ViInt16 level)
{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (VI_SUCCESS > errStatus)
    {
        statusUpdate( vi, 0, errStatus );
    }

    age1960_TRACE(thisPtr, "age1960_diagTrac",",%d,%d",destination,level);

    thisPtr->traceDest = destination;
    thisPtr->traceLevel = level;
#if defined _WIN32 && !defined _CVI_
    if (destination==age1960_TRAC_EVENTLOG && !thisPtr->hEventSource)
    {
        thisPtr->hEventSource = RegisterEventSource(NULL, "age1960_driver");
    }
#endif
    return statusUpdate( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC age1960_diagTracCallback
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Enables the diagnostic tracing to call an arbitrary function
 *           with the tracing string and a handle.
 *            The function prototype for the function must be:
 *
 *            TraceCallback(ViAddr,ViString);
 *
 * PARAM 1 : ViSession vi
 * IN
 *            Instrument Handle returned from age1960_init().
 *
 * PARAM 2 : ViAddr function
 * IN
 *            Selects what function is called for tracing
 *
 * PARAM 3 : ViAddr handle
 * IN
 *            Handle which is passed back with the tracing callback function
 *           is called.
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "age1960_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC age1960_diagTracCallback(
    ViSession vi,
    ViAddr function,
    ViAddr handle)
{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (VI_SUCCESS > errStatus)
    {
        statusUpdate( vi, 0, errStatus );
    }

    age1960_TRACE(thisPtr, "age1960_diagTracCallback",",%lx,%lx",function,handle);

    thisPtr->traceCallback = function;
    thisPtr->traceHandle   = handle;
    return statusUpdate( vi, thisPtr, VI_SUCCESS );
}

/*-----------------------------------------------------------------------------
 * FUNC    : ViStatus _VI_FUNC age1960_diagTrac_Q
 *-----------------------------------------------------------------------------
 *
 * PURPOSE :  Queries diagnostic tracing of driver calls.  Tracing may be
 *            sent to stderr or to the windows event log debug string or
 *            callback routine.  The trace may show all calls or only calls
 *            with errors.
 *
 * PARAM 1 : ViSession vi
 * IN
 *            Instrument Handle returned from age1960_init().
 *
 * PARAM 2 : ViPInt16 destination
 * OUT
 *            Returns stderr or windows event log.
 *
 *      Macro Name                     Value  Description
 *      -----------------------------------------------------------
 *      age1960_TRAC_STDERR               0  Standard error
 *      age1960_TRAC_EVENTLOG             1  Windows Event Log
 *      age1960_TRAC_DEBUGSTRING          2  Windows Debug String
 *      age1960_TRAC_CALLBACK             3  Callback function
 *
 * PARAM 3 : ViPInt16 level
 * OUT
 *            Returns none, error, or all.
 *
 *      Macro Name               Value  Description
 *      -----------------------------------------------------------
 *      age1960_TRAC_NONE           0  None
 *      age1960_TRAC_ERROR          1  Trace parameter errors
 *      age1960_TRAC_ALL            2  Trace all function calls
 *
 * RETURN  :  VI_SUCCESS: No error. Non VI_SUCCESS: Indicates error
 *           condition. To determine error message, pass the return value to
 *           routine "age1960_error_message".
 *
 *-----------------------------------------------------------------------------
 */
ViStatus _VI_FUNC age1960_diagTrac_Q(
    ViSession vi,
    ViPInt16 destination,
    ViPInt16 level)
{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    errStatus = viGetAttribute(vi, VI_ATTR_USER_DATA, (ViAddr) &thisPtr);
    if (VI_SUCCESS > errStatus)
    {
        statusUpdate( vi, 0, errStatus );
    }

    age1960_TRACE(thisPtr, "age1960_diagTrac_Q","");

    /* Perform Error Checking on Each Parameter */
    *destination = thisPtr->traceDest;
    *level = thisPtr->traceLevel;
    return statusUpdate( vi, thisPtr, VI_SUCCESS );
}

/*****************************************************************************/
static void stripLF( ViChar s[])
/*****************************************************************************/
/* fcn to remove /n at the end of line                                       */
/*****************************************************************************/

{
    int         len;

    len = strlen(s);

    if (s[len-1] == '\n')
        s[len-1] = '\0';

    return;

}      /* ----- end of function ----- */
/*****************************************************************************/
static void stripQuotes( ViChar s[])
/*****************************************************************************/
/* fcn to remove quotes from a string                                        */
/*****************************************************************************/

{
    int         len;
    int         counter;

    len = strlen(s);
    /* remove the leading and trailing \" */
    if (s[0] == '\"')
    {
        /* if there is only one rev of the TA, the string has a \n\0 as the terminator */
        if (strstr(s,"\n") != NULL)   /* remove the \n */
            len = len-1 ;
        s[len-1] = ' ';
        for (counter = 1; counter < len-1; counter++)
            s[counter-1] = s[counter];
        s[counter-1] = '\0';
    }

    return;

}      /* ----- end of function ----- */

/* revisit: remove: change CELL1 to CELL2 when Mom supports CELL2      */
/* for now all the code is in place to support CELL2.  If Cell2 is     */
/* selected this driver will just set cell 1 without giving any errors */

static const char * const age1960_cell_a[] =
{
    "","", ":CELL1",0
};

static const char * const age1960_seqSetting_a[] =
{
    "", ":SEQ",0
};

static const char * const age1960_measFcns_a[] =
{
    "AAUD", "BERR", "DAUD", "FBER", "ORFS",
    "PFER","PVT","TXP", "DPOW", "IQT",0
};

static const char * const age1960_ques_regs_a[] =
{
    ":QUES", ":QUES:CALL", ":QUES:ERR",
    ":QUES:HARD", ":QUES:CALL:GSM", ":QUES:ERR:GSM", 0
};

static const char * const age1960_oper_regs_a[] =
{
    ":OPER", ":OPER:NMRR", ":OPER:CALL",
    ":OPER:NMRR:GSM", ":OPER:CALL:GSM", 0
};

static const char * const age1960_completion_a[] =
{
    "CONN", "ALER",0
};

static const char * const age1960_bandSetting_a[] =
{
    "",":PGSM", ":EGSM",
    ":DCS", ":PCS", ":GSM850", 0
};

static const char * const age1960_band_a[] =
{
    "","PGSM", "EGSM", "DCS",
    "PCS", "GSM850", 0
};

static const char * const age1960_operatingMode_a[] =
{
    "TEST", "CELL",0
};

static const char * const age1960_burst_a[] =
{
    "TSC0", "TSC1", "TSC2",
    "TSC3","TSC4", "TSC5",
    "TSC6", "TSC7","RACH",0
};

static const char * const age1960_trigSource_a[] =
{
    "AUTO", "PROT", "RISE","IMM",0
};

static const char * const age1960_burstSync_a[] =
{
    "MID", "AMPL", "NONE",0
};

static const char * const age1960_BERRType_a[] =
{
    "TYPEIA", "TYPEII", "TYPEIB",
    "RESTYPEIA", "RESTYPEII", "RESTYPEIB", 0
};

static const char * const age1960_loopbackSetting_a[] =
{
    "OFF", "A", "B", "C", 0
};

static const char * const age1960_channelMode_a[] =
{
    "FRSPEECH", "EFRSPEECH", 0
};

static const char * const age1960_iqt_reference_a[] =
{
    "NEG67KHZ", "ZEROKHZ", "POS67KHZ", "AUTO", 0
};

static const char * const age1960_filter_a[] =
{
    "NONE", "TBP", "CMES", "BPAS50", "BPAS300", 0
};

static const char * const age1960_AFAN_Detector_a[] =
{
    "RMS", "PEAK", 0
};

static const char * const age1960_pvt_mask_for_pcs_a[] =
{
    "NARR", "REL", 0
};

static const char * const age1960_ORFSLimitSource_a[] =
{
    "ETSI", "MAN", 0
};



/* The following line is used to remain consistent with 1961A code  */
/* and to allow for flexibility in the future should a E1985 driver */
/* be created.                                                      */
static const char * const age1960_TAPTAGSM_a[] =  {"", ":GSM", 0};

static void srq_cleanup(ViSession  instrumentHandle)
/*****************************************************************************/
/*                                                                           */
/*  This function is called by confForInterrupt,							 */
/*  age1960_waitForInitNone, and age1960_waitForConnect.                     */
/*  It resets status reporting structures so registers do not cause an       */
/*  interrupt. Any VISA errors generated by an action in this function       */
/*  are ignored.                                                             */
/*                                                                           */
/*  ViSession instrumentHandle (in)                                          */
/*         Instrument handle returned from age1960_init().                   */
/*****************************************************************************/

{
    /* Disable any register from requesting service                            */
    /* Clear ALL event registers (*CLS)                                        */
    /* Added the viReadSTB for National Instruments cards.  They need to read  */
    /* STB to ensure that SRQ will interupt again. See National's help for     */
    /* VI_EVENT_SERVICE_REQ                                                    */


    ViUInt16 junkForNI;

    viReadSTB(instrumentHandle,&junkForNI);

    viPrintf(instrumentHandle,"STAT:QUES:ENAB 0"
             ";:STAT:QUES:ERR:ENAB 0"
             ";:STAT:QUES:ERR:COMM:ENAB 0"
             ";:STAT:QUES:ERR:GSM:ENAB 0"
             ";:STAT:QUES:CALL:ENAB 0"
             ";:STAT:QUES:CALL:COMM:ENAB 0"
             ";:STAT:QUES:CALL:GSM:ENAB 0"
             ";:STAT:QUES:HARD:ENAB 0\n"); /* added mom1.2 changes for Status subsystem */

    viPrintf(instrumentHandle,"STAT:OPER:ENAB 0"
             ";:STAT:OPER:HARD:ENAB 0"
             ";:STAT:OPER:NMRR:ENAB 0"
             ";:STAT:OPER:NMRR:GSM:ENAB 0"
             ";:STAT:OPER:NMRR:COMM:ENAB 0"
             ";:STAT:OPER:CALL:ENAB 0"
             ";:STAT:OPER:CALL:COMM:ENAB 0"
             ";:STAT:OPER:CALL:GSM:ENAB 0"
             ";*SRE 0;*CLS\n"); /* added mom1.2 changes for Status subsystem */

    viDisableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE);
    return;

}        /* ----- end of function ----- */


/*****************************************************************************/
/* This routine is only called after a measurement is read when              */
/* sendQueryRequest was used and if thisPtr->useSRQ == VI_TRUE.              */
/*  Returns ViStatus                                                         */
/*                                                                           */
/*  input:                                                                   */
/*      instrumentHandle session                                             */
/*****************************************************************************/

static ViStatus _VI_FUNC cleanupMAVafterRead(ViSession instrumentHandle)
{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;
    ViUInt16  junkForNI;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (thisPtr->useSRQ == VI_FALSE)
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

    viReadSTB(instrumentHandle, &junkForNI);

    if((errStatus = viPrintf(instrumentHandle,"*SRE 0\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}

/*****************************************************************************/
/* This routine is only called by sendQueryRequest when a VISA call returns  */
/* an error. It tries to put VISA back into a good state where it won't be   */
/* looking for SRQ, It tries to put the instrument into a good state where   */
/* it parses new messages,stops any measurements in progress, and disables   */
/* the generation of SRQ.                                                    */
/*                                                                           */
/*  Returns nothing                                                          */
/*                                                                           */
/*  input:                                                                   */
/*      instrumentHandle session                                             */
/*****************************************************************************/

static void sendQueryRequest_cleanup(ViSession instrumentHandle)
{
    ViUInt16 junkForNI;

    /* disable SRQ from causing VISA events */
    viDisableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE);
    /* reset the parser */
    viClear(instrumentHandle);
    /* abort any measurement in progress,
        nothing causes SRQ,
    */
    viReadSTB(instrumentHandle, &junkForNI);

    viPrintf(instrumentHandle, "*SRE 0;*CLS\n");
}

static ViStatus _VI_FUNC sendQueryRequest(ViSession instrumentHandle,
        ViString  query,
        ViInt32  timeout)
/*****************************************************************************/
/*  If thisPtr->useSRQ == VI_TRUE, this function will query the instrument   */
/*  and wait for MAV to pull SRQ  or the timeout to expire                   */
/*  If thisPtr->useSRQ == VI_FALSE, this function will query the instrument. */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   query.                                                      */
/*      Query to be sent to the instrument.                                  */
/*    ViInt32    timeout (in)                                                */
/*      Maximum time to wait before timing out (in milliseconds) if waiting  */
/*      for MAV.                                                             */
/*      This function will only wait in second resolution.  Therefore,       */
/*      timeout will be rounded up the the nearest second                    */
/*****************************************************************************/

{
    ViStatus    errStatus;
    ViEventType eventType;
    ViEvent     context;
    ViUInt16    spollResult = 0;
    time_t      tStart;
    ViInt32     timeoutLeft;
    ViInt32     maxTimeToWait;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (thisPtr->useSRQ == VI_FALSE)
    {
        if((errStatus = viPrintf(instrumentHandle, "%s\n",query)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        return VI_SUCCESS;
    }

    /* Have VISA generate an event when SRQ goes true */
    if((errStatus=viEnableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE,VI_NULL)) < VI_SUCCESS)
    {
        sendQueryRequest_cleanup(instrumentHandle);
        return errStatus;
    }
    /* Enable MAV to generate SRQ, and ask for the result */
    if((errStatus = viPrintf(instrumentHandle,"*SRE 16;:%s\n",query)) < VI_SUCCESS)
    {
        sendQueryRequest_cleanup(instrumentHandle);
        return errStatus;
    }
    /* Look at the status byte */
    if((errStatus = viReadSTB(instrumentHandle,&spollResult)) < VI_SUCCESS)
    {
        sendQueryRequest_cleanup(instrumentHandle);
        return errStatus;
    }

    timeout = timeout + 1000; /* allow 1000 msec more than the measurement is set to */
    /* in order to keep a VISA error from occurring instead */
    /* of a measurement timeout returning NANs. */

    maxTimeToWait = 1000 * (ViInt32) ceil((double)timeout/1000); /* up maxTimeToWait to the */
    /* nearest whole second */
    timeoutLeft = maxTimeToWait;
    tStart=time(NULL); /* Get the current time to second resolution*/

    /* Loop until RQS and MAV are true */
    while((spollResult & 0x50 )!= 0x50)
    {
        /* Go to sleep until an SRQ (or timeout) */
        if((errStatus = viWaitOnEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,
                                      timeoutLeft, &eventType,&context)) < VI_SUCCESS)
        {
            /* If a timeout, maybe SRQ is stuck high by some other instrument. */
            /* Since the 8960 should be ready by now, check the status byte one last time */
            if( errStatus == VI_ERROR_TMO )
            {
                viReadSTB(instrumentHandle,&spollResult);
                if((spollResult & 0x50 ) == 0x50) break;
            }
            sendQueryRequest_cleanup(instrumentHandle);
            return errStatus;
        }
        /* Always close the context to avoid a memory leak. If the viWaitOnEvent was*/
        /*  unsuccessful, there is no need to close the context as it is not valid. */
        if ((errStatus = viClose(context)) < VI_SUCCESS)
        {
            sendQueryRequest_cleanup(instrumentHandle);
            return errStatus;
        }
        if((errStatus = viReadSTB(instrumentHandle,&spollResult)) < VI_SUCCESS)
        {
            sendQueryRequest_cleanup(instrumentHandle);
            return errStatus;
        }

        /* Just to be safe check that some time is left.  It should never go */
        /* negative unless the viWaitOnEvent timed out which should be caught */
        /* earlier. */

        timeoutLeft = maxTimeToWait - 1000 * (ViInt32) difftime(time(NULL),tStart);
        if(timeoutLeft<=0)
        {
            sendQueryRequest_cleanup(instrumentHandle);
            return VI_ERROR_TMO;
        }
    }

    if((errStatus = viDisableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE)) < VI_SUCCESS)
    {
        sendQueryRequest_cleanup(instrumentHandle);
        return errStatus;
    }

    return VI_SUCCESS;
}


static ViInt32 ReadTimeout(ViSession instrumentHandle, ViString measFcn)
/*****************************************************************************/
/* This routine is only called by Read functions to get the current          */
/* measurement timeout setting                                               */
/*    ViString   measFcn.                                                    */
/*      measFcn to be sent to the instrument.                                */
/*    return     timeout in milliseconds                                     */
/*****************************************************************************/
{
    ViChar   tmpMsg[20];
    ViReal64 TimeoutSec;
    ViStatus  errStatus;

    sprintf(tmpMsg, "SET:%s:TIM?",measFcn );
    errStatus =  sendQueryRequest(instrumentHandle, tmpMsg, 8000 );
    if (errStatus < VI_SUCCESS) return 0;

    errStatus = viScanf(instrumentHandle, "%lf%*t", &TimeoutSec);
    if (errStatus < VI_SUCCESS) return 0;

    errStatus =  cleanupMAVafterRead(instrumentHandle);
    if (errStatus < VI_SUCCESS) return 0;

    return (ViInt32) TimeoutSec*1000;
}

static ViStatus _VI_FUNC getManualBand(ViSession  instrumentHandle, ViPInt16 currentManualBand)
/*****************************************************************************/
/*  This function will read the manual Band from the instrument.		     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/
{

    ViStatus errStatus;
    struct   age1960_globals *thisPtr;
    ViChar   manualBandResult[50];

    GET_GLOBALS(instrumentHandle, thisPtr)

    *currentManualBand = age1960_UNKNOWN_BAND;

    if ((errStatus = delayVQueryf(instrumentHandle, "RFAN:MAN:BAND?\n",
                                  "%s%*t",manualBandResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if (strcmp(manualBandResult,"PGSM") == 0)
        *currentManualBand =  age1960_PGSM_BAND;
    if (strcmp(manualBandResult,"EGSM") == 0)
        *currentManualBand =  age1960_EGSM_BAND;
    if (strcmp(manualBandResult,"DCS") == 0)
        *currentManualBand =  age1960_DCS1800_BAND;
    if (strcmp(manualBandResult,"PCS") == 0)
        *currentManualBand =  age1960_PCS1900_BAND;
    if (strcmp(manualBandResult,"GSM850") == 0)
        *currentManualBand =  age1960_GSM850_BAND;

    return VI_SUCCESS;

}     /* ----- end of function ----- */

static ViStatus getTCHBand(ViSession  instrumentHandle, ViPInt16 currentTCHBand)
/*****************************************************************************/
/*  This function will read the TCH Band from the instrument.		         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/
{

    ViStatus errStatus;
    struct   age1960_globals *thisPtr;
    ViChar   TCHBandResult[50];

    GET_GLOBALS(instrumentHandle, thisPtr)

    *currentTCHBand = age1960_UNKNOWN_BAND;

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:TCH:BAND?\n",
                                  "%s%*t",TCHBandResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if (strcmp(TCHBandResult,"PGSM") == 0)
        *currentTCHBand =  age1960_PGSM_BAND;
    if (strcmp(TCHBandResult,"EGSM") == 0)
        *currentTCHBand =  age1960_EGSM_BAND;
    if (strcmp(TCHBandResult,"DCS") == 0)
        *currentTCHBand =  age1960_DCS1800_BAND;
    if (strcmp(TCHBandResult,"PCS") == 0)
        *currentTCHBand =  age1960_PCS1900_BAND;
    if (strcmp(TCHBandResult,"GSM850") == 0)
        *currentTCHBand =  age1960_GSM850_BAND;

    return VI_SUCCESS;

}     /* ----- end of function ----- */

static ViStatus getCell1Band(ViSession  instrumentHandle, ViPInt16 currentCell1Band)
/*****************************************************************************/
/*  This function will read the Cell 1 Band from the instrument.		     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/
{

    ViStatus errStatus;
    struct   age1960_globals *thisPtr;
    ViChar   cellBandResult[50];

    GET_GLOBALS(instrumentHandle, thisPtr)

    *currentCell1Band = age1960_UNKNOWN_BAND;

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:CELL1:BAND?\n",
                                  "%s%*t", cellBandResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if (strcmp(cellBandResult,"PGSM") == 0)
        *currentCell1Band =  age1960_PGSM_BAND;
    if (strcmp(cellBandResult,"EGSM") == 0)
        *currentCell1Band =  age1960_EGSM_BAND;
    if (strcmp(cellBandResult,"DCS") == 0)
        *currentCell1Band =  age1960_DCS1800_BAND;
    if (strcmp(cellBandResult,"PCS") == 0)
        *currentCell1Band =  age1960_PCS1900_BAND;
    if (strcmp(cellBandResult,"GSM850") == 0)
        *currentCell1Band =  age1960_GSM850_BAND;

    return VI_SUCCESS;

}     /* ----- end of function ----- */

static ViStatus getCell2Band(ViSession  instrumentHandle, ViPInt16 currentCell2Band)
/*****************************************************************************/
/*  This function will read the Cell 2 Band from the instrument.			 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/
{

    ViStatus errStatus;
    struct   age1960_globals *thisPtr;
    ViChar   cellBandResult[50];

    GET_GLOBALS(instrumentHandle, thisPtr)

    *currentCell2Band = age1960_UNKNOWN_BAND;

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:CALL2:BAND?\n",
                                  "%s%*t", cellBandResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if (strcmp(cellBandResult,"PGSM") == 0)
        *currentCell2Band =  age1960_PGSM_BAND;
    if (strcmp(cellBandResult,"EGSM") == 0)
        *currentCell2Band =  age1960_EGSM_BAND;
    if (strcmp(cellBandResult,"DCS") == 0)
        *currentCell2Band =  age1960_DCS1800_BAND;
    if (strcmp(cellBandResult,"PCS") == 0)
        *currentCell2Band =  age1960_PCS1900_BAND;
    if (strcmp(cellBandResult,"GSM850") == 0)
        *currentCell2Band =  age1960_GSM850_BAND;

    return VI_SUCCESS;

}     /* ----- end of function ----- */

/*****************************************************************************/
/* Sort function use to sort the BA table ARFCNs in accending order          */
/*****************************************************************************/

static int age1960_compareInt32(const void * elem1, const void * elem2)

{
    if (*(ViInt32*)elem1 == *(ViInt32*)elem2)
        return 0;
    if (*(ViInt32*)elem1 < *(ViInt32*)elem2)
        return -1;
    else
        return 1;

}   /* ----- end of function ----- */


/*****************************************************************************/
static short respToMeasFcnDone( ViChar s[])
/*****************************************************************************/
/*  This function will convert string response data to integer measurement   */
/*  function done information in the following form:                         */
/*                                                                           */
/*      WAIT   =-2  Wait                        (age1960_WAIT                */
/*      NONE   =-1  None have been INIT         (age1960_NONEINIT)           */
/*      AAUD   = 0  Analog Audio Meas Fcn       (age1960_AAUDIO)             */
/*      BERR   = 1  Bit Error Rate Meas Fcn     (age1960_BER)                */
/*      DAUD   = 2  Decoded Audio Meas Fcn      (age1960_DAUDIO)             */
/*      FBER   = 3  Fast Bit Error Meas Fcn     (age1960_FBER)               */
/*      ORFS   = 4  ORFSpectrum Meas Fcn        (age1960_ORFS)               */
/*      PFER   = 5  Phase Freq Error Meas Fcn   (age1960_PFER)               */
/*      PVT    = 6  Power vs Time Meas Fcn      (age1960_PVTIME)             */
/*      TXP    = 7  TX Power Meas Fcn           (age1960_TXPOWER)            */
/*      DPOW   = 8  Dynamic Power               (age1960_DPOWER)             */
/*      IQT    = 9  IQ Tuning                   (age1960_IQTUNING)           */
/*****************************************************************************/

{

    if (strcmp(s,"WAIT") == 0) return  age1960_WAIT;
    if (strcmp(s,"NONE") == 0) return  age1960_NONEINIT;
    if (strcmp(s,"AAUD") == 0) return  age1960_AAUDIO;
    if (strcmp(s,"BERR") == 0) return  age1960_BER;
    if (strcmp(s,"DAUD") == 0) return  age1960_DAUDIO;
    if (strcmp(s,"FBER") == 0) return  age1960_FBER;
    if (strcmp(s,"ORFS") == 0) return  age1960_ORFS;
    if (strcmp(s,"PFER") == 0) return  age1960_PFER;
    if (strcmp(s,"PVT")  == 0) return  age1960_PVTIME;
    if (strcmp(s,"TXP")  == 0) return  age1960_TXPOWER;
    if (strcmp(s,"DPOW") == 0) return  age1960_DPOWER;
    if (strcmp(s,"IQT")  == 0) return  age1960_IQTUNING;

    return -99;

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkTCHARFCN(
    ViSession  instrumentHandle,
    ViInt32    TCHARFCN,
    ViInt16    TCHARFCNBand)
/*****************************************************************************/
/*  This function will check the range of the proposed TCH ARFCN given the   */
/*  desired TCH band. If the desired TCH band is age1960_CURRENT_BAND it 	 */
/*  will read the current TCH band from the instrument.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    TCHARFCN (in)                                               */
/*      The TCH ARFCN number to be checked.                                  */
/*    ViInt16     TCHARFCNBand                                               */
/*      The TCH ARFCN Band that the TCH ARFCN will be checked against.       */
/*      The TCH Bands are:                                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*      The limits for the channel depend on which TCH band  is              */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*            GSM 850:   128 to 251                                          */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (TCHARFCNBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getTCHBand(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = TCHARFCNBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        CHK_LONG_RANGE(TCHARFCN,age1960_PGSM_MIN_CH,
                       age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_DCS1800_BAND:
        CHK_LONG_RANGE(TCHARFCN,age1960_DCS1800_MIN_CH,
                       age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_EGSM_BAND:
        CHK_LONG_RANGE_2(TCHARFCN, age1960_EGSM_MIN_CH_1, age1960_EGSM_MAX_CH_1,
                         age1960_EGSM_MIN_CH_2, age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER2)
        break;
    case age1960_PCS1900_BAND:
        CHK_LONG_RANGE(TCHARFCN,age1960_PCS1900_MIN_CH,
                       age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_GSM850_BAND:
        CHK_LONG_RANGE(TCHARFCN,age1960_GSM850_MIN_CH,
                       age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkManualARFCN(
    ViSession  instrumentHandle,
    ViInt32    manualARFCN,
    ViInt16    manualARFCNBand)
/*****************************************************************************/
/*  This function will check the range of the proposed manual ARFCN given    */
/*  the desired manual band. If the desired manual band is                   */
/*  age1960_CURRENT_BAND it will read the current manual band from the 		 */
/*	instrument.                                 							 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    manualARFCN (in)                                            */
/*      The manual ARFCN number to be checked.                               */
/*    ViInt16    manualARFCNBand                                             */
/*      The manual ARFCN Band that the manual ARFCN will be checked          */
/*       against.                                                            */
/*      The expected Bands are:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*                                                                           */
/*      The limits for the channel depend on which expected band  is         */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*            GSM 850:   128 to 251                                          */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (manualARFCNBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getManualBand(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = manualARFCNBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        CHK_LONG_RANGE(manualARFCN,age1960_PGSM_MIN_CH,
                       age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_DCS1800_BAND:
        CHK_LONG_RANGE(manualARFCN,age1960_DCS1800_MIN_CH,
                       age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_EGSM_BAND:
        CHK_LONG_RANGE_2(manualARFCN, age1960_EGSM_MIN_CH_1,
                         age1960_EGSM_MAX_CH_1, age1960_EGSM_MIN_CH_2,
                         age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER2)
        break;
    case age1960_PCS1900_BAND:
        CHK_LONG_RANGE(manualARFCN,age1960_PCS1900_MIN_CH,
                       age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    case age1960_GSM850_BAND:
        CHK_LONG_RANGE(manualARFCN,age1960_GSM850_MIN_CH,
                       age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER2)
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkBCHARFCNCell1(
    ViSession  instrumentHandle,
    ViInt32    BCHARFCN,
    ViInt16    BCHARFCNBand)
/*****************************************************************************/
/*  This function will check the range of the proposed BCH ARFCN given       */
/*  the desired Cell 1 BCH band. If the desired Cell 1 BCH band is           */
/*  age1960_CURRENT_BAND it will read the current Cell 1 BCH band from 		 */
/*	the instrument.                             							 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    BCHARFCN (in)                                               */
/*      The BCH ARFCN number to be checked.                                  */
/*    ViInt16    BCHARFCNBand (in)                                           */
/*      The BCH ARFCN Band that the BCH ARFCN will be checked                */
/*       against.                                                            */
/*      The BCH Bands are:                                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*                                                                           */
/*      The limits for the channel depend on which expected band  is         */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*			  GSM 850:   128 to 251											 */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (BCHARFCNBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getCell1Band(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = BCHARFCNBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_PGSM_MIN_CH,
                       age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_DCS1800_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_DCS1800_MIN_CH,
                       age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_EGSM_BAND:
        CHK_LONG_RANGE_2(BCHARFCN, age1960_EGSM_MIN_CH_1, age1960_EGSM_MAX_CH_1,
                         age1960_EGSM_MIN_CH_2, age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER3)
        break;
    case age1960_PCS1900_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_PCS1900_MIN_CH,
                       age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_GSM850_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_GSM850_MIN_CH,
                       age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkBCHARFCNCell2(
    ViSession  instrumentHandle,
    ViInt32    BCHARFCN,
    ViInt16    BCHARFCNBand)
/*****************************************************************************/
/*  This function will check the range of the proposed BCH ARFCN given       */
/*  the desired BCH band. If the desired BCH cell 2 band is                  */
/*  age1960_CURRENT_BAND it will read the current BCH cell 2 band from the 	 */
/*	instrument.                             								 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    BCHARFCN (in)                                               */
/*      The BCH ARFCN number to be checked.                                  */
/*    ViInt16    BCHARFCNBand (in)                                           */
/*      The BCH ARFCN Band that the BCH ARFCN will be checked                */
/*       against.                                                            */
/*      The BCH Bands are:                                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*                                                                           */
/*      The limits for the channel depend on which expected band  is         */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*			  GSM 850:   128 to 251											 */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (BCHARFCNBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getCell2Band(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = BCHARFCNBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_PGSM_MIN_CH,
                       age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_DCS1800_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_DCS1800_MIN_CH,
                       age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_EGSM_BAND:
        CHK_LONG_RANGE_2(BCHARFCN, age1960_EGSM_MIN_CH_1, age1960_EGSM_MAX_CH_1,
                         age1960_EGSM_MIN_CH_2, age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER3)
        break;
    case age1960_PCS1900_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_PCS1900_MIN_CH,
                       age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    case age1960_GSM850_BAND:
        CHK_LONG_RANGE(BCHARFCN,age1960_GSM850_MIN_CH,
                       age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER3)
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER4);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkBATableCell1(
    ViSession  instrumentHandle,
    ViInt32    BATableArraySize,
    ViInt32    BATableArray[],
    ViInt16    BATableBand)
/*****************************************************************************/
/*  This function will check the range of the proposed BA Table ARFCNs given */
/*  the desired BA Table band. If the desired BA Table band is               */
/*  age1960_CURRENT_BAND it will read the current cell 1 band from the 		 */
/*	instrument.                                 							 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    BATableArraySize (in)                                       */
/*      The number of ARFCNs in the BATableArray.                            */
/*    ViInt32    BATableArray (in)                                           */
/*      The ARFCNs to be set in the BA Table.                                */
/*    ViInt16    BATableBand                                                 */
/*      The BA Table Band that the BA ARFCNs will be checked                 */
/*       against.                                                            */
/*      The expected Bands are:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*                                                                           */
/*      The limits for the channel depend on which expected band  is         */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*            GSM 850:   128 to 251                                          */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;
    int        i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (BATableBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getCell1Band(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = BATableBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_PGSM_MIN_CH,
                           age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_DCS1800_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_DCS1800_MIN_CH,
                           age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_EGSM_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE_2(BATableArray[i], age1960_EGSM_MIN_CH_1,
                             age1960_EGSM_MAX_CH_1, age1960_EGSM_MIN_CH_2,
                             age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER4)
            break;
    case age1960_PCS1900_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_PCS1900_MIN_CH,
                           age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_GSM850_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_GSM850_MIN_CH,
                           age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER5);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkBATableCell2(
    ViSession  instrumentHandle,
    ViInt32    BATableArraySize,
    ViInt32    BATableArray[],
    ViInt16    BATableBand)
/*****************************************************************************/
/*  This function will check the range of the proposed BA Table ARFCNs given */
/*  the desired BA Table band. If the desired BA Table band is               */
/*  age1960_CURRENT_BAND it will read the current cell 2 band from the 		 */
/*	instrument.                                								 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    BATableArraySize (in)                                       */
/*      The number of ARFCNs in the BATableArray.                            */
/*    ViInt32    BATableArray (in)                                           */
/*      The ARFCNs to be set in the BA Table.                                */
/*    ViInt16    BATableBand                                                 */
/*      The BA Table Band that the BA ARFCNs will be checked                 */
/*       against.                                                            */
/*      The expected Bands are:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*                                                                           */
/*      The limits for the channel depend on which expected band  is         */
/*      selected and are as follows:                                         */
/*            PGSM 900:  1 to 124                                            */
/*            EGSM:      0 to 124 and 975 to 1023                            */
/*            DCS 1800:  512 to 885                                          */
/*            PCS 1900:  512 to 810                                          */
/*            GSM 850:   128 to 251                                          */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;
    int        i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (BATableBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getCell2Band(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = BATableBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_PGSM_MIN_CH,
                           age1960_PGSM_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_DCS1800_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_DCS1800_MIN_CH,
                           age1960_DCS1800_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_EGSM_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE_2(BATableArray[i], age1960_EGSM_MIN_CH_1,
                             age1960_EGSM_MAX_CH_1, age1960_EGSM_MIN_CH_2,
                             age1960_EGSM_MAX_CH_2, VI_ERROR_PARAMETER4)
            break;
    case age1960_PCS1900_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_PCS1900_MIN_CH,
                           age1960_PCS1900_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    case age1960_GSM850_BAND:
        for (i = 0; i<BATableArraySize; i++)
            CHK_LONG_RANGE(BATableArray[i],age1960_GSM850_MIN_CH,
                           age1960_GSM850_MAX_CH, VI_ERROR_PARAMETER4)
            break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER5);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
static ViStatus age1960_checkMSTXLevel(
    ViSession  instrumentHandle,
    ViInt32    MSTXLevel,
    ViInt16    MSTXLevelBand)
/*****************************************************************************/
/*  This function will check the range of the proposed TX Level given the    */
/*  current TCH band.														 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    MSTXLevel (in)                                              */
/*      The MS TX Level number to be checked.                                */
/*    ViInt16    MSTXLevelBand                                               */
/*      The expected MS TX Level Band that the MS TX level will be checked   */
/*       against.                                                            */
/*      The expected Bands are:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*      The limits for the MS TX Level depend on which band is               */
/*      selected and are as follows:                                         */
/*            PGSM:      0 to 15                                             */
/*            EGSM:      0 to 31                                             */
/*            DCS 1800:  0 to 31                                             */
/*            PCS 1900:  0 to 15 or 30 to 31                                 */
/*            GSM 850:   0 to 31                                             */
/* Note: The VI_ERROR_PARAMETERn error messages have to match the calling    */
/*       function's parameter list not this function's parameter list.       */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt16    bandToCheck;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)


    if (MSTXLevelBand == age1960_CURRENT_BAND)
    {
        if ((errStatus = getTCHBand(instrumentHandle,&bandToCheck)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        bandToCheck = MSTXLevelBand;
    }
    switch(bandToCheck)
    {
    case age1960_PGSM_BAND:
        CHK_LONG_RANGE(MSTXLevel,age1960_PGSM_MIN_TXL,
                       age1960_PGSM_MAX_TXL, VI_ERROR_PARAMETER2)
        break;
    case age1960_DCS1800_BAND:
        CHK_LONG_RANGE(MSTXLevel,age1960_DCS1800_MIN_TXL,
                       age1960_DCS1800_MAX_TXL, VI_ERROR_PARAMETER2)
        break;
    case age1960_EGSM_BAND:
        CHK_LONG_RANGE(MSTXLevel, age1960_EGSM_MIN_TXL,
                       age1960_EGSM_MAX_TXL, VI_ERROR_PARAMETER2)
        break;
    case age1960_PCS1900_BAND:
        CHK_LONG_RANGE_2(MSTXLevel,age1960_PCS1900_MIN_TXL_1,
                         age1960_PCS1900_MAX_TXL_1,age1960_PCS1900_MIN_TXL_2,
                         age1960_PCS1900_MAX_TXL_2, VI_ERROR_PARAMETER2)
        break;
    case age1960_GSM850_BAND:
        CHK_LONG_RANGE(MSTXLevel, age1960_GSM850_MIN_TXL,
                       age1960_GSM850_MAX_TXL, VI_ERROR_PARAMETER2)
        break;
    default:
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/********************************************************************************************/
static ViStatus _VI_FUNC prepareForPvtPlot(ViSession    instrumentHandle,
        ViPInt16     Trig,
        ViPInt16     Sync,
        ViPInt16     BurstType,
        ViPReal64	TrigDelay,
        ViPReal64	ExpPower,
        ViPReal64	UpLinkFreq,
        ViPReal64	AmplOffset,
        ViPInt16     TXLevel,
        ViPInt16     Band )
/********************************************************************************************/
/* This function was added due to problems found in the age1960_PowerVsTimePlot_Q           */
/*  function. When reading the TXP prior to the FETCH:PVT:MTR? the box would assert.        */
/*  This was due to the trigstr returning corrupted from the box. This only happened when   */
/*  building the release dll. The debug dll worked fine. Something to do with the			*/
/*  optimisation on the different builds. The stack for the age1960_PowerVsTimePlot_Q       */
/*  would have to be quite large due to the plot data, so seperating the prepare out into   */
/*  this function fixed the problem..... weird stuff.                        				*/
/*                                                                           				*/
/* 	The FETCH:PVT:MTR? command is fairly fragile, get anything wrong and it 				*/
/*  hangs or asserts the box. If operating mode is TEST then make sure the 	 				*/
/*  ordinary read pvt works first e.g. BCH+TCH is required.					 				*/
/*  																	     				*/
/*  input:                                                                   				*/
/*      instrumentHandle session                                             				*/
/*																			 				*/
/*	   ViSession    instrumentHandle,										 				*/
/*	   ViPInt16     Trig: trigger source, can be AUTO/RF RISE/IMMEDIATE/PROTOCOL 			*/
/*					see enum type.    											 			*/
/*	   ViPInt16     Sync: enum burst sync. mid/ampl/none.                  					*/
/*	   ViPInt16     BurstType: expected burst type						 					*/
/*	   ViPReal64 	TrigDelay delay for the PVT measurement					 				*/
/*	   ViPReal64 	ExpPower: power in dBm    									 			*/
/*	   ViPReal64 	UpLinkFreq: The uplink frequency in Hz						 			*/
/*	   ViPReal64 	AmplOffset: The RF path amplitude offset in dB. 			 			*/
/*	   ViPInt16     TXLevel: The mobile TX level     						 				*/
/*	   ViPInt16     Band: Traffic band. EGSM/PGSM/							 				*/
/********************************************************************************************/
{
    ViReal64	MeasFreq;
    ViReal64  DownLinkFreq;
    ViReal64  TempAmplOffset;
    ViInt16   AmplOffsetState;
    ViInt16   RFANAutoTuned;
    ViChar    TrigStr[5];
    ViChar    SyncStr[5];
    ViChar    CurrentBandStr[5];
    ViChar    TCHBandStr[5];
    ViChar    ManualBandStr[5];
    ViChar    OperModeStr[5];
    ViChar    ConnStateStr[5];
    ViChar    expectedBurstStr[5];
    ViInt16   Bcc;

    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)


    if((errStatus = age1960_getCorrectionFreq_Q(instrumentHandle,age1960_CELL_1,&DownLinkFreq,UpLinkFreq)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    MeasFreq=*UpLinkFreq;
    if ((errStatus = age1960_getApproxOffset_Q (instrumentHandle, MeasFreq, &TempAmplOffset)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);


    if((errStatus =  sendQueryRequest(instrumentHandle, "SET:PVT:BSYN?;TRIG:SOUR?;DEL?;:"
                                      "RFAN:EXP:POW?;:CALL:TCH:BAND?;:CALL:MS:TXL?;:CALL:BCC?"
                                      ";:SYST:CORR:STAT?;:CALL:OPER:MODE?"
                                      ";:RFAN:MAN:BAND?;:RFAN:CONT:AUTO?;:CALL:STATUS:STATE?",
                                      8)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus = viScanf(instrumentHandle,"%[^;];%[^;];%lf;%lf;%[^;];%hd;"
                             "%hd;%hd;%[^;];%[^;];%ld;%s%*t",
                             SyncStr,
                             TrigStr,
                             TrigDelay,
                             ExpPower,
                             TCHBandStr,
                             TXLevel,
                             &Bcc,
                             &AmplOffsetState,
                             OperModeStr,
                             ManualBandStr,
                             &RFANAutoTuned,
                             ConnStateStr)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((strcmp(ConnStateStr,"CONN") != 0) && (strcmp(OperModeStr, "TEST") != 0 ))
        return statusUpdate(instrumentHandle,thisPtr,age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED);

    if (strcmp(TrigStr,"AUTO") == 0 )
    {
        if(strcmp(OperModeStr, "CELL")== 0)	*Trig = 0;
        if(strcmp(OperModeStr, "TEST")== 0) *Trig = 12;
    }
    if (strcmp(TrigStr,"RISE") == 0) *Trig = 12;
    if (strcmp(TrigStr,"IMM") == 0)  *Trig = 13;
    if (strcmp(TrigStr,"PROT") == 0) *Trig = 0;

    /* Convert sync information */
    if (strcmp(SyncStr,"MID") == 0)  *Sync = 0;
    if (strcmp(SyncStr,"AMPL") == 0) *Sync = 1;
    if (strcmp(SyncStr,"NONE") == 0) *Sync = 1;

    /* Convert band information */
    if (RFANAutoTuned == VI_TRUE)
        strcpy(CurrentBandStr,TCHBandStr);
    else
        strcpy(CurrentBandStr,ManualBandStr);

    if (strcmp(CurrentBandStr,"PGSM") == 0) *Band = 0;
    if (strcmp(CurrentBandStr,"EGSM") == 0) *Band = 1;
    if (strcmp(CurrentBandStr,"DCS") == 0)  *Band = 2;
    if (strcmp(CurrentBandStr,"PCS") == 0)  *Band = 3;
    if (strcmp(CurrentBandStr,"GSM850") == 0)  *Band = 4;


    /* Convert Burst Type information ... taken from GEG */
    if (strcmp(ConnStateStr,"CONN") == 0)  *BurstType = Bcc;
    if (strcmp(OperModeStr, "TEST") == 0 )
    {
        if((errStatus =  sendQueryRequest(instrumentHandle, "CALL:BURST:TYPE?",8)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle, "%s%*t", expectedBurstStr)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if(strcmp(expectedBurstStr, "TSC0") == 0) *BurstType = 0;
        if(strcmp(expectedBurstStr, "TSC1") == 0) *BurstType = 1;
        if(strcmp(expectedBurstStr, "TSC2") == 0) *BurstType = 2;
        if(strcmp(expectedBurstStr, "TSC3") == 0) *BurstType = 3;
        if(strcmp(expectedBurstStr, "TSC4") == 0) *BurstType = 4;
        if(strcmp(expectedBurstStr, "TSC5") == 0) *BurstType = 5;
        if(strcmp(expectedBurstStr, "TSC6") == 0) *BurstType = 6;
        if(strcmp(expectedBurstStr, "TSC7") == 0) *BurstType = 7;
        if(strcmp(expectedBurstStr, "RACH") == 0)
        {
            *BurstType = 10;
        }
    }

    *AmplOffset = TempAmplOffset*AmplOffsetState;


    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}         /* ----- end of function ----- */

/*****************************************************************************/
static ViStatus _VI_FUNC waitForInterrupt(
    ViSession  instrumentHandle,
    ViInt32    timeout)
/*****************************************************************************/
/*  This function will wait for SRQ to be pulled or the timeout              */
/*  to expire.                                                               */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    timeout (in)                                                */
/*      Maximum time to wait before timing out (in milliseconds).            */
/*      This function will only wait in second resolution.  Therefore,       */
/*      timeout will be rounded up the the nearest second                    */
/*****************************************************************************/

{
    ViStatus    errStatus;
    ViEventType eventType;
    ViEvent     context;
    ViInt32     stat;
    time_t      tStart;
    struct      age1960_globals *thisPtr;
    ViInt32     timeoutLeft;
    ViInt32     maxTimeToWait;
    ViChar      registerQuery[50];

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "waitForInterrupt",",%ld",timeout);

    switch(thisPtr->interruptType)
    {
    case age1960_WAITFOR_NMRR:
        strcpy(registerQuery,"STAT:OPER:NMRR:GSM:EVENT?");
        break;
    case age1960_WAITFOR_CALL:
        strcpy(registerQuery,"STAT:OPER:CALL:GSM:EVENT?"); /* added mom1.2 changes for Status subsystem*/
        break;
    case age1960_WAITFOR_MEAS:
        strcpy(registerQuery,"STAT:OPER:EVENT?");
        break;
    default: /* Should never get here */
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
    }

    /* Have VISA generate an event when SRQ goes true */
    if((errStatus=viEnableEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,VI_QUEUE,VI_NULL)) < VI_SUCCESS)
    {
        srq_cleanup(instrumentHandle);
        return errStatus;
    }

    /* Get the register bit */
    if((errStatus = age1960_cmdInt32_Q(instrumentHandle, registerQuery, &stat))<VI_SUCCESS)
    {
        srq_cleanup(instrumentHandle);
        return errStatus;
    }

    maxTimeToWait = 1000 * (ViInt32) ceil((double)timeout/1000); /* up maxTimeToWait to the */
    /* nearest whole second */
    timeoutLeft = maxTimeToWait;
    tStart=time(NULL); /* Get the current time to second resolution*/

    while((stat &  thisPtr->interruptBitPower)!= thisPtr->interruptBitPower )
    {
        /* Go to sleep until an SRQ (or timeout) */
        if((errStatus = viWaitOnEvent(instrumentHandle,VI_EVENT_SERVICE_REQ,timeoutLeft,&eventType,&context)) < VI_SUCCESS)
        {
            /* If a timeout, maybe SRQ is stuck high by some other instrument. */
            /* Since the instrument should be ready by now, check the status byte one last time */
            if( errStatus == VI_ERROR_TMO )
            {
                if((errStatus = age1960_cmdInt32_Q(instrumentHandle,registerQuery,&stat))<VI_SUCCESS)
                {
                    srq_cleanup(instrumentHandle);
                    return VI_ERROR_TMO; /* use the timeout errStatus as the returned error */
                }
                if ((stat & thisPtr->interruptBitPower) == thisPtr->interruptBitPower) break;
            }
            srq_cleanup(instrumentHandle);
            return VI_ERROR_TMO;  /* use the timeout errStatus as the returned error */
        }

        /* Always close the context to avoid a memory leak. If the viWaitOnEvent was*/
        /*  unsuccessful, there is no need to close the context as it is not valid. */
        if((errStatus = viClose(context))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }
        if((errStatus = age1960_cmdInt32_Q(instrumentHandle,registerQuery,&stat))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }
        /*   Just to be safe check that some time is left.  It should never go  */
        /*   negative unless the viWaitOnEvent timed out which should be caught */
        /*   earlier. */
        timeoutLeft = maxTimeToWait - 1000 * (ViInt32) difftime(time(NULL),tStart);
        if(timeoutLeft<=0)
        {
            srq_cleanup(instrumentHandle);
            return VI_ERROR_TMO;
        }
    }
    /* Disable SRQ causing an event */
    if((errStatus = viDisableEvent(instrumentHandle, VI_EVENT_SERVICE_REQ, VI_QUEUE))<VI_SUCCESS)
    {
        srq_cleanup(instrumentHandle);
        return errStatus;
    }
    srq_cleanup(instrumentHandle);
    return VI_SUCCESS;
}        /* ----- end of function ----- */

/*****************************************************************************/

static ViStatus _VI_FUNC confForInterrupt(
    ViSession  instrumentHandle,
    ViInt16    interruptType,
    ViInt16    interruptCondition)
/*****************************************************************************/
/*  This function will configure the status subsystem to wait for the        */
/*  desired interrupt.  After this function is called the                    */
/*  waitForInterrupt must be called.										 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    interruptType (in)                                          */
/*      Indicates the type of interrupt to configure for.                    */
/*      Available choices are:                                               */
/*      0  Wait for specific meas results ready    (age1960_WAITFOR_NMRR)    */
/*      1  Wait for call conditions                (age1960_WAITFOR_CALL)    */
/*      2  Wait for any meas results ready         (age1960_WAITFOR_MEAS)    */
/*    ViInt16    interruptCondition (in)                                     */
/*      Indicates the type of interrupt conditions to configure for.         */
/*      Available choices for age1960_WAITFOR_NMRR are:                      */
/*      0  Analog Audio Meas Fcn        (age1960_AAUDIO)                     */
/*      1  Bit Error Rate Meas Fcn      (age1960_BER)                        */
/*      2  Decoded Audio Meas Fcn       (age1960_DAUDIO)                     */
/*      3  Fast Bit Error Meas Fcn      (age1960_FBER)                       */
/*      4  ORFSpectrum Meas Fcn         (age1960_ORFS)                       */
/*      5  Phase Freq Error Meas Fcn    (age1960_PFER)                       */
/*      6  Power vs Time Meas Fcn       (age1960_PVTIME)                     */
/*      7  TX Power Meas Fcn            (age1960_TXPOWER)                    */
/*      8  Dynamic Power Meas Fcn       (age1960_DPOWER)                     */
/*      9  IQ Tuning Meas Fcn           (age1960_IQTUNING)                   */
/*      Available choices for age1960_WAITFOR_CALL are:                      */
/*      0  Call at the Connected state  (age1960_CALL_CONNECTED)             */
/*      1  Call at the Alerting state   (age1960_CALL_ALERTING)              */
/*      2  Call at the Idle state       (age1960_CALL_IDLE)                  */
/*****************************************************************************/

{

    ViStatus    errStatus ;
    struct      age1960_globals *thisPtr;
    ViInt32     bitPower;
    ViChar	  NMRRregister[30];

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "confForInterrupt",",%hd,%hd",interruptType, interruptCondition);

    CHK_ENUM(interruptType, 2, VI_ERROR_PARAMETER2)

    if(interruptType == age1960_WAITFOR_NMRR)
    {
        CHK_ENUM(interruptCondition, 7, VI_ERROR_PARAMETER3)

        /* Get COMM (bit 1), GSM (bit 2) from the STAT:OPER:NMRR register via */
        /* Get NMRR Summary(bit 9) in the STAT:OPER register set propagated to bit 7 of stb */
        if((errStatus = viPrintf(instrumentHandle,"*CLS;*SRE 0;:STAT:OPER:NMRR:PTR 6;NTR 0;ENAB 6"
                                 ";:STAT:OPER:PTR 512;NTR 0;ENAB 512"
                                 ";*SRE 128\n"))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }

        switch(interruptCondition)
        {
        case age1960_TXPOWER:
            bitPower = 2;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_PVTIME:
            bitPower = 4;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_PFER:
            bitPower = 8;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_ORFS:
            bitPower = 16;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_AAUDIO:
            bitPower = 32;
            strcpy(NMRRregister, "STAT:OPER:NMRR:COMM:PTR");
            break;
        case age1960_DAUDIO:
            bitPower = 64;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_FBER:
            bitPower = 128;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_BER:
            bitPower = 256;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_IQTUNING:
            bitPower = 512;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        case age1960_DPOWER:
            bitPower = 1024;
            strcpy(NMRRregister, "STAT:OPER:NMRR:GSM:PTR");
            break;
        default: /* Should never get here */
            return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
        }
        /* Setup the STAT:OPER:NMRR:GSM register or COMM register*/

        if((errStatus = viPrintf(instrumentHandle, "%s %ld;NTR 0;ENAB %ld\n",
                                 NMRRregister, bitPower, bitPower))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }
    }
    if(interruptType == age1960_WAITFOR_CALL)
    {
        CHK_ENUM(interruptCondition, 2, VI_ERROR_PARAMETER3)
        /* Get Call Summary(bit 10) in the STAT:OPER register set propagated to bit 7 of stb
          and bit 2 of STAT:OPER:CALL to prop to bit 10 of call summary. */
        if((errStatus = viPrintf(instrumentHandle,"*CLS;*SRE 0;STAT:OPER:PTR 1024;NTR 0;ENAB 1024"
                                 ";:STAT:OPER:CALL:PTR 4;NTR 0;ENAB 4;*SRE 128\n"))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }

        switch(interruptCondition)
        {
        case age1960_CALL_IDLE:
            bitPower = 2;
            break;
        case age1960_CALL_CONNECTED:
            bitPower = 4;
            break;
        case age1960_CALL_ALERTING:
            bitPower = 8;
            break;
        default: /* Should never get here */
            return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER3);
        }
        /* Setup the STAT:OPER:CALL:GSM register */
        if((errStatus = viPrintf(instrumentHandle, "STAT:OPER:CALL:GSM:PTR %ld;NTR 0;ENAB %ld\n",
                                 bitPower, bitPower))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }
    }

    if(interruptType == age1960_WAITFOR_MEAS)
    {

        bitPower = 512;

        /* Get bit 2 os STAT:OPER:NMRR to prop to NMRR Summary(bit 9) in the STAT:OPER
        register set propagated to bit 7 of stb */
        if((errStatus = viPrintf(instrumentHandle,"*CLS;*SRE 0;STAT:OPER:PTR 512;NTR 0;ENAB 512"
                                 ";:STAT:OPER:NMRR:PTR 6;NTR 0;ENAB 6;*SRE 128\n"))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }

        /* Setup the STAT:OPER:NMRR:GSM register for any measurement */
        if((errStatus = viPrintf(instrumentHandle,
                                 "STAT:OPER:NMRR:COMM:PTR 32767;NTR 0;ENAB 32767"
                                 ";:STAT:OPER:NMRR:GSM:PTR 32767;NTR 0;ENAB 32767\n"))<VI_SUCCESS)
        {
            srq_cleanup(instrumentHandle);
            return errStatus;
        }
    }

    /* keep the bit and the interrupt type to be used by the */
    /* waitForInterrupt  function */
    thisPtr->interruptBitPower  = bitPower;
    thisPtr->interruptType = interruptType;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
/*  This function first queries the instrument to determine the model number */
/*  of the TA that is currently loaded. If the TA has the syntax capability  */
/*  the format will be cataloged to see if it contains GSM. If it contains   */
/*  GSM, the index is set to 1 which throughout this driver will append the  */
/*  proper suffix to certain commands to allow proper operation of this      */
/*  driver with a "Combo TA".                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*                                                                           */
/*****************************************************************************/

static ViStatus determineSuffix(ViSession instrumentHandle)
{
    ViStatus  errStatus;
    char formatInfo[256];
    ViChar currentTAName[256];
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    /*************************************************************/
    /* Set initial values of variables in global memory.         */
    /*************************************************************/

    thisPtr->TAIndex = 0;
    strcpy(thisPtr->TAModel,"Unknown");

    /*************************************************************/
    /* Determine the instrument's current TA Model.              */
    /*************************************************************/

    if ((errStatus =  sendQueryRequest(instrumentHandle,
                                       "SYST:CURR:TA:MODEL?;:SYST:APPL:CURR:REV?",10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if((errStatus = viScanf(instrumentHandle, "\"%[^\"]\";\"%[^\"]%*t",
                            thisPtr->TAModel, thisPtr->TArev)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    /*************************************************************/
    /* Some versions of the following TA models will not have    */
    /* the SYST:APPL:CAT:FORM command capability. This is okay   */
    /* because they are not combo TA's. If the following strcmp  */
    /* statement is true, combo TA's with GSM capability is	   	 */
    /* what the code looks for.                                  */
    /*************************************************************/

    if (strcmp(thisPtr->TAModel, "E6701A") != 0 &&
            strcmp(thisPtr->TAModel, "E1960A") != 0 &&
            strcmp(thisPtr->TAModel, "E1961A") != 0 &&
            strcmp(thisPtr->TAModel, "E1962A") != 0 &&
            strcmp(thisPtr->TAModel, "E1964A") != 0)
    {

        if((errStatus = sendQueryRequest(instrumentHandle,
                                         "SYST:APPL:CAT:FORM?",10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        if((errStatus = viScanf(instrumentHandle, "%s%*t",
                                formatInfo)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        if((errStatus = cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        /* GSM in the string means it is licensed and usable */
        if(strstr(formatInfo, age1960_SUPPORTED_FORMAT_STRING) != NULL)
        {
            thisPtr->TAIndex=1;

        }
        else
        {
            /* Get the currently loaded TA name from testset and determine if it contains GSM; if it */
            /* does then it means the GSM part of the combo TA is not licensed. If a TA does not     */
            /* contain GSM such as E1962B and other possible single or combo TA's, then drop through.*/
            if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:CURR?",30000)) < VI_SUCCESS)
            {
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
            if ((errStatus = viScanf(instrumentHandle, "%s%*t",currentTAName)) < VI_SUCCESS)
            {
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
            if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            {
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
            if (strstr(currentTAName,"GSM") != NULL)
            {
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_LICENSE);
            }
        }
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}

/*****************************************************************************/
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
/*****************************************************************************/




/*****************************************************************************/
ViStatus _VI_FUNC age1960_init(
    ViRsrc resourceName,
    ViBoolean IDQuery,
    ViBoolean resetDevice,
    ViPSession instrumentHandle)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      resourceName (in)                                       */
/*      The instrument's VISA address.                                       */
/*      GPIB Example: "GPIB0::20::INSTR"                                     */
/*    ViBoolean      IDQuery (in)                                            */
/*      If VI_TRUE, validate instrument identity                             */
/*    ViBoolean      resetDevice (in)                                        */
/*      If VI_TRUE, perform an instrument reset                              */
/*    ViPSession     instrumentHandle (out)                                  */
/*      Pointer to Instrument Handle.  Returns VI_NULL if the init fails.    */
/*****************************************************************************/

{
    struct     age1960_globals *thisPtr;
    ViStatus   errStatus;
    ViSession  defRM;
    ViChar     idn_buf[256];

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

    if ((errStatus = viOpen(defRM, resourceName, VI_NULL, VI_NULL,
                            instrumentHandle)) < VI_SUCCESS)
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

    if ((thisPtr =
                (struct age1960_globals *)malloc(sizeof(struct age1960_globals))) == 0)
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

    /*************************************************************/
    /* Set initial values of variables in global memory.         */
    /*************************************************************/
    thisPtr->traceCallback = NULL;
    thisPtr->errNumber = VI_SUCCESS;
    thisPtr->errMessage[0] = 0;
    thisPtr->errQueryDetect = VI_FALSE;
    thisPtr->dTime = 0;
    strcpy(thisPtr->address, resourceName);
    thisPtr->useSRQ = VI_FALSE;
    thisPtr->TAModel[0] = '\0';
    thisPtr->TArev[0] = '\0';
    thisPtr->loopback = 0; /* A.01.00 defect4 is fixed here. Need to initialise
                        loopback here to 0 */

    if ((errStatus = viGetAttribute(*instrumentHandle, VI_ATTR_INTF_TYPE,
                                    &thisPtr->interfaceType)) < VI_SUCCESS)
        return initError(instrumentHandle, defRM, errStatus);


    /*************************************************************/
    /* Reset the instrument if so instructed.                    */
    /*   GPIB reset includes a 488.2 device clear.               */
    /*************************************************************/

    if( VI_TRUE == resetDevice )
    {
        if (thisPtr->interfaceType == VI_INTF_GPIB)
            if (viClear(*instrumentHandle) <  VI_SUCCESS)
                return initError(instrumentHandle, defRM,
                                 age1960_INSTR_ERROR_RESET_FAILED);

        if (age1960_reset(*instrumentHandle) <  VI_SUCCESS)
            return initError(instrumentHandle, defRM,
                             age1960_INSTR_ERROR_RESET_FAILED);
    }

    /*************************************************************/
    /* Validate the instrument's identity.                       */
    /*************************************************************/

    if( VI_TRUE == IDQuery )
    {
        if (thisPtr->interfaceType == VI_INTF_GPIB)
            if(resetDevice != VI_TRUE)   /* only send the viClear if the reset was   */
            {
                /* not performed, since the reset above did */
                /* a viClear                                */
                if (viClear(*instrumentHandle) <  VI_SUCCESS)
                    return initError(instrumentHandle, defRM, VI_ERROR_FAIL_ID_QUERY);
            }
        if ((errStatus = delayVQueryf(*instrumentHandle, "*IDN?\n", "%t",idn_buf)) < VI_SUCCESS)
            return initError(instrumentHandle, defRM, errStatus);

        if (strncmp(idn_buf, age1960_IDN_STRING, strlen(age1960_IDN_STRING)))
            return initError(instrumentHandle, defRM, VI_ERROR_FAIL_ID_QUERY);
    }

    /*************************************************************/
    /* Determine suffix for single or combo TA.                  */
    /*************************************************************/

    if ((errStatus = determineSuffix(*instrumentHandle)) < VI_SUCCESS)   /* mbn 12/20 */
        return statusUpdate(*instrumentHandle,thisPtr,errStatus);

    return statusUpdate(*instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_close(
    ViSession instrumentHandle)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      instrumentHandle (in)                                   */
/*      Instrument Handle returned from age1960_init().                      */
/*****************************************************************************/

{
    struct age1960_globals *thisPtr;
    ViStatus errStatus;
    ViSession defRM;

    GET_GLOBALS(instrumentHandle, thisPtr)

    /*************************************************************/
    /* Retrieve Resource Management session, free global memory  */
    /*   for the session (if it exists), and close the vi and    */
    /*   RM sessions.                                            */
    /*************************************************************/

    if ((errStatus = viGetAttribute(instrumentHandle, VI_ATTR_RM_SESSION, &defRM))
            < VI_SUCCESS)
    {
        thisPtr->errNumber = errStatus;
        return errStatus;
    }

    if( thisPtr)
        free(thisPtr);

    return viClose(defRM);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_reset(
    ViSession instrumentHandle)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession      instrumentHandle (in)                                   */
/*      Instrument Handle returned from age1960_init().                      */
/*****************************************************************************/

{
    struct age1960_globals *thisPtr;
    ViStatus errStatus;
    ViInt32     presentTimeout;
    ViChar     idn_buf[256];

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = delayVPrintf(instrumentHandle, "*RST\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    /* make sure the instrument completes the reset prior to letting the code continue */
    /* get original timeout value */
    if ((errStatus =  viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, &presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* set the timeout to 12 seconds to prevent timeout error - allows unit to complete reset before query */
    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, 12000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = delayVQueryf(instrumentHandle, "*IDN?\n", "%t",idn_buf)) < VI_SUCCESS)
    {
        viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    /* set the timeout back to original value */
    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* Clear error queue */
    if((errStatus = delayVPrintf(instrumentHandle, "*CLS\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    /* Set the globals to the reset states */

    thisPtr->interruptBitPower = 0;
    thisPtr->interruptType = 0;
    thisPtr->loopback = 0;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_self_test(
    ViSession instrumentHandle,
    ViPInt16  selfTestResult,
    ViPString selfTestMessage)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession   instrumentHandle (in)                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPInt16    selfTestResult (out)                                       */
/*      Self-test result                                                     */
/*    ViPString   selfTestMessage[] (out)                                    */
/*      Self-test status message string, limited to 256 characters.          */
/*****************************************************************************/
{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;
    ViInt32     presentTimeout;
    *selfTestResult = -1;
    selfTestMessage[0] = 0;
    sprintf(selfTestMessage, age1960_MSG_SELF_TEST_FAILED);

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_self_test","");

    /* make sure the instrument completes the reset prior to letting the code continue */
    /* get original timeout value */
    if ((errStatus =  viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, &presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* set the timeout to 15 seconds to prevent timeout error - allows unit to complete self-test query */
    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, 15000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if((errStatus = delayVQueryf(instrumentHandle, "*TST?\n", "%hd%*t",
                                 selfTestResult)) < VI_SUCCESS)
    {
        *selfTestResult = -1;
        /* set the timeout back to original value */
        if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    switch( *selfTestResult)
    {
    case 0:
        sprintf(selfTestMessage, age1960_MSG_SELF_TEST_PASSED);
        break;

    default:
        sprintf(selfTestMessage, age1960_MSG_SELF_TEST_FAILED);
        break;
    }
    /* set the timeout back to original value */
    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}   /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_error_query(
    ViSession  instrumentHandle,
    ViPInt32   errorCode,
    ViPString  errorMessage)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession    instrumentHandle (in)                                     */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPInt32     errorCode (out)                                           */
/*      An instrument error code                                             */
/*    ViPString    errorMessage[] (out)                                      */
/*      Instrument error message string, limited to 256 characters.          */
/*****************************************************************************/

{
    struct age1960_globals *thisPtr;
    ViStatus errStatus;

    *errorCode = -1;
    errorMessage[0] = 0;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = delayVQueryf( instrumentHandle, "SYST:ERR?\n", "%ld,%t",
                                   errorCode, errorMessage)) < VI_SUCCESS)
    {
        *errorCode = -1;
        errorMessage[0] = 0;
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    }

    /* get rid of extra LF at the end of the error_message */
    errorMessage[strlen(errorMessage)-1] = 0;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_error_message(
    ViSession  instrumentHandle,
    ViStatus   statusCode,
    ViPString  message)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession   instrumentHandle (in)                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*      May be VI_NULL for this function                                     */
/*    ViStatus    statusCode (in)                                            */
/*      The error return value from an instrument driver function            */
/*    ViPString   message[] (out)                                            */
/*      Error message string.  This is limited to 256 characters.            */
/*****************************************************************************/

{
    struct    age1960_globals *thisPtr;
    ViStatus  errStatus;
    ViInt32   idx;

    message[0] = 0;

    thisPtr = NULL;
    if (instrumentHandle != VI_NULL)
        errStatus = viGetAttribute(instrumentHandle, VI_ATTR_USER_DATA,
                                   (ViAddr) &thisPtr);

    if (statusCode == VI_SUCCESS)
    {
        sprintf(message, age1960_MSG_NO_ERRORS);
        if (thisPtr) thisPtr->errNumber = VI_SUCCESS;
        return VI_SUCCESS;
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

    for (idx=0; idx < (ViInt32) (sizeof instrErrMsgTable /
                                 sizeof(struct instrErrStruct)); idx++)
    {
        if (instrErrMsgTable[idx].errStatus == statusCode)
        {
            if ((thisPtr) && (thisPtr->errNumber == statusCode))
            {
                sprintf(message, "%s;  %s",
                        instrErrMsgTable[idx].errMessage,
                        thisPtr->errMessage);
                thisPtr->errMessage[0] = 0;   /* Added GB to clear appended error */
                /* message. */
            }
            else
                strcpy(message, instrErrMsgTable[idx].errMessage);

            if (thisPtr) thisPtr->errNumber = VI_SUCCESS;
            return VI_SUCCESS;
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

    errStatus = viStatusDesc(instrumentHandle, statusCode, message);
    if (errStatus == VI_SUCCESS)
    {
        if( (thisPtr) && (thisPtr->errNumber == statusCode))
        {
            strcat(message, ";  ");
            strcat(message, thisPtr->errMessage);
        }
        if (thisPtr) thisPtr->errNumber = VI_SUCCESS;
        return VI_SUCCESS;
    }

    /*************************************************************/
    /*   At this point vi is either INVALID or VI_NULL           */
    /*************************************************************/

    if (instrumentHandle == VI_NULL)
    {
        strcpy(message, age1960_MSG_VI_OPEN_ERR);
        if (thisPtr) thisPtr->errNumber = VI_SUCCESS;
        return VI_SUCCESS;
    }

    sprintf(message, age1960_MSG_INVALID_STATUS "  %ld"
            age1960_MSG_INVALID_STATUS_VALUE, (long)statusCode);

    if (thisPtr) thisPtr->errNumber = VI_ERROR_PARAMETER2;
    return VI_ERROR_PARAMETER2;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_revision_query(
    ViSession    instrumentHandle,
    ViPString    instrumentDriverRevision,
    ViPString    firmwareRevision)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession    instrumentHandle (in)                                     */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPString    instrumentDriverRevision[] (out)                          */
/*      Instrument driver revision.  This is limited to 256 characters.      */
/*    ViPString    firmwareRevision[] (out)                                  */
/*      Instrument firmware revision.  This is limited to 256 characters.    */
/*****************************************************************************/

{
    struct    age1960_globals *thisPtr;
    ViStatus  errStatus;

    GET_GLOBALS(instrumentHandle, thisPtr)

    instrumentDriverRevision[0] = 0;
    firmwareRevision[0] = 0;

    sprintf(instrumentDriverRevision, "%s", age1960_REV_CODE);

    if ((errStatus = delayVQueryf( instrumentHandle, "SYST:CURR:TA:REV?\n", "\"%[^\"]%*t",firmwareRevision))
            < VI_SUCCESS)
    {
        instrumentDriverRevision[0] = 0;
        firmwareRevision[0]  = 0;
        return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_timeOut (
    ViSession  instrumentHandle,
    ViInt32    setTimeOut)
/*****************************************************************************/
/*  Sets the current setting of the driver timeOut value of the instrument   */
/*  in milliseconds.                                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt32    setTimeOut (in)                                             */
/*      The period of time which VISA will allow an instrument command to    */
/*      wait before stopping it's execution.                                 */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    CHK_LONG_RANGE(setTimeOut, age1960_TIMEOUT_MIN,
                   age1960_TIMEOUT_MAX, VI_ERROR_PARAMETER2);

    if ((errStatus = viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                    setTimeOut)) < VI_SUCCESS)
    {
        thisPtr->errNumber = errStatus;
        return errStatus;
    }

    thisPtr->errNumber = VI_SUCCESS;
    return VI_SUCCESS;

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_timeOut_Q (
    ViSession  instrumentHandle,
    ViPInt32   timeOut)
/*****************************************************************************/
/*  Returns the current setting of the timeOut value of the instrument in    */
/*  milliseconds.                                                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPInt32   timeOut (out)                                               */
/*      The period of time which VISA will allow an instrument command to    */
/*      wait before stopping it's execution.                                 */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if ((errStatus = viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                    timeOut)) < VI_SUCCESS)
    {
        thisPtr->errNumber = errStatus;
        return errStatus;
    }

    thisPtr->errNumber = VI_SUCCESS;
    return VI_SUCCESS;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_errorQueryDetect(
    ViSession  instrumentHandle,
    ViBoolean  setErrorQueryDetect)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViBoolean  setErrorQueryDetect (in)                                    */
/*      Enables (VI_TRUE) or disables (VI_FALSE) automatic instrument error  */
/*      querying                                                             */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    CHK_BOOLEAN(setErrorQueryDetect, VI_ERROR_PARAMETER2);

    thisPtr->errQueryDetect = setErrorQueryDetect;

    thisPtr->errNumber = VI_SUCCESS;
    return VI_SUCCESS;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_errorQueryDetect_Q(
    ViSession   instrumentHandle,
    ViPBoolean  errorQueryDetect)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession   instrumentHandle (in)                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPBoolean  errorQueryDetect (out)                                     */
/*      Equals VI_TRUE if automatic instrument error querying is being       */
/*      being performed.                                                     */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    *errorQueryDetect = thisPtr->errQueryDetect;

    thisPtr->errNumber = VI_SUCCESS;
    return VI_SUCCESS;
}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_dcl(
    ViSession  instrumentHandle)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    if (thisPtr->interfaceType == VI_INTF_GPIB)
        if ((errStatus = viClear(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate( instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_opc_Q(
    ViSession   instrumentHandle,
    ViPBoolean  instrumentReady)
/*****************************************************************************/
/*  Sends an *OPC? command to the instrument and returns VI_TRUE when all    */
/*  pending operations are complete.                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession   instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPBoolean  instrumentReady (out)                                      */
/*      Returns VI_TRUE when pending operations are complete.                */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    thisPtr->dTime = 200;

    if ((errStatus = delayVQueryf(instrumentHandle, "*OPC?\n", "%hd%*t",
                                  instrumentReady)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readStatusByte_Q(
    ViSession  instrumentHandle,
    ViPInt16   statusByte)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPInt16  statusByte (out)                                             */
/*      Returns the contents of the status byte                              */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;
    ViUInt16  stb;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readStatusByte_Q", "");

    if ((errStatus = viReadSTB(instrumentHandle, &stb)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus );

    *statusByte = (ViInt16)stb;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_operEvent_Q(
    ViSession  instrumentHandle,
    ViInt16    operationName,
    ViPInt32   operationEventRegister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16    operationName (in)                                          */
/*      The name of the operational event register to query.  The available  */
/*       names are:                                                          */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_OPER_SUMMARY      0       Operation Summary Register         */
/*      age1960_OPER_NMRR         1       Operation NMRR Register            */
/*      age1960_OPER_CALL         2       Operation Call Register            */
/*		age1960_OPER_NMRR_GSM	  3		  Operation NMRR GSM				 */
/*		age1960_OPER_CALL_GSM	  4		  Operational Call GSM				 */
/*    ViPInt32  operationEventRegister (out)                                 */
/*      Returns the contents of the operation event register                 */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_operEvent_Q", ",%hd", operationName);

    CHK_ENUM(operationName, 4, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVQueryf(instrumentHandle,"STAT%s:EVEN?\n", "%ld%*t",
                                  age1960_oper_regs_a[operationName],
                                  operationEventRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_operCond_Q(
    ViSession  instrumentHandle,
    ViInt16    operationName,
    ViPInt32   operationConditionRegister )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16    operationName (in)                                          */
/*      The name of the operation condition register to query.  The          */
/*       available names are:                                                */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_OPER_SUMMARY      0       Operation Summary Register         */
/*      age1960_OPER_NMRR         1       Operation NMRR Register            */
/*      age1960_OPER_CALL         2       Operation Call Register            */
/*		age1960_OPER_NMRR_GSM	  3		  Operation NMRR GSM				 */
/*		age1960_OPER_CALL_GSM	  4		  Operational Call GSM				 */
/*    ViPInt32  operationConditionRegister (out)                             */
/*      Returns the contents of the operation condition register             */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_operCond_Q", ",%hd", operationName);

    CHK_ENUM(operationName, 4, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVQueryf(instrumentHandle, "STAT%s:COND?\n", "%ld%*t",
                                  age1960_oper_regs_a[operationName],
                                  operationConditionRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_quesEvent_Q(
    ViSession  instrumentHandle,
    ViInt16    questionableName,
    ViPInt32   questionableEventRegister )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16    questionableName (in)                                       */
/*      The name of the questionable event register to query.  The available */
/*       names are:                                                          */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_QUES_SUMMARY      0       Questionable Summary Register      */
/*      age1960_QUES_CALL         1       Questionable Call Register         */
/*      age1960_QUES_ERROR        2       Questionable Error Register        */
/*      age1960_QUES_HARDWARE     3       Questionable Hardware Register     */
/*		age1960_QUES_CALL_GSM	  4		  Questionable Call GSM				 */
/*		age1960_QUES_ERROR_GSM	  5		  Questionable Error GSM			 */
/*    ViPInt32  questionableEventRegister (out)                              */
/*      Returns the contents of the questionable event register              */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_quesEvent_Q", ",%hd", questionableName);

    CHK_ENUM(questionableName, 5, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVQueryf(instrumentHandle,"STAT%s:EVEN?\n", "%ld%*t",
                                  age1960_ques_regs_a[questionableName],
                                  questionableEventRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_quesCond_Q(
    ViSession  instrumentHandle,
    ViInt16    questionableName,
    ViPInt32   questionableConditionRegister )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16    questionableName (in)                                       */
/*      The name of the questionable condition register to query.  The       */
/*       available names are:                                                */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_QUES_SUMMARY      0       Questionable Summary Register      */
/*      age1960_QUES_CALL         1       Questionable Call Register         */
/*      age1960_QUES_ERROR        2       Questionable Error Register        */
/*      age1960_QUES_HARDWARE     3       Questionable Hardware Register     */
/*		age1960_QUES_CALL_GSM	  4		  Questionable Call GSM				 */
/*		age1960_QUES_ERROR_GSM	  5		  Questionable Error GSM			 */
/*    ViPInt32  questionableConditionRegister (out)                          */
/*      Returns the contents of the questionable condition register          */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_quesCond_Q", ",%hd,%s", questionableName,
                  age1960_ques_regs_a[questionableName]);

    CHK_ENUM(questionableName, 5, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVQueryf(instrumentHandle, "STAT%s:COND?\n", "%ld%*t",
                                  age1960_ques_regs_a[questionableName],
                                  questionableConditionRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_quesRegisterEnable(
    ViSession  instrumentHandle,
    ViInt16    questionableName,
    ViInt32    quesPosTransition,
    ViInt32    quesNegTransition,
    ViInt32    quesEnableBits)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16   questionableName (in)                                        */
/*      The name of the questionable register to enable.  The available      */
/*       names are:                                                          */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_QUES_SUMMARY      0       Questionable Summary Register      */
/*      age1960_QUES_CALL         1       Questionable Call Register         */
/*      age1960_QUES_ERROR        2       Questionable Error Register        */
/*      age1960_QUES_HARDWARE     3       Questionable Hardware Register     */
/*		age1960_QUES_CALL_GSM	  4		  Questionable Call GSM				 */
/*		age1960_QUES_ERROR_GSM	  5		  Questionable Error GSM			 */
/*    ViInt32   quesPosTransition (in)                                       */
/*      The bit(s) to detect positive edge transitions from the condition    */
/*      register.                                                            */
/*    ViInt32   quesNegTransition (in)                                       */
/*      The bit(s) to detect negative edge transitions from the condition    */
/*      register.                                                            */
/*    ViInt32   quesEnableBits (in)                                          */
/*      The bit(s) to enable from the event register.                        */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_quesRegisterEnable", ",%hd,%ld,%ld,%ld",
                  questionableName, quesPosTransition, quesNegTransition,
                  quesEnableBits);

    CHK_ENUM(questionableName, 5, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(quesPosTransition, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER3)

    CHK_LONG_RANGE(quesNegTransition, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER4)

    CHK_LONG_RANGE(quesEnableBits, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER5)

    if ((errStatus = delayVPrintf(instrumentHandle, "STAT%s:PTR %ld;NTR %ld;ENAB %ld\n",
                                  age1960_ques_regs_a[questionableName],
                                  quesPosTransition,
                                  quesNegTransition,
                                  quesEnableBits)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_operRegisterEnable(
    ViSession  instrumentHandle,
    ViInt16    operationName,
    ViInt32    operPosTransition,
    ViInt32    operNegTransition,
    ViInt32    operEnableBits)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt16   operationName (in)                                           */
/*      The name of the operation register to enable.  The available         */
/*       names are:                                                          */
/*                                                                           */
/*      Constant Name            Value    Description                        */
/*      -----------------------------------------------------------          */
/*      age1960_OPER_SUMMARY      0       Operation Summary Register         */
/*      age1960_OPER_NMRR         1       Operation NMRR Register            */
/*      age1960_OPER_CALL         2       Operation Call Register            */
/*		age1960_OPER_NMRR_GSM	  3		  Operation NMRR GSM				 */
/*		age1960_OPER_CALL_GSM	  4		  Operational Call GSM				 */
/*    ViInt32   operPosTransition (in)                                       */
/*      The bit(s) to detect positive edge transitions from the condition    */
/*      register.                                                            */
/*    ViInt32   operNegTransition (in)                                       */
/*      The bit(s) to detect negative edge transitions from the condition    */
/*      register.                                                            */
/*    ViInt32   operEnableBits (in)                                          */
/*      The bit(s) to enable from the event register.                        */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_operRegisterEnable", ",%hd,%ld,%ld,%ld",
                  operationName, operPosTransition, operNegTransition,
                  operEnableBits);

    CHK_ENUM(operationName, 4, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(operPosTransition, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER3)

    CHK_LONG_RANGE(operNegTransition, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER4)

    CHK_LONG_RANGE(operEnableBits, age1960_REGISTER_MIN,
                   age1960_REGISTER_MAX, VI_ERROR_PARAMETER5)

    if ((errStatus = delayVPrintf(instrumentHandle, "STAT%s:PTR %ld;NTR %ld;ENAB %ld\n",
                                  age1960_oper_regs_a[operationName],
                                  operPosTransition,
                                  operNegTransition,
                                  operEnableBits)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_stdEvent_Q(
    ViSession  instrumentHandle,
    ViPInt32   stdEventRegister)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession vi (in)                                                      */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViPInt32   stdEventRegister (out)                                      */
/*      Returns the contents of the Standard Event Register                  */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_stdEvent_Q", "");

    if ((errStatus = delayVQueryf(instrumentHandle,"*ESR?\n", "%ld%*t",
                                  stdEventRegister)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_stdEventEnable(
    ViSession  instrumentHandle,
    ViInt32    stdEnableBits)
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession instrumentHandle (in)                                        */
/*      Instrument Handle returned from age1960_init().                      */
/*    ViInt32   stdEnableBits (in)                                           */
/*      The bit(s) to enable from the standard event status register.        */
/*****************************************************************************/

{
    ViStatus   errStatus;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_stdEventEnable", ",%ld", stdEnableBits);

    CHK_LONG_RANGE(stdEnableBits, age1960_STD_REGISTER_MIN,
                   age1960_STD_REGISTER_MAX, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "*ESE %ld\n",
                                  stdEnableBits)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmd(
    ViSession instrumentHandle,
    ViString  sendStringCommand)
/*****************************************************************************/
/*  Send a SCPI command, it does not look for a response                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession     instrumentHandle (in)                                    */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString      sendStringCommand (in)                                   */
/*      The SCPI command string to be sent to the instrument                 */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmd", ",%s", sendStringCommand);

    if ((errStatus = delayVPrintf(instrumentHandle,"%s\n",
                                  sendStringCommand)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate( instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdString_Q(
    ViSession       instrumentHandle,
    ViString        queryStringCommand,
    ViInt32         stringSize,
    ViChar _VI_FAR  stringResult[] )
/*****************************************************************************/
/*  PARAMETERS                                                               */
/*    ViSession       instrumentHandle (in)                                  */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString        queryStringCommand (in)                                */
/*      The SCPI command string to be sent to the instrument                 */
/*    ViInt32         stringSize (in)                                        */
/*      The size of the char array (result) passed to the function to hold   */
/*      the string returned by the instrument                                */
/*    ViChar _VI_FAR  stringResult[] (out)                                   */
/*      The string returned by the instrument                                */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;
    int       sz;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdString_Q", ",%s,%ld", queryStringCommand,
                  stringSize);

    /* Command strings must have at least one non-null character */

    CHK_LONG_RANGE(stringSize, age1960_CMDSTRING_Q_MIN,
                   age1960_CMDSTRING_Q_MAX, VI_ERROR_PARAMETER2)

    sz = (int)stringSize;
    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%#t",
                                  queryStringCommand, &sz,
                                  stringResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    stripLF(stringResult);
    stringResult[sz]='\0';
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdInt(
    ViSession  instrumentHandle,
    ViString   sendIntegerCommand,
    ViInt32    sendInteger )
/*****************************************************************************/
/*  Sends an instrument command which takes a single integer parameter.      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   sendIntegerCommand (in)                                     */
/*      The instrument command string to be sent to the instrument.          */
/*    ViPInt32   sendInteger (in)                                            */
/*      The integer sent to the instrument at the end of the instrument      */
/*      command.  Can be ViInt16 or ViInt32.                                 */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdInt", ",%s,%ld", sendIntegerCommand,
                  sendInteger);

    if ((errStatus = delayVPrintf(instrumentHandle, "%s %ld\n", sendIntegerCommand,
                                  sendInteger)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdInt16_Q(
    ViSession  instrumentHandle,
    ViString   queryI16Command,
    ViPInt16   i16Result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an int16.  A non numeric instrument response returns zero in        */
/*    result.                                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   queryI16Command (in)                                        */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPInt16   i16Result (out)                                             */
/*      The integer returned from the instrument.                            */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdInt16_Q", ",%s", queryI16Command);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%hd%*t",
                                  queryI16Command, i16Result)) < VI_SUCCESS)
        return statusUpdate( instrumentHandle, thisPtr, errStatus );

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdInt32_Q(
    ViSession instrumentHandle,
    ViString  queryI32Command,
    ViPInt32  i32Result)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an int32.  A non numeric instrument response returns zero in        */
/*    result.                                                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   queryI32Command (in)                                        */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPInt32   i32Result (out)                                             */
/*      The integer returned from the instrument.                            */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdInt32_Q", ",%s", queryI32Command);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%ld%*t",
                                  queryI32Command, i32Result)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdInt16Arr_Q(
    ViSession instrumentHandle,
    ViString  queryI16ArrayCommand,
    ViInt32   i16ArraySize,
    ViInt16   _VI_FAR i16ArrayResult[],
    ViPInt32  i16ArrayCount)
/*****************************************************************************/
/*  This function assumes that the return result is comma or semi_colon      */
/*  separated integer data.                                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   queryI16ArrayCommand (in)                                   */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    i16ArraySize (in)                                           */
/*      The number of elements allocated to the results array.               */
/*    ViInt16    _VI_FAR i16ArrayResult[] (out)                              */
/*      Array of 16-bit integers - result from instrument.                   */
/*    ViPInt32   i16ArrayCount (out)                                         */
/*      The actual number of elements returned by the instrument             */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integer 16>{, | ; <Integer 16>}                                       */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals * thisPtr;
    ViChar    *stringResult;
    ViInt32   sz;
    ViChar    *inst_val;
    int       idx = 0;

    *i16ArrayCount = idx;

    GET_GLOBALS(instrumentHandle , thisPtr)

    age1960_TRACE(thisPtr, "queryI16ArrayCommand", ",%s,%ld", queryI16ArrayCommand,
                  i16ArraySize);

    CHK_LONG_RANGE(i16ArraySize, age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER3)

    sz = (i16ArraySize*19)+2;/*string size to receive comma separated integer data*/

    if ((stringResult = (char*)malloc(sz)) == NULL)
        return statusUpdate(instrumentHandle,thisPtr,age1960_INSTR_ERROR_ARRAY_TOO_LARGE);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%#t",
                                  queryI16ArrayCommand, &sz,
                                  stringResult)) < VI_SUCCESS)
    {
        free(stringResult);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    }

    stringResult[sz-1]='\0';

    /* convert all ';' to ',' */
    while (strchr(stringResult, ';') != NULL )
        stringResult[strcspn( stringResult, ";" )] = ',';

    inst_val = strtok(stringResult,",");
    while ((inst_val != NULL) && (i16ArraySize >= idx))
    {
        i16ArrayResult[idx] = (ViInt16)atoi(inst_val);
        idx++;
        inst_val = strtok(NULL,",");
    }
    free(stringResult);
    *i16ArrayCount = idx;

    return statusUpdate(instrumentHandle ,thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdInt32Arr_Q(
    ViSession instrumentHandle,
    ViString  queryI32ArrayCommand,
    ViInt32   i32ArraySize,
    ViInt32   _VI_FAR i32ArrayResult[],
    ViPInt32  i32ArrayCount)
/*****************************************************************************/
/*  This function assumes that the return result is comma or                 */
/*  semi-colon separated integer32 data.                                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   queryI32ArrayCommand (in)                                   */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    i32ArraySize (in)                                           */
/*      The number of elements allocated to the results array.               */
/*    ViInt32    _VI_FAR i32ArrayResult[] (out)                              */
/*      Array of 32-bit integers - result from instrument.                   */
/*    ViPInt32   i32ArrayCount (out)                                         */
/*      The actual number of elements returned by the instrument             */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integer 32>{, | ; <Integer 32>}                                       */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals * thisPtr;
    ViChar    *stringResult;
    ViInt32   sz;
    ViChar    *inst_val;
    int       idx = 0;

    *i32ArrayCount = idx;

    GET_GLOBALS(instrumentHandle , thisPtr)

    age1960_TRACE(thisPtr, "queryI32ArrayCommand", ",%s,%ld", queryI32ArrayCommand,
                  i32ArraySize);

    CHK_LONG_RANGE(i32ArraySize, age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER3)

    sz = (i32ArraySize*19)+2;/*string size to receive comma separated integer data*/

    if ((stringResult = (char*)malloc(sz)) == NULL)
        return statusUpdate(instrumentHandle,thisPtr,age1960_INSTR_ERROR_ARRAY_TOO_LARGE);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%#t",
                                  queryI32ArrayCommand, &sz,
                                  stringResult)) < VI_SUCCESS)
    {
        free(stringResult);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    stringResult[sz-1]='\0';

    /* convert all ';' to ',' */
    while (strchr(stringResult, ';') != NULL )
        stringResult[strcspn( stringResult, ";" )] = ',';

    inst_val = strtok(stringResult,",");
    while ((inst_val != NULL) && (i32ArraySize >= idx))
    {
        i32ArrayResult[idx] = atoi(inst_val);
        idx++;
        inst_val = strtok(NULL,",");
    }
    free(stringResult);
    *i32ArrayCount = idx;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdReal(
    ViSession  instrumentHandle,
    ViString   sendRealCommand,
    ViReal64   sendReal )
/*****************************************************************************/
/*  Sends an instrument command which takes a single real parameter.         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   sendRealCommand (in)                                        */
/*      The instrument command string to be sent to the instrument.          */
/*    ViReal64   sendReal (in)                                               */
/*      The real number sent to the instrument at the end of the instrument  */
/*      command.  Can be ViReal32 or ViReal64.  10 digits of precision are   */
/*      used to allow GHz to be sent with 1 Hz resolution.                   */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdReal", ",%s,%lg", sendRealCommand,
                  sendReal);

    if ((errStatus = delayVPrintf(instrumentHandle, "%s %.10g\n", sendRealCommand,
                                  sendReal )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdReal64_Q(
    ViSession  instrumentHandle,
    ViString   queryRealCommand,
    ViPReal64  realResult)
/*****************************************************************************/
/*  Sends scpi command and waits for a response that must be representable   */
/*    as an real64.  A non numeric instrument response returns zero in       */
/*    result, in case of underflow undefined.                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   queryRealCommand (in)                                       */
/*      The SCPI command string to be sent to the instrument.                */
/*    ViPReal64  realResult (out)                                            */
/*      The real returned from the instrument.                               */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdReal64_Q", ",%s", queryRealCommand);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%lf%*t",
                                  queryRealCommand, realResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdReal64Arr_Q(
    ViSession  instrumentHandle,
    ViString   realArrayCommand,
    ViInt32    realArraySize,
    ViReal64   _VI_FAR realArrayResult[],
    ViPInt32   realArrayCount)
/*****************************************************************************/
/*  This function assumes that the return result is a block of comma         */
/*  or semi-colon separated real data.                                       */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   realArrayCommand (in)                                       */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    realArraySize (in)                                          */
/*      The number of elements allocated to the results array.               */
/*    ViReal64   _VI_FAR realArrayResult[] (out)                             */
/*      Array of 64-bit reals - result from instrument.                      */
/*    ViPInt32   realArrayCount (out)                                        */
/*      The actual number of elements returned by the instrument             */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following format:                                                      */
/*      <real>{, | ; <real>}                                                 */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals * thisPtr;
    ViChar    *stringResult;
    ViInt32   sz;
    ViChar    *inst_val;
    int       idx = 0;

    setlocale(LC_ALL, "");  /* required for .dll build */

    *realArrayCount = idx;

    GET_GLOBALS(instrumentHandle , thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdReal64Arr_Q", ",%s,%ld", realArrayCommand,
                  realArraySize);

    CHK_LONG_RANGE(realArraySize, age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER3)

    sz = (realArraySize*19)+2;/* string size to receive comma separated real data */

    if ((stringResult = (char*)malloc(sz)) == NULL)
        return statusUpdate(instrumentHandle,thisPtr,age1960_INSTR_ERROR_ARRAY_TOO_LARGE);

    if ((errStatus = delayVQueryf(instrumentHandle, "%s\n", "%#t",
                                  realArrayCommand, &sz,
                                  stringResult)) < VI_SUCCESS)
    {
        free(stringResult);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    stringResult[sz-1]='\0';

    /* convert all ';' to ',' */
    while (strchr(stringResult, ';') != NULL )
        stringResult[strcspn( stringResult, ";" )] = ',';

    inst_val = strtok(stringResult,",");
    while ((inst_val != NULL) && (realArraySize > idx))
    {
        realArrayResult[idx] = atof(inst_val);
        idx++;
        inst_val = strtok(NULL,",");
    }
    free(stringResult);
    *realArrayCount = idx;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdIntArr(
    ViSession  instrumentHandle,
    ViString   sendIntegerArrayCommand,
    ViInt32    sendIntegerArraySize,
    ViInt32    _VI_FAR sendIntegerArray[])
/*****************************************************************************/
/*  Sends an instrument command which takes an array of comma separated      */
/*  integer parameter.                                                       */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   sendIntegerArrayCommand (in)                                */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    sendIntegerArraySize (in)                                   */
/*      The number of points in the sendInterArray to be sent to the         */
/*       intruument.                                                         */
/*    ViPInt32   sendIntegerArray (in)                                       */
/*      The integer array sent to the instrument at the end of the           */
/*      instrument command.                                                  */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdIntArr", ",%s,%ld,%ld", sendIntegerArrayCommand,
                  sendIntegerArraySize,sendIntegerArray[0]);

    CHK_LONG_RANGE(sendIntegerArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX,VI_ERROR_PARAMETER3)

    if ((errStatus = delayVPrintf(instrumentHandle, "%s %,*ld\n",
                                  sendIntegerArrayCommand, sendIntegerArraySize,
                                  sendIntegerArray)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_cmdRealArr(
    ViSession  instrumentHandle,
    ViString   sendRealArrayCommand,
    ViInt32    sendRealArraySize,
    ViReal64   _VI_FAR sendRealArray[])
/*****************************************************************************/
/*  Sends an instrument command which takes an array of comma separated      */
/*  Real parameter.                                                          */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   sendRealArrayCommand (in)                                   */
/*      The instrument command string to be sent to the instrument.          */
/*    ViInt32    sendArraySize (in)                                          */
/*      The number of points in the sendRealArray to be sent to the          */
/*       intruument.                                                         */
/*    ViReal64   sendRealArray (in)                                          */
/*      The Real array sent to the instrument at the end of the instrument   */
/*      instrument command.                                                  */
/*****************************************************************************/

{
    ViStatus errStatus;
    int i;

    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cmdRealArr", ",%s,%ld,%lg", sendRealArrayCommand,
                  sendRealArraySize, sendRealArray[0]);

    CHK_LONG_RANGE(sendRealArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX ,VI_ERROR_PARAMETER3)

    /* have to do the following method since NI does not handle the following */
    /*if ((errStatus = delayVPrintf(instrumentHandle, "%s %,*lf\n", */
    /*                              sendIntegerArrayCommand, sendIntegerArraySize, */
    /*                              sendIntegerArray)) < VI_SUCCESS) */
    /*    return statusUpdate(instrumentHandle,thisPtr,errStatus);  */

    if ((errStatus = delayVPrintf(instrumentHandle, "%s %.10g",
                                  sendRealArrayCommand,sendRealArray[0])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    for ( i=1; i < sendRealArraySize; i++ )
    {
        if((errStatus = delayVPrintf( instrumentHandle, ",%.10g", sendRealArray[i]))
                < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if ((errStatus = delayVPrintf(instrumentHandle, "\n" )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */




/*****************************************************************************/
ViStatus _VI_FUNC age1960_partialPreset(
    ViSession  instrumentHandle)
/*****************************************************************************/
/*  This function will perform a SYST:PRESet3 and a SET:CONT:OFF.  This      */
/*  command is an RUI partial preset.  This function should be use to quickly*/
/*  place the instrument in a known state without changing any of the "SETup"*/
/*  settings ( other than continuous/single.                                 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/
{

    ViStatus    errStatus ;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_partialPreset","");

    if ((errStatus = delayVPrintf(instrumentHandle, "SYST:PRES3;:SET:CONT:OFF\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readTXPower_Q(
    ViSession  instrumentHandle,
    ViBoolean  autoRange,
    ViPInt32   TXPowerIntegrity,
    ViPReal64  TXPower)
/*****************************************************************************/
/*  This function will perform a READ:TXPower? and return the Integrity      */
/*  and TX Power result.  It will also auto range the instrument if          */
/*  desired.                                                                 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViBoolean  autoRange (in)                                              */
/*      Set to True if this functions should try to auto range the instruemnt*/
/*      by resetting the expected power setting from the value               */
/*      read for TX Power.                                                   */
/*    ViPInt32   TXPowerIntegrity (out)                                      */
/*      The returned value from the instrument for TX power integrity.       */
/*    ViPReal64  TXPower (out)                                               */
/*      The returned value from the instrument for TX power result.          */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real Measured Results>                            */
/*                                                                           */
/* SAMPLE CODE:                                                              */
/* // Read TX Power from then instrument after the instrument has been       */
/* // auto ranged.                                                           */
/*                                                                           */
/* ViStatus   errStatus;                                                     */
/* ViInt32    TXPowerIntegrity;                                              */
/* ViReal64   TXPower;                                                       */
/*                                                                           */
/* errStatus = age1960_readTXPower_Q(vi, VI_TRUE,                            */
/*                  &TXPowerIntegrity, &TXPower);                            */
/* END SAMPLE CODE:                                                          */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readTXPower_Q", ",%hu", autoRange);

    CHK_BOOLEAN(autoRange, VI_ERROR_PARAMETER2);

    /* Default all measurement results to NAN */
    *TXPowerIntegrity = age1960_NO_RESULT_AVAIL;
    *TXPower = age1960_NOT_A_NUMBER;

    if (autoRange == VI_TRUE)
    {
        if ((errStatus =  age1960_confAutoRange(instrumentHandle, age1960_AUTORNG_TXPOWER, 15)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        /* the last reading is good so just do a fetch to get it so we */
        /* don't have to initiate the measurement again                */
        if ((errStatus = delayVQueryf(instrumentHandle, "FETCH:TXP?\n" , "%ld,%lf%*t",
                                      TXPowerIntegrity, TXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {

        if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:TXP?",
                                           ReadTimeout(instrumentHandle,"TXP") )) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);


        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                                 TXPowerIntegrity, TXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readPowerVsTime_Q(
    ViSession  instrumentHandle,
    ViInt32    pvsTimeArraySize,
    ViPInt32   pvsTimeIntegrity,
    ViPReal64  pvsTimeMaskFail,
    ViPReal64  pvsTimeTXPower,
    ViReal64   _VI_FAR pvsTimeArray[],
    ViPInt32   pvsTimeCount)
/*****************************************************************************/
/*  This function will perform a READ:PVTime and return the Integrity,       */
/*  Pass Fail Mask, TX Power, and the Power vs Time array.                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    pvsTimeArraySize (in)                                       */
/*      The size of the pvsTimeArray[] array.                                */
/*    ViPInt32   pvsTimeIntegrity (out)                                      */
/*      The returned value from the instrument for P vs T integrity.         */
/*    ViPReal64  pvsTimeMaskFail (out)                                       */
/*      The returned value from the instrument for the mask pass fail info.  */
/*    ViPReal64  pvsTimeTXPower (out)                                        */
/*      The returned value from the instrument for P vs T TX power result.   */
/*    ViReal64   pvsTimeArray[] (out)                                        */
/*      The returned values from the instrument for P vs T results.          */
/*    VIPint32   pvsTimeCount  (out)                                         */
/*      The returned value from the instrument query for                     */
/*      SETup:PVTime:POINts?, which contain the number of points returned    */
/*      by pvsTimeArray[].                                                   */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real Pass Fail Mask>,<Real TX Power>,             */
/*    [<Real Power vs Time 1>{,<Real Power vs Time N>}].                     */
/*       With N being the number of points from                              */
/*       SETup:PVTime:TIME:POINts? query                                     */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus;
    ViInt32   points;
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readTXPower_Q",",%ld",pvsTimeArraySize);

    *pvsTimeCount = 0;

    CHK_LONG_RANGE(pvsTimeArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER2)

    /* Default all measurement results to NAN */
    *pvsTimeIntegrity = age1960_NO_RESULT_AVAIL;
    *pvsTimeTXPower = age1960_NOT_A_NUMBER;
    *pvsTimeMaskFail = 1;
    for (i = 0; i<pvsTimeArraySize; i++)
        pvsTimeArray[i] = age1960_NOT_A_NUMBER;

    /* Get the number of points that will be tested */
    if ((errStatus = delayVQueryf(instrumentHandle,
                                  "SET:PVT:TIME:POIN?\n","%ld%ld%*t",
                                  &points)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (points > pvsTimeArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:PVT?",
                                       ReadTimeout(instrumentHandle,"PVT") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (points == 0)
    {
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf%*t",
                                 pvsTimeIntegrity, pvsTimeMaskFail,
                                 pvsTimeTXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%,#lf%*t",
                                 pvsTimeIntegrity, pvsTimeMaskFail,
                                 pvsTimeTXPower, &points,
                                 pvsTimeArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    *pvsTimeCount = points;

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readPFER_Q(
    ViSession instrumentHandle,
    ViPInt32   PFERIntegrity,
    ViPReal64  PFERRMSError,
    ViPReal64  PFERPeakError,
    ViPReal64  PFERFreqError)
/*****************************************************************************/
/*  This function will perform a READ:PFER and return the Integrity,         */
/*  RMS Phase Error (Max), Peak Phase Error (Max),                           */
/*  and Frequency Error (Worst)                                              */
/*  array.                                                                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   PFERIntegrity (out)                                         */
/*      The returned value from the instrument for PFER integrity.           */
/*    ViPReal64  PFERRMSError (out)                                          */
/*      The returned value from the instrument for PFER RMS Error(Max) result*/
/*    ViPReal64  PFERPeakError (out)                                         */
/*      The returned value from the instrument for PFER Peak Phase Error     */
/*      (Max) result                                                         */
/*    ViPReal64  PFERFreqError (out)                                         */
/*      The returned value from the instrument for PFER Frequency Error      */
/*      (Worst) result.                                                      */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real RMS Phase Error>,<Real Peak Phase Error>,    */
/*    <Real Frequency Error>                                                 */
/*****************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readPFER_Q","");

    /* Default all measurement results to NAN */
    *PFERIntegrity = age1960_NO_RESULT_AVAIL;
    *PFERRMSError = age1960_NOT_A_NUMBER;
    *PFERPeakError = age1960_NOT_A_NUMBER;
    *PFERFreqError = age1960_NOT_A_NUMBER;


    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:PFER?",
                                       ReadTimeout(instrumentHandle,"PFER") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf%*t",
                             PFERIntegrity, PFERRMSError, PFERPeakError,
                             PFERFreqError )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readORFS_Q(
    ViSession  instrumentHandle,
    ViInt32    ORFSSwArraySize,
    ViInt32    ORFSModArraySize,
    ViPInt32   ORFSIntegrity,
    ViPReal64  ORFSTXPower,
    ViPReal64  ORFSBWPower,
    ViReal64   _VI_FAR ORFSSwArray[],
    ViReal64   _VI_FAR ORFSModArray[],
    ViPInt32   ORFSSwCount,
    ViPInt32   ORFSModCount)
/*****************************************************************************/
/*  This function will perform a READ:ORFS and return the Integrity,         */
/*  TX Power, 30 kHz BW Power, Switching results array , and Modulation      */
/*  results array.                                                           */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    ORFSSwArraySize (in)                                        */
/*      The size of the orfsSwArray[] array.                                 */
/*    ViInt32    ORFSModArraySize (in)                                       */
/*      The size of the orfsModArray[] array.                                */
/*    ViPInt32   ORFSIntegrity (out)                                         */
/*      The returned value from the instrument for ORFS integrity.           */
/*    ViPReal64  ORFSTXPower (out)                                           */
/*      The returned value from the instrument for ORFS TX power result.     */
/*    ViPReal64  ORFSBWPower (out)                                           */
/*      The returned value from the instrument for ORFS 30 kHz BW power      */
/*      result.                                                              */
/*    ViReal64   ORFSSwArray[] (out)                                         */
/*      The returned values from the instrument for ORFS Switching results.  */
/*    ViReal64   ORFSModArray[] (out)                                        */
/*      The returned values from the instrument for ORFS Modulation results. */
/*    VIPint32   ORFSSwCount  (out)                                          */
/*      The returned value from the instrument query for                     */
/*      SETup:ORFS:SWITching:FREQuencyPOINTs?, which contain the number of   */
/*      points returned by orfsSwArray[].                                    */
/*    VIPint32   ORFSModCount  (out)                                         */
/*      The returned value from the instrument query for                     */
/*      SETup:ORFS:MODulation:FREQuencyPOINTs?, which contain the number of  */
/*      points returned by orfsModArray[].                                   */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real TX Power>,                                   */
/*    <[Real ORFS Switching result 1>{,<Real ORFS Switching result i>}],     */
/*    [<Real 30 kHz BW Power>,                                               */
/*    <Real ORFS Modulation result 1>{,<Real ORFS Modulation result j>}],    */
/*    With i being the number of points from                                 */
/*    SETup:ORFS:SWITching:FREQuency:POINts? query and                       */
/*    j being the number of points from                                      */
/*    SETup:ORFS:MODulation:FREQuency:POINts? query                          */
/*    If either of the POINTs are 0 the optional information in [] will not  */
/*    be returned from the instrument and the values returned to the calling */
/*    function will not be set.                                              */
/*****************************************************************************/

{
    ViStatus  errStatus;
    ViInt32   swPoints;
    ViInt32   modPoints;
    struct    age1960_globals *thisPtr;
    int       i;


    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readORFS_Q",",%ld,%ld",ORFSSwArraySize,
                  ORFSModArraySize);

    *ORFSSwCount = 0;
    *ORFSModCount = 0;

    CHK_LONG_RANGE(ORFSSwArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX,VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(ORFSModArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX,VI_ERROR_PARAMETER3)

    /* Default all measurement results to NAN */
    *ORFSIntegrity = age1960_NO_RESULT_AVAIL;
    *ORFSTXPower = age1960_NOT_A_NUMBER;
    *ORFSBWPower = age1960_NOT_A_NUMBER;
    for (i = 0; i<ORFSSwArraySize; i++)
        ORFSSwArray[i] = age1960_NOT_A_NUMBER;
    for (i = 0; i<ORFSModArraySize; i++)
        ORFSModArray[i] = age1960_NOT_A_NUMBER;

    if ((errStatus = delayVQueryf(instrumentHandle,
                                  "SET:ORFS:SWIT:FREQ:POIN?"
                                  ";:SET:ORFS:MOD:FREQ:POIN?\n","%ld;%ld%*t",
                                  &swPoints, &modPoints)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints > ORFSSwArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if (modPoints > ORFSModArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:ORFS?",
                                       ReadTimeout(instrumentHandle,"ORFS") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints != 0 && modPoints != 0)
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%,#lf,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower,
                                 &swPoints, ORFSSwArray,
                                 ORFSBWPower, &modPoints,
                                 ORFSModArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints == 0 && modPoints != 0)
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower,
                                 ORFSBWPower, &modPoints,
                                 ORFSModArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints != 0 && modPoints == 0)
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower,
                                 &swPoints, ORFSSwArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints == 0 && modPoints == 0)
    {
        if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                                 ORFSIntegrity, ORFSTXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    *ORFSSwCount = swPoints;
    *ORFSModCount = modPoints;

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readFBER_Q(
    ViSession  instrumentHandle,
    ViPInt32   FBERIntegrity,
    ViPReal64  FBERBitsTested,
    ViPReal64  FBERRatio,
    ViPReal64  FBERCount)
/*****************************************************************************/
/*  This function will perform a READ:FBERror and return the Integrity,      */
/*  bits tested, fast bit error ratio, and fast bit error count.             */
/*  It will also turn off all measurement functions that cannot run          */
/*  concurrently with FBER.                                                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   FBERIntegrity (out)                                         */
/*      The returned value from the instrument for fast bit error integrity. */
/*    ViPReal64  FBERBitsTested (out)                                        */
/*      The returned value from the instrument for number of bits tested.    */
/*    ViPReal64  FBERRatio (out)                                             */
/*      The returned value from the instrument for fast bit error ratio.     */
/*    ViPReal64  FBERCount (out)                                             */
/*      The returned value from the instrument for fast bit error count.     */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*    <Integrity Integer>,<Real Fast Bits Tested>,                           */
/*    <Real Fast Bit Error Ratio>, <Real Fast Bit Error Count>               */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readFBER_Q","");

    /* Default all measurement results to NAN */
    *FBERIntegrity= age1960_NO_RESULT_AVAIL;
    *FBERBitsTested = age1960_NOT_A_NUMBER;
    *FBERRatio = age1960_NOT_A_NUMBER;
    *FBERCount = age1960_NOT_A_NUMBER;

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:BERR:OFF"
                                  ";:INIT:DAUD:OFF\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:FBER?",
                                       ReadTimeout(instrumentHandle,"FBER") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf%*t",
                             FBERIntegrity, FBERBitsTested,
                             FBERRatio, FBERCount))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readBER_Q(
    ViSession  instrumentHandle,
    ViPInt32   BERIntegrity,
    ViPReal64  TypeIaBERBitsTested,
    ViPReal64  TypeIaBERRatio,
    ViPReal64  TypeIaBERCount,
    ViPReal64  TypeIbBERBitsTested,
    ViPReal64  TypeIbBERRatio,
    ViPReal64  TypeIbBERCount,
    ViPReal64  TypeIIBERBitsTested,
    ViPReal64  TypeIIBERRatio,
    ViPReal64  TypeIIBERCount)
/********************************************************************************/
/*  This function will perform a READ:BERRor:FULL and return the Integrity,		*/
/*  then the bits tested, bit error ratio, and bit error count for each of		*/
/*  3 bit types. It will be either residual or non-residual depending on		*/
/*  what is set in the configure.												*/
/*  It will also turn off all measurement functions that cannot run				*/
/*  concurrently with BER.														*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViPInt32    BERIntegrity (out)											*/
/*      The returned value from the instrument for bit error integrity.			*/
/*    ViPReal64   TypeIaBERBitsTested (out)										*/
/*      The returned value from the instrument for number of bits tested.		*/
/*    ViPReal64   TypeIaBERRatio (out)                                          */
/*      The returned value from the instrument for bit error ratio.				*/
/*    ViPReal64   TypeIaBERCount (out)											*/
/*      The returned value from the instrument for bit error count.				*/
/*  The function assumes that the data returned by the instrument has the		*/
/*    following form:															*/
/*    <Integrity Integer>,<Real Bits Tested>,<Real Bit Error Ratio>,			*/
/*    <Real Bit Error Count>													*/
/********************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readBER_Q","");

    /* Default all measurement results to NAN */
    *BERIntegrity= age1960_NO_RESULT_AVAIL;
    *TypeIaBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIaBERRatio = age1960_NOT_A_NUMBER;
    *TypeIaBERCount = age1960_NOT_A_NUMBER;
    *TypeIbBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIbBERRatio = age1960_NOT_A_NUMBER;
    *TypeIbBERCount = age1960_NOT_A_NUMBER;
    *TypeIIBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIIBERRatio = age1960_NOT_A_NUMBER;
    *TypeIIBERCount = age1960_NOT_A_NUMBER;

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:FBER:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:BERR:FULL?",
                                       ReadTimeout(instrumentHandle,"BERR") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle,
                             "%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf%*t",
                             BERIntegrity, TypeIaBERBitsTested,
                             TypeIaBERRatio, TypeIaBERCount,
                             TypeIbBERBitsTested, TypeIbBERRatio,
                             TypeIbBERCount, TypeIIBERBitsTested,
                             TypeIIBERRatio, TypeIIBERCount))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readAAudio_Q(
    ViSession  instrumentHandle,
    ViPInt32   AAudioIntegrity,
    ViPReal64  AAudioResult)
/*****************************************************************************/
/*  This function will perform a READ:AAUDio and return the Integrity,       */
/*  and audio measurement result.                                            */
/*                                                                           */
/*  This function supports earlier versions of the E1960A TA. Greater		 */
/*  functionality can be achieved using the age1960_readAFAN() function if	 */
/*  your version of the E1960A TA will support it (A.07xx or later).		 */
/*																			 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   AAudioIntegrity (out)                                       */
/*      The returned value from the instrument for analog audio integrity.   */
/*    ViPReal64  AAudioResult (out)                                          */
/*      The returned value from the instrument for analog audio result.      */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readAAudio_Q", "");

    /* Default all measurement results to NAN */
    *AAudioIntegrity = age1960_NO_RESULT_AVAIL;
    *AAudioResult = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:AAUD?",
                                       ReadTimeout(instrumentHandle,"AAUD") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             AAudioIntegrity,
                             AAudioResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*******************************************************************************/
ViStatus _VI_FUNC age1960_readAFAN_Q(
    ViSession instrumentHandle,
    ViBoolean  AFANAutoRange,
    ViPInt32   AFANIntegrity,
    ViPReal64  AFANAvgAudioLevel,
    ViPReal64  AFANAvgSINAD,
    ViPReal64  AFANAvgDist)
/********************************************************************************/
/*  This function will perform a READ:AAUD and return the Integrity,			*/
/*  Average Audio Level, Average SINAD, Average Distortion (if available).		*/
/*																				*/
/*  Note: This function adds functionality that was not available in earlier	*/
/*  versions of the E1960A TA. If backward compatibility is required, use the	*/
/*  age1960_readAAudio() function.												*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViBoolean  AFANAutoRange (in)												*/
/*      Set to True if this functions should try to auto range the instruemnt	*/
/*      by resetting the AFAN expected peak voltage given the current			*/
/*      reading for AFAN level.													*/
/*    ViPInt32   AFANIntegrity (out)											*/
/*      The returned value from the instrument for AFAN integrity.				*/
/*    ViPReal64  AFANAvgAudioLevel (out)										*/
/*      The returned value from the instrument for Average Audio Level result	*/
/*    ViPReal64  AFANAvgSINAD (out)												*/
/*      The returned value from the instrument for Average SINAD result.		*/
/*    ViPReal64  AFANAvgDist (out)												*/
/*      The returned value from the instrument for Average Dist result.			*/
/*																				*/
/********************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readAFAN_Q", ",%hu", AFANAutoRange);

    CHK_BOOLEAN(AFANAutoRange, VI_ERROR_PARAMETER2);

    /* Default all measurement results to NAN */
    *AFANIntegrity = age1960_NO_RESULT_AVAIL;
    *AFANAvgAudioLevel = age1960_NOT_A_NUMBER;
    *AFANAvgSINAD = age1960_NOT_A_NUMBER;
    *AFANAvgDist = age1960_NOT_A_NUMBER;

    if (AFANAutoRange == VI_TRUE)
    {
        /* auto range can fail if the signal is very low */
        age1960_confAutoRange(instrumentHandle, age1960_AUTORNG_AFANALYZER, 15);
    }

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:AAUD?",
                                       ReadTimeout(instrumentHandle,"AAUD") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf%*t",
                             AFANIntegrity, AFANAvgAudioLevel,
                             AFANAvgSINAD, AFANAvgDist)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);



    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_readDAudio_Q(
    ViSession  instrumentHandle,
    ViPInt32   DAudioIntegrity,
    ViPReal64  DAudioResult)
/*****************************************************************************/
/*  This function will perform a READ:DAUDio and return the Integrity,       */
/*  and audio measurement result.                                            */
/*  It will also turn off all measurement functions that cannot run          */
/*  concurrently with DAudio.  If a ber or fber measurement is turned off,   */
/*  the loopback will be reset to the value in thisPtr->loopback             */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   DAudioIntegrity (out)                                       */
/*      The returned value from the instrument for analog audio integrity.   */
/*    ViPReal64  DAudioResult (out)                                          */
/*      The returned value from the instrument for analog audio result.      */
/*                                                                           */
/*    <Integrity Integer>,<Real Decoded Audio Result>                        */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readDAudio_Q", "");

    /* Default all measurement results to NAN */
    *DAudioIntegrity= age1960_NO_RESULT_AVAIL;
    *DAudioResult = age1960_NOT_A_NUMBER;

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:BERR:OFF"
                                  ";:INIT:FBER:OFF\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
        /* will turn it off */
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                      age1960_loopbackSetting_a[thisPtr->loopback]))
                < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:DAUD?",
                                       ReadTimeout(instrumentHandle,"DAUD") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             DAudioIntegrity,
                             DAudioResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_readDynPower_Q(
    ViSession  instrumentHandle,
    ViInt32    dynPowerArraySize,
    ViInt32    _VI_FAR dynPowerIntegrity[],
    ViReal64   _VI_FAR dynPowerArray[],
    ViPInt32   dynPowerCount)
/*****************************************************************************/
/* This function will perform a READ:DPOWer and return the array size,		 */
/* the Integrity for each burst in an array, then the power measurement		 */
/*  for each burst also in array form, then the number of bursts measured.	 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    dynPowerArraySize (in)                                      */
/*      The size of the dynPowerArray[] array.                               */
/*    ViPInt32   dynPowerIntegrity[] (out)                                   */
/*      The returned value from the instrument for each burst integrity.     */
/*    ViReal64   dynPowerArray[] (out)                                       */
/*      The returned values from the instrument for power in each burst.     */
/*    VIPint32   dynPowerCount  (out)                                        */
/*      The returned value from the instrument query that                    */
/*      contains the number of points returned by dynPowerArray[].           */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer N>,<Real TX Power N >,								 */
/*       With N being the number of points from                              */
/*       SET:DPOW:COUNT:NUMB? query											 */
/*****************************************************************************/
{
    ViStatus  errStatus;
    ViInt32   points;
    ViInt32	rangePoints;
    ViInt32   integrity;
    ViInt32	tempdynPowerIntegrity[100];
    ViReal64  tempdynPowerArray[100];
    ViChar	rangeSCPI1[256];
    ViChar	rangeSCPI2[256];
    ViInt32	rangeLoop;
    struct    age1960_globals *thisPtr;
    int       i;
    int		x;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readDynPower_Q",",%ld",dynPowerArraySize);

    *dynPowerCount = 0;

    CHK_LONG_RANGE(dynPowerArraySize,age1960_DPOW_ARRAY_MIN,
                   age1960_DPOW_ARRAY_MAX, VI_ERROR_PARAMETER2)

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SET:DPOW:COUNT:NUMB:GSM?;:INIT:DPOW;:FETCH:DPOW:INT:GSM?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle,  "%ld;%ld%*t",
                             &points, &integrity)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (points > dynPowerArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if ( integrity == 13 || integrity == 1)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_UNDEFINED_DATA);

    if ( integrity == 2)
        return statusUpdate(instrumentHandle, thisPtr,
                            VI_ERROR_TMO);

    /* Default all measurement results to NAN */

    for (i = 0; i< dynPowerArraySize; i++)
    {
        dynPowerIntegrity[i] = 1; /* 1 = no data available */
        dynPowerArray[i] = age1960_NOT_A_NUMBER;

    }

    // Default
    rangeLoop=1;

    rangeLoop = (ViInt32) ceil((ViReal64) points / 100.0);

    while (rangeLoop > 0)
    {
        sprintf(rangeSCPI1, "FETC:DPOW:NUMB:RANG%ld:GSM?", rangeLoop);
        sprintf(rangeSCPI2, "FETC:DPOW:RANG%ld:GSM?", rangeLoop);

        if ((errStatus =  sendQueryRequest(instrumentHandle, rangeSCPI1, 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld%*t",
                                 &rangePoints)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  sendQueryRequest(instrumentHandle, rangeSCPI2, 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%,#ld,%,#lf%*t",
                                 &rangePoints, tempdynPowerIntegrity,
                                 &rangePoints, tempdynPowerArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        for (x = 0; x < rangePoints; x++)
        {
            dynPowerIntegrity[((rangeLoop-1)*100) + x] = tempdynPowerIntegrity[x];
            dynPowerArray[((rangeLoop-1)*100) + x] = tempdynPowerArray[x];
        }
        rangeLoop -= 1;
    }


    *dynPowerCount = points;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_initMeasFcn(
    ViSession  instrumentHandle,
    ViInt16    measFcn)
/*****************************************************************************/
/*  This function will perform a INIT:<measFcn>                              */
/*  It will also turn off all measurement functions that cannot run          */
/*  concurrently if the new measurement function cannot run concurrently.    */
/*  If a concurrent measurement is turned off and loopback was set via       */
/*  the loopback function, loopback will be reset.  Should change to use     */
/*  the INIT:ON? command when it works correctly.                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16     measFcn (in)                                               */
/*      The measurement function to init in the following form:              */
/*                                                                           */
/*      AAUD   = 0  Analog Audio Meas Fcn       (age1960_AAUDIO)             */
/*      BERR   = 1  Bit Error Rate              (age1960_BER)                */
/*      DAUD   = 2  Decoded Audio Meas Fcn      (age1960_DAUDIO)             */
/*      FBER   = 3  Fast Bit Error Meas Fcn     (age1960_FBER)               */
/*      ORFS   = 4  ORFSpectrum Meas Fcn        (age1960_ORFS)               */
/*      PFER   = 5  Phase Freq Error Meas Fcn   (age1960_PFER)               */
/*      PVT    = 6  Power vs Time Meas Fcn      (age1960_PVTIME)             */
/*      TXP    = 7  TX Power Meas Fcn           (age1960_TXPOWER)            */
/*      DPOW   = 8  Dynamic Power function 		(age1960_DPOWER)		     */
/*      IQT    = 9  IQ Tuning function 			(age1960_IQTUNING)		     */
/*****************************************************************************/

{
    ViStatus    errStatus;
    ViChar      queryInit[150];

    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_initMeasFcn", ",%hd", measFcn);

    CHK_ENUM(measFcn, age1960_NUMBER_OF_MEAS_FCNS-1, VI_ERROR_PARAMETER2)

    if ((errStatus =  delayVQueryf(instrumentHandle, "INIT:ON?\n", "%s%*t",
                                   queryInit)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* Before turning off non concurrent meas's check to see if they need
      to be turned off. This saves on speed. This fixes defect 5 on A.01.00 */
    if ( strstr(queryInit, age1960_measFcns_a[age1960_BER]) != VI_NULL  &&
            measFcn == age1960_BER )
    {
        if ((errStatus = delayVPrintf(instrumentHandle,
                                      "INIT:FBER:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
            /* will turn it off   */
        {
            if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                          age1960_loopbackSetting_a[thisPtr->loopback]))
                    < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
    }

    if ( strstr(queryInit, age1960_measFcns_a[age1960_FBER]) != VI_NULL  &&
            measFcn == age1960_FBER ) /*  */
    {
        if ((errStatus = delayVPrintf(instrumentHandle,
                                      "INIT:BERR:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
            /* will turn it off   */
        {
            if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                          age1960_loopbackSetting_a[thisPtr->loopback]))
                    < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
    }

    if ( strstr(queryInit, age1960_measFcns_a[age1960_DAUDIO]) != VI_NULL  &&
            measFcn == age1960_DAUDIO ) /*  */
    {
        if ((errStatus = delayVPrintf(instrumentHandle,
                                      "INIT:BERR:OFF;:INIT:FBER:OFF\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
            /* will turn it off   */
        {
            if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                          age1960_loopbackSetting_a[thisPtr->loopback]))
                    < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
    }

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:%s\n",
                                  age1960_measFcns_a[measFcn])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_initMeasFcns(
    ViSession  instrumentHandle,
    ViInt32    measFcnsToInit,
    ViInt16    _VI_FAR measFcnsArray[])
/*****************************************************************************/
/*  This function will perform a INIT:<measFcn>{;<measFcn}                   */
/*  It will also turn off all measurement functions that cannot run          */
/*  concurrently if the new measurement function cannot run concurrently.    */
/*  If a concurrent measurement is turned off and loopback was set via       */
/*  the loopback function, loopback will be reset.  Should change to use     */
/*  the INIT:ON? command when it works correctly.                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    measFcnsToInit (in)                                         */
/*      The number of measure functions in measFcnsArray to init.            */
/*    ViInt16    measFcnsArray[]  (in)                                       */
/*      The integer array that contains the meas functions to init in one    */
/*      statement.  The integer values and corresponding measurement         */
/*      functions are as follows:                                            */
/*                                                                           */
/*      AAUD   = 0  Analog Audio Meas Fcn       (age1960_AAUDIO)             */
/*      BERR   = 1  Bit Error Rate Meas Fcn     (age1960_BER)                */
/*      DAUD   = 2  Decoded Audio Meas Fcn      (age1960_DAUDIO)             */
/*      FBER   = 3  Fast Bit Error Meas Fcn     (age1960_FBER)               */
/*      ORFS   = 4  ORFSpectrum Meas Fcn        (age1960_ORFS)               */
/*      PFER   = 5  Phase Freq Error Meas Fcn   (age1960_PFER)               */
/*      PVT    = 6  Power vs Time Meas Fcn      (age1960_PVTIME)             */
/*      TXP    = 7  TX Power Meas Fcn           (age1960_TXPOWER)            */
/*      DPOW   = 8  Dynamic Power               (age1960_DPOWER)             */
/*      IQT    = 9  IQ Tuning               	(age1960_IQTUNING)           */
/*****************************************************************************/

{
    ViStatus    errStatus;
    ViChar      initFcns[255];
    ViChar      queryInit[150];
    int         i;
    int         haveNonConcurrent = 0;

    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_initMeasFcns", ",%ld,%hd", measFcnsToInit,
                  measFcnsArray[0]);

    CHK_LONG_RANGE(measFcnsToInit, 1, age1960_NUMBER_OF_MEAS_FCNS,
                   VI_ERROR_PARAMETER2)

    strcpy(initFcns,"");

    /* Note what meas's have already been init'ed */
    if ((errStatus =  delayVQueryf(instrumentHandle, "INIT:ON?\n", "%s%*t",
                                   queryInit)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    for (i=0; i< (measFcnsToInit ); i++)
    {
        CHK_ENUM(measFcnsArray[i], age1960_NUMBER_OF_MEAS_FCNS-1,
                 VI_ERROR_PARAMETER3)

        /* Check for non concurrent meas's */
        if (measFcnsArray[i] == age1960_BER || measFcnsArray[i] ==  age1960_FBER
                || measFcnsArray[i] == age1960_DAUDIO)
            haveNonConcurrent++;

        /* Before turning off non concurrent meas's check to see if they need
          to be turned off. This saves on speed */
        if ( strstr(queryInit, age1960_measFcns_a[age1960_BER]) != VI_NULL  &&
                measFcnsArray[i] == age1960_BER ) /*  */
        {
            if ((errStatus = delayVPrintf(instrumentHandle,
                                          "INIT:FBER:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
                /* will turn it off   */
            {
                if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                              age1960_loopbackSetting_a[thisPtr->loopback]))
                        < VI_SUCCESS)
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
        }

        if ( strstr(queryInit, age1960_measFcns_a[age1960_FBER]) != VI_NULL  &&
                measFcnsArray[i] == age1960_FBER ) /*  */
        {
            if ((errStatus = delayVPrintf(instrumentHandle,
                                          "INIT:BERR:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
                /* will turn it off   */
            {
                if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                              age1960_loopbackSetting_a[thisPtr->loopback]))
                        < VI_SUCCESS)
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
        }

        if ( strstr(queryInit, age1960_measFcns_a[age1960_DAUDIO]) != VI_NULL  &&
                measFcnsArray[i] == age1960_DAUDIO ) /*  */
        {
            if ((errStatus = delayVPrintf(instrumentHandle,
                                          "INIT:BERR:OFF;:INIT:FBER:OFF\n")) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            if (thisPtr->loopback != 0) /*reset the loopback since BERR:OFF or FBERR:OFF */
                /* will turn it off   */
            {
                if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                              age1960_loopbackSetting_a[thisPtr->loopback]))
                        < VI_SUCCESS)
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
        }


        strcat(initFcns,age1960_measFcns_a[measFcnsArray[i]]);
        if (i < measFcnsToInit-1) strcat(initFcns,";");
    }

    if (haveNonConcurrent > 1)
        return statusUpdate(instrumentHandle,thisPtr, age1960_INSTR_TOO_MANY_NONCONCURRENT);

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:%s\n",
                                  initFcns)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchTXPower_Q(
    ViSession  instrumentHandle,
    ViPInt32   TXPowerIntegrity,
    ViPReal64  TXPower)
/*****************************************************************************/
/*  This function will perform a FETCh:TXPower? and return the Integrity     */
/*  and TX Power result.                                                     */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   TXPowerIntegrity (out)                                      */
/*      The returned value from the instrument for TX power integrity.       */
/*    ViPReal64  TXPower (out)                                               */
/*      The returned value from the instrument for TX power result.          */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real Measured Results>                            */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchTXPower_Q", "");

    /* Default all measurement results to NAN */
    *TXPowerIntegrity = age1960_NO_RESULT_AVAIL;
    *TXPower = age1960_NOT_A_NUMBER;


    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:TXP?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             TXPowerIntegrity, TXPower )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchPowerVsTime_Q(
    ViSession  instrumentHandle,
    ViInt32    pvsTimeArraySize,
    ViPInt32   pvsTimeIntegrity,
    ViPReal64  pvsTimeMaskFail,
    ViPReal64  pvsTimeTXPower,
    ViReal64   _VI_FAR pvsTimeArray[],
    ViPInt32   pvsTimeCount)
/*****************************************************************************/
/*  This function will perform a FETCh:PVTime and return the Integrity,      */
/*  Pass Fail mask, TX Power, Peak Plus Power, Peak Minus Power              */
/*  and the Power vs Time array.                                             */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    pvsTimeArraySize (in)                                       */
/*      The size of the pvsTimeArray[] array.                                */
/*    ViPInt32   pvsTimeIntegrity (out)                                      */
/*      The returned value from the instrument for P vs T integrity.         */
/*    ViPReal64  pvsTimeMaskFail (out)                                       */
/*      The returned value from the instrument for the mask pass fail info.  */
/*    ViPReal64  pvsTimeTXPower (out)                                        */
/*      The returned value from the instrument for P vs T TX power result.   */
/*    ViReal64   pvsTimeArray[] (out)                                        */
/*      The returned values from the instrument for P vs T results.          */
/*    VIPint32   pvsTimeCount  (out)                                         */
/*      The returned value from the instrument query for                     */
/*      SETup:PVT:TIME:POINts?, which contain the number of points returned  */
/*      by pvsTimeArray[].                                                   */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real Pass Fail Mask>,<Real TX Power>,             */
/*    [<Real Power vs Time 1>{,<Real Power vs Time N>}].                     */
/*       With N being the number of points from                              */
/*       SETup:PVTime:TIME:POINts? query                                     */
/*****************************************************************************/

{
    ViStatus  errStatus;
    ViInt32   points;
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchPowerVsTime_Q",",%ld",pvsTimeArraySize);

    *pvsTimeCount = 0;

    CHK_LONG_RANGE(pvsTimeArraySize,age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER2)

    /* Default all measurement results to NAN */
    *pvsTimeIntegrity = age1960_NO_RESULT_AVAIL;
    *pvsTimeTXPower = age1960_NOT_A_NUMBER;
    *pvsTimeMaskFail = 1;
    for (i = 0; i<pvsTimeArraySize; i++)
        pvsTimeArray[i] = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SET:PVT:TIME:POIN?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle,  "%ld%*t", &points)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (points > pvsTimeArraySize)
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if (points == 0)
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:PVT?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf,%lf%*t",
                                 pvsTimeIntegrity, pvsTimeMaskFail,
                                 pvsTimeTXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:PVT?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf,%lf,%,#lf%*t",
                                 pvsTimeIntegrity, pvsTimeMaskFail, pvsTimeTXPower,
                                 &points, pvsTimeArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    *pvsTimeCount = points;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchDynPower_Q(
    ViSession  instrumentHandle,
    ViInt32    dynPowerArraySize,
    ViInt32    _VI_FAR dynPowerIntegrity[],
    ViReal64   _VI_FAR dynPowerArray[],
    ViPInt32   dynPowerCount)
/*****************************************************************************/
/* This function will perform a FETCh:DPOWer and return the array size,		 */
/* the Integrity for each burst in an array, then the power measurement		 */
/*  for each burst also in array form, then the number of bursts measured.	 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    dynPowerArraySize (in)                                      */
/*      The size of the dynPowerArray[] array.                               */
/*    ViPInt32   dynPowerIntegrity[] (out)                                   */
/*      The returned value from the instrument for each burst integrity.     */
/*    ViReal64   dynPowerArray[] (out)                                       */
/*      The returned values from the instrument for power in each burst.     */
/*    VIPint32   dynPowerCount  (out)                                        */
/*      The returned value from the instrument query                         */
/*      that contains the number of points returned by dynPowerArray[].      */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus;
    ViInt32   points;
    ViInt32   integrity;
    ViInt32	rangePoints;
    ViInt32	tempdynPowerIntegrity[100];
    ViReal64  tempdynPowerArray[100];
    ViChar	rangeSCPI1[256];
    ViChar	rangeSCPI2[256];
    ViInt32	rangeLoop;
    struct    age1960_globals *thisPtr;
    int       i;
    int		x;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchDynPower_Q",",%ld",dynPowerArraySize);

    *dynPowerCount = 0;

    CHK_LONG_RANGE(dynPowerArraySize,age1960_DPOW_ARRAY_MIN,
                   age1960_DPOW_ARRAY_MAX, VI_ERROR_PARAMETER2)

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SET:DPOW:COUNT:NUMB:GSM?;:FETC:DPOW:INT:GSM?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle,  "%ld;%ld%*t",
                             &points, &integrity)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (points > dynPowerArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);
    /* The next if's are required since dyn power does not return the correct
    number of parameters when integrity is 1,13 and 2. Defect logged 11364. */

    if ( integrity == 13 || integrity == 1)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_UNDEFINED_DATA);

    if ( integrity == 2)
        return statusUpdate(instrumentHandle, thisPtr,
                            VI_ERROR_TMO);

    /* Default all measurement results to NAN */

    for (i = 0; i< dynPowerArraySize; i++)
    {
        dynPowerIntegrity[i] = 1; /* 1 = no data available */
        dynPowerArray[i] = age1960_NOT_A_NUMBER;

    }

    // Default
    rangeLoop=1;

    rangeLoop = (ViInt32) ceil((ViReal64) points / 100.0);

    while (rangeLoop > 0)
    {
        sprintf(rangeSCPI1, "FETC:DPOW:NUMB:RANG%ld:GSM?", rangeLoop);
        sprintf(rangeSCPI2, "FETC:DPOW:RANG%ld:GSM?", rangeLoop);

        if ((errStatus =  sendQueryRequest(instrumentHandle, rangeSCPI1, 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld%*t",
                                 &rangePoints)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  sendQueryRequest(instrumentHandle, rangeSCPI2, 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%,#ld,%,#lf%*t",
                                 &rangePoints, tempdynPowerIntegrity,
                                 &rangePoints, tempdynPowerArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        for (x = 0; x < rangePoints; x++)
        {
            dynPowerIntegrity[((rangeLoop-1)*100) + x] = tempdynPowerIntegrity[x];
            dynPowerArray[((rangeLoop-1)*100) + x] = tempdynPowerArray[x];
        }
        rangeLoop -= 1;
    }


    *dynPowerCount = points;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/********************************************************************************/
ViStatus _VI_FUNC age1960_fetchPFER_Q(
    ViSession instrumentHandle,
    ViPInt32   PFERIntegrity,
    ViPReal64  PFERRMSError,
    ViPReal64  PFERPeakError,
    ViPReal64  PFERFreqError)
/********************************************************************************/
/*  This function will perform a FETCh:PFER and return the Integrity,			*/
/*  RMS Phase Error (Max), Peak Phase Error (Max),								*/
/*  and Frequency Error (Worst)													*/
/*  array.																		*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViPInt32   PFERIntegrity (out)											*/
/*      The returned value from the instrument for PFER integrity.				*/
/*    ViPReal64  PFERRMSError (out)												*/
/*      The returned value from the instrument for PFER RMS Error(Max) result	*/
/*    ViPReal64  PFERPeakError (out)											*/
/*      The returned value from the instrument for PFER Peak Phase				*/
/*      Error (Max) result														*/
/*    ViPReal64  PFERFreqError (out)											*/
/*      The returned value from the instrument for PFER Frequency Error			*/
/*      (Worst) result.															*/
/*																				*/
/*  The function assumes that the data returned by the instrument has the		*/
/*    following form:															*/
/*																				*/
/*    <Integrity Integer>,<Real RMS Phase Error>,<Real Peak Phase Error>,		*/
/*    <Real Frequency Error>													*/
/********************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchPFER_Q","");

    /* Default all measurement results to NAN */
    *PFERIntegrity = age1960_NO_RESULT_AVAIL;
    *PFERRMSError = age1960_NOT_A_NUMBER;
    *PFERPeakError = age1960_NOT_A_NUMBER;
    *PFERFreqError = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:PFER?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf%*t",
                             PFERIntegrity, PFERRMSError, PFERPeakError,
                             PFERFreqError )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchIQTuning_Q(
    ViSession  instrumentHandle,
    ViInt32    IQArraySize,
    ViPInt32   IQIntegrity,
    ViPReal64   IQSpurFreq,
    ViPReal64   IQSpurLevel,
    ViReal64   _VI_FAR IQResultsArray[],
    ViReal64   _VI_FAR IQFreqsArray[],
    ViPInt32   IQTuningCount)
/********************************************************************************/
/*   This function will perform a FETCh:IQT command								*/
/*  This function will return the Integrity, spur freq and level an				*/
/*  array of levels, an array of frequencies for the different levels, and a	*/
/*  count of the number of results returned.									*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViInt32    IQArraySize (in)												*/
/*      The size of the IQResultsArr[] and the IQFreqsArr[] array.				*/
/*    ViPInt32   IQIntegrity (out)												*/
/*      The returned value from the instrument for IQ tuning integrity.			*/
/*    ViPReal64   IQSpurFreq (out)												*/
/*      This returned value for the frequency the spur is set at.				*/
/*    ViPReal64   IQSpurLevel													*/
/*      The returned level at the spur freq.									*/
/*    ViReal64   IQResultsArray[] (out)											*/
/*      The returned values for the IQ tuning, referenced to the desired		*/
/*      frequency value.														*/
/*    ViReal64   IQFreqsArray[] (out)											*/
/*      The returned values of the frequencies in Hz where the IQ tuning		*/
/*      results were measured.													*/
/*    VIPint32   IQTuningCount  (out)											*/
/*      The returned value of the number of points that were measured.			*/
/*																				*/
/********************************************************************************/

{
    ViStatus  errStatus;
    ViReal64  tempFreqArray[]  = {-270.833, -203.125, -135.417, -67.7083, 0,
                                  67.7083, 135.417, 203.125, 270.833
                                 };
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchIQTuning_Q",",%ld",IQArraySize);

    *IQTuningCount = 0;
    *IQIntegrity = age1960_NO_RESULT_AVAIL;

    CHK_LONG_RANGE(IQArraySize, age1960_IQ_ARRAY_MIN,
                   age1960_IQ_ARRAY_MAX, VI_ERROR_PARAMETER2)

    for (i = 0; i<IQArraySize; i++)
    {
        IQResultsArray[i] = age1960_NOT_A_NUMBER;
    }

    for (i = 0; i<9; i++)
    {
        IQFreqsArray[i] = tempFreqArray[i];
    }

    if ((errStatus =  sendQueryRequest(instrumentHandle,
                                       "FETCH:IQT?;:SET:IQT:SPUR:FREQ?",
                                       ReadTimeout(instrumentHandle,"IQT") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%,9lf,%lf;%lf%*t",
                             IQIntegrity, IQResultsArray,
                             IQSpurLevel, IQSpurFreq))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    *IQTuningCount = 9;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchORFS_Q(
    ViSession  instrumentHandle,
    ViInt32    ORFSSwArraySize,
    ViInt32    ORFSModArraySize,
    ViPInt32   ORFSIntegrity,
    ViPReal64  ORFSTXPower,
    ViPReal64  ORFSBWPower,
    ViReal64   _VI_FAR ORFSSwArray[],
    ViReal64   _VI_FAR ORFSModArray[],
    ViPInt32   ORFSSwCount,
    ViPInt32   ORFSModCount)
/*****************************************************************************/
/*  This function will perform a FETCh:ORFS and return the Integrity,        */
/*  TX Power, 30 kHz BW Power, Switching results array, and Modulation       */
/*  results array.                                                           */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    ORFSSwArraySize (in)                                        */
/*      The size of the orfsSwArray[] array.                                 */
/*    ViInt32    ORFSModArraySize (in)                                       */
/*      The size of the orfsModArray[] array.                                */
/*    ViPInt32   ORFSIntegrity (out)                                         */
/*      The returned value from the instrument for ORFS integrity.           */
/*    ViPReal64  ORFSTXPower (out)                                           */
/*      The returned value from the instrument for ORFS TX power result.     */
/*    ViPReal64  ORFSBWPower (out)                                           */
/*      The returned value from the instrument for ORFS 30 kHz BW power      */
/*      result.                                                              */
/*    ViReal64   ORFSSwArray[] (out)                                         */
/*      The returned values from the instrument for ORFS Switching results.  */
/*    ViReal64   ORFSModArray[] (out)                                        */
/*      The returned values from the instrument for ORFS Modulation results. */
/*    VIPint32   ORFSSwCount  (out)                                          */
/*      The returned value from the instrument query for                     */
/*      SETup:ORFS:SWITching:FREQuencyPOINTs?, which contain the number of   */
/*      points returned by orfsSwArray[].                                    */
/*    VIPint32   ORFSModCount  (out)                                         */
/*      The returned value from the instrument query for                     */
/*      SETup:ORFS:MODulation:FREQuencyPOINTs?, which contain the number of  */
/*      points returned by orfsModArray[].                                   */
/*                                                                           */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*                                                                           */
/*    <Integrity Integer>,<Real TX Power>,                                   */
/*    <[Real ORFS Switching result 1>{,<Real ORFS Switching result i>}],     */
/*    [<Real 30 kHz BW Power>,                                               */
/*    <Real ORFS Modulation result 1>{,<Real ORFS Modulation result j>}],    */
/*    With i being the number of points from                                 */
/*    SETup:ORFS:SWITching:FREQuency:POINts? query and                       */
/*    j being the number of points from                                      */
/*    SETup:ORFS:MODulation:FREQuency:POINts? query                          */
/*    If either of the POINTs are 0 the optional information in [] will not  */
/*    be returned from the instrument and the values returned to the calling */
/*    function will not be set.                                              */
/*****************************************************************************/

{
    ViStatus  errStatus;
    ViInt32   swPoints;
    ViInt32   modPoints;
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchORFS_Q",",%ld,%ld", ORFSSwArraySize,
                  ORFSModArraySize);

    *ORFSSwCount = 0;
    *ORFSModCount = 0;

    CHK_LONG_RANGE(ORFSSwArraySize, age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(ORFSModArraySize, age1960_ARRAY_MIN,
                   age1960_ARRAY_MAX, VI_ERROR_PARAMETER3)

    /* Default all measurement results to NAN */
    *ORFSIntegrity = age1960_NO_RESULT_AVAIL;
    *ORFSTXPower = age1960_NOT_A_NUMBER;
    *ORFSBWPower = age1960_NOT_A_NUMBER;
    for (i = 0; i<ORFSSwArraySize; i++)
        ORFSSwArray[i] = age1960_NOT_A_NUMBER;
    for (i = 0; i<ORFSModArraySize; i++)
        ORFSModArray[i] = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SET:ORFS:SWIT:FREQ:POIN?;:SET:ORFS:MOD:FREQ:POIN?",
                                       10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle,  "%ld;%ld%*t",
                             &swPoints, &modPoints)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (swPoints > ORFSSwArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);

    if (modPoints > ORFSModArraySize)
        return statusUpdate(instrumentHandle, thisPtr,
                            age1960_INSTR_ERROR_ARRAY_TOO_SMALL);
    if (swPoints != 0 && modPoints != 0)
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:ORFS?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf,%,#lf,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower, &swPoints,
                                 ORFSSwArray, ORFSBWPower, &modPoints,
                                 ORFSModArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (swPoints == 0 && modPoints != 0)
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:ORFS?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower,
                                 ORFSBWPower, &modPoints,
                                 ORFSModArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (swPoints != 0 && modPoints == 0)
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:ORFS?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf,%,#lf%*t",
                                 ORFSIntegrity, ORFSTXPower,
                                 &swPoints, ORFSSwArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        *ORFSBWPower = age1960_NOT_A_NUMBER; /* set BW power to NAN since it was not measured */
    }

    if (swPoints == 0 && modPoints == 0)
    {
        if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:ORFS?", 10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus = viScanf(instrumentHandle,  "%ld,%lf%*t",
                                 ORFSIntegrity, ORFSTXPower)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        *ORFSBWPower = age1960_NOT_A_NUMBER; /* set BW power to NAN since it was not measured */
    }

    *ORFSSwCount = swPoints;
    *ORFSModCount = modPoints;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchFBER_Q(
    ViSession  instrumentHandle,
    ViPInt32   FBERIntegrity,
    ViPReal64  FBERBitsTested,
    ViPReal64  FBERRatio,
    ViPReal64  FBERCount)
/*****************************************************************************/
/*  This function will perform a FETCh:FBERror and return the Integrity,     */
/*  bits tested, fast bit error ratio, and fast bit error count.             */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   FBERIntegrity (out)                                         */
/*      The returned value from the instrument for fast bit error integrity. */
/*    ViPReal64  FBERBitsTested (out)                                        */
/*      The returned value from the instrument for number of bits tested.    */
/*    ViPReal64  FBERRatio (out)                                             */
/*      The returned value from the instrument for fast bit error ratio.     */
/*    ViPReal64  FBERCount (out)                                             */
/*      The returned value from the instrument for fast bit error count.     */
/*  The function assumes that the data returned by the instrument has the    */
/*    following form:                                                        */
/*    <Integrity Integer>,<Real Fast Bits Tested>,                           */
/*    <Real Fast Bit Error Ratio>, <Real Fast Bit Error Count>               */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchFBER_Q","");

    /* Default all measurement results to NAN */
    *FBERIntegrity = age1960_NO_RESULT_AVAIL;
    *FBERBitsTested = age1960_NOT_A_NUMBER;
    *FBERRatio = age1960_NOT_A_NUMBER;
    *FBERCount = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:FBER?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf%*t",
                             FBERIntegrity, FBERBitsTested,
                             FBERRatio, FBERCount))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchBER_Q(
    ViSession  instrumentHandle,
    ViPInt32   BERIntegrity,
    ViPReal64  TypeIaBERBitsTested,
    ViPReal64  TypeIaBERRatio,
    ViPReal64  TypeIaBERCount,
    ViPReal64  TypeIbBERBitsTested,
    ViPReal64  TypeIbBERRatio,
    ViPReal64  TypeIbBERCount,
    ViPReal64  TypeIIBERBitsTested,
    ViPReal64  TypeIIBERRatio,
    ViPReal64  TypeIIBERCount)
/********************************************************************************/
/*  This function will perform a FETCh:BERRor:FULL and return the Integrity,	*/
/*  then the bits tested, bit error ratio, and bit error count for each of		*/
/*  3 bit types. It will be either residual or non-residual depending on		*/
/*  what is set in the configure.												*/
/*  It will also turn off all measurement functions that cannot run				*/
/*  concurrently with BER.														*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViPInt32    BERIntegrity (out)											*/
/*      The returned value from the instrument for bit error integrity.			*/
/*    ViPReal64   TypeIaBERBitsTested (out)										*/
/*      The returned value from the instrument for number of bits tested.		*/
/*    ViPReal64   TypeIaBERRatio (out)											*/
/*      The returned value from the instrument for bit error ratio.				*/
/*    ViPReal64   TypeIaBERCount (out)											*/
/*      The returned value from the instrument for bit error count.				*/
/*  The function assumes that the data returned by the instrument has the		*/
/*    following form:															*/
/*    <Integrity Integer>,<Real Bits Tested>,<Real Bit Error Ratio>,			*/
/*    <Real Bit Error Count>													*/
/********************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchBER_Q","");

    /* Default all measurement results to NAN */
    *BERIntegrity= age1960_NO_RESULT_AVAIL;
    *TypeIaBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIaBERRatio = age1960_NOT_A_NUMBER;
    *TypeIaBERCount = age1960_NOT_A_NUMBER;
    *TypeIbBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIbBERRatio = age1960_NOT_A_NUMBER;
    *TypeIbBERCount = age1960_NOT_A_NUMBER;
    *TypeIIBERBitsTested = age1960_NOT_A_NUMBER;
    *TypeIIBERRatio = age1960_NOT_A_NUMBER;
    *TypeIIBERCount = age1960_NOT_A_NUMBER;

    if ((errStatus = delayVPrintf(instrumentHandle, "INIT:FBER:OFF;:INIT:DAUD:OFF\n")) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETCh:BERR:FULL?",
                                       ReadTimeout(instrumentHandle,"BERR") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle,
                             "%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf%*t",
                             BERIntegrity, TypeIaBERBitsTested,
                             TypeIaBERRatio, TypeIaBERCount,
                             TypeIbBERBitsTested, TypeIbBERRatio,
                             TypeIbBERCount, TypeIIBERBitsTested,
                             TypeIIBERRatio, TypeIIBERCount))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchAAudio_Q(
    ViSession  instrumentHandle,
    ViPInt32   AAudioIntegrity,
    ViPReal64  AAudioResult)
/*****************************************************************************/
/*  This function will perform a FETCh:AAUDio and return the Integrity,      */
/*  and audio measurement result.                                            */
/*                                                                           */
/*  This function supports earlier versions of the E1960A TA. Greater		 */
/*  functionality can be achieved using the age1960_fetchfAFAN() function	 */
/*  if your version of the E1960A TA will support it (A.07xx or later).		 */
/*																			 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   AAudioIntegrity (out)                                       */
/*      The returned value from the instrument for analog audio integrity.   */
/*    ViPReal64  AAudio (out)                                                */
/*      The returned value from the instrument for analog audio result.      */
/*                                                                           */
/*    <Integrity Integer>,<Real Analog Audio Result>                         */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchAAudio_Q","");

    /* Default all measurement results to NAN */
    *AAudioIntegrity = age1960_NO_RESULT_AVAIL;
    *AAudioResult = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:AAUD?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             AAudioIntegrity, AAudioResult)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/********************************************************************************/
ViStatus _VI_FUNC age1960_fetchAFAN_Q(
    ViSession instrumentHandle,
    ViPInt32   AFANIntegrity,
    ViPReal64  AFANAvgAudioLevel,
    ViPReal64  AFANAvgSINAD,
    ViPReal64  AFANAvgDist)
/********************************************************************************/
/*  This function will perform a FETC:AAUD and return the Integrity,			*/
/*  Average Audio Level, Average SINAD, Average Distortion (if available).		*/
/*																				*/
/*  Note: This function adds functionality that was not available in earlier	*/
/*  versions of the E1960A TA. If backward compatibility is required, use the	*/
/*  age1960_fetchAAudio() function.												*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViPInt32   AFANIntegrity (out)											*/
/*      The returned value from the instrument for AFAN integrity.				*/
/*    ViPReal64  AFANAvgAudioLevel (out)										*/
/*      The returned value from the instrument for Average Audio Level result	*/
/*    ViPReal64  AFANAvgSINAD (out)												*/
/*      The returned value from the instrument for Average SINAD result.		*/
/*    ViPReal64  AFANAvgDist (out)												*/
/*      The returned value from the instrument for Average Dist result.			*/
/*																				*/
/********************************************************************************/

{
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchAFAN_Q", "");

    /* Default all measurement results to NAN */
    *AFANIntegrity = age1960_NO_RESULT_AVAIL;
    *AFANAvgAudioLevel = age1960_NOT_A_NUMBER;
    *AFANAvgSINAD = age1960_NOT_A_NUMBER;
    *AFANAvgDist = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:AAUD?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle,"%ld,%lf,%lf,%lf%*t",
                             AFANIntegrity, AFANAvgAudioLevel,
                             AFANAvgSINAD, AFANAvgDist)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_fetchDAudio_Q(
    ViSession  instrumentHandle,
    ViPInt32   DAudioIntegrity,
    ViPReal64  DAudioResult)
/*****************************************************************************/
/*  This function will perform a FETCh:DAUDio and return the Integrity,      */
/*  and audio measurement result.                                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPInt32   DAudioIntegrity (out)                                       */
/*      The returned value from the instrument for analog audio integrity.   */
/*    ViPReal64  DAudio (out)                                                */
/*      The returned value from the instrument for analog audio result.      */
/*                                                                           */
/*    <Integrity Integer>,<Real TCH Audio Result>                            */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_fetchDAudio_Q","");

    /* Default all measurement results to NAN */
    *DAudioIntegrity = age1960_NO_RESULT_AVAIL;
    *DAudioResult = age1960_NOT_A_NUMBER;

    if ((errStatus =  sendQueryRequest(instrumentHandle, "FETC:DAUD?", 10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             DAudioIntegrity, DAudioResult))  < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/********************************************************************************/
ViStatus _VI_FUNC age1960_waitForInitDone_Q(
    ViSession  instrumentHandle,
    ViReal64   initDoneTimeOut,
    ViPInt16   measFcnDone)
/********************************************************************************/
/*  This function will wait until an initiated measurement					    */
/*     is complete.  The routine stops trying after the timeOut value is		*/
/*     reached.																	*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViReal64   initDoneTtimeOut (in)											*/
/*      Set the maximum time this function will try (in seconds).				*/
/*    ViPInt16   measFcnDone (out)												*/
/*      An integer that repesents the following measurement function is			*/
/*      done and ready to be fetched.											*/
/*																				*/
/*      WAIT   =-2  Wait                        (age1960_WAIT)					*/
/*      NONE   =-1  None have been INIT         (age1960_NONEINIT)				*/
/*      AAUD   = 0  Analog Audio Meas Fcn       (age1960_AAUDIO)				*/
/*      BERR   = 1  Bit Error Rate Meas Fcn     (age1960_BER)					*/
/*      DAUD   = 2  Decoded Audio Meas Fcn      (age1960_DAUDIO)				*/
/*      FBER   = 3  Fast Bit Error Meas Fcn     (age1960_FBER)					*/
/*      ORFS   = 4  ORFSpectrum Meas Fcn        (age1960_ORFS)					*/
/*      PFER   = 5  Phase Freq Error Meas Fcn   (age1960_PFER)					*/
/*      PVT    = 6  Power vs Time Meas Fcn      (age1960_PVTIME)				*/
/*      TXP    = 7  TX Power Meas Fcn           (age1960_TXPOWER)				*/
/*      DPOW   = 8  Dynamic Power Meas Fcn      (age1960_DPOWER)				*/
/*      IQT    = 9  IQ Tuning Meas Fcn          (age1960_IQTUNING)				*/
/*																				*/
/********************************************************************************/

{

    ViStatus  errStatus;
    ViChar    response[20];
    long      maxTimeToWait;
    long      tStart;
    short     fcnDone = age1960_WAIT;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_waitForInitDone_Q",",%lg",initDoneTimeOut);

    CHK_REAL_RANGE(initDoneTimeOut, age1960_INIT_TIMEOUT_MIN,
                   age1960_INIT_TIMEOUT_MAX, VI_ERROR_PARAMETER2)

    if (thisPtr->useSRQ == VI_FALSE)   /* if the user does not want to use SRQ interrupts */
    {
        maxTimeToWait = (long) ceil(initDoneTimeOut);     /* up maxTimeToWait to the */
        /* nearest whole second    */

        tStart = time(NULL); /* Get the current time to second resolution*/

        while(fcnDone == age1960_WAIT)
        {
            if ((errStatus = delayVQueryf(instrumentHandle, "INIT:DONE?\n", "%s%*t",
                                          response)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            fcnDone = respToMeasFcnDone(response);
            if (fcnDone != age1960_WAIT) break;
            if (maxTimeToWait -  difftime(time(NULL),tStart) <0)
                return VI_ERROR_TMO;  /* use the timeout errStatus as the returned error */
            doDelay(200000);          /* wait 200 msec */
        }
        *measFcnDone = fcnDone;
        return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);
    }    /* end of useSRQ == VI_FALSE */

    if ((errStatus = confForInterrupt(instrumentHandle, age1960_WAITFOR_MEAS,0)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* see if any measurement function is already done */

    if ((errStatus = delayVQueryf(instrumentHandle, "INIT:DONE?\n", "%s%*t",
                                  response)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    fcnDone = respToMeasFcnDone(response);

    if (fcnDone != age1960_WAIT)
    {
        *measFcnDone = fcnDone;
        srq_cleanup(instrumentHandle);
        return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);
    }


    /* Wait for instrument to interrupt indicating that a new measurement is completed */

    if ((errStatus = waitForInterrupt (instrumentHandle, (long) (initDoneTimeOut*1000))) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = delayVQueryf(instrumentHandle, "INIT:DONE?\n", "%s%*t",
                                  response)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    fcnDone = respToMeasFcnDone(response);
    *measFcnDone = fcnDone;

    if (fcnDone == age1960_WAIT) /* instrument responding with "WAIT" for */
        /* measurement function to finish */
        /* A Measurement function was not completed within the given time. */
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_INITTO);

    return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_originateCall(
    ViSession  instrumentHandle,
    ViInt16    completion,
    ViReal64   originateTimeOut)
/*****************************************************************************/
/*  This function will perform a CALL:ORIG                                   */
/*  If the instrument indicates that the call is already CONNECTED this      */
/*  function will return with VI_SUCCESS.  If the instrument indicated that  */
/*  it is not in the proper state to originate a call this function will     */
/*  return with the errStatus set. After the call has been originated this   */
/*  function will re-check the call status state to verify that it is        */
/*  CONNECTED, if it is not this function will return an error condition.    */
/*  This function will use repeat paging.                                    */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    completion (in)                                             */
/*      Indicates the completion criteria for the call origination.  The     */
/*      available choices are:                                               */
/*      0  Call at the Connected state  (age1960_CALL_CONNECTED)             */
/*      1  Call at the Alerting state   (age1960_CALL_ALERTING)              */
/*    ViReal64   originateTimeOut (in)                                       */
/*      Set the maximum time (in seconds) this function will wait for the    */
/*      call to connect.  If originateTimeOut is set to 0, this function will*/
/*      just originate the call and not wait until it is alerting or         */
/*      connected.                                                           */
/*****************************************************************************/

{

    ViStatus    errStatus ;
    ViChar      callStatus[10];
    ViChar      mode[5];
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_originateCall",",%hd,%lg",completion, originateTimeOut);

    CHK_ENUM(completion, 1, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(originateTimeOut, age1960_CP_TIMEOUT_MIN, age1960_CP_TIMEOUT_MAX, VI_ERROR_PARAMETER3)

    /* Check if call is already connected or */
    /* not in the correct state to be connected. */

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:STAT?\n", "%s%*t",
                                  callStatus)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (strcmp(callStatus,"CONN") == 0)
        return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:OPER:MODE?\n", "%s%*t",
                                  mode)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (strcmp(mode,"CELL") != 0)
        return statusUpdate(instrumentHandle,thisPtr,
                            age1960_INSTR_CANNOT_ORIG_NOW);

    if (strcmp(callStatus,"IDLE") != 0)
        return statusUpdate(instrumentHandle,thisPtr,
                            age1960_INSTR_CANNOT_ORIG_NOW);

    if (originateTimeOut>0)
    {
        if ((errStatus = confForInterrupt(instrumentHandle, age1960_WAITFOR_CALL,
                                          completion)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:PAG:REP:GSM ON;:CALL:ORIG\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = waitForInterrupt (instrumentHandle, (long) (originateTimeOut*1000))) < VI_SUCCESS)
        {
            delayVPrintf(instrumentHandle, "CALL:PAG:REP:GSM OFF;:CALL:END\n"); /* cleanup the call processing */
            return statusUpdate(instrumentHandle,thisPtr, age1960_INSTR_ERROR_CALL_FAILED);
        }
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:PAG:REP:GSM OFF\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:ORIG\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_endCall(
    ViSession  instrumentHandle,
    ViBoolean  endSeq)
/*****************************************************************************/
/*  This function will perform a CALL:END                                    */
/*  If the instrument indicates that the call is not CONNECTED this          */
/*  function will return with VI_SUCCESS.  If end Seq is set to VI_TRUE,     */
/*  After the call has been ended this function will re-check the call       */
/*  status state to verify that it is not in the CONNECTED state, if it is   */
/*  this function will return an error condition.                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViBoolean   endSeq (in)                                                */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the Call is released or the internal timer has expired.        */
/*****************************************************************************/

{

    ViStatus    errStatus ;
    ViChar      callStatus[20];

    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_endCall",",hu",endSeq);

    CHK_BOOLEAN(endSeq, VI_ERROR_PARAMETER2);

    /* Check if call is already disconnected. */

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:STAT?\n", "%s%*t",
                                  callStatus)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (strcmp(callStatus,"IDLE") == 0)
        return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:END%s\n",
                                  age1960_seqSetting_a[endSeq])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ( endSeq == VI_TRUE)
    {
        /* Verify that the call is disconnected.*/

        if ((errStatus = delayVQueryf(instrumentHandle, "CALL:STAT?\n", "%s%*t",
                                      callStatus)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if (strcmp(callStatus,"IDLE") != 0)
            return statusUpdate(instrumentHandle,thisPtr,
                                age1960_INSTR_ERROR_CALL_FAILED);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_TCHARFCN(
    ViSession  instrumentHandle,
    ViInt32    TCHARFCN,
    ViInt16    TCHARFCNBand,
    ViBoolean  TCHARFCNSeq)
/*****************************************************************************/
/*  This function will perform a CALL:TCHannel[:ARFCn] for the desired 		 */
/*	TCH band.                                              					 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    TCHARFCN (in)                                               */
/*      The TCH ARFCN number to be set.                                      */
/*    ViInt16    TCHARFCNBand (in)                                           */
/*      The desired TCH ARFCN Band to set the TCH ARFCN for.  The bands      */
/*      are defined as follows:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*																			 */
/*    ViBoolean   TCHARFCNSeq (in)                                           */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the TCH has changed or the internal timer has expired.         */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_TCHARFCN",",%ld,%hd,%hu", TCHARFCN, TCHARFCNBand,
                  TCHARFCNSeq);

    CHK_ENUM(TCHARFCNBand, 5, VI_ERROR_PARAMETER3)

    CHK_BOOLEAN(TCHARFCNSeq, VI_ERROR_PARAMETER4);

    if ((errStatus = age1960_checkTCHARFCN(instrumentHandle,
                                           TCHARFCN, TCHARFCNBand))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH%s%s %ld\n",
                                  age1960_bandSetting_a[TCHARFCNBand],
                                  age1960_seqSetting_a[TCHARFCNSeq],
                                  TCHARFCN)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_manualChan(
    ViSession  instrumentHandle,
    ViInt32    manualChan,
    ViInt16    manualChanBand)
/*****************************************************************************/
/*  This function will perform a RFAN:MAN:CHAN for the desired expected 	 */
/*	band. By setting this command the instrument will place the receiver 	 */
/*	control mode to manual.  If it is desired to use the control mode in 	 */
/*	auto it must be set using the age1960_confRcvControl function.           */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    manualChan (in)                                             */
/*      The Expected Channel number to be set.                               */
/*    ViInt16    manualChanBand (in)                                         */
/*      The desired expected Band to set the expected channel for.           */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus;

    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_manualChan",",%ld,%hd", manualChan,
                  manualChanBand);

    CHK_ENUM(manualChanBand, 5, VI_ERROR_PARAMETER3)

    if ((errStatus = age1960_checkManualARFCN(instrumentHandle, manualChan,
                     manualChanBand))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = delayVPrintf(instrumentHandle, "RFAN:MAN:CHAN%s %ld\n",
                                  age1960_bandSetting_a[manualChanBand],
                                  manualChan)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_expectedPower(
    ViSession  instrumentHandle,
    ViReal64   expectedPower,
    ViInt16    expectedPowerBand)
/*****************************************************************************/
/*  This function will perform a RFANalyzer:EXPected:POWer for the derired 	 */
/*	expected power band.                                   					 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViReal64    expectedPower (in)                                         */
/*      The expectedPower in dBm.                                            */
/*    ViInt16     expectedPowerBand (in)                                     */
/*      The desired expected Power Band to set the expected Power for.       */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_expectedPower",",%lg,hd", expectedPower,
                  expectedPowerBand);

    CHK_REAL_RANGE(expectedPower, age1960_EXPECTED_POWER_MIN,
                   age1960_EXPECTED_POWER_MAX, VI_ERROR_PARAMETER2)

    CHK_ENUM(expectedPowerBand, 5, VI_ERROR_PARAMETER3)

    if ((errStatus = delayVPrintf(instrumentHandle, "RFAN:EXP:POW%s %lg\n",
                                  age1960_bandSetting_a[expectedPowerBand],
                                  expectedPower)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confRcvControl(
    ViSession  instrumentHandle,
    ViInt16    rcvControlMode)
/*****************************************************************************/
/*  This function will perform a RFANalyzer:CONT:AUTO<boolean>               */
/*   When setting this control to AUTO ON, the test set will couple the      */
/*   downlink channel to the RF analyzer receiver channel.                   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    rcvControlMode (in)                                         */
/*      The receiver control modes are defined as follows:                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_MAN_RCV_CONTROL   0          Manual receiver control         */
/*      age1960_AUTO_RCV_CONTROL  1          Auto receiver control           */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus;

    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confRcvControl",",%hd", rcvControlMode);

    CHK_ENUM(rcvControlMode, 1, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "RFAN:CONT:AUTO %hd\n",
                                  rcvControlMode)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_MSTXLevel(
    ViSession  instrumentHandle,
    ViInt32    MSTXLevel,
    ViInt16    MSTXLevelBand,
    ViBoolean  MSTXLevelSeq)
/*****************************************************************************/
/*  This function will perform a CALL:MS:TXLevel for the desired TCH Band.   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    MSTXLevel (in)                                              */
/*      The MS TX Level number to be set.                                    */
/*    ViInt16    MSTXLevelBand (in)                                          */
/*      The desired MS TX Level Band to set the MS TX Level for.             */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*																			 */
/*    ViBoolean   MSTXLevelSeq (in)                                          */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the MSTXLevel has changed or the internal timer has expired.   */
/*****************************************************************************/

{

    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_MSTXLevel",",%ld,%hd,%hu", MSTXLevel,
                  MSTXLevelBand, MSTXLevelSeq);

    CHK_ENUM(MSTXLevelBand, 5, VI_ERROR_PARAMETER3)

    CHK_BOOLEAN(MSTXLevelSeq, VI_ERROR_PARAMETER4);

    if ((errStatus = age1960_checkMSTXLevel(instrumentHandle, MSTXLevel,
                                            MSTXLevelBand))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:MS:TXL%s%s %ld\n",
                                  age1960_bandSetting_a[MSTXLevelBand],
                                  age1960_seqSetting_a[MSTXLevelSeq],
                                  MSTXLevel)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_TCHTimeslot(
    ViSession  instrumentHandle,
    ViInt32    TCHTimeslot,
    ViBoolean  TCHTimeslotSeq)
/*****************************************************************************/
/*  This function will perform a CALL:TCHannel:TSLot                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    TCHTimeslot (in)                                            */
/*      The TCH timeslot number to be set.                                   */
/*    ViBoolean   TCHTimeslotSeq (in)                                        */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the time slot has changed or the internal timer has expired.   */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_TCHTimeslot",",%ld,%hu", TCHTimeslot,
                  TCHTimeslotSeq);

    CHK_LONG_RANGE(TCHTimeslot, age1960_TIMESLOT_MIN,
                   age1960_TIMESLOT_MAX, VI_ERROR_PARAMETER2)

    CHK_BOOLEAN(TCHTimeslotSeq, VI_ERROR_PARAMETER3);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:TSL%s %ld\n",
                                  age1960_seqSetting_a[TCHTimeslotSeq],
                                  TCHTimeslot)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_MSTimeAdvance(
    ViSession  instrumentHandle,
    ViInt32    MSTimingAdvance,
    ViBoolean  MSTimingAdvanceSeq)
/*****************************************************************************/
/*  This function will perform a CALL:MS:TADV                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    MSTimingAdvance (in)                                        */
/*      The MS timing advance to be set.                                     */
/*    ViBoolean   MSTimingAdvanceSeq (in)                                    */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the MS TimingAdvance has changed or the internal timer has     */
/*      expired.                                                             */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_MSTimeAdvance",",%ld,%hu", MSTimingAdvance,
                  MSTimingAdvanceSeq);

    CHK_LONG_RANGE(MSTimingAdvance, age1960_TADVANCE_MIN,
                   age1960_TADVANCE_MAX, VI_ERROR_PARAMETER2)

    CHK_BOOLEAN(MSTimingAdvanceSeq, VI_ERROR_PARAMETER3);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:MS:TADV%s %ld\n",
                                  age1960_seqSetting_a[MSTimingAdvanceSeq],
                                  MSTimingAdvance)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_BCHARFCN(
    ViSession  instrumentHandle,
    ViInt16    BCHARFCNCell,
    ViInt32    BCHARFCN,
    ViInt16    BCHARFCNBand,
    ViBoolean  BCHARFCNSeq)
/*****************************************************************************/
/*  This function will perform a CALL:[CELL[1]]:BCHannel                     */
/*   for the desired band and cell.                                          */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    BCHARFCNCell (in)                                           */
/*      The BCH ARFCN cell to set the BCH ARFCN for.                         */
/*      The cells are definded as follows:                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CELL_1           1           Cell 1                          */
/*      age1960_CELL_2           2           Cell 2                          */
/*                                                                           */
/*    ViInt32    BCHARFCN (in)                                               */
/*      The BCH ARFCN number to be set.                                      */
/*    ViInt16    BCHARFCNBand (in)                                           */
/*      The desired BCH ARFCN Band to set the BCH ARFCN for.                 */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*																			 */
/*    ViBoolean   BCHARFCNSeq (in)                                           */
/*      Set to VI_TRUE if this function should use the sequence node to wait */
/*      until the BCN has changed or the internal timer has expired.         */
/*****************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_BCHARFCN",",%hd,%ld,%hd,%hu", BCHARFCNCell,
                  BCHARFCN, BCHARFCNBand, BCHARFCNSeq);

    CHK_INT_RANGE(BCHARFCNCell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    CHK_ENUM(BCHARFCNBand, 5, VI_ERROR_PARAMETER4)

    if (BCHARFCNCell ==  age1960_CELL_1)
    {

        if ((errStatus = age1960_checkBCHARFCNCell1(instrumentHandle, BCHARFCN,
                         BCHARFCNBand))< VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if (BCHARFCNCell ==  age1960_CELL_2)
    {

        if ((errStatus = age1960_checkBCHARFCNCell2(instrumentHandle, BCHARFCN,
                         BCHARFCNBand))< VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    CHK_BOOLEAN(BCHARFCNSeq, VI_ERROR_PARAMETER5);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL%s:BCH%s%s %ld\n",
                                  age1960_cell_a[BCHARFCNCell],
                                  age1960_bandSetting_a[BCHARFCNBand],
                                  age1960_seqSetting_a[BCHARFCNSeq],
                                  BCHARFCN)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_cellBand(
    ViSession  instrumentHandle,
    ViInt16    cellBandCell,
    ViInt16    cellBand)
/*****************************************************************************/
/*  This function will set the cell band. The TCH band also gets set to the  */
/*	same band by this function.								                 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    cellBandCell (in)                                           */
/*      The cell to set the cell band for.                                   */
/*      The cells are definded as follows:                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CELL_1           1           Cell 1                          */
/*      age1960_CELL_2           2           Cell 2                          */
/*                                                                           */
/*    ViInt16    cellBand (in)                                               */
/*      The desired Cell Band to set.                                        */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_PGSM_BAND         1          PGSM 900 Band                   */
/*      age1960_EGSM_BAND         2          EGSM Band                       */
/*      age1960_DCS1800_BAND      3          DCS 1800 Band                   */
/*      age1960_PCS1900_BAND      4          PCS1900 Band                    */
/*      age1960_GSM850_BAND       5          GSM850 Band                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cellBand",",%hd,%hd", cellBandCell, cellBand);

    CHK_INT_RANGE(cellBandCell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    CHK_INT_RANGE(cellBand,1,5,VI_ERROR_PARAMETER3)

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL%s:BAND%s %s\n",
                                  age1960_cell_a[cellBandCell],
                                  age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                  age1960_band_a[cellBand])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_TCHBand(
    ViSession  instrumentHandle,
    ViInt16    TCHBand)
/*****************************************************************************/
/*  This function will set the TCH band using CALL:TCH:BAND. It will read 	 */
/*	the error message from the instrument to determine if the setting was 	 */
/*	successful. 															 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    TCHBand (in)                                                */
/*      The desired TCH Band to set.                                         */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_PGSM_BAND         1          PGSM 900 mode                   */
/*      age1960_EGSM_BAND         2          EGSM mode                       */
/*      age1960_DCS1800_BAND      3          DCS 1800 mode                   */
/*      age1960_PCS1900_BAND      4          PCS1900 mode                    */
/*      age1960_GSM850_BAND       5          GSM850 mode                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    ViInt32    instErr;
    ViChar     errorMessage[256];

    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_TCHBand",",%hd", TCHBand);

    CHK_INT_RANGE(TCHBand, 1, 5, VI_ERROR_PARAMETER2)

    /* clear the error message buffer in the instrument*/

    if ((errStatus = delayVPrintf(instrumentHandle, "*CLS;CALL:TCH:BAND %s\n",
                                  age1960_band_a[TCHBand])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    age1960_error_query (instrumentHandle, &instErr,errorMessage);

    if (instErr == 237 )   /* Requested TCH band is invalid in current state */
    {
        /* cleanup the error message that is displayed on the test set */

        doDelay(75000);   /* Remove this should not be required, but the instrument */
        /* presently requires a 75 msec wait*/

        delayVPrintf(instrumentHandle, "*CLS\n");
        return statusUpdate(instrumentHandle,thisPtr, age1960_INSTR_CANNOT_CHNGBAND_NOW);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_manualBand(
    ViSession  instrumentHandle,
    ViInt16    manualBand)
/*****************************************************************************/
/*  This function will set the expected band using RFAN:MAN:BAND .			 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    manualBand (in)                                             */
/*      The desired manual Band to set.                                      */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_PGSM_BAND         1          PGSM 900 mode                   */
/*      age1960_EGSM_BAND         2          EGSM mode                       */
/*      age1960_DCS1800_BAND      3          DCS 1800 mode                   */
/*      age1960_PCS1900_BAND      4          PCS1900 mode                    */
/*      age1960_GSM850_BAND       5          GSM850 mode                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_manualBand",",%hd", manualBand);

    CHK_INT_RANGE(manualBand,  1, 5, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "RFAN:MAN:BAND %s\n",
                                  age1960_band_a[manualBand])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_operatingMode(
    ViSession  instrumentHandle,
    ViInt16    operatingMode)
/*****************************************************************************/
/*  This function will set the desired operating mode using CALL:OPER:MODE.  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    operatingMode (in)                                          */
/*      The desired operating mode.  Will be one of the following            */
/*                                                                           */
/*      Constant Name       Value        Description                         */
/*      -----------------------------------------------------------          */
/*      age1960_TEST_MODE    0          Test mode                            */
/*      age1960_ACELL_MODE   1          Active Cell mode                     */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus   errStatus ;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_operatingMode",",%hd", operatingMode);

    CHK_ENUM(operatingMode, 1, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:OPER:MODE %s\n",
                                  age1960_operatingMode_a[operatingMode]))< VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_BATable(
    ViSession  instrumentHandle,
    ViInt16    BATableCell,
    ViInt32    BATableArraySize,
    ViInt32    _VI_FAR BATableArray[],
    ViInt16    BATableBand)
/*****************************************************************************/
/*  Sends comma separated array for the BA Table.  Each ARFCN in the BA      */
/*  will be checked against the desired BATable Band.  The ARFCNs will be    */
/*  sorted in accending order and checked for duplicate ARFCN numbers        */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    BATableCell (in)                                            */
/*      The BA Table cell to set the BA Table ARFCNs for.                    */
/*      The cells are definded as follows:                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CELL_1           1           Cell 1                          */
/*      age1960_CELL_2           2           Cell 2                          */
/*                                                                           */
/*    ViInt32    BATableArraySize (in)                                       */
/*      The number of points in the BAArray to be sent to the instrument.    */
/*    ViInt32    BATableArray (in)                                           */
/*      The integer array of BA Table ARFCNs.                                */
/*    ViInt16    BATableBand (in)                                            */
/*      The desired BA Table Band to set the BA Table for.                   */
/*      The bands are defined as follows:                                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CURRENT_BAND      0          Current Band Set                */
/*      age1960_PGSM_BAND         1          PGSM 900 band                   */
/*      age1960_EGSM_BAND         2          EGSM band                       */
/*      age1960_DCS1800_BAND      3          DCS1800 band                    */
/*      age1960_PCS1900_BAND      4          PCS1900 band                    */
/*      age1960_GSM850_BAND       5          GSM850 band                     */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct   age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_BATable",",%hd,%ld,%ld,%hd", BATableCell,
                  BATableArraySize, BATableArray[0], BATableBand);

    CHK_INT_RANGE(BATableCell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(BATableArraySize,age1960_BA_TABLE_PTS_MIN,
                   age1960_BA_TABLE_PTS_MAX, VI_ERROR_PARAMETER3)

    CHK_ENUM(BATableBand, 5, VI_ERROR_PARAMETER5)

    if (BATableCell == age1960_CELL_1)
    {

        if ((errStatus = age1960_checkBATableCell1(instrumentHandle,
                         BATableArraySize, BATableArray,
                         BATableBand))< VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if (BATableCell == age1960_CELL_2)
    {

        if ((errStatus = age1960_checkBATableCell2(instrumentHandle,
                         BATableArraySize, BATableArray,
                         BATableBand))< VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if (BATableArraySize == 0)
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL%s:BA:TABL%s\n",
                                      age1960_cell_a[BATableCell],
                                      age1960_bandSetting_a[BATableBand])) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL%s:BA:TABL%s %,*ld\n",
                                      age1960_cell_a[BATableCell],
                                      age1960_bandSetting_a[BATableBand],
                                      BATableArraySize, BATableArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_burstType(
    ViSession  instrumentHandle,
    ViInt16    burstType)
/*****************************************************************************/
/*  This function will set the burst type using CALL:BURS:TYPE.              */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    burstType (in)                                              */
/*      The desired burst type condition to set.                             */
/*      The burst type conditions are defined as follows:                    */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_TSC0_BURST       0           TSC0 burst type                 */
/*      age1960_TSC1_BURST       1           TSC1 burst type                 */
/*      age1960_TSC2_BURST       2           TSC2 burst type                 */
/*      age1960_TSC3_BURST       3           TSC3 burst type                 */
/*      age1960_TSC4_BURST       4           TSC4 burst type                 */
/*      age1960_TSC5_BURST       5           TSC5 burst type                 */
/*      age1960_TSC6_BURST       6           TSC6 burst type                 */
/*      age1960_TSC7_BURST       7           TSC7 burst type                 */
/*      age1960_RACH_BURST       8           RACH burst type                 */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus   errStatus ;

    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_burstType",",%hd", burstType);

    CHK_ENUM(burstType, 8, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:BURS:TYPE %s\n",
                                  age1960_burst_a[burstType])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_amplitudeOffset(
    ViSession  instrumentHandle,
    ViReal64   _VI_FAR frequencyArray[],
    ViReal64   _VI_FAR offsetArray[],
    ViInt32    ampOffsetItems)
/*****************************************************************************/
/*  This function will fill the RF IN/OUT Amplitude Offset table.  This      */
/*  function will automatically set the state of the frequency correction to */
/*  ON. The offset will be used to adjust the downlink and uplink			 */
/*  frequencies.  When operating at frequencies not specified in the array,  */
/*  but within the boundary defined by the lowest frequency and the highest  */
/*  frequency, linear interpolation will be used to define offset values.    */
/*  When operating at frequencies outside the boundaries defined by the      */
/*  lowest frequency and the highest frequency, the offset associated with   */
/*  the closest frequency will be used.                                      */
/*                                                                           */
/*  In earlier versions of the GSM TA only 1 offset value could be set.		 */
/*  Beginning with E1960A TA version A.07.xx and E1985A TA version A.01.xx	 */
/*  the offset table can include up to 20 frequencies and associated offset	 */
/*  values.																	 */
/*																			 */
/*  WARNING!  The receiver may be overranged or underranged due to the       */
/*            combined expected power setting and the amplitude offset for   */
/*            the current frequency setting.  Turning on the automatic       */
/*            logging using age1960_errorQueryDetect will alert you to this  */
/*            problem.  Overranging or underranging the receiver may result  */
/*            in measurements being unavailable or invalid.                  */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViReal64   frequencyArray (in)                                         */
/*      The Real array of frequencies in Hz.                                 */
/*    ViReal64   offsetArray (in)                                            */
/*      The Real array of offsets in dB.                                     */
/*    ViInt32    ampOffsetItems (in)                                         */
/*      The number of items to send to the instrument. This number must be   */
/*      equal to or less than the actual frequency and offset array sizes.   */
/*																			 */
/*      age1960_AMPOFFSET_ITEMS_MIN    0   min items sent. Zero turns off    */
/*										   amplitude offsets.                */
/*      age1960_AMPOFFSET_ITEMS_MAX    20  max items sent.                   */
/*****************************************************************************/
{

    ViInt32   index;
    ViChar    frequencies[300];
    ViChar    offsets[300];
    ViChar    szString[20];
    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_amplitudeOffset", ",%lg,%lg,%ld",
                  frequencyArray[0], offsetArray[0],ampOffsetItems);

    /* check the size of the arrays */
    CHK_LONG_RANGE(ampOffsetItems, age1960_AMPOFFSET_ITEMS_MIN, age1960_AMPOFFSET_ITEMS_MAX, VI_ERROR_PARAMETER4)
    /* loop through - checking the boundries  */

    /* clear the string arrays */
    strcpy(frequencies,"");
    strcpy(offsets,"");

    if (ampOffsetItems == 0)
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "SYST:CORR:STAT 0\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
    }

    for (index=0; index<ampOffsetItems; index++)
    {

        CHK_REAL_RANGE(frequencyArray[index], age1960_OFFSET_FREQ_MIN,
                       age1960_OFFSET_FREQ_MAX, VI_ERROR_PARAMETER2)

        /* create a comma separated array*/
        sprintf(szString, "%lg", frequencyArray[index]);
        strcat(frequencies,szString);
        if (index != ampOffsetItems-1)  strcat(frequencies, ",");


        CHK_REAL_RANGE(offsetArray[index], age1960_PATH_GAIN_MIN,
                       age1960_PATH_GAIN_MAX, VI_ERROR_PARAMETER3)

        /* create a comma separated array*/
        sprintf(szString, "%lg", offsetArray[index]);
        strcat(offsets,szString);
        if (index != ampOffsetItems-1)  strcat(offsets, ",");

    }

    if ((errStatus = delayVPrintf(instrumentHandle, "SYST:CORR:STAT 1;FREQ %s;GAIN %s\n",
                                  frequencies,offsets)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_cellPower(
    ViSession  instrumentHandle,
    ViInt16    cellPowerCell,
    ViReal64   cellPower,
    ViReal64   cellPowerDelta)
/********************************************************************************/
/*  This function will set the instrument's cell output power level in dBm.		*/
/*  This function will also allow for the correction of any differences			*/
/*  between uplink path offset that was set with the age1960_amplitudeOffset()	*/
/*  function and downlink path offset requirements when a single offset is used.*/
/*																	            */
/*  This function will also turn the power state on.							*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViInt16    cellPowerCell (in)												*/
/*      The cell to set the cell Power for.										*/
/*      The cells are definded as follows:										*/
/*																				*/
/*				age1960_CELL_1           1           Cell 1						*/
/*				age1960_CELL_2           2           Cell 2						*/
/*																				*/
/*    ViReal64    cellPower (in)												*/
/*      The cell power in dBm at the UUT plane.									*/
/*																				*/
/*				age1960_CELL_POWER_MIN	-127.000000								*/
/*				age1960_CELL_POWER_MAX	-10.000000								*/
/*																				*/
/*	  ViReal64    cellPowerDelta (in)											*/
/*		The delta value that will be added to the Cell Power setting to correct	*/
/*		for differences between the downlink and uplink path losses if only a	*/
/*		single value is set with age1960_amplitudeOffset() function. Older		*/
/*		versions of firmware used only a single path loss correction value and	*/
/*		this parameter was needed to correct for differences between uplink		*/
/*		frequency and downlink frequency path losses. Newer versions of			*/
/*		firmware allow up to 20 frequencies and associated path losses to be	*/
/*		entered, thereby negating the need for this parameter. If you are using	*/
/*		more than one entry in the RF IN/OUT Amplitude offset table than this	*/
/*		parameter should always have a value of 0.								*/
/*																				*/
/*				CELL POWER DELTA_MIN	-75.000000								*/
/*				CELL POWER DELTA_MAX	75.000000								*/
/*																				*/
/********************************************************************************/

{

    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_cellPower",",%hd,%lg,%lg", cellPowerCell,
                  cellPower, cellPowerDelta);

    CHK_INT_RANGE(cellPowerCell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(cellPower + cellPowerDelta, age1960_CELL_POWER_MIN,
                   age1960_CELL_POWER_MAX, VI_ERROR_PARAMETER3);

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL%s:POW:AMPL%s %lg"
                                  ";:CALL%s:POW:STAT%s ON\n",
                                  age1960_cell_a[cellPowerCell],
                                  age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                  cellPower + cellPowerDelta,
                                  age1960_cell_a[cellPowerCell],
                                  age1960_TAPTAGSM_a[thisPtr->TAIndex])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/********************************************************************************/
ViStatus _VI_FUNC age1960_channelMode (ViSession instrumentHandle,
                                       ViInt16 channelMode)
/********************************************************************************/
/*  This function will set the channel mode using CALL:TCH:CMODE				*/
/*   It will read the error message from the instrument to determine if the		*/
/*   setting was successful.													*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViInt16    channelMode (in)												*/
/*      The desired mode to set.												*/
/*      The modes are defined as follows:										*/
/*																				*/
/*      Constant Name            Value        Description						*/
/*      -----------------------------------------------------------				*/
/*      age1960_FR_SPEECH         0          Full Rate Speech					*/
/*      age1960_EFR_SPEECH        1          Enhanced Full Rate Speech			*/
/*																				*/
/* 																				*/
/********************************************************************************/
{
    ViStatus   errStatus ;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_channelMode",",%hd", channelMode);

    CHK_ENUM(channelMode, 1, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:CMODE %s\n",
                                  age1960_channelMode_a[channelMode]))< VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);


}        /* ----- end of function ----- */


/****************************************************************************/
ViStatus _VI_FUNC age1960_FACCHSignaling(
    ViSession  instrumentHandle,
    ViBoolean  facchState)
/****************************************************************************/
/* This function will set the TX Level FACCH Signaling on or off.			*/
/*                                                                          */
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                     */
/*    ViBoolean  facchState (in)											*/
/*      Set to VI_TRUE this function will set the FACCH signaling to on.	*/
/*      If VI_FALSE the FACCH signaling will								*/
/*      be disabled.                                                        */
/****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_FACCHSignaling", ",%hu", facchState);

    CHK_BOOLEAN(facchState, VI_ERROR_PARAMETER2);

    if (facchState == VI_TRUE)
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:SIGNaling:MS:TXLevel:FACCH 1\n"))< VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        if ((errStatus = delayVPrintf(instrumentHandle, "CALL:SIGNaling:MS:TXLevel:FACCH 0\n"))< VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_integrityMessage(
    ViSession  instrumentHandle,
    ViInt32    integrityCode,
    ViPString  integrityMessage)
/*****************************************************************************/
/*  This function will convert an integrity code to the integrity message.   */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    integrityCode (in)                                          */
/*      The integrity value returned from a measurement function query       */
/*    ViPString   integrityMessage[] (out)                                   */
/*      The integrity message string.  This is limited to 256 characters.    */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_integrityMessage",",%ld", integrityCode);

    strcpy(integrityMessage, "Unknown integrity code");

    CHK_LONG_RANGE(integrityCode,age1960_INTEGRITY_MIN,
                   age1960_INTEGRITY_MAX, VI_ERROR_PARAMETER2)

    switch(integrityCode)
    {
    case 0:
        strcpy(integrityMessage, "Normal results");
        break;
    case 1:
        strcpy(integrityMessage, "No results available");
        break;
    case 2:
        strcpy(integrityMessage, "Measurement timeout");
        break;
    case 3:
        strcpy(integrityMessage, "Hardware is not installed");
        break;
    case 4:
        strcpy(integrityMessage, "Hardware error");
        break;
    case 5:
        strcpy(integrityMessage, "Over range");
        break;
    case 6:
        strcpy(integrityMessage, "Under range");
        break;
    case 7:
        strcpy(integrityMessage, "Burst short");
        break;
    case 8:
        strcpy(integrityMessage, "Trigger early or fall early");
        break;
    case 9:
        strcpy(integrityMessage, "Trigger late or rise late");
        break;
    case 10:
        strcpy(integrityMessage, "Signal too noisy");
        break;
    case 11:
        strcpy(integrityMessage, "Sync not found");
        break;
    case 12:
        strcpy(integrityMessage, "Oven out of range");
        break;
    case 13:
        strcpy(integrityMessage, "Unidentified error");
        break;
    case 14:
        strcpy(integrityMessage, "PCM full scale warning");
        break;
    case 15:
        strcpy(integrityMessage, "Questionable result for PGSM");
        break;
    case 16:
        strcpy(integrityMessage, "Questionable result due to channel mode");
        break;
    case 17:
        strcpy(integrityMessage, "Can not correlate");
        break;
    case 18:
        strcpy(integrityMessage, "Frequency out of range");
        break;
    case 19:
        strcpy(integrityMessage, "Uncalibrated due to temperature");
        break;
    case 20:
        strcpy(integrityMessage, "Potential RX saturation");
        break;
    case 21:
        strcpy(integrityMessage, "Parameter error");
        break;
    case 22:
        strcpy(integrityMessage, "Unsupported configuration");
        break;
    case 23:
        strcpy(integrityMessage, "Call processing operation failed");
        break;
    default: /* Should never get here unless the range check is changed and */
        /* a new case is not added */
        return statusUpdate(instrumentHandle, thisPtr, VI_ERROR_PARAMETER2);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);


}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_measurementTimeout(
    ViSession  instrumentHandle,
    ViInt16    timeoutMeasFcn,
    ViReal64   timeoutSec,
    ViBoolean  timeoutOnOff)
/*****************************************************************************/
/*  This function will set the measurement timeout value and state for the   */
/*  desired measurement function(s).                                         */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    timeoutMeasFcn (in)                                         */
/*      The measurement function to set the timeout for.  The available      */
/*      measurement functions are:                                           */
/*      AAUD   = 0  Analog Audio Meas Fcn       (age1960_AAUDIO)             */
/*      BERR   = 1  Bit Error Rate Meas Fcn     (age1960_BER)                */
/*      DAUD   = 2  Decoded Audio Meas Fcn      (age1960_DAUDIO)             */
/*      FBER   = 3  Fast Bit Error Meas Fcn     (age1960_FBER)               */
/*      ORFS   = 4  ORFSpectrum Meas Fcn        (age1960_ORFS)               */
/*      PFER   = 5  Phase Freq Error Meas Fcn   (age1960_PFER)               */
/*      PVT    = 6  Power vs Time Meas Fcn      (age1960_PVTIME)             */
/*      TXP    = 7  TX Power Meas Fcn           (age1960_TXPOWER)            */
/*      DPOW   = 8  Dynamic Power function 		(age1960_DPOWER)			 */
/*      IQT	   = 9  IQ Tuning function 			(age1960_IQTUNING)			 */
/*    ViReal64   timeoutSec (in)                                             */
/*      The measurement timeout value to set in second.                      */
/*    ViBoolean  timeoutOnOff (in)                                           */
/*      The measurement timeout state.                                       */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_measurementTimeout",",%hd,%lg,%hu",
                  timeoutMeasFcn, timeoutSec, timeoutOnOff);

    CHK_ENUM(timeoutMeasFcn, age1960_NUMBER_OF_MEAS_FCNS-1, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(timeoutSec, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER3)

    CHK_BOOLEAN(timeoutOnOff, VI_ERROR_PARAMETER4);

    if((errStatus = delayVPrintf(instrumentHandle, "SET:%s:TIM:TIME %lg;STAT %hu\n",
                                 age1960_measFcns_a[timeoutMeasFcn],
                                 timeoutSec,
                                 timeoutOnOff)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confTXPower(
    ViSession  instrumentHandle,
    ViInt32    TXPowerMeasCount,
    ViInt16    TXPowerTrigSource,
    ViReal64   TXPowerTrigDelay,
    ViBoolean  TXPowerTrigQual,
    ViReal64   TXPowerMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:TXPower for the given parameters.     */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the TXPowerMeasCount */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    TXPowerMeasCount (in)                                       */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off                        */
/*    ViInt16    TXPowerTrigSource (in)                                      */
/*      The type of trigger source.  The available trigger sources are:      */
/*       0     Auto trigger source              (age1960_TRIG_AUTO)          */
/*       1     Protocol trigger source          (age1960_TRIG_PROT)          */
/*       2     RF Rise trigger source           (age1960_TRIG_RF_RISE)       */
/*       3     Immediate trigger source         (age1960_TRIG_IMMEDIATE      */
/*     ViReal64  TXPowerTrigDelay  (in)                                      */
/*       The trigger delay time in seconds.                                  */
/*     ViBoolean TXPowerTrigQual  (in)                                       */
/*       Set the trigger qualifier to On or Off.                             */
/*     ViReal64   TXPowerMaxTime  (in)                                       */
/*       The maximum time in seconds that the instrument will wait for       */
/*        the measurement to complete.                                       */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confTXPower",",%ld,%hd,%lg,%hu,%lg",
                  TXPowerMeasCount, TXPowerTrigSource, TXPowerTrigDelay,
                  TXPowerTrigQual, TXPowerMaxTime);

    CHK_LONG_RANGE(TXPowerMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_ENUM(TXPowerTrigSource, 3, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(TXPowerTrigDelay, age1960_TRIG_DELAY_MIN,
                   age1960_TRIG_DELAY_MAX, VI_ERROR_PARAMETER4)

    CHK_BOOLEAN(TXPowerTrigQual, VI_ERROR_PARAMETER5)

    CHK_REAL_RANGE(TXPowerMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER6)

    if (TXPowerMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:TXP:COUN:STAT 0"
                                     ";:SET:TXP:CONT 0;TIM %lg;TRIG:SOUR %s"
                                     ";DEL %lg;QUAL %hu\n",
                                     TXPowerMaxTime,
                                     age1960_trigSource_a[TXPowerTrigSource],
                                     TXPowerTrigDelay,
                                     TXPowerTrigQual)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:TXP:COUN:SNUM %ld"
                                     ";:SET:TXP:CONT 0;TIM %lg"
                                     ";TRIG:SOUR %s;DEL %lg;QUAL %hu\n",
                                     TXPowerMeasCount,
                                     TXPowerMaxTime,
                                     age1960_trigSource_a[TXPowerTrigSource],
                                     TXPowerTrigDelay,
                                     TXPowerTrigQual)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confDynPower(
    ViSession  instrumentHandle,
    ViInt32    DynPowerMeasCount,
    ViReal64   DynPowerExpMaxDiff,
    ViReal64   DynPowerMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:DPOWer for the given parameters.      */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the DynPowerMeasCount*/
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    DynPowerMeasCount (in)                                      */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off                        */
/*     ViReal64  DynPowerExpMaxDiff (in)                                     */
/*       The expeceted max difference between bursts in dB's                 */
/*     ViReal64   DynPowerMaxTime  (in)                                      */
/*       The maximum time in seconds that the instrument will wait for       */
/*        the measurement to complete.                                       */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confDynPower",",%ld,%lg,%lg",
                  DynPowerMeasCount, DynPowerExpMaxDiff, DynPowerMaxTime);

    CHK_LONG_RANGE(DynPowerMeasCount, age1960_DPOW_COUNT_MIN,
                   age1960_DPOW_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(DynPowerExpMaxDiff, age1960_EXP_DIFF_MIN,
                   age1960_EXP_DIFF_MAX, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(DynPowerMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER4)

    if((errStatus = delayVPrintf(instrumentHandle, "SET:DPOW:CONT 0"
                                 ";:SET:DPOW:EMD%s %lg;:SET:DPOW:TIM%s %lg"
                                 ";:SET:DPOW:COUN:NUMB%s %ld\n",
                                 age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                 DynPowerExpMaxDiff,
                                 age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                 DynPowerMaxTime,
                                 age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                 DynPowerMeasCount)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confPowerVsTime(
    ViSession  instrumentHandle,
    ViInt32    pvsTimeMeasCount,
    ViInt16    pvsTimeTrigSource,
    ViReal64   pvsTimeTrigDelay,
    ViInt16    pvsTimeSync,
    ViInt32    pvsTimePoints,
    ViReal64   pvsTimeOffsets[],
    ViReal64   pvsTimeMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:PVTime for the given parameters.      */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the pvsTimeMeasCount */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    pvsTimeMeasCount (in)                                       */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off                        */
/*    ViInt16    pvsTimeTrigSource (in)                                      */
/*      The type of trigger source.  The available trigger sources are:      */
/*       0     Auto trigger source              (age1960_TRIG_AUTO)          */
/*       1     Protocol trigger source          (age1960_TRIG_PROT)          */
/*       2     RF Rise trigger source           (age1960_TRIG_RF_RISE)       */
/*       3     Immediate trigger source         (age1960_TRIG_IMMEDIATE      */
/*     ViReal64  pvsTimeTrigDelay  (in)                                      */
/*       The trigger delay time in seconds.                                  */
/*     ViInt16   pvsTimeSync  (in)                                           */
/*      The type of burst synchronization.  The available burst syncs are:   */
/*       0     Midamble                         (age1960_BSYNC_MID)          */
/*       1     Amplitude                        (age1960_BSYNC_AMPL)         */
/*       2     None                             (age1960_BSYNC_NONE)         */
/*     ViInt32    pvsTimePoints (in)                                         */
/*      The number offset points to be set that are contained in the array   */
/*      pvsTimeOffsets[].                                                    */
/*     ViReal64   pvsTimeOffsets[] (in)                                      */
/*      The array of power vs time offset points to be test in seconds.      */
/*     ViReal64   TXPowerMaxTime  (in)                                       */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confPowerVsTime",",%ld,%hd,%lg,%hd,%ld,%lg,%lg",
                  pvsTimeMeasCount, pvsTimeTrigSource, pvsTimeTrigDelay,
                  pvsTimeSync, pvsTimePoints, pvsTimeOffsets[0], pvsTimeMaxTime);

    CHK_LONG_RANGE(pvsTimeMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_ENUM(pvsTimeTrigSource, 3, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(pvsTimeTrigDelay, age1960_TRIG_DELAY_MIN,
                   age1960_TRIG_DELAY_MAX, VI_ERROR_PARAMETER4)

    CHK_ENUM(pvsTimeSync, 2, VI_ERROR_PARAMETER5)

    CHK_LONG_RANGE(pvsTimePoints, age1960_PVT_POINTS_MIN,
                   age1960_PVT_POINTS_MAX, VI_ERROR_PARAMETER6)

    if (pvsTimePoints>0)
    {
        for (i=0; i < pvsTimePoints; i++)
        {
            CHK_REAL_RANGE(pvsTimeOffsets[i], age1960_PVT_OFFSET_MIN,
                           age1960_PVT_OFFSET_MAX, VI_ERROR_PARAMETER7)
        }
    }

    CHK_REAL_RANGE(pvsTimeMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER8)

    if (pvsTimeMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:PVT:COUN:STAT 0"
                                     ";:SET:PVT:CONT 0;BSYN %s;TIM %lg;TRIG:SOUR %s"
                                     ";DEL %lg\n",
                                     age1960_burstSync_a[pvsTimeSync],
                                     pvsTimeMaxTime,
                                     age1960_trigSource_a[pvsTimeTrigSource],
                                     pvsTimeTrigDelay)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:PVT:COUN:SNUM %ld"
                                     ";:SET:PVT:BSYN %s"
                                     ";CONT 0;TIM %lg;TRIG:SOUR %s;DEL %lg\n",
                                     pvsTimeMeasCount,
                                     age1960_burstSync_a[pvsTimeSync],
                                     pvsTimeMaxTime,
                                     age1960_trigSource_a[pvsTimeTrigSource],
                                     pvsTimeTrigDelay)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (pvsTimePoints == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:PVT:TIME\n")) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus =  age1960_cmdRealArr(instrumentHandle,"SET:PVT:TIME ",
                                            pvsTimePoints, pvsTimeOffsets)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confPFER(
    ViSession  instrumentHandle,
    ViInt32    PFERMeasCount,
    ViInt16    PFERTrigSource,
    ViReal64   PFERTrigDelay,
    ViBoolean  PFERTrigQual,
    ViInt16    PFERSync,
    ViReal64   PFERMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:PFERror for the given parameters.     */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the PFERMeasCount    */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    PFERMeasCount (in)                                          */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off                        */
/*    ViInt16    PFERTrigSource (in)                                         */
/*      The type of trigger source.  The available trigger sources are:      */
/*       0     Auto trigger source              (age1960_TRIG_AUTO)          */
/*       1     Protocol trigger source          (age1960_TRIG_PROT)          */
/*       2     RF Rise trigger source           (age1960_TRIG_RF_RISE)       */
/*       3     Immediate trigger source         (age1960_TRIG_IMMEDIATE      */
/*     ViReal64  PFERTrigDelay  (in)                                         */
/*       The trigger delay time in seconds.                                  */
/*     ViBoolean PFERTrigQual  (in)                                          */
/*       Set the trigger qualifier to On or Off.                             */
/*     ViInt16   PFERSync  (in)                                              */
/*      The type of burst synchronization.  The available burst syncs are:   */
/*       0     Midamble                         (age1960_BSYNC_MID)          */
/*       1     Amplitude                        (age1960_BSYNC_AMPL)         */
/*       2     None                             (age1960_BSYNC_NONE)         */
/*     ViReal64   TXPowerMaxTime  (in)                                       */
/*       The maximum time in seconds that the instrument will wait for       */
/*        the measurement to complete.                                       */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confPFER",",%ld,%hd,%lg,%hu,%hd,%lg",
                  PFERMeasCount, PFERTrigSource, PFERTrigDelay, PFERTrigQual,
                  PFERSync, PFERMaxTime);

    CHK_LONG_RANGE(PFERMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_ENUM(PFERTrigSource, 3, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(PFERTrigDelay, age1960_TRIG_DELAY_MIN,
                   age1960_TRIG_DELAY_MAX, VI_ERROR_PARAMETER4)

    CHK_BOOLEAN(PFERTrigQual, VI_ERROR_PARAMETER5)

    CHK_ENUM(PFERSync, 2, VI_ERROR_PARAMETER6)

    CHK_REAL_RANGE(PFERMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER7)

    if (PFERMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:PFER:COUN:STAT 0"
                                     ";:SET:PFER:CONT 0;TIM %lg;BSYN %s;TRIG:SOUR %s"
                                     ";DEL %lg;QUAL %hu\n",
                                     PFERMaxTime,
                                     age1960_burstSync_a[PFERSync],
                                     age1960_trigSource_a[PFERTrigSource],
                                     PFERTrigDelay,
                                     PFERTrigQual)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:PFER:COUN:SNUM %ld"
                                     ";:SET:PFER:CONT 0;TIM %lg"
                                     ";BSYN %s;TRIG:SOUR %s;DEL %lg;QUAL %hu\n",
                                     PFERMeasCount,
                                     PFERMaxTime,
                                     age1960_burstSync_a[PFERSync],
                                     age1960_trigSource_a[PFERTrigSource],
                                     PFERTrigDelay,
                                     PFERTrigQual)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confORFS(
    ViSession  instrumentHandle,
    ViInt32    ORFSSwMeasCount,
    ViInt32    ORFSModMeasCount,
    ViInt16    ORFSTrigSource,
    ViReal64   ORFSTrigDelay,
    ViInt32    ORFSSwPoints,
    ViReal64   ORFSSwFreqs[],
    ViInt32    ORFSModPoints,
    ViReal64   ORFSModFreqs[],
    ViReal64   ORFSMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:ORFSpectrum for the given parameters. */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the ORFSModMeasCount */
/*  value or the ORFSModMeasCount is >0 if they are both 0 this state will   */
/*  be set of OFF.  If only one of the count values is 0 an error message    */
/*  will be generated.                                                       */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    ORFSSwMeasCount (in)                                        */
/*      The number of multi measurements to be made for switching.  If == 0  */
/*      then the multi measurement function will be turned off               */
/*    ViInt32    ORFSModMeasCount (in)                                       */
/*      The number of multi measurements to be made for modulation.  If == 0 */
/*      then the multi measurement function will be turned off               */
/*    ViInt16    ORFSTrigSource (in)                                         */
/*      The type of trigger source.  The available trigger sources are:      */
/*       0     Auto trigger source              (age1960_TRIG_AUTO)          */
/*       1     Protocol trigger source          (age1960_TRIG_PROT)          */
/*       2     RF Rise trigger source           (age1960_TRIG_RF_RISE)       */
/*       3     Immediate trigger source         (age1960_TRIG_IMMEDIATE      */
/*     ViReal64   ORFSTrigDelay  (in)                                        */
/*       The trigger delay time in seconds.                                  */
/*     ViInt32    ORFSSwPoints (in)                                          */
/*      The number offset points to be set that are contained in the array   */
/*      ORFSSwFreqs[].                                                       */
/*     ViReal64   ORFSSwFreqs[] (in)                                         */
/*      The array of ORFS frequency points to be test for switching.         */
/*     ViInt32    ORFSModPoints (in)                                         */
/*      The number offset points to be set that are contained in the array   */
/*      ORFSModFreqs[].                                                      */
/*     ViReal64   ORFSModFreqs[] (in)                                        */
/*      The array of ORFS frequency points to be test for modulation.        */
/*     ViReal64   ORFSMaxTime  (in)                                          */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confPowerVsTime",",%ld,%ld,%hd,%lg,%ld,%lg,%ld,%lg,%lg",
                  ORFSSwMeasCount, ORFSModMeasCount, ORFSTrigSource, ORFSTrigDelay,
                  ORFSSwPoints, ORFSSwFreqs[0], ORFSModPoints, ORFSModFreqs[0],
                  ORFSMaxTime);

    CHK_LONG_RANGE(ORFSSwMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(ORFSModMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER3)

    CHK_ENUM(ORFSTrigSource, 3, VI_ERROR_PARAMETER4)

    CHK_REAL_RANGE(ORFSTrigDelay, age1960_TRIG_DELAY_MIN,
                   age1960_TRIG_DELAY_MAX, VI_ERROR_PARAMETER5)

    CHK_LONG_RANGE(ORFSSwPoints, age1960_ORFSSW_POINTS_MIN,
                   age1960_ORFSSW_POINTS_MAX, VI_ERROR_PARAMETER6)

    if (ORFSSwPoints>0)
    {
        for (i=0; i < ORFSSwPoints; i++)
        {
            CHK_REAL_RANGE_2(ORFSSwFreqs[i], age1960_ORFSFREQ_MIN_1,
                             age1960_ORFSFREQ_MAX_1, age1960_ORFSFREQ_MIN_2,
                             age1960_ORFSFREQ_MAX_2, VI_ERROR_PARAMETER7)
        }
    }

    CHK_LONG_RANGE(ORFSModPoints, age1960_ORFSMOD_POINTS_MIN,
                   age1960_ORFSMOD_POINTS_MAX, VI_ERROR_PARAMETER8)

    if (ORFSModPoints>0)
    {
        for (i=0; i < ORFSModPoints; i++)
        {
            CHK_REAL_RANGE_2(ORFSModFreqs[i], age1960_ORFSFREQ_MIN_1,
                             age1960_ORFSFREQ_MAX_1, age1960_ORFSFREQ_MIN_2,
                             age1960_ORFSFREQ_MAX_2, age1960_INSTR_ERROR_PARAMETER9)
        }
    }

    CHK_REAL_RANGE(ORFSMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, age1960_INSTR_ERROR_PARAMETER10)

    /* Check to see if SW or MOD count is zero and the other is not this */
    /* will be a error condition. */

    if ( (ORFSSwMeasCount == 0 && ORFSModMeasCount != 0) ||
            (ORFSModMeasCount == 0 && ORFSSwMeasCount != 0))
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_ORFS_MEAS_COUNT);

    if (ORFSSwMeasCount == 0 && ORFSModMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:ORFS:COUN:STAT 0"
                                     ";:SET:ORFS:CONT 0;TIM %lg;TRIG:SOUR %s"
                                     ";DEL %lg\n",
                                     ORFSMaxTime,
                                     age1960_trigSource_a[ORFSTrigSource],
                                     ORFSTrigDelay)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:ORFS:MOD:COUN:SNUM %ld"
                                     ";:SET:ORFS:SWIT:COUN:SNUM %ld"
                                     ";:SET:ORFS:CONT 0;TIM %lg;TRIG:SOUR %s;DEL %lg\n",
                                     ORFSModMeasCount,
                                     ORFSSwMeasCount,
                                     ORFSMaxTime,
                                     age1960_trigSource_a[ORFSTrigSource],
                                     ORFSTrigDelay)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (ORFSSwPoints == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:ORFS:SWIT:FREQ\n")) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus =  age1960_cmdRealArr(instrumentHandle,"SET:ORFS:SWIT:FREQ ",
                                            ORFSSwPoints, ORFSSwFreqs)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (ORFSModPoints == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:ORFS:MOD:FREQ\n")) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus =  age1960_cmdRealArr(instrumentHandle,"SET:ORFS:MOD:FREQ ",
                                            ORFSModPoints, ORFSModFreqs)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confFastBER(
    ViSession  instrumentHandle,
    ViInt32    FBERMeasCount,
    ViInt32    FBERManualDelay,
    ViBoolean  FBERAutoDelay,
    ViBoolean  FBERSignallingControl,
    ViReal64   FBERHoldoffTime,
    ViReal64   FBERMaxTime)

/*****************************************************************************/
/*  This function will perform a SETup:FBERror for the given parameters.     */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    FBERMeasCount (in)                                          */
/*      The number of bits to be measured.                                   */
/*    ViInt32    FBERManualDelay (in)                                        */
/*      The number of frames to wait for the uplink and downlink to          */
/*      correlate. This value will only have an effect if the FBERAutoDelay  */
/*      is set to false i.e. use manual delay.                               */
/*    ViBoolean  FBERAutoDelay  (in)                                         */
/*      Selects whether or not to automatically detect the loopback delay    */
/*      or to use the user specified delay.                                  */
/*    ViBoolean  FBERSignallingControl (in)                                  */
/*      Selects whether or not to have the signalling open and close the     */
/*      loopback before and after sending signalling commands to the mobile  */
/*    ViReal64   FBERHoldoffTime  (in)                                       */
/*      The time in seconds that the instrument will wait after the loopback */
/*      has changed before starting the FBER measurement. Zero (0) will      */
/*      turn this feature off.                                               */
/*    ViReal64   FBERMaxTime  (in)                                           */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus   errStatus ;
    ViBoolean  HoldoffState = VI_ON;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confFastBER",",%ld,%ld,%hu,%hu,%lg,%lg",
                  FBERMeasCount, FBERManualDelay, FBERAutoDelay,
                  FBERSignallingControl, FBERHoldoffTime, FBERMaxTime);

    CHK_LONG_RANGE(FBERMeasCount, age1960_FBER_COUNT_MIN,
                   age1960_FBER_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(FBERManualDelay, age1960_FBER_DELAY_MIN,
                   age1960_FBER_DELAY_MAX, VI_ERROR_PARAMETER3)

    CHK_BOOLEAN(FBERAutoDelay, VI_ERROR_PARAMETER4)

    CHK_BOOLEAN(FBERSignallingControl, VI_ERROR_PARAMETER5)

    CHK_REAL_RANGE(FBERHoldoffTime, age1960_HOLDOFF_TIME_MIN,
                   age1960_HOLDOFF_TIME_MAX, VI_ERROR_PARAMETER6)

    CHK_REAL_RANGE(FBERMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER7)

    if (FBERHoldoffTime == 0) HoldoffState = VI_OFF;

    if((errStatus = delayVPrintf(instrumentHandle, "SET:FBER:COUN %ld;CONT 0;TIM %lg"
                                 ";CLSD %lg;SLC %hu;MAN:DEL %ld;:SET:FBER:LDC:AUTO %hu"
                                 ";:SET:FBER:CLSD:STAT %hu\n",
                                 FBERMeasCount,
                                 FBERMaxTime,
                                 FBERHoldoffTime,
                                 FBERSignallingControl,
                                 FBERManualDelay,
                                 FBERAutoDelay,
                                 HoldoffState)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confBER(
    ViSession  instrumentHandle,
    ViInt32    BERMeasCount,
    ViInt16    BERMeasType,
    ViInt32    BERManualDelay,
    ViBoolean  BERAutoDelay,
    ViBoolean  BERSignallingControl,
    ViReal64   BERHoldoffTime,
    ViReal64   BERMaxTime)

/*****************************************************************************/
/*  This function will perform a SETup:BERRor for the given parameters.      */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    BERMeasCount (in)                                           */
/*      The number of bits to be measured.                                   */
/*    ViInt16    BERMeasType (in)                                            */
/*      The type of bit error rate measurement to be made.  The available    */
/*      types are:                                                           */
/*          age1960_BER_TYPE_IA        0    Type IA BER Measurement          */
/*          age1960_BER_TYPE_II        1    Type II BER Measurement          */
/*          age1960_BER_TYPE_IB        2    Type IB BER Measurement          */
/*          age1960_BER_RES_TYPE_IA    3    Residual Type IA BER Measurement */
/*          age1960_BER_RES_TYPE_II    4    Residual Type II BER Measurement */
/*          age1960_BER_RES_TYPE_IB    5    Residual Type IB BER Measurement */
/*    ViInt32    BERManualDelay (in)                                         */
/*      The number of frames to wait for the uplink and downlink to          */
/*      correlate. This value will only have an effect if the BERAutoDelay   */
/*      is set to false i.e. use manual delay.                               */
/*    ViBoolean  BERAutoDelay  (in)                                          */
/*      Selects whether or not to automatically detect the loopback delay    */
/*      or to use the user specified delay.                                  */
/*    ViBoolean  BERSignallingControl (in)                                   */
/*      Selects whether or not to have the signalling open and close the     */
/*      loopback before and after sending signalling commands to the mobile  */
/*    ViReal64   BERHoldoffTime  (in)                                        */
/*      The time in seconds that the instrument will wait after the loopback */
/*      has changed before starting the BER measurement. Zero (0) will       */
/*      turn this feature off.                                               */
/*    ViReal64   BERMaxTime  (in)                                            */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus   errStatus ;
    ViBoolean  HoldoffState = VI_ON;
    struct     age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confBER",",%ld,%hd,%ld,%hu,%hu,%lg,%lg",
                  BERMeasCount, BERMeasType, BERManualDelay,
                  BERAutoDelay, BERSignallingControl,
                  BERHoldoffTime, BERMaxTime);

    CHK_LONG_RANGE(BERMeasCount, age1960_BER_COUNT_MIN,
                   age1960_BER_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_ENUM(BERMeasType, 5, VI_ERROR_PARAMETER3)

    CHK_LONG_RANGE(BERManualDelay, age1960_BER_DELAY_MIN,
                   age1960_BER_DELAY_MAX, VI_ERROR_PARAMETER4)

    CHK_BOOLEAN(BERAutoDelay, VI_ERROR_PARAMETER5)

    CHK_BOOLEAN(BERSignallingControl, VI_ERROR_PARAMETER6)

    CHK_REAL_RANGE(BERHoldoffTime, age1960_HOLDOFF_TIME_MIN,
                   age1960_HOLDOFF_TIME_MAX, VI_ERROR_PARAMETER7)

    CHK_REAL_RANGE(BERMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER8)

    if (BERHoldoffTime == 0) HoldoffState = VI_OFF;

    if((errStatus = delayVPrintf(instrumentHandle, "SET:BERR:COUN %ld;TYPE %s;CONT 0;TIM %lg"
                                 ";CLSD %lg;SLC %hu;MAN:DEL %ld;:SET:BERR:LDC:AUTO %hu"
                                 ";:SET:BERR:CLSD:STAT %hu\n",
                                 BERMeasCount,
                                 age1960_BERRType_a[BERMeasType],
                                 BERMaxTime,
                                 BERHoldoffTime,
                                 BERSignallingControl,
                                 BERManualDelay,
                                 BERAutoDelay,
                                 HoldoffState)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confAAudio(
    ViSession  instrumentHandle,
    ViInt32    AAudioMeasCount,
    ViReal64   AAudioExpVolt,
    ViReal64   AAudioFiltFreq,
    ViBoolean  AAudioFiltState,
    ViReal64   AAudioMaxTime)

/*****************************************************************************/
/*  This function will perform a SETup:AAUDio for the given parameters.      */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the AAudioMeasCount  */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  This function supports earlier versions of the E1960A TA. Greater		 */
/*  functionality can be achieved using the age1960_confAFAN() function if	 */
/*  your version of the E1960A TA will support it (A.07xx or later).		 */
/*																			 */
/*	PARAMETERS:
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    AAudioMeasCount (in)                                        */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off.                       */
/*    ViReal64   AAudioExpVolt (in)                                          */
/*      The expected voltage level for the signal in volts peak.             */
/*    ViReal64   AAudioFiltFreq  (in)                                        */
/*      The desired bandpass filter frequency in Hz.                         */
/*    ViBoolean  AAudioFiltState  (in)                                       */
/*      Set the bandpass filter to On or Off.                                */
/*    ViReal64   AAudioMaxTime  (in)                                         */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confAAudio",",%ld,%lg,%lg,%hu,%lg",
                  AAudioMeasCount, AAudioExpVolt, AAudioFiltFreq,
                  AAudioFiltState, AAudioMaxTime);

    CHK_LONG_RANGE(AAudioMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(AAudioExpVolt, age1960_EXP_VOLT_MIN,
                   age1960_EXP_VOLT_MAX, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(AAudioFiltFreq, age1960_AAUDFLT_FREQ_MIN,
                   age1960_AAUDFLT_FREQ_MAX, VI_ERROR_PARAMETER4)

    CHK_BOOLEAN(AAudioFiltState, VI_ERROR_PARAMETER5)

    CHK_REAL_RANGE(AAudioMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER6)

    if (AAudioMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:AAUD:COUN:STAT 0"
                                     ";:SET:AAUD:CONT 0;TIM %lg"
                                     ";:SET:AAUD:FILT:FREQ %lg;STAT %hu"
                                     ";:SET:AAUD:EXP:VOLT %lg\n",
                                     AAudioMaxTime,
                                     AAudioFiltFreq,
                                     AAudioFiltState,
                                     AAudioExpVolt)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:AAUD:COUN:SNUM %ld"
                                     ";:SET:AAUD:CONT 0;TIM %lg"
                                     ";:SET:AAUD:FILT:FREQ %lg;STAT %hu"
                                     ";:SET:AAUD:EXP:VOLT %lg\n",
                                     AAudioMeasCount,
                                     AAudioMaxTime,
                                     AAudioFiltFreq,
                                     AAudioFiltState,
                                     AAudioExpVolt)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confAFAN(
    ViSession  instrumentHandle,
    ViInt32    AFANMeasCount,
    ViBoolean  AFANSinadDist,
    ViReal64   AFANFundFreq,
    ViInt16    AFANFilterType,
    ViReal64   AFANBPFCntFreq,
    ViInt16    AFANDetType,
    ViReal64   AFANPeakVolt,
    ViBoolean  AFANDeemphasis,
    ViReal64   AFANExpandorLevel,
    ViReal64   AFANMaxTime)
/*****************************************************************************/
/*  This function will perform a SETup:AAUDio for the given parameters.		 */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the AFANMeasCount    */
/*  value is >0 otherwise this state will be set to OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*																			 */
/*  Note: This function adds functionality that was made available in		 */
/*  version A.07.xx of the E1960A TA. If backward compatibility is required, */
/*  use the age1960_confAAudio() function.									 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    AFANMeasCount (in)                                          */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off.                       */
/*    ViBoolean  AFANSinadDist  (in)                                         */
/*      Set the SINAD and distortion state to On or Off.                     */
/*    ViReal64   AFANFundFreq (in)                                           */
/*      The Fundamental frequency for the SINAD and Distortion measurement.  */
/*    ViInt16  AFANFilterType (in)                                           */
/*      The Filter type to be used.  The available fileter types are:        */
/*       0     None                             (age1960_FILTER_NONE)        */
/*       1     Tunable Band Pass                (age1960_FILTER_BANDPASS)    */
/*       2     C-Message                        (age1960_FILTER_CMESSAGE)    */
/*       3     Band Pass 50-15kHz               (age1960_FILTER_50_15KHZ)    */
/*       4     Band Pass 300-15kHz              (age1960_FILTER_300_15KHZ)   */
/*    ViReal64   AFANBPFCntFreq (in)                                         */
/*      The center frequency for the tunable bandpass filter if it is on.    */
/*    ViInt16  AFANDetType (in)                                              */
/*      The Detector type to be used.  The available detector types are:     */
/*       0     RMS                             (age1960_DET_RMS)             */
/*       1     Positive PEAK                   (age1960_DET_PPEAK)           */
/*    ViReal64   AFANPeakVolt (in)                                           */
/*      The expected peak voltage.                                           */
/*    ViBoolean  AFANDeemphasis  (in)                                        */
/*      Set the de-emphasis state to On or Off.                              */
/*    ViReal64   AFANExpandorLevel (in)                                      */
/*      The OFF will be set via a level of 0.  Other levels will set the     */
/*      desired level.                                                       */
/*    ViReal64   AFANMaxTime  (in)                                           */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;
    ViInt16   ExpState = 1;
    ViInt16   CountState = 1;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confAFAN",",%ld,%hu,%lg,%hd,%lg,%hd,%lg,%hu,%lg,%lg",
                  AFANMeasCount, AFANSinadDist, AFANFundFreq, AFANFilterType,
                  AFANBPFCntFreq, AFANDetType, AFANPeakVolt, AFANDeemphasis,
                  AFANExpandorLevel, AFANMaxTime);

    CHK_LONG_RANGE(AFANMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_BOOLEAN(AFANSinadDist, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(AFANFundFreq, age1960_FUND_FREQ_MIN,
                   age1960_FUND_FREQ_MAX, VI_ERROR_PARAMETER4)

    CHK_ENUM(AFANFilterType, 4, VI_ERROR_PARAMETER5)

    CHK_REAL_RANGE(AFANBPFCntFreq, age1960_BPF_FREQ_MIN,
                   age1960_BPF_AFAN_FREQ_MAX, VI_ERROR_PARAMETER6)

    CHK_ENUM(AFANDetType, 1, VI_ERROR_PARAMETER7)

    CHK_REAL_RANGE(AFANPeakVolt, age1960_EXP_VOLT_MIN,
                   age1960_EXP_VOLT_MAX, VI_ERROR_PARAMETER8)

    CHK_BOOLEAN(AFANDeemphasis, age1960_INSTR_ERROR_PARAMETER9)

    CHK_REAL_RANGE(AFANExpandorLevel, age1960_EXPANDOR_LVL_MIN,
                   age1960_EXPANDOR_LVL_MAX, age1960_INSTR_ERROR_PARAMETER10)

    CHK_REAL_RANGE(AFANMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, age1960_INSTR_ERROR_PARAMETER11)

    if (AFANMeasCount == 0)
    {
        CountState = 0;
        AFANMeasCount = 1;
    }

    if (AFANExpandorLevel == 0)
    {
        ExpState = 0;
        AFANExpandorLevel = 1;
    }

    if((errStatus = delayVPrintf(instrumentHandle, "SET:AAUD:CONT 0;:SET:AAUD:COUN:STAT 0"
                                 ";:SET:AAUD:TIM %lg;COUN:STAT %hd;NUMB %ld"
                                 ";:SET:AAUD:SDIS:STAT %hu;FREQ %lg"
                                 ";:SET:AAUD:FILT:TYPE %s;FREQ %lg"
                                 ";:SET:AAUD:DET %s;EXPANDOR:SRL %lg;STAT %hd"
                                 ";:SET:AAUD:EXP:VOLT:PEAK %lg"
                                 ";:SET:AAUD:DEMP:STAT %hu"
                                 ";:SET:AAUD:FREQ:STAT ON\n",
                                 AFANMaxTime, CountState, AFANMeasCount,
                                 AFANSinadDist, AFANFundFreq,
                                 age1960_filter_a[AFANFilterType], AFANBPFCntFreq,
                                 age1960_AFAN_Detector_a[AFANDetType], AFANExpandorLevel, ExpState,
                                 AFANPeakVolt,
                                 AFANDeemphasis )) < VI_SUCCESS ) return statusUpdate(instrumentHandle,thisPtr,errStatus);


    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confDAudio(
    ViSession  instrumentHandle,
    ViInt32    DAudioMeasCount,
    ViReal64   DAudioFiltFreq,
    ViBoolean  DAudioFiltState,
    ViReal64   DAudioMaxTime)

/*****************************************************************************/
/*  This function will perform a SETup:DAUDio for the given parameters.      */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the DAudioMeasCount  */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    DAudioMeasCount (in)                                        */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off.                       */
/*    ViReal64   DAudioFiltFreq  (in)                                        */
/*      The desired bandpass filter frequency in Hz.                         */
/*    ViBoolean  DAudioFiltState  (in)                                       */
/*      Set the bandpass filter to On or Off.                                */
/*    ViReal64   DAudioMaxTime  (in)                                         */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confDAudio",",%ld,%lg,%hu,%lg",
                  DAudioMeasCount, DAudioFiltFreq,
                  DAudioFiltState, DAudioMaxTime);

    CHK_LONG_RANGE(DAudioMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(DAudioFiltFreq, age1960_DAUDFLT_FREQ_MIN,
                   age1960_DAUDFLT_FREQ_MAX, VI_ERROR_PARAMETER3)

    CHK_BOOLEAN(DAudioFiltState, VI_ERROR_PARAMETER4)

    CHK_REAL_RANGE(DAudioMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER5)

    if (DAudioMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:DAUD:COUN:STAT 0"
                                     ";:SET:DAUD:CONT 0;TIM %lg"
                                     ";:SET:DAUD:FILT:FREQ %lg;STAT %hu\n",
                                     DAudioMaxTime,
                                     DAudioFiltFreq,
                                     DAudioFiltState)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:DAUD:COUN:SNUM %ld"
                                     ";:SET:DAUD:CONT 0;TIM %lg"
                                     ";:SET:DAUD:FILT:FREQ %lg;STAT %hu\n",
                                     DAudioMeasCount,
                                     DAudioMaxTime,
                                     DAudioFiltFreq,
                                     DAudioFiltState)) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confIQTuning(
    ViSession  instrumentHandle,
    ViInt32    IQTMeasCount,
    ViInt16    IQTTrigSource,
    ViReal64   IQTTrigDelay,
    ViInt16    IQTRefOffsetFrequency,
    ViBoolean  IQTSpurFreqState,
    ViReal64   IQTSpurFrequency,
    ViReal64   IQTMaxTime)

/*****************************************************************************/
/*   This function will perform a SETup:IQTuning for the given parameters.   */
/*  The :CONTinuous state will be set to OFF (single) by this function.      */
/*  The :COUNt:STATe will be set to the ON condition if the IQTMeasCount     */
/*  value is >0 otherwise this state will be set of OFF.                     */
/*  The :TIMeout:STATe will be set to the ON condition.                      */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    IQTMeasCount (in)                                           */
/*      The number of multi measurements to be made.  If == 0 then the       */
/*      multi measurement function will be turned off.                       */
/*    ViInt16    IQTTrigSource  (in)                                         */
/*      The trig source for the measurement. It can be RF Rise of Immediate. */
/*    ViReal64   IQTTrigDelay  (in)                                          */
/*       The trigger delay time in seconds.                                  */
/*    ViInt16    IQTRefOffsetFrequency  (in)                                 */
/*       This sets the frequency all levels are relative to. It can take     */
/*       the enum's "NEG67KHZ", "ZEROKHZ", "POS67KHZ", "AUTO".               */
/*    ViBoolean  IQTSpurFreqState (in)                                       */
/*       This is the toggle to set whither the spur frequency is measured    */
/*       or not. 1 means it is measured.                                     */
/*    ViReal64   IQTSpurFrequency (in)                                       */
/*       This sets the spur frequency.                                       */
/*    ViReal64   IQTMaxTime  (in)                                            */
/*      The maximum time in seconds that the instrument will wait for        */
/*      the measurement to complete.                                         */
/*****************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confIQTuning",",%ld,%hd,%lg,%hd,%hu,%lg,%lg",
                  IQTMeasCount, IQTTrigSource,
                  IQTTrigDelay, IQTRefOffsetFrequency,
                  IQTSpurFreqState, IQTSpurFrequency,
                  IQTMaxTime );

    CHK_LONG_RANGE(IQTMeasCount, age1960_MEAS_COUNT_MIN,
                   age1960_MEAS_COUNT_MAX, VI_ERROR_PARAMETER2)

    CHK_INT_RANGE(IQTTrigSource, age1960_TRIG_RF_RISE, age1960_TRIG_IMMEDIATE, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(IQTTrigDelay, age1960_TRIG_DELAY_MIN,
                   age1960_TRIG_DELAY_MAX, VI_ERROR_PARAMETER4)

    CHK_ENUM(IQTRefOffsetFrequency, 3, VI_ERROR_PARAMETER5)

    CHK_BOOLEAN(IQTSpurFreqState, VI_ERROR_PARAMETER6)

    CHK_REAL_RANGE_2(IQTSpurFrequency, age1960_IQT_SPURFREQ_MIN_1, age1960_IQT_SPURFREQ_MAX_1,
                     age1960_IQT_SPURFREQ_MIN_2, age1960_IQT_SPURFREQ_MAX_2, VI_ERROR_PARAMETER7)

    CHK_REAL_RANGE(IQTMaxTime, age1960_MEAS_TIMEOUT_MIN,
                   age1960_MEAS_TIMEOUT_MAX, VI_ERROR_PARAMETER8)

    if (IQTMeasCount == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:IQT:COUN:STAT%s 0"
                                     ";:SET:IQT:CONT%s 0;:SET:IQT:TIM%s %lg"
                                     ";:SET:IQT:SPUR:FREQ%s %lg;:SET:IQT:SPUR:STAT%s %hu"
                                     ";:SET:IQT:REF:CHAR:GSM %s;:SET:IQT:TRIG:DEL%s %lg"
                                     ";:SET:IQT:TRIG:SOUR%s %s\n",
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     IQTMaxTime,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     IQTSpurFrequency,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     IQTSpurFreqState,
                                     age1960_iqt_reference_a[IQTRefOffsetFrequency],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     IQTTrigDelay,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_trigSource_a[IQTTrigSource])) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle, "SET:IQT:COUN:SNUM%s %ld"
                                     ";:SET:IQT:CONT%s 0;:SET:IQT:TIM%s %lg"
                                     ";:SET:IQT:SPUR:FREQ%s %lg;:SET:IQT:SPUR:STAT%s %hu"
                                     ";:SET:IQT:REF:CHAR:GSM %s;:SET:IQT:TRIG:DELAY%s %lg"
                                     ";:SET:IQT:TRIG:SOUR%s %s\n",
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],IQTMeasCount,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],IQTMaxTime,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex], IQTSpurFrequency,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],IQTSpurFreqState,
                                     age1960_iqt_reference_a[IQTRefOffsetFrequency],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],IQTTrigDelay,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_trigSource_a[IQTTrigSource])) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confAutoRange(
    ViSession  instrumentHandle,
    ViInt16	   measFcnToUse,
    ViInt32    tries)
/*****************************************************************************/
/*  This function will perform a READ:TXPower? and set the expected power    */
/*  setting to the returned TX power value until the integrity indicates     */
/*  no overrange or no underrange conditions.                                */
/*                                                                           */
/*  For the AFAN range this function will set the expected voltage to 2x the */
/*  current reading for an overrange condition and .5 the reading for an     */
/*  underrange condition and repeat this action until the range is correct   */
/*  or the try counter has been passed.                                      */
/*                                                                           */
/*  WARNING!  If the offset values are large, it may cause the receiver      */
/*            to be overranged, resulting in measurements being unavailable  */
/*            or invalid.  Turning on the automatic logging using            */
/*            age1961_errorQueryDetect will alert you to this problem.       */
/*                                                                           */
/*																			 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16   measFcnToUse (in)                                            */
/*      The measurement function to use for autoranging                      */
/*      age1960_AUTORNG_AFANALYZER (AFAN) = 0  Audio Frequency Analyzer Meas */
/*      age1960_AUTORNG_TXPOWERT (TXP)    = 1  TX Power Meas				 */
/*    ViInt32  tries (in)                                                    */
/*      The number of times to try to set the range correctly.               */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus    errStatus ;
    ViInt32     integrity;
    ViReal64    TXPower;
    ViBoolean   triedMax = VI_FALSE;
    ViBoolean   triedMin = VI_FALSE;
    ViBoolean   inRange  = VI_FALSE;
    ViReal64    maxExpected;
    ViReal64    minExpected;
    ViReal64    presentExpPow;
    ViReal64    pathGain;
    ViInt16     pathState;
    ViReal64	  measFreq;
    ViReal64	  downlinkFreq;
    ViReal64	  uplinkFreq;
    ViReal64	  AFANlevel;
    ViReal64	  presentExpLevel;

    int       i;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confAutoRange",",%ld",tries);


    CHK_ENUM(measFcnToUse, 1, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(tries, age1960_AUTORANGE_TRIES_MIN,
                   age1960_AUTORANGE_TRIES_MAX, VI_ERROR_PARAMETER3)

    /* Determine which function to autorange */
    if ( measFcnToUse == age1960_AUTORNG_TXPOWER )
    {


        if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:CORR:STAT?;:RFAN:EXP:POW?",
                                           8000 )) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = viScanf(instrumentHandle, "%hd;%lf%*t",
                                 &pathState, &presentExpPow)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = age1960_getPresentFreq_Q(instrumentHandle, &downlinkFreq, &uplinkFreq, &measFreq)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        if ((errStatus = age1960_getApproxOffset_Q (instrumentHandle, measFreq, &pathGain)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        minExpected = -25.0 - pathGain * (ViReal64)pathState;
        maxExpected = 43.0 - pathGain * (ViReal64)pathState;

        for ( i=1; i <= tries +2; i++ )
        {

            if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:TXP?",
                                               ReadTimeout(instrumentHandle,"TXP") )) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                                     &integrity, &TXPower)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            switch(integrity)
            {
            case 1:  /* No results available */
            case 2:  /* Measurement timeout */
            case 3:  /* Hardware is not installed */
            case 4:  /* Hardware error */
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_AUTORANGE_FAILED);
            case 5:  /* Over range */

                inRange = VI_FALSE;

                if (presentExpPow + 13.0 > maxExpected)
                {
                    if (triedMax == VI_TRUE) return statusUpdate(instrumentHandle, thisPtr,
                                                        age1960_INSTR_ERROR_AUTORANGE_FAILED);
                    presentExpPow =  maxExpected;
                    triedMax = VI_TRUE;
                }
                else
                    presentExpPow = presentExpPow + 13.0;

                if((errStatus = delayVPrintf(instrumentHandle, "RFAN:EXP:POW %lf DBM\n",
                                             presentExpPow)) < VI_SUCCESS )
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);

                break;

            case 6:  /* Under range */

                inRange = VI_FALSE;

                presentExpPow  = TXPower;

                if (presentExpPow > maxExpected)
                {
                    if (triedMax == VI_TRUE) return statusUpdate(instrumentHandle, thisPtr,
                                                        age1960_INSTR_ERROR_AUTORANGE_FAILED);
                    presentExpPow =  maxExpected;
                    triedMax = VI_TRUE;
                }

                if (presentExpPow < minExpected)
                {
                    if (triedMin == VI_TRUE) return statusUpdate(instrumentHandle, thisPtr,
                                                        age1960_INSTR_ERROR_AUTORANGE_FAILED);
                    presentExpPow =  minExpected;
                    triedMin = VI_TRUE;
                }

                if((errStatus = delayVPrintf(instrumentHandle, "RFAN:EXP:POW %lf DBM\n",
                                             presentExpPow)) < VI_SUCCESS )
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);

                break;

            default: /* 0 or the rest of the integrity values are ok for range  */

                if (inRange == VI_TRUE)
                    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

                presentExpPow  = TXPower;

                if (presentExpPow > maxExpected)   presentExpPow =  maxExpected;

                if (presentExpPow < minExpected)   presentExpPow =  minExpected;

                if((errStatus = delayVPrintf(instrumentHandle, "RFAN:EXP:POW %lf DBM\n",
                                             presentExpPow)) < VI_SUCCESS )
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);

                inRange = VI_TRUE;
                doDelay(50000); /* required for HP E1960 display to be updated */

            }
        }
    }
    if ( measFcnToUse == age1960_AUTORNG_AFANALYZER )
    {
        // auto range for AFAN measurement

        if((errStatus = delayVPrintf(instrumentHandle,  "SET:AAUD:EXP:VOLT:PEAK 20.0\n")) < VI_SUCCESS )
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        for ( i=1; i <= tries; i++ )
        {

            if ((errStatus =  sendQueryRequest(instrumentHandle, "INIT:AAUD;:FETCH:AAUD:INT?;VOLT?",
                                               ReadTimeout(instrumentHandle, "AAUD") )) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if ((errStatus = viScanf(instrumentHandle, "%ld;%lf%*t",
                                     &integrity, &AFANlevel)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if(AFANlevel < .001)
            {
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_AUTORANGE_FAILED);
            }

            switch(integrity)
            {
            case 1:  /* No results available */
            case 2:  /* Measurement timeout */
            case 3:  /* Hardware is not installed */
            case 4:  /* Hardware error */
            case 10: /* Signal too noisy */
            case 11: /* Sync not found */
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_AUTORANGE_FAILED);
            case 5:  /* Over range */
            case 6:  /* Under range */

                inRange = VI_FALSE;

                if (AFANlevel * 1.414 > 20.0 )
                {
                    if (triedMax == VI_TRUE) return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_AUTORANGE_FAILED);
                    presentExpLevel =  20.0;
                    triedMax = VI_TRUE;
                }
                else
                {
                    presentExpLevel = AFANlevel * 1.414;
                }
                if((errStatus = delayVPrintf(instrumentHandle,  "SET:AAUD:EXP:VOLT:PEAK %lf\n",
                                             presentExpLevel)) < VI_SUCCESS )
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);

                break;

            default: /* 0 or the rest of the integrity values are ok for range  */

                if (inRange == VI_TRUE)
                {
                    return  VI_SUCCESS;
                }

                presentExpLevel  = AFANlevel * 1.414;

                if (presentExpLevel > 20.0)   presentExpLevel =  20.0;

                if (presentExpLevel < .001)   presentExpLevel =  .001;

                if((errStatus = delayVPrintf(instrumentHandle,  "SET:AAUD:EXP:VOLT:PEAK %lf\n",
                                             presentExpLevel)) < VI_SUCCESS )
                    return statusUpdate(instrumentHandle,thisPtr,errStatus);

                inRange = VI_TRUE;

            }
        }
    }
    return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_AUTORANGE_FAILED);

}        /* ----- end of function ----- */



/*****************************************************************************/
ViStatus _VI_FUNC age1960_waitForConnect(
    ViSession  instrumentHandle,
    ViReal64   connectTimeOut)
/*****************************************************************************/
/*  This function will wait until the instrument indicates that a call is    */
/*  connected.  This function should be use for MS originated calls after    */
/*  age1960_confForMSOriginate has been called.                              */
/*  If the instrument indicates that the call is already CONNECTED this      */
/*  function will return with VI_SUCCESS.  If the instrument indicated that  */
/*  it is not in the proper state for a call this function will              */
/*  return with the errStatus set. After the call has been connected this    */
/*  function will re-check the call status state to verify that it is        */
/*  CONNECTED, if it is not this function will return an error condition.    */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViReal64   connectTimeOut (in)                                         */
/*      Set the maximum time (in seconds) this function will wait for the    */
/*      call to connect.                                                     */
/*****************************************************************************/

{

    ViStatus    errStatus ;
    ViChar      callStatus[10];
    ViChar      mode[5];
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_waitForConnect",",%lg", connectTimeOut);

    CHK_REAL_RANGE(connectTimeOut, age1960_CP_TIMEOUT_MIN,
                   age1960_CP_TIMEOUT_MAX, VI_ERROR_PARAMETER2)

    /* Check if call is already connected or */
    /* not in the correct state to be connected. */

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:STAT?\n", "%s%*t",
                                  callStatus)) < VI_SUCCESS)
    {
        srq_cleanup(instrumentHandle);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (strcmp(callStatus,"CONN") == 0)
    {
        srq_cleanup(instrumentHandle);
        return statusUpdate(instrumentHandle,thisPtr, VI_SUCCESS);
    }

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:OPER:MODE?\n", "%s%*t",
                                  mode)) < VI_SUCCESS)
    {
        srq_cleanup(instrumentHandle);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (strcmp(mode,"CELL") != 0)
    {
        srq_cleanup(instrumentHandle);
        return statusUpdate(instrumentHandle,thisPtr,
                            age1960_INSTR_CANNOT_ORIG_NOW);
    }

    if ((errStatus = waitForInterrupt (instrumentHandle, (long) (connectTimeOut*1000))) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr, age1960_INSTR_ERROR_CALL_FAILED);

    if ((errStatus = delayVQueryf(instrumentHandle, "CALL:STAT?\n", "%s%*t",
                                  callStatus)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (strcmp(callStatus, age1960_completion_a[age1960_CALL_CONNECTED]) != 0)
        return statusUpdate(instrumentHandle,thisPtr,
                            age1960_INSTR_ERROR_CALL_FAILED);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_confForMSOriginate(
    ViSession  instrumentHandle)
/*****************************************************************************/
/*  This function will configure the instrument to wait for a MS originated  */
/*  call.  After this function is call the age1960_waitForConnect function   */
/*  should be use to wait for the call to be connected                       */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*****************************************************************************/

{

    ViStatus    errStatus ;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confForMSOriginate","");

    if ((errStatus = confForInterrupt(instrumentHandle, age1960_WAITFOR_CALL,
                                      age1960_CALL_CONNECTED)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_getPresentFreq_Q(
    ViSession  instrumentHandle,
    ViPReal64  downlinkFreq,
    ViPReal64  uplinkFreq,
    ViPReal64  measurementFreq)
/*****************************************************************************/
/*  This function will determine the present downlink, uplink and			 */
/*  measurement frequencies of the instrument.								 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPReal64  downlinkFreq (out)                                          */
/*      The current downlink frequency that the test set is tuned to.        */
/*    ViPReal64  uplinkFreq (out)                                            */
/*      The current downlink frequency that the test set is tuned to.        */
/*    ViPReal64  measurementFreq (out)                                       */
/*      The current measurement frequency in Hz that the test set is tuned	 */
/*      to. If the measurement frequency control is set to Auto, then the	 */
/*		measurement frequency will be the same as the uplink frequency.		 */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus;

    ViInt32   RFANAuto;
    ViChar    operMode[5];
    ViReal64  RFANFreq;
    ViReal64  RFGenFreq;
    ViInt32   BCH;
    ViInt32   TCH;
    ViChar    cellband[16];
    ViChar    downlinkFcn[7];
    ViChar    Band[16];
    ViChar    queryMessage[256];
    ViInt32   channel;

    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_getPresentFreq_Q","");


    /* Default all measurement results to NAN */
    *downlinkFreq = age1960_NOT_A_NUMBER;
    *uplinkFreq = age1960_NOT_A_NUMBER;
    *measurementFreq = age1960_NOT_A_NUMBER;

    sprintf(queryMessage, "CALL:OPER:MODE?;:CALL:BAND?;BCH?;:"
            "CALL:TCH?;:CALL:TCH:BAND?;:"
            "CALL:FUNC:DOWN?;:CALL:RFG:FREQ:GSM?;:"
            "RFAN:CONT:AUTO?;:RFAN:MAN:FREQ?");

    if ((errStatus =  sendQueryRequest(instrumentHandle,queryMessage,8)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus = viScanf(instrumentHandle,
                             "%[^;];%[^;];%ld;%ld;%[^;];%[^;];%lg;%ld;%lg%*t",
                             operMode, cellband, &BCH, &TCH, Band, downlinkFcn,
                             &RFGenFreq, &RFANAuto, &RFANFreq)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* Get the uplink Frequency */

    if (RFANAuto == 1)
    {
        /* calculate uplink frequency based on band and channel */
        if(!strcmp(Band,"PGSM"))
            *uplinkFreq = 890e6 + TCH*200e3;
        else if(!strcmp(Band,"EGSM"))
        {
            if(TCH >= age1960_EGSM_MIN_CH_1 && TCH <= age1960_EGSM_MAX_CH_1)
                *uplinkFreq = 890e6 + TCH*200e3;
            else
                *uplinkFreq = 685.2e6 + TCH*200e3;
        }
        else if (!strcmp(Band,"DCS"))
            *uplinkFreq = 1607.8e6 + TCH*200e3;
        else if(!strcmp(Band,"PCS"))
            *uplinkFreq = 1747.8e6 + TCH*200e3;
        else if(!strcmp(Band,"GSM850"))
            *uplinkFreq = 798.6e6 + TCH*200e3;
        else /* Something went wrong if we didn't find by now, so return an error. */
            return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_UNEXPECTED);
    }
    else
        *uplinkFreq = RFANFreq;

    *measurementFreq=*uplinkFreq;

    /* Get the downlink Frequency */

    if(!strcmp(operMode,"CELL")) channel = TCH;
    if(!strcmp(operMode,"TEST"))
    {
        if (!strcmp(downlinkFcn,"CW"))
        {
            *downlinkFreq = RFGenFreq;
            return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
        }
        if (!strcmp(downlinkFcn,"BCH")) channel = BCH;
        if (!strcmp(downlinkFcn,"BCHTCH")) channel = TCH;
    }
    /* calculate downlink frequency based on band and channel */
    if(!strcmp(Band,"PGSM"))
        *downlinkFreq =  935e6 + channel*200e3;
    else if(!strcmp(Band,"EGSM"))
    {
        if(channel >= age1960_EGSM_MIN_CH_1 && channel <= age1960_EGSM_MAX_CH_1)
            *downlinkFreq =  935e6 + channel*200e3;
        else
            *downlinkFreq =  730.2e6 + channel*200e3;
    }
    else if(!strcmp(Band,"DCS"))
        *downlinkFreq =  1702.8e6 + channel*200e3;
    else if(!strcmp(Band,"PCS"))
        *downlinkFreq =  1827.8e6 + channel*200e3;
    else if(!strcmp(Band,"GSM850"))
        *downlinkFreq =  843.6e6 + channel*200e3;
    else /* Something went wrong if we didn't find it by now, so return an error. */
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_UNEXPECTED);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_getApproxOffset_Q(
    ViSession  instrumentHandle,
    ViReal64   frequency,
    ViPReal64  offset)
/********************************************************************************/
/*  This function will get the closest offset value from the RF In/Out			*/
/*  amplitude offset table based on the frequency. The offset value returned	*/
/*  will be an interpolated value if there is no exact frequency match in the	*/
/*  offset table. The amplitude offsets can be set using						*/
/*  the age1960_amplitudeOffset() function.										*/
/*																				*/
/*  In earlier versions of the GSM TA only 1 offset value could be set and		*/
/*  therefore only one value was ever returned. Beginning with E1960A TA		*/
/*  version A.07.xx and E1985A TA version A.01.xx the offset table can include	*/
/*  up to 20 frequencies and associated offset values.							*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViReal64   frequency (in)													*/
/*      Desired frequency offset to retrieve									*/
/*    ViPReal64   offset (out)													*/
/*      Offset from instrument, this may be interpulated from the closest		*/
/*      two frequencies' offsets.												*/
/********************************************************************************/

{

    ViStatus    errStatus ;
    ViInt32     state;
    ViReal64    freqArray[20];
    ViReal64    offsetArray[20];
    ViReal64    lowerFreq;
    ViReal64    upperFreq;
    ViReal64    slope;
    ViInt32     lowerIndex;
    ViInt32     upperIndex;
    ViInt32     index;

    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_getApproxOffset",",%lg", frequency);

    CHK_REAL_RANGE(frequency, age1960_OFFSET_FREQ_MIN,
                   age1960_OFFSET_FREQ_MAX, VI_ERROR_PARAMETER2)

    /* initialize */
    *offset = 0;
    lowerIndex = -1;    /* out of bounds lower */
    upperIndex = 20;    /* out of bounds upper */
    lowerFreq = 0;
    upperFreq = 9e99;
    for (index = 0; index<20; index ++)
    {
        freqArray[index] = age1960_NOT_A_NUMBER;
        offsetArray[index] = age1960_NOT_A_NUMBER;
    }

    if ((errStatus = delayVQueryf(instrumentHandle, "SYST:CORR:STAT?\n", "%ld%*t",
                                  &state)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (state == 0) return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:CORR:GAIN?",10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus = viScanf(instrumentHandle, "%,20lf%*t",offsetArray)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  delayVQueryf(instrumentHandle, "SYST:CORR:GAIN?\n",
                                   "%,20lf%*t",offsetArray)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  delayVQueryf(instrumentHandle, "SYST:CORR:FREQ?\n",
                                   "%,20lf%*t",freqArray)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* loop through the freq array, getting to two closest */
    for (index = 0; index <20; index++)
    {
        if (lowerFreq < freqArray[index] && freqArray[index] <= frequency && freqArray[index] != age1960_NOT_A_NUMBER)
        {
            if (lowerFreq != freqArray[index])
            {
                lowerFreq = freqArray[index];
                lowerIndex = index;
            }
        }
        if (frequency <= freqArray[index] && freqArray[index] <= upperFreq && freqArray[index] != age1960_NOT_A_NUMBER)
        {
            if (upperFreq != freqArray[index])
            {
                upperFreq = freqArray[index];
                upperIndex = index;
            }
        }
    }

    if (lowerIndex != -1 && upperIndex != 20)  /* There is both a lower and upper freq */
    {
        if (lowerIndex == upperIndex)
            *offset = offsetArray[upperIndex];
        else
        {
            slope =   (offsetArray[upperIndex] - offsetArray[lowerIndex])/( upperFreq - lowerFreq);
            *offset = slope * frequency + offsetArray[upperIndex]- slope * upperFreq;
        }
    }
    else if (lowerIndex == -1 && upperIndex != 20)  *offset = offsetArray[upperIndex];      /* There is only an upper freq */
    else if (upperIndex == 20 && lowerIndex != -1)  *offset = offsetArray[lowerIndex]; /* There is only a lower freq */

    /* If the offset value is equal to NAN for some reason then set it equal to zero.
       This will happen if the offset table state is ON but all of the individual offsets are off. */
    if (*offset > 9.9E37) *offset = 0;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_getCorrectionFreq_Q(
    ViSession  instrumentHandle,
    ViInt16    cell,
    ViPReal64  downlinkFreq,
    ViPReal64  uplinkFreq)
/*****************************************************************************/
/*  This function will determine the present downlink frequency and the      */
/*  present uplink frequencies. The frequencies could then be used to find	 */
/*  path loss factors. This function was used in earlier versions of the	 */
/*  GSM TA. Newer versions will use age1960_getPresentFreq_Q().				 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    cell (in)                                                   */
/*      The cell to return the current frequencies for.                      */
/*      The cells are definded as follows:                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CELL_1           1           Cell 1                          */
/*      age1960_CELL_2           2           Cell 2                          */
/*    ViPReal64  downlinkFreq (out)                                          */
/*      The current downlink frequency that the test set is tuned to.        */
/*    ViPReal64  uplinkFreq (out)                                            */
/*      The current downlink frequency that the test set is tuned to.        */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus  errStatus;

    ViInt32   RFANAuto;
    ViChar    operMode[5];
    ViReal64  RFANFreq;
    ViReal64  RFGenFreq;
    ViInt32   BCH;
    ViInt32   TCH;
    ViChar    cellband[16];
    ViChar    downlinkFcn[7];
    ViChar    Band[16];
    ViChar    queryMessage[256];
    ViInt32   channel;

    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_getCorrectionFreq_Q",",%hd", cell);

    CHK_INT_RANGE(cell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    /* Default all measurement results to NAN */
    *downlinkFreq = age1960_NOT_A_NUMBER;
    *uplinkFreq = age1960_NOT_A_NUMBER;

    sprintf(queryMessage, "CALL:OPER:MODE?;:CALL%s:BAND?;BCH?;:"
            "CALL:TCH?;:CALL:TCH:BAND?;:"
            "CALL:FUNC:DOWN?;:CALL%s:RFG:FREQ?;:"
            "RFAN:CONT:AUTO?;:RFAN:MAN:FREQ?",
            age1960_cell_a[cell],
            age1960_cell_a[cell]);

    if ((errStatus =  sendQueryRequest(instrumentHandle,queryMessage,8)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus = viScanf(instrumentHandle,
                             "%[^;];%[^;];%ld;%ld;%[^;];%[^;];%lg;%ld;%lg%*t",
                             operMode, cellband, &BCH, &TCH, Band, downlinkFcn,
                             &RFGenFreq, &RFANAuto, &RFANFreq)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* Get the uplink Frequency */

    if (RFANAuto == 1)
    {
        /* calculate uplink frequency based on band and channel */
        if(!strcmp(Band,"PGSM"))
            *uplinkFreq = 890e6 + TCH*200e3;
        else if(!strcmp(Band,"EGSM"))
        {
            if(TCH >= age1960_EGSM_MIN_CH_1 && TCH <= age1960_EGSM_MAX_CH_1)
                *uplinkFreq = 890e6 + TCH*200e3;
            else
                *uplinkFreq = 685.2e6 + TCH*200e3;
        }
        else if(!strcmp(Band,"DCS"))
            *uplinkFreq = 1607.8e6 + TCH*200e3;
        else if(!strcmp(Band,"PCS"))
            *uplinkFreq = 1747.8e6 + TCH*200e3;
        else if(!strcmp(Band,"GSM850"))
            *uplinkFreq = 798.6e6 + TCH*200e3;
        else /* Something went wrong if we didn't find by now, so return an error. */
            return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_UNEXPECTED);
    }
    else
        *uplinkFreq = RFANFreq;

    /* Get the downlink Frequency */

    if(!strcmp(operMode,"CELL")) channel = TCH;
    if(!strcmp(operMode,"TEST"))
    {
        if (!strcmp(downlinkFcn,"CW"))
        {
            *downlinkFreq = RFGenFreq;
            return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
        }
        if (!strcmp(downlinkFcn,"BCH")) channel = BCH;
        if (!strcmp(downlinkFcn,"BCHTCH")) channel = TCH;
    }
    /* calculate downlink frequency based on band and channel */
    if(!strcmp(Band,"PGSM"))
        *downlinkFreq =  935e6 + channel*200e3;
    else if(!strcmp(Band,"EGSM"))
    {
        if(channel >= age1960_EGSM_MIN_CH_1 && channel <= age1960_EGSM_MAX_CH_1)
            *downlinkFreq =  935e6 + channel*200e3;
        else
            *downlinkFreq =  730.2e6 + channel*200e3;
    }
    else if(!strcmp(Band,"DCS"))
        *downlinkFreq =  1702.8e6 + channel*200e3;
    else if(!strcmp(Band,"PCS"))
        *downlinkFreq =  1827.8e6 + channel*200e3;
    else if(!strcmp(Band,"GSM850"))
        *downlinkFreq =  843.6e6 + channel*200e3;
    else /* Something went wrong if we didn't find it by now, so return an error. */
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_ERROR_UNEXPECTED);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
}

/* ----- end of function ----- *

//*****************************************************************************/
ViStatus _VI_FUNC age1960_UUTLevel_Q(
    ViSession instrumentHandle,
    ViInt16 cell,
    ViPReal64 level)
/*****************************************************************************/
/*  This function will return the global value for the current level that    */
/*  is set at the UUT plane.                                                 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    cell (in)                                                   */
/*      The cell to return the current UUT level for.                        */
/*      The cells are definded as follows:                                   */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_CELL_1           1           Cell 1                          */
/*      age1960_CELL_2           2           Cell 2                          */
/*    ViPReal64  level (out)                                                 */
/*      The current cell power level at the UUT plane.                       */
/*                                                                           */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_UUTLevel_Q",",%hd", cell);

    CHK_INT_RANGE(cell, age1960_CELL_1, age1960_CELL_2,
                  VI_ERROR_PARAMETER2)

    *level = age1960_NOT_A_NUMBER;

    /* Get cell power */
    if((errStatus = delayVQueryf(instrumentHandle,
                                 "CALL%s:POW:AMPL%s?\n",
                                 "%lf%*t",
                                 age1960_cell_a[cell],
                                 age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                 level)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_SACCHReport_Q(
    ViSession instrumentHandle,
    ViBoolean  waitForNewSACCH,
    ViInt32    numberOfReadings,
    ViPReal64  RXLevel,
    ViPReal64  TXLevel,
    ViPReal64  RXQuality,
    ViPReal64  TimingAdvance)
/*****************************************************************************/
/*  This function will read the SACCH Report.                                */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViBoolean    waitForNewSACCH (in)                                      */
/*      If set to VI_TRUE the function will read int desired number of SACCH */
/*      reports using the :NEW node before returning the last results        */
/*    ViInt32  numberOfReadings (in)                                         */
/*      The number of times that you would like the SACCH report queried.    */
/*    ViPReal64  RXLevel (out)                                               */
/*      The returned RX Level value from the SACCH report.                   */
/*    ViPReal64  TXLevel (out)                                               */
/*      The returned TX Level value from the SACCH report.                   */
/*    ViPReal64  RXQuality (out)                                             */
/*      The returned RX quality value from the SACCH report.                 */
/*    ViPReal64  TimingAdvance (out)                                         */
/*      The returned Timing Advance value from the SACCH report.             */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;
    int i;
    ViReal64  junk;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_SACCHReport_Q",",%hu,%ld", waitForNewSACCH, numberOfReadings);

    CHK_BOOLEAN(waitForNewSACCH, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(numberOfReadings, age1960_SACCHREADINGS_MIN,
                   age1960_SACCHREADINGS_MAX, VI_ERROR_PARAMETER3)

    if (waitForNewSACCH == VI_FALSE)
    {
        /* should not have to look at MAV since this reading should return immediately */
        for(i=0; i < numberOfReadings; i++)
        {
            if ((errStatus = viPrintf(instrumentHandle, "CALL:MS:REP:TADV?;TXL?;RXL?;RXQ?\n")) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            if ((errStatus = viScanf(instrumentHandle, "%lg;%lg;%lg;%lg%*t",
                                     TimingAdvance, TXLevel,RXLevel, RXQuality)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

        }
    }
    else
    {
        /* look for new report with a timeout of 8 seconds look at MAV to keep HP-IB */
        /* from being tied up */
        for(i=0; i < numberOfReadings; i++)
        {
            if ((errStatus =  sendQueryRequest(instrumentHandle, "CALL:MS:REP:RXQ:NEW?",8000 )) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            if ((errStatus = viScanf(instrumentHandle, "%lg%*t",&junk)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

        }
        if ((errStatus = viPrintf(instrumentHandle, "CALL:MS:REP:TADV?;TXL?;RXL?;RXQ?\n")) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = viScanf(instrumentHandle, "%lg;%lg;%lg;%lg%*t",
                                 TimingAdvance, TXLevel,RXLevel, RXQuality)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/

ViStatus _VI_FUNC age1960_waitForClearHPIB(ViSession instrumentHandle, ViInt32  timeOut)
/********************************************************************************/
/*  This function will use the SYST:SYNC? to ensure that the GPIB buffer		*/
/*  has processed all previous commands											*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViInt32    timeOut(in)													*/
/*      Maximun time in seconds to wait for the instrument to process all		*/
/*      prevoius commands.														*/
/********************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;
    ViInt32  junk;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_waitForClearHPIB",",%ld", timeOut);

    CHK_LONG_RANGE(timeOut,  age1960_MAV_TIMEOUT_MIN,
                   age1960_MAV_TIMEOUT_MAX, VI_ERROR_PARAMETER2)

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:SYNC?",
                                       1000*timeOut-1000)) < VI_SUCCESS)
        /* -1000 since wait for MAV will add 1000 */
        return statusUpdate(instrumentHandle,thisPtr,errStatus);


    if ((errStatus = viScanf(instrumentHandle, "%ld%*t",
                             &junk)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/

ViStatus _VI_FUNC age1960_useSRQMAV(ViSession instrumentHandle, ViBoolean  useSRQMAV)
/*****************************************************************************/
/*  This function will determine how the driver should handle waiting for bus*/
/*  activity to complete.                                                    */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViBoolean    useSQRMAV(in)                                             */
/*      If set to VI_TRUE the driver will use SRQs to keep for hanging the   */
/*      HP-IB for any length of time.  This mode will only be used if testing*/
/*      with muli test processes runing on a single computer.                */
/*      If set to VI_FAULE the driver will not worry about hanging the HP-IB.*/
/*      will reduce the amount of HP-IB commands that are sent to the        */
/*      instrument.
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_useSRQMAV",",%hu", useSRQMAV);

    CHK_BOOLEAN(useSRQMAV, VI_ERROR_PARAMETER2)

    thisPtr->useSRQ = useSRQMAV;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/

ViStatus _VI_FUNC age1960_useSRQMAV_Q(ViSession instrumentHandle, ViPBoolean  useSRQMAV)
/*****************************************************************************/
/*  This function will determine how the driver is set to handle waiting     */
/*  for bus activity to complete.                                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPBoolean    useSQRMAV (out)                                          */
/*       How the driver is set to handle SRQs.                               */
/*****************************************************************************/

{
    ViStatus errStatus;
    struct age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_useSRQMAV_Q","");

    *useSRQMAV = thisPtr->useSRQ;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_loopback(
    ViSession  instrumentHandle,
    ViInt16    loopbackMode)
/*****************************************************************************/
/*  This function will perform a CALL:TCHannel:LOOPback                      */
/*   for the desired loopback mode.                                          */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt16    loopbackMode (in)                                           */
/*      The desired loopback mode to set the TCH for.  The loopback modes    */
/*      are defined as follows:                                              */
/*                                                                           */
/*      Constant Name            Value        Description                    */
/*      -----------------------------------------------------------          */
/*      age1960_LOOPBACK_OFF      0          Turn Loopback off               */
/*      age1960_LOOPBACK_A        1          Loopback to type A              */
/*      age1960_LOOPBACK_B        2          Loopback to type B              */
/*      age1960_LOOPBACK_C        3          Loopback to type C              */
/*                                                                           */
/*****************************************************************************/

{

    ViStatus  errStatus;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_loopback",",%hd", loopbackMode);

    CHK_ENUM(loopbackMode, 3, VI_ERROR_PARAMETER2)

    if ((errStatus = delayVPrintf(instrumentHandle, "CALL:TCH:LOOP %s\n",
                                  age1960_loopbackSetting_a[loopbackMode])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    thisPtr->loopback = loopbackMode;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*****************************************************************************/
ViStatus _VI_FUNC age1960_readIQTuning_Q(
    ViSession  instrumentHandle,
    ViInt32    IQArraySize,
    ViPInt32   IQIntegrity,
    ViPReal64   IQSpurFreq,
    ViPReal64   IQSpurLevel,
    ViReal64   _VI_FAR IQResultsArray[],
    ViReal64   _VI_FAR IQFreqsArray[],
    ViPInt32   IQTuningCount)
/********************************************************************************/
/*   This function will perform a READ:IQT command								*/
/*  This function will return the Integrity, spur freq and level an				*/
/*  array of levels, an array of frequencies for the different levels, and a	*/
/*  count of the number of results returned.									*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViInt32    IQArraySize (in)												*/
/*      The size of the IQResultsArr[] and the IQFreqsArr[] array.				*/
/*    ViPInt32   IQIntegrity (out)												*/
/*      The returned value from the instrument for IQ tuning integrity.			*/
/*    ViPReal64   IQSpurFreq (out)												*/
/*      This returned value for the frequency the spur is set at.				*/
/*    ViPReal64   IQSpurLevel (out)												*/
/*      The returned level at the spur freq.									*/
/*    ViReal64   IQResultsArray[] (out)											*/
/*      The returned values for the IQ tuning, referenced to the desired		*/
/*      frequency value.														*/
/*    ViReal64   IQFreqsArray[] (out)											*/
/*      The returned values of the frequencies in Hz where the IQ tuning		*/
/*      results were measured.													*/
/*    VIPint32   IQTuningCount  (out)											*/
/*      The returned value of the number of points that were measured.			*/
/*																				*/
/********************************************************************************/

{
    ViStatus  errStatus;
    ViReal64  tempFreqArray[]  = {-270.833, -203.125, -135.417, -67.7083, 0,
                                  67.7083, 135.417, 203.125, 270.833
                                 };
    struct    age1960_globals *thisPtr;
    int       i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_readIQTuning_Q",",%ld",IQArraySize);

    *IQTuningCount = 0;
    *IQIntegrity = age1960_NO_RESULT_AVAIL;

    CHK_LONG_RANGE(IQArraySize, age1960_IQ_ARRAY_MIN,
                   age1960_IQ_ARRAY_MAX, VI_ERROR_PARAMETER2)

    for (i = 0; i<IQArraySize; i++)
    {
        IQResultsArray[i] = age1960_NOT_A_NUMBER;
    }

    /* The for's here were seperated originally they were together which VB did not like. It was causing a access
      violation. freq array only has 9 values and originally we were filling it with 500, 491 would have been garbage.*/
    for (i = 0; i<9; i++)
    {
        IQFreqsArray[i] = tempFreqArray[i];
    }

    if ((errStatus =  sendQueryRequest(instrumentHandle, "READ:IQT?;:SET:IQT:SPUR:FREQ?",
                                       ReadTimeout(instrumentHandle,"IQT") )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%,9lf,%lf;%lf%*t",
                             IQIntegrity, IQResultsArray,
                             IQSpurLevel, IQSpurFreq))< VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    *IQTuningCount = 9;

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_PowerVsTimePlot_Q(
    ViSession   instrumentHandle,
    ViInt32     pvsTimePlotArraySize,
    ViPInt32    pvsTimePlotIntegrity,
    ViPReal64   pvsTimePlotMaskFail,
    ViPReal64   pvsTimePlotPower,
    ViPReal64   pvsTimeStartTime,
    ViPReal64   pvsTimeIncrement,
    ViReal64    _VI_FAR pvsTimePlotArray[])
/*****************************************************************************/
/*  This function will perform a FETCH:PVT:MTR?. This function will return   */
/*  the Integrity, a pass/fail flag, the TX power measurement, the plot start*/
/*  time(usec), the time increment for each point (usec) and the array of    */
/*  2220 points for the power vs time plot.  This function will read the     */
/*  instrument setting for the required parameters for the FETCH:PVT:MTR?    */
/*  query.																	 */
/*														                     */
/*  Note: This function is only designed to work when the instrument is		 */
/*  either in Test operating mode, or Cell operating mode and a call is		 */
/*  CONNECTED. If neither of these conditions is met the function will		 */
/*  return age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED as the error status.		 */
/*																			 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViInt32    pvsTimePlotArrSize (in)                                     */
/*      The size of the pvsTimePlot[] array. This size has to be 2220        */
/*      elements.  The only reason that the size is passed to this function  */
/*      is to verify that the calling function has sized the array properly  */
/*      to keep this funciton from writing to incorrect memory space.        */
/*    ViInt32    pvsTimePlotArraySize (in)                                   */
/*      The size of the pvsTimeArray array.                                  */
/*    ViPInt32   pvsTimePlotIntegrity (out)                                  */
/*      The returned integrity value from the instrument for power vs time.  */
/*    ViPReal64   pvsTimePlotMaskFail (out)                                  */
/*      The returned pass fail flag (0 for pass) from the instrument for     */
/*		power vs time.														 */
/*    ViPReal64  pvsTimePlotPower (out)                                      */
/*      The returned TX Power from the instrument for power vs time.         */
/*    ViPReal64  pvsTimeStartTime (out)                                      */
/*      The returned starting time (usec) for the power vs time plot.        */
/*    ViPReal64  pvsTimeIncrement (out)                                      */
/*      The returned increment time (usec) for each of the power vs time     */
/*      points.                                                              */
/*    ViReal64   pvsTimePlotArray[] (out)                                    */
/*      The array of points (2220) for  the power vs time plot.              */
/*****************************************************************************/
{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    ViInt32   tmpIntegrity;
    ViReal64  tmpPassFail;
    ViReal64  junk;
    ViReal64  lotsOfJunk[60];
    ViReal64  UpLinkFreq;
    ViInt16   Trig;
    ViInt16   Sync;
    ViInt16   BurstType;
    ViReal64  TrigDelay;
    ViReal64  ExpPower;
    ViReal64  AmplOffset;
    ViInt16   TXLevel;
    ViInt16   Band;
    ViChar    queryMsg[500];
    ViChar    Msg[100];
    ViInt32   tmpIntegrityFromRead;
    ViReal64  tmpPowerFromRead;

    int       i;


    /* dummy offsets that are required by the FETCH:PVT:MTR?*/
    ViReal64  Offsets[] = {-.000028, -.000018, -.0000100, .0000000, .0003212,
                           .0003312, .0003392, .0003492, .0005428, .0005528,
                           .0005608, .0005708
                          };

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_PowerVsTimePlot_Q",",%ld", pvsTimePlotArraySize);

    CHK_LONG_RANGE(pvsTimePlotArraySize, age1960_PVTPLOT_ARRAY_MIN,
                   age1960_PVTPLOT_ARRAY_MAX, VI_ERROR_PARAMETER2)

    /* setup defualts */
    *pvsTimePlotIntegrity = age1960_NO_RESULT_AVAIL;
    *pvsTimePlotMaskFail = age1960_NOT_A_NUMBER;
    *pvsTimePlotPower	= age1960_NOT_A_NUMBER;
    *pvsTimeStartTime	= age1960_NOT_A_NUMBER;
    *pvsTimeIncrement	= age1960_NOT_A_NUMBER;

    /* get the information required for the FETCH:PVT:MTR? query from settings */
    /* on the instrument*/

    if((errStatus = prepareForPvtPlot(instrumentHandle, &Trig, &Sync,
                                      &BurstType, &TrigDelay, &ExpPower, &UpLinkFreq, &AmplOffset,
                                      &TXLevel, &Band)) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if((errStatus = sendQueryRequest(instrumentHandle, "READ:TXP?",
                                     ReadTimeout(instrumentHandle,"PVT"))) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    //if ((errStatus =  viPrintf(instrumentHandle, "READ:TXP?\n" )) < VI_SUCCESS)
    //     return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf%*t",
                             &tmpIntegrityFromRead, &tmpPowerFromRead)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    sprintf(queryMsg, "FETCH:PVT:MTR? %hd,%hd,%hd,%hd,%lg,%lg,%lg,%hd,%lg,%hd,%hd",
            Trig, Sync, BurstType,
            1, TrigDelay, (ExpPower + AmplOffset),
            UpLinkFreq, 0, AmplOffset,
            TXLevel, Band);
    for(i=0; i<12; i++)
    {
        sprintf(Msg, ",%lg",Offsets[i]);
        strcat (queryMsg, Msg);
    }

    sprintf(Msg, ",%hd",0);
    strcat (queryMsg, Msg);

    if((errStatus = sendQueryRequest(instrumentHandle, queryMsg,
                                     ReadTimeout(instrumentHandle,"PVT"))) < VI_SUCCESS )
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "%ld,%lf,%lf,%lf,%,55lf,%lf,%lf,%,2220lf%*t",
                             &tmpIntegrity,
                             &junk,
                             &tmpPassFail,
                             pvsTimePlotPower,
                             lotsOfJunk,
                             pvsTimeStartTime,
                             pvsTimeIncrement,
                             pvsTimePlotArray)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    *pvsTimePlotMaskFail =  tmpPassFail;

    *pvsTimePlotPower -= AmplOffset;

    *pvsTimePlotIntegrity = 13; /*Unidentified Error*/

    if (tmpIntegrity == 0) *pvsTimePlotIntegrity = 0; /*Normal results*/

    if ((tmpIntegrity & 1048576) == 1048576) *pvsTimePlotIntegrity = 12; /*Oven Out Of Range*/

    if ((tmpIntegrity & 1) == 1) *pvsTimePlotIntegrity = 11;       /*Sync Not Found */

    if ((tmpIntegrity & 512) == 512) *pvsTimePlotIntegrity = 10;   /*Signal Too Noisy */

    if ((tmpIntegrity & 4) == 4) *pvsTimePlotIntegrity = 9;        /*Trigger Late or Rise Late*/

    if ((tmpIntegrity & 2) == 2) *pvsTimePlotIntegrity = 8;        /*Trigger Early or Fall Early*/

    if ((tmpIntegrity & 64) == 64) *pvsTimePlotIntegrity = 7;      /*Burst Short*/

    if ((tmpIntegrity & 16) == 16) *pvsTimePlotIntegrity = 6;      /*Under Range  */

    if ((tmpIntegrity & 8) == 8) *pvsTimePlotIntegrity = 5;        /*Over Range     */

    if ((tmpIntegrity & 32) == 32) *pvsTimePlotIntegrity = 4;      /*Hardware Error   */

    if ((tmpIntegrity & 1073741824) == 1073741824) *pvsTimePlotIntegrity = 3; /*Hardware Not Installed*/

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/****************************************************************************/
static ViBoolean mystrcmpi(
    ViString    string1,
    ViString    string2)
/****************************************************************************/
/*  This function will compare two strings, case independent.  VI_TRUE will */
/*  be returned is the two strings compare exactly, otherwise VI_FALSE will */
/*  be returned.                                                            */
/*                                                                          */
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from hpe1960_init().                     */
/*    ViString    string1 (in)                                              */
/*      First string of the compare                                         */
/*    ViString    string2 (in)                                              */
/*      Second string of the compare                                        */
/*                                                                          */
/****************************************************************************/
{
    ViBoolean result;
    int index = 0;

    result = VI_TRUE;
    /* while they are equal and not at the end of the string */
    while (toupper(string1[index]) == toupper(string2[index])
            && toupper(string1[index]) != 0)
    {
        index++;
    }

    if (toupper(string1[index]) != toupper(string2[index]))
        result = VI_FALSE;

    return result;
}     /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_currentTA(
    ViSession  instrumentHandle,
    ViString   TAName,
    ViString   TARevision)
/*****************************************************************************/
/*  Select the desired TA and the TA's revision.  If the selected TA is      */
/*  currently loaded with the desired revision then do nothing.              */
/*  If the TARevision is "Latest" or "" load the latest revision for         */
/*  the desired TA.  If the TA or the revision does not exist, return an     */
/*  error.                                                                   */
/*  Once a desired TA is successfully loaded, a suffix is determined		 */
/*  per a called function that enables both a single and combo partition TA  */
/*  (with GSM capability) to function properly with this driver.             */
/*																			 */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViString   TAName (in)                                                 */
/*      The name of the TA to load.                                          */
/*    ViString  TARevision (in)                                              */
/*      The revision number of the TA to load.  TARevision is set to         */
/*      "Latest", or "" then latest TA on the system will be loaded.         */
/*****************************************************************************/

{
    ViStatus    errStatus;
    time_t      tStart;
    ViInt32     timeoutLeft;
    ViInt32     maxTimeToWait;
    ViChar      currentApplication[150];
    ViChar      currentRevision[150];
    ViChar      quotedTAName[150];
    ViChar      lTAName[150];
    ViChar      lTARevision[150];
    ViChar      quotedTARevision[150];
    ViChar      tempQuotedTARevision[150];
    ViChar      queryMessage[150];
    ViChar      license[10];
    ViChar		TACatalog[512];
    ViInt32     systemError;
    ViInt32     presentTimeout;
    ViBoolean   mav;
    ViUInt16  junkForNI;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_currentTA", ",%s,%s", TAName, TARevision);

    if ((errStatus =  viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                     &presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* this can take a while so we should always wait for srq */
    if ((errStatus =  age1960_useSRQMAV_Q(instrumentHandle, &mav)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    if ((errStatus =  age1960_useSRQMAV(instrumentHandle, VI_TRUE)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* read the catalog string */
    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:CAT:NAME?", 20000)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus = viScanf(instrumentHandle, "%t", TACatalog)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    /* check to see if the desired TA name is in the catalog */
    if (strstr(TACatalog,TAName) == NULL)
    {
        /* check to see if we are looking for one of the combo TA's */
        if ((strcmp(TAName,"GSM_AMPS/136 Mobile Test")==0) ||
                (strcmp(TAName,"GSM_AMPS/136_GPRS Mobile")==0) ||
                (strcmp(TAName,"GSM_AMPS/136_GPRS_WCDMA")==0))
        {

            if (strstr(TACatalog,"GSM_AMPS/136 Mobile Test") != NULL)
                TAName = "GSM_AMPS/136 Mobile Test";
            else if (strstr(TACatalog,"GSM_AMPS/136_GPRS Mobile") != NULL)
                TAName = "GSM_AMPS/136_GPRS Mobile";
            else if (strstr(TACatalog,"GSM_AMPS/136_GPRS_WCDMA") != NULL)
                TAName = "GSM_AMPS/136_GPRS_WCDMA";
            else
            {
                /* reset the srq */
                age1960_useSRQMAV(instrumentHandle, mav);
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_AVAILABLE);
            }
        }
        else
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_AVAILABLE);
        }

    }

    sprintf( quotedTAName, "\"%s\"", TAName);
    sprintf( lTAName, "'%s'", TAName);

    /* Do we have to get the revision from the list of revisions? */
    if  (mystrcmpi(TARevision, "latest") == VI_TRUE || strcmp(TARevision, "") == 0)
    {
        sprintf(queryMessage,"SYST:APPL:CAT:REV? %s", quotedTAName);

        if ((errStatus =  sendQueryRequest(instrumentHandle, queryMessage,
                                           45000)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
        /* just read the first revision and throw the others away since the first is the latest */
        if ((errStatus = viScanf(instrumentHandle, "%[^,]%*t",
                                 tempQuotedTARevision)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }

        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
        if (strstr(tempQuotedTARevision,"\"") == NULL)    /* not quoted */
        {
            sprintf( quotedTARevision, "\"%s\"", tempQuotedTARevision);
            sprintf( lTARevision, "'%s'", tempQuotedTARevision);
        }
        else
        {
            /* must remove the quotes and replace them in case there is a \n\0 in the string */
            stripQuotes(tempQuotedTARevision);
            sprintf( quotedTARevision, "\"%s\"", tempQuotedTARevision);  /* add quotes */
            sprintf( lTARevision, "'%s'", tempQuotedTARevision);
        }

    }
    else
    {
        sprintf( quotedTARevision, "\"%s\"", TARevision);
        sprintf( lTARevision, "'%s'", TARevision);
    }

    /* Get the currently loaded TA name and revision */
    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:CURR?;:SYST:APPL:CURR:REV?",
                                       30000)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus = viScanf(instrumentHandle, "%[^;];%s%*t",
                             currentApplication, currentRevision)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    /* do we presently have the correct TA loaded? */
    if (mystrcmpi(quotedTAName, currentApplication) == VI_TRUE
            && mystrcmpi(quotedTARevision, currentRevision) == VI_TRUE)
    {

        /****************************************************************************/
        /* Determine suffix based on whether TA model is a combo or single          */
        /* partition TA.                                                            */
        /****************************************************************************/

        if ((errStatus = determineSuffix(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,VI_SUCCESS);
    }

    /* is the desired TA available? */
    if ((errStatus = delayVPrintf(instrumentHandle, "*CLS;:SYST:APPL:SEL:REV %s,%s\n",
                                  quotedTAName, quotedTARevision)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:ERR?", 45000)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus = viScanf(instrumentHandle, "%ld%*t", &systemError)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }
    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle,thisPtr,errStatus);
    }

    if (systemError == -220)
    {
        /* reset the srq */
        age1960_useSRQMAV(instrumentHandle, mav);
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_AVAILABLE);
    }

    /* 	the name of the combo TA can change when the revision is selected, so it
    	is necessary to modify the stored name to sync up with the name found in
    	the instrument TA list now that the revision has been selected. */
    if ((strcmp(TAName,"GSM_AMPS/136 Mobile Test")==0) ||
            (strcmp(TAName,"GSM_AMPS/136_GPRS Mobile")==0) ||
            (strcmp(TAName,"GSM_AMPS/136_GPRS_WCDMA")==0))
    {

        if (strstr(quotedTARevision,"A.01") != NULL)
            TAName = "GSM_AMPS/136 Mobile Test";
        else if ((strstr(quotedTARevision,"A.02") != NULL) || (strstr(quotedTARevision,"A.03") != NULL))
            TAName = "GSM_AMPS/136_GPRS Mobile";
        else /* the revision is B.xx.xx or higher so default to the latest known combo name */
            TAName = "GSM_AMPS/136_GPRS_WCDMA";

        sprintf( quotedTAName, "\"%s\"", TAName);
        sprintf( lTAName, "'%s'", TAName);

    }

    /* Does the desired TA have a License */
    if (mystrcmpi(quotedTAName, "\"GSM Mobile Test\"") == VI_TRUE)
    {
        if (strcmp(quotedTARevision, "\"A.05.01\"")>= 0)    /* if the GSM rev has licensing */
        {
            /* we need to check for a license first */
            sprintf(queryMessage,"SYST:APPL:CAT:LIC? %s,%s", lTAName, lTARevision);
            if ((errStatus =  sendQueryRequest(instrumentHandle, queryMessage,
                                               45000)) < VI_SUCCESS)
            {
                /* reset the srq */
                age1960_useSRQMAV(instrumentHandle, mav);
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
            if ((errStatus = viScanf(instrumentHandle, "%s%*t",license)) < VI_SUCCESS)
            {
                /* reset the srq */
                age1960_useSRQMAV(instrumentHandle, mav);
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }
            if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            {
                /* reset the srq */
                age1960_useSRQMAV(instrumentHandle, mav);
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }

            if (strcmp(license,"LIC"))   /* if the TA is not licensed */
            {
                /* reset the srq */
                age1960_useSRQMAV(instrumentHandle, mav);
                return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_LICENSE);
            }
        }
    }
    else    /* not GSM so check for licensing */
    {
        /* we need to check for a license first */
        sprintf(queryMessage,"SYST:APPL:CAT:LIC? %s,%s", lTAName, lTARevision);
        if ((errStatus =  sendQueryRequest(instrumentHandle, queryMessage,
                                           45000)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
        if ((errStatus = viScanf(instrumentHandle, "%s%*t",license)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }
        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle,thisPtr,errStatus);
        }

        if (strcmp(license,"LIC"))   /* if the TA is not licensed */
        {
            /* reset the srq */
            age1960_useSRQMAV(instrumentHandle, mav);
            return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_LICENSE);
        }
    }

    /* reset the srq */
    if ((errStatus =  age1960_useSRQMAV(instrumentHandle, mav)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* desired TA is available so load it and wait for instrument to respond to GPIB*/
    if ((errStatus = delayVPrintf(instrumentHandle, "SYST:APPL:SEL %s\n",
                                  quotedTAName)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* Loop until the instrument will accept the *IDN? query */
    doDelay(2000000);  /* wait 2 sec for the switching to start*/

    timeoutLeft = age1960_TA_SWITCH_MAX_TIME;
    maxTimeToWait =  age1960_TA_SWITCH_MAX_TIME;
    tStart=time(NULL); /* Get the current time to second resolution*/

    /* set the spin timeout to .5 seconds so the bus is not locked for a long time */
    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                     500 )) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    /* the NI cards time out and then never recover with Queryf, but they will recover with printf/scanf */
    viPrintf(instrumentHandle, "*IDN?\n");
    errStatus = viScanf(instrumentHandle, "%s%*t", queryMessage);
    age1960_dcl(instrumentHandle);// clean up the bus
    viReadSTB(instrumentHandle, &junkForNI);  /* read stb junk to clean up NI card timeout */

    while(errStatus != VI_SUCCESS)
    {

        /* the NI cards time out and then never recover with Queryf, but they will recover with printf/scanf */
        age1960_dcl(instrumentHandle); /* added to handle query unterminated condition; do not remove */
        viPrintf(instrumentHandle, "*IDN?\n");
        errStatus = viScanf(instrumentHandle, "%s%*t", queryMessage);
        doDelay(2000000);
        age1960_dcl(instrumentHandle); /*  clean up the bus */
        age1960_dcl(instrumentHandle); /* added to handle query unterminated condition; do not remove */
        age1960_dcl(instrumentHandle); /* added to handle query unterminated condition; do not remove */
        viReadSTB(instrumentHandle, &junkForNI);  /* read stb junk to clean up NI card timeout */

        if (errStatus == VI_SUCCESS) break;

        timeoutLeft = maxTimeToWait -  (ViInt32) difftime(time(NULL),tStart);
        if(timeoutLeft<=0)
        {
            if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                             presentTimeout)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            return statusUpdate(instrumentHandle,thisPtr,VI_ERROR_TMO);

        }
        doDelay(2000000);  /* wait 2 sec */
    }

    /************************************************************/
    /* Determine suffix for single or combo TA since we have	*/
    /* switched to a new TA.					                */
    /************************************************************/

    if((errStatus = determineSuffix(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    /* Clear error queue */
    viPrintf(instrumentHandle, "*CLS\n");

    if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                     presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_currentTA_Q(
    ViSession  instrumentHandle,
    ViPString  TANameQuery,
    ViPString  TARevisionQuery)
/*****************************************************************************/
/*  Read the current TA and the TA's revision. This funciton will remove the */
/*  set of " around the TA name and TA revision.                             */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPString   TANameQuery (out)                                          */
/*      The name of the TA loaded.                                           */
/*    ViPString   TARevisionQuery (out)                                      */
/*      The revision number of the TA loaded.                                */
/*****************************************************************************/

{
    ViStatus    errStatus;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_currentTA_Q", "");

    /* Get the currently loaded TA name and revision */
    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:CURR?;:SYST:APPL:CURR:REV?",
                                       10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "\"%[^\"]\";\"%[^\"]%*t",
                             TANameQuery, TARevisionQuery)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_currentTAModel_Q(
    ViSession  instrumentHandle,
    ViPString  TAModel)
/*****************************************************************************/
/*  Read the current TA model number. This funciton will remove the          */
/*  set of " around the TA model.                                            */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*    ViPString   TAModel (out)                                              */
/*      The name of the TA loaded.                                           */
/*****************************************************************************/

{
    ViStatus    errStatus;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_currentTAModel_Q", "");

    /* Get the currently loaded TA model number */
    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:CURR:TA:MOD?",
                                       10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "\"%[^\"]%*t",
                             TAModel)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_currentActiveTAP(
    ViSession  instrumentHandle,
    ViString   TAPName)
/********************************************************************************/
/*  This function is meant to work with the E1985A Fast Switching TA. The		*/
/*  function fast switches the current active Test Application Partition (TAP)	*/
/*  to the TAP specified. If the current Test Application is E1960A and the TAP	*/
/*  specified is GSM, the function returns without doing anything. If the TA is	*/
/*  E1960A and the TAP specified is something other than GSM (e.g. AMPS/136),	*/
/*  then the error age1961_INSTR_TA_NOT_LICENSE is used to indicate an error	*/
/*  condition. If the TA is not E1960A and does not support the command			*/
/*  SYST:APPL:CAT:FORM?, then a UUT error will occur. The error status is		*/
/*  returned.																	*/
/*																				*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViString   TAPName (in)													*/
/*      The name of the active TAP to load.										*/
/********************************************************************************/

{
    ViStatus    errStatus;
    ViInt32     presentTimeout;
    ViBoolean   mav;
    ViChar      quotedTAPName[50];
    char formatInfo[256];
    struct      age1960_globals *thisPtr;
    ViChar     idn_buf[256];

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_currentActiveTAP", ",%s", TAPName);

    if ((errStatus =  viGetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE,
                                     &presentTimeout)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    sprintf( quotedTAPName, "\"%s\"", TAPName);

    /* Return SUCCESS if the current Test Application (TA) is E1960A and TAPName is GSM  */
    if (strcmp(thisPtr->TAModel, "E1960A") == 0 && strcmp(TAPName, age1960_SUPPORTED_FORMAT_STRING) == 0)
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

    /* Return ERROR if the current TA is E1960A and TAPName is NOT GSM */
    if (strcmp(thisPtr->TAModel, "E1960A") == 0 && strcmp(TAPName, age1960_SUPPORTED_FORMAT_STRING) != 0)
        return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_LICENSE);

    /* this can take a while so we should always wait for srq */
    if ((errStatus =  age1960_useSRQMAV_Q(instrumentHandle, &mav)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  age1960_useSRQMAV(instrumentHandle, VI_TRUE)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if (strcmp(thisPtr->TAModel, "E1960A") != 0)
    {

        if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:CAT:FORM?",10000)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        if ((errStatus = viScanf(instrumentHandle, "%s%*t", formatInfo)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);

        if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle,thisPtr,errStatus);

        /* Desired TAPName in the formatInfo string means it is licensed and usable */
        if (strstr(formatInfo,quotedTAPName) != NULL)
        {
            /* fast switch to the desired active TAP */
            if ((errStatus = delayVPrintf(instrumentHandle, "SYST:APPL:FORMAT:NAME %s\n",
                                          quotedTAPName)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            /* set the timeout to 10 seconds to prevent timeout error - allows unit to complete reset before query */
            if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, 10000)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

            /* *IDN? is used just to insure the format change has completed */
            if ((errStatus = delayVQueryf(instrumentHandle, "*IDN?\n", "%t",idn_buf)) < VI_SUCCESS)
            {
                viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout);
                return statusUpdate(instrumentHandle,thisPtr,errStatus);
            }

            /* set the timeout back to original value */
            if ((errStatus =  viSetAttribute(instrumentHandle, VI_ATTR_TMO_VALUE, presentTimeout)) < VI_SUCCESS)
                return statusUpdate(instrumentHandle,thisPtr,errStatus);

        }
        else
        {
            return statusUpdate(instrumentHandle, thisPtr, age1960_INSTR_TA_NOT_LICENSE);
        }
    }

    /* reset the srq */
    if ((errStatus =  age1960_useSRQMAV(instrumentHandle, mav)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_currentActiveTAP_Q
(ViSession  instrumentHandle,
 ViPString  TAPNameQuery)
/********************************************************************************/
/*  This function is meant to work with the E1985A Fast Switching TA. The		*/
/*  function returns the currently active Test Application Partition (TAP) name.*/
/*  If the Test Application does not support SYST:APPL:FORMAT:NAME? command, a	*/
/*  UUT error will occur resulting in a time out being returned.				*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViPString   TAPNameQuery (out)											*/
/*      The name of the TA loaded.												*/
/********************************************************************************/

{
    ViStatus    errStatus;
    struct      age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_currentActiveTAP_Q", "");

    strcpy(TAPNameQuery,"UNKNOWN");

    /* Return GSM if the current Test Application is E1960A */
    if (strncmp(thisPtr->TAModel, "E1960A", 6) == 0)
    {
        strcpy(TAPNameQuery,age1960_SUPPORTED_FORMAT_STRING);
        return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);
    }
    /* Get the active TAP name */
    if ((errStatus =  sendQueryRequest(instrumentHandle, "SYST:APPL:FORMAT:NAME?",
                                       10000)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus = viScanf(instrumentHandle, "\"%[^\"]%*t",
                             TAPNameQuery)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    if ((errStatus =  cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle,thisPtr,errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS );

}        /* ----- end of function ----- */



/*****************************************************************************/
ViStatus _VI_FUNC age1960_confPFER_CIFT
(ViSession instrumentHandle,
 ViBoolean  PFERCIFTState,
 ViReal64   PFERConfidenceLevel,
 ViReal64   PFERPeakPhaseErrorMaxLimit,
 ViReal64   PFERRmsPhaseErrorMaxLimit,
 ViReal64   PFERFrequencyErrorMaxLimit)
/********************************************************************************/
/*  This function configures the PFER measurements Confidence Interval for Fast */
/*  Termination (CIFT) features.  CIFT uses statistical means to determine      */
/*  whether a measurement is likely to pass or fail, without performing the     */
/*  whole measurement.															*/
/*																				*/
/*  PARAMETERS																	*/
/*    ViSession  instrumentHandle (in)											*/
/*      Instrument handle returned from age1960_init().							*/
/*    ViBoolean  PFERCIFTState (in)		    									*/
/*      Enables or disable CIFT.												*/
/*	  ViReal64	 PFERConfidenceLevel (in)										*/
/*		The degree of confidence that the measurement will pass.				*/
/*	  ViReal64   PFERPeakPhaseErrorMaxLimit (in)								*/
/*		The peak phase error limit.												*/
/*	  ViReal64	 PFERRmsPhaseErrorMaxLimit (in)									*/
/*		The rms phase error limit.												*/
/*	  ViReal64	 PFERFrequencyErrorMaxLimit (in)								*/
/*		The frequency error limit.												*/
/********************************************************************************/

{
    ViStatus  errStatus ;
    struct    age1960_globals *thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confPFER_CIFT",",%hd,%lg,%lg,%lg,%lg",
                  PFERCIFTState, PFERConfidenceLevel,
                  PFERPeakPhaseErrorMaxLimit,
                  PFERRmsPhaseErrorMaxLimit,
                  PFERFrequencyErrorMaxLimit);

    CHK_BOOLEAN(PFERCIFTState, VI_ERROR_PARAMETER2)

    CHK_REAL_RANGE(PFERConfidenceLevel, age1960_CIFT_CONF_LEVEL_MIN,
                   age1960_CIFT_CONF_LEVEL_MAX, VI_ERROR_PARAMETER3)

    CHK_REAL_RANGE(PFERPeakPhaseErrorMaxLimit, age1960_PFER_PEAK_PHASE_LIMIT_MIN,
                   age1960_PFER_PEAK_PHASE_LIMIT_MAX, VI_ERROR_PARAMETER4)

    CHK_REAL_RANGE(PFERRmsPhaseErrorMaxLimit, age1960_PFER_RMS_PHASE_LIMIT_MIN,
                   age1960_PFER_RMS_PHASE_LIMIT_MAX, VI_ERROR_PARAMETER5)

    CHK_REAL_RANGE(PFERFrequencyErrorMaxLimit, age1960_PFER_FREQ_ERROR_LIMIT_MIN,
                   age1960_PFER_FREQ_ERROR_LIMIT_MAX, VI_ERROR_PARAMETER6)



    if(PFERCIFTState == 0)
    {
        if((errStatus = delayVPrintf(instrumentHandle,
                                     "SET:PFER:CONF:STAT%s 0;:"
                                     "SET:PFER:CONF:LEV%s %lg;:"
                                     "SET:PFER:PEAK:LIM%s %lg;:"
                                     "SET:PFER:RMS:LIM%s %lg;:"
                                     "SET:PFER:FERR:LIM%s %lg\n",
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERConfidenceLevel,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERPeakPhaseErrorMaxLimit,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERRmsPhaseErrorMaxLimit,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERFrequencyErrorMaxLimit)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        if((errStatus = delayVPrintf(instrumentHandle,
                                     "SET:PFER:CONF:SLEV%s %lg;:"
                                     "SET:PFER:PEAK:LIM%s %lg;:"
                                     "SET:PFER:RMS:LIM%s %lg;:"
                                     "SET:PFER:FERR:LIM%s %lg\n",
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERConfidenceLevel,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERPeakPhaseErrorMaxLimit,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERRmsPhaseErrorMaxLimit,
                                     age1960_TAPTAGSM_a[thisPtr->TAIndex],
                                     PFERFrequencyErrorMaxLimit)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/****************************************************************************/
ViStatus _VI_FUNC age1960_confPVTMask(
    ViSession  instrumentHandle,
    ViInt16    PVTMaskForPCS)
/****************************************************************************/
/*  This function is used to set the Power Vs. Time measurement PCS ETSI	*/
/*	Limit field. This function is provided because there is an				*/
/*	inconsistency in the GSM specifications that determines the mask		*/
/*	specification for the PCS band. See Help file for expanded description.	*/
/*																			*/
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                     */
/*    ViInt16    PVTMaskForPCS (in)                                         */
/*		The PCS ETSI Limit mask setting.									*/
/*       0       NARROW			(age1960_PVT_MASK_NARROW)                   */
/*       1       RELAXED		(age1960_PVT_MASK_RELAXED)                  */
/*                                                                          */
/****************************************************************************/

{
    ViStatus				errStatus;
    struct age1960_globals	*thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confPVTMask", ",%hu", PVTMaskForPCS);

    CHK_ENUM(PVTMaskForPCS, 1, VI_ERROR_PARAMETER2)

    if((errStatus = delayVPrintf(instrumentHandle,
                                 "SET:PVT:LIM:ETSI:PCS %s\n",
                                 age1960_pvt_mask_for_pcs_a[PVTMaskForPCS])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/****************************************************************************/
ViStatus _VI_FUNC age1960_confORFSLimitSource(
    ViSession  instrumentHandle,
    ViInt16    limitSource)
/****************************************************************************/
/*	This function configures the Output RF Spectrum measurement Limit 		*/
/*	Source field. When set to ETSI the limit mask values are not user 		*/
/*	settable and the default limit values are used. When set to Manual, 	*/
/*	the the Modulation Limits and Switching Limits values used to determine	*/
/*	whether or not a measurement passed are user settable for each offset. 	*/
/*	To set the user defined values, use the functions 						*/
/*	age1960_confORFSSwitchLimits() and age1960_confORFSModLimits().			*/
/*																			*/
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                     */
/*    ViInt16    limitSource(in)                                            */
/*      The limit source setting		.					    			*/
/*       0       ETSI mask				(age1960_ORFS_LIMIT_ETSI)	    	*/
/*       1       Manual mask (custom)	(age1960_ORFS_LIMIT_MAN)	    	*/
/****************************************************************************/

{
    ViStatus				errStatus;
    struct age1960_globals	*thisPtr;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confORFSLimitSource", ",%hu", limitSource);

    CHK_ENUM(limitSource, 1, VI_ERROR_PARAMETER2)

    if((errStatus = delayVPrintf(instrumentHandle,
                                 "SET:ORFS:LIM:SOUR %s\n",
                                 age1960_ORFSLimitSource_a[limitSource])) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_confORFSSwitchLimits(
    ViSession  instrumentHandle,
    ViInt16    measurementBand,
    ViInt32    switchingOffsetPoints,
    ViReal64   _VI_FAR switchingLimitsArray[])
/****************************************************************************/
/*	This function configures the Output RF Spectrum measurement Switching 	*/
/*	Limits pass/fail mask. The limit values are setup for the individual 	*/
/*	measurement band selected, or for the current measurement band. 		*/
/*	Note: the limit values set by this function will only be used when the 	*/
/*	ORFS measurement Limit Source setting is set to Manual (use the 		*/
/*	function age1960_ORFSLimitSource() for changing Limit Source field 		*/
/*	setting).																*/
/*																			*/
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                     */
/*    ViInt16    measurementBand (in)                                       */
/*      The measurement band that mask will be applied to.			     	*/
/*      Constant Name            Value        Description                   */
/*      -----------------------------------------------------------         */
/*      age1960_CURRENT_BAND      0          Current Band Set               */
/*      age1960_PGSM_BAND         1          PGSM 900 band                  */
/*      age1960_EGSM_BAND         2          EGSM band                      */
/*      age1960_DCS1800_BAND      3          DCS1800 band                   */
/*      age1960_PCS1900_BAND      4          PCS1900 band                   */
/*      age1960_GSM850_BAND       5          GSM850 band                    */
/*                                                                          */
/*     ViInt32    switchingOffsetPoints(in)                                 */
/*      The number offset points to be set that are contained in the array  */
/*      switchingLimitsArray[].                                             */
/*     ViReal64   switchingLimitsArray[] (in)                               */
/*      The array of ORFS switching point limit values.		           		*/
/****************************************************************************/

{
    ViStatus				errStatus;
    ViChar					cmd[32];
    struct age1960_globals	*thisPtr;
    int						i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confORFSSwitchLimits",",%hu,%ld,%lg",
                  measurementBand, switchingOffsetPoints, switchingLimitsArray[0]);

    CHK_ENUM(measurementBand, 5, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(switchingOffsetPoints, age1960_ORFSSW_LIM_POINTS_MIN,
                   age1960_ORFSSW_LIM_POINTS_MAX, VI_ERROR_PARAMETER3)

    for(i = 0; i < switchingOffsetPoints; i++)
    {
        CHK_REAL_RANGE(switchingLimitsArray[i],
                       age1960_ORFSSW_LIMIT_MIN,
                       age1960_ORFSSW_LIMIT_MAX,
                       VI_ERROR_PARAMETER4)
    }

    if (measurementBand == age1960_CURRENT_BAND)
    {
        sprintf(cmd, "SET:ORFS:SWIT:LIM:MAN ");
        if((errStatus = age1960_cmdRealArr(instrumentHandle,
                                           cmd,
                                           switchingOffsetPoints,
                                           switchingLimitsArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        sprintf(cmd, "SET:ORFS:SWIT:LIM:MAN%s ", age1960_bandSetting_a[measurementBand]);
        if((errStatus = age1960_cmdRealArr(instrumentHandle,
                                           cmd,
                                           switchingOffsetPoints,
                                           switchingLimitsArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/****************************************************************************/
ViStatus _VI_FUNC age1960_confORFSModLimits(
    ViSession  instrumentHandle,
    ViInt16    measurementBand,
    ViInt32    modulationOffsetPoints,
    ViReal64   _VI_FAR modulationLimitsArray[])
/****************************************************************************/
/*	This function configures the Output RF Spectrum measurement Modulation 	*/
/*	Limits pass/fail mask. The limit values are setup for the individual 	*/
/*	measurement band selected, or for the current measurement band. 		*/
/*	Note: the limit values set by this function will only be used when the 	*/
/*	ORFS measurement Limit Source setting is set to Manual (use the 		*/
/*	function age1960_ORFSLimitSource() for changing Limit Source field 		*/
/*	setting).																*/
/*                                                                          */
/*  PARAMETERS                                                              */
/*    ViSession  instrumentHandle (in)                                      */
/*      Instrument handle returned from age1960_init().                     */
/*    ViInt16    measurementBand (in)                                       */
/*      The measurement band that mask will be applied to.			     	*/
/*      Constant Name            Value        Description                   */
/*      -----------------------------------------------------------         */
/*      age1960_CURRENT_BAND      0          Current Band Set               */
/*      age1960_PGSM_BAND         1          PGSM 900 band                  */
/*      age1960_EGSM_BAND         2          EGSM band                      */
/*      age1960_DCS1800_BAND      3          DCS1800 band                   */
/*      age1960_PCS1900_BAND      4          PCS1900 band                   */
/*      age1960_GSM850_BAND       5          GSM850 band                    */
/*                                                                          */
/*     ViInt32    modulationOffsetPoints(in)                                */
/*      The number of offset points to be set that are contained in the     */
/*	  array modulationLimitsArray[].                                       	*/
/*     ViReal64   modulationLimitsArray[] (in)                              */
/*      The array of ORFS modulation point limit values.		           	*/
/****************************************************************************/

{
    ViStatus				errStatus;
    ViChar					cmd[32];
    struct age1960_globals	*thisPtr;
    int						i;

    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_confORFSModLimits",",%hu,%ld,%lg",
                  measurementBand, modulationOffsetPoints,
                  modulationLimitsArray[0]);

    CHK_ENUM(measurementBand, 5, VI_ERROR_PARAMETER2)

    CHK_LONG_RANGE(modulationOffsetPoints, age1960_ORFSMOD_LIM_POINTS_MIN,
                   age1960_ORFSMOD_LIM_POINTS_MAX, VI_ERROR_PARAMETER3)

    for(i = 0; i < modulationOffsetPoints; i++)
    {
        CHK_REAL_RANGE(modulationLimitsArray[i],
                       age1960_ORFSMOD_LIMIT_MIN,
                       age1960_ORFSMOD_LIMIT_MAX,
                       VI_ERROR_PARAMETER4)
    }

    if (measurementBand == age1960_CURRENT_BAND)
    {
        sprintf(cmd, "SET:ORFS:MOD:LIM:MAN ");
        if((errStatus = age1960_cmdRealArr(instrumentHandle,
                                           cmd,
                                           modulationOffsetPoints,
                                           modulationLimitsArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }
    else
    {
        sprintf(cmd, "SET:ORFS:MOD:LIM:MAN%s ",
                age1960_bandSetting_a[measurementBand]);
        if((errStatus = age1960_cmdRealArr(instrumentHandle,
                                           cmd,
                                           modulationOffsetPoints,
                                           modulationLimitsArray)) < VI_SUCCESS)
            return statusUpdate(instrumentHandle, thisPtr, errStatus);
    }

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


/*******************************************************************************/
ViStatus _VI_FUNC age1960_fetchORFSSummaryPassFail_Q(
    ViSession  instrumentHandle,
    ViPReal64   overallPFSummary,
    ViPReal64   switchingPFSummary,
    ViPReal64   modulationPFSummary)
/********************************************************************************/
/*	This function returns the Output RF Spectrum measurement pass/fail 			*/
/*	summary results when those results are evaluated against the switching and 	*/
/*	modulation limits masks. The following values are returned:					*/
/*																				*/
/*	1) The summary pass fail result when switching AND modulation limits masks 	*/
/*	are evaluated.																*/
/*	2) The summary pass/fail result when only the switching limits mask is 		*/
/*	evaluated.																	*/
/*	3) The summary pass/fail result when only the modulation limits mask is 	*/
/*	evaluated.																	*/
/*																				*/
/*  PARAMETERS                                                               	*/
/*    ViSession  instrumentHandle (in)                                       	*/
/*      Instrument handle returned from age1960_init().                     	*/
/*    ViPReal64  overallPFSummary(out)                                      	*/
/*		The overall Pass/Fail result when both switching limits AND modulation	*/
/*		limits masks are evaluated. A NAN will be returned if the measurement	*/
/*		does not complete, or neither switching or modulation offset limits 	*/
/*		were enabled. If no ORFS due to switching offsets are enabled, the 		*/
/*		measurement will pass if all ORFS due to modulation results that were 	*/
/*		enabled passed (and vice-versa).										*/
/*    ViPReal64  switchingPFSummary(out)                                   		*/
/*      The switching Pass/Fail result when switching mask limits are 			*/
/*		evaluated. A NAN will be returned if the measurement does not complete	*/
/*		or if no switching offsets are enabled.									*/
/*    ViPReal64  modulationPFSummary(out)                                   	*/
/*		The modulation Pass/Fail result when modulation mask limits are 		*/
/*		evaluated. A NAN will be returned if the measurement does not complete	*/
/*		or if no modulation offsets are enabled.								*/
/********************************************************************************/
{
    ViStatus	errStatus;
    struct age1960_globals	*thisPtr;


    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "fetchORFSSummaryPassFail_Q", "");

    /* Default all measurement results to NAN */
    *overallPFSummary= age1960_NOT_A_NUMBER;
    *switchingPFSummary= age1960_NOT_A_NUMBER;
    *modulationPFSummary= age1960_NOT_A_NUMBER;

    if((errStatus = sendQueryRequest(instrumentHandle, "FETC:ORFS:LIM?"
                                     ";:FETC:ORFS:SWIT:LIM?"
                                     ";:FETC:ORFS:MOD:LIM?",
                                     ReadTimeout(instrumentHandle,"ORFS"))) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if((errStatus = viScanf(instrumentHandle, "%lf;%lf;%lf%*t",
                            overallPFSummary,
                            switchingPFSummary,
                            modulationPFSummary)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    if((errStatus = cleanupMAVafterRead(instrumentHandle)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */

/*****************************************************************************/
ViStatus _VI_FUNC age1960_burstTimingError_Q(
    ViSession  instrumentHandle,
    ViPReal64 burstTimingError)
/*****************************************************************************/
/*  This function returns the Burst Timing Error.							 */
/*                                                                           */
/*  PARAMETERS                                                               */
/*    ViSession  instrumentHandle (in)                                       */
/*      Instrument handle returned from age1960_init().                      */
/*	  ViPReal64	 burstTimingError (out)										 */
/*		The returned Burst Timing Error.									 */
/*****************************************************************************/

{

    ViStatus				errStatus;
    struct age1960_globals	*thisPtr;


    GET_GLOBALS(instrumentHandle, thisPtr)

    age1960_TRACE(thisPtr, "age1960_burstTimingError_Q","");

    *burstTimingError = age1960_NOT_A_NUMBER;

    /* Get Absolute power */
    if((errStatus = delayVQueryf(instrumentHandle,
                                 "CALL:STAT:TCH:TERR?\n",
                                 "%lf%*t",
                                 burstTimingError)) < VI_SUCCESS)
        return statusUpdate(instrumentHandle, thisPtr, errStatus);

    return statusUpdate(instrumentHandle, thisPtr, VI_SUCCESS);

}        /* ----- end of function ----- */


