/*****************************************************************************/
/*  age1960.h                                                                */
/*  Copyright (C) 1999 - 2002 Agilent Technologies Company                   */
/*---------------------------------------------------------------------------*/
/*  Driver for age1960, GSM Mobile Test for 8960 Series 10 E5515             */
/*  Driver Version: A.05.00                                                  */
/*****************************************************************************/

#ifndef age1960_INCLUDE
#define age1960_INCLUDE
#include "vpptype.h"


/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

#define age1960_REV_CODE    "A.05.00"                     /* Driver Revision */

    /*****************************************************************************/
    /*  STANDARD SECTION                                                         */
    /*  Constants and function prototypes for Agilent standard functions.        */
    /*****************************************************************************/

    /***************************************************/
    /*  Standard constant error conditions returned    */
    /*  by driver functions.                           */
    /*    Agilent Common Error numbers start at BFFC0D00    */
    /*    The parameter errors extend the number of    */
    /*      errors over the eight defined in VPP 3.4   */
    /***************************************************/

#define age1960_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define age1960_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define age1960_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define age1960_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define age1960_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define age1960_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */
#define age1960_INSTR_NO_LAST_COMMA       (_VI_ERROR+0x3FFC0D08L) /* 0xBFFC0D08 */

#define age1960_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define age1960_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define age1960_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define age1960_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define age1960_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define age1960_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define age1960_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define age1960_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define age1960_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define age1960_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

#define age1960_CMDSTRING_Q_MIN              2
#define age1960_CMDSTRING_Q_MAX          32767
#define age1960_CMDINT_MIN         -2147483647
#define age1960_CMDINT_MAX          2147483647
#define age1960_CMDREAL_MIN           -1E+300
#define age1960_CMDREAL_MAX            1E+300

    /***************************************************/
    /*  Constants used by system status functions      */
    /*    These defines are the available status       */
    /*    registers that can be read or programmed.    */
    /*    They are instrument specific.                */
    /***************************************************/

#define age1960_QUES_SUMMARY                  0
#define age1960_QUES_CALL                     1
#define age1960_QUES_ERROR                    2
#define age1960_QUES_HARDWARE                 3
#define age1960_QUES_CALL_GSM                 4
#define age1960_QUES_ERROR_GSM                5

#define age1960_OPER_SUMMARY                  0
#define age1960_OPER_NMRR                     1
#define age1960_OPER_CALL                     2
#define age1960_OPER_NMRR_GSM                 3
#define age1960_OPER_CALL_GSM                 4

    /***************************************************/
    /*  Constants used by system status functions      */
    /*    These defines are bit numbers which define   */
    /*    the operation and questionable registers.    */
    /*    They are instrument specific.                */
    /***************************************************/

#define age1960_REGISTER_MIN                 0
#define age1960_REGISTER_MAX                 32767

#define age1960_STD_REGISTER_MIN             0
#define age1960_STD_REGISTER_MAX             255

#define age1960_ESE_OPC                      1
#define age1960_ESE_AVAIL1                   2
#define age1960_ESE_QUERY_ERR                4
#define age1960_ESE_DEVICE_ERR               8
#define age1960_ESE_EXEC_ERR                16
#define age1960_ESE_CMD_ERR                 32
#define age1960_ESE_AVAIL6                  64
#define age1960_ESE_PWR_ON                 128

#define age1960_STB_OPC                      1
#define age1960_STB_AVAIL1                   2
#define age1960_STB_ERR_QUEUE                4
#define age1960_STB_QUES_SUM                 8
#define age1960_STB_MAV                     16
#define age1960_STB_ESE_SUM                 32
#define age1960_STB_RQS                     64
#define age1960_STB_OPER_SUM               128

#define age1960_QUES_SUM_AVAIL0              1
#define age1960_QUES_SUM_ERRORS              2
#define age1960_QUES_SUM_AVAIL2              4
#define age1960_QUES_SUM_AVAIL3              8
#define age1960_QUES_SUM_AVAIL4             16
#define age1960_QUES_SUM_AVAIL5             32
#define age1960_QUES_SUM_AVAIL6             64
#define age1960_QUES_SUM_AVAIL7            128
#define age1960_QUES_SUM_AVAIL8            256
#define age1960_QUES_SUM_AVAIL9            512
#define age1960_QUES_SUM_CALL             1024
#define age1960_QUES_SUM_HW               2048
#define age1960_QUES_SUM_AVAIL12          4096
#define age1960_QUES_SUM_AVAIL13          8192
#define age1960_QUES_SUM_AVAIL14         16384
#define age1960_QUES_SUM_AVAIL15         32768

#define age1960_OPER_SUM_AVAIL0              1
#define age1960_OPER_SUM_AVAIL1              2
#define age1960_OPER_SUM_AVAIL2              4
#define age1960_OPER_SUM_AVAIL3              8
#define age1960_OPER_SUM_AVAIL4             16
#define age1960_OPER_SUM_AVAIL5             32
#define age1960_OPER_SUM_AVAIL6             64
#define age1960_OPER_SUM_AVAIL7            128
#define age1960_OPER_SUM_AVAIL8            256
#define age1960_OPER_SUM_NMRR              512
#define age1960_OPER_SUM_CALL             1024
#define age1960_OPER_SUM_AVAIL11          2048
#define age1960_OPER_SUM_SYSSYNC          4096
#define age1960_OPER_SUM_AVAIL13          8192
#define age1960_OPER_SUM_AVAIL14         16384
#define age1960_OPER_SUM_AVAIL15         32768

#define age1960_QUES_ERR_AVAIL0              1
#define age1960_QUES_ERR_100                 2
#define age1960_QUES_ERR_200                 4
#define age1960_QUES_ERR_300                 8
#define age1960_QUES_ERR_400                16
#define age1960_QUES_ERR_500                32
#define age1960_QUES_ERR_600                64
#define age1960_QUES_ERR_700               128
#define age1960_QUES_ERR_800               256
#define age1960_QUES_ERR_900               512
#define age1960_QUES_ERR_AVAIL10          1024
#define age1960_QUES_ERR_AVAIL11          2048
#define age1960_QUES_ERR_AVAIL12          4096
#define age1960_QUES_ERR_AVAIL13          8192
#define age1960_QUES_ERR_AVAIL14          6384
#define age1960_QUES_ERR_AVAIL15         32768

#define age1960_OPER_NMRR_AVAIL0             1
#define age1960_OPER_NMRR_TXP                2
#define age1960_OPER_NMRR_PVT                4
#define age1960_OPER_NMRR_PFER               8
#define age1960_OPER_NMRR_ORFS              16
#define age1960_OPER_NMRR_AAUD              32
#define age1960_OPER_NMRR_DAUD              64
#define age1960_OPER_NMRR_FBER             128
#define age1960_OPER_NMRR_BER              256
#define age1960_OPER_NMRR_IQT              512
#define age1960_OPER_NMRR_DPOW            1024
#define age1960_OPER_NMRR_AVAIL11         2048
#define age1960_OPER_NMRR_AVAIL12         4096
#define age1960_OPER_NMRR_AVAIL13         8192
#define age1960_OPER_NMRR_AVAIL14        16384
#define age1960_OPER_NMRR_AVAIL15        32768

#define age1960_QUES_CALL_AVAIL0             1
#define age1960_QUES_CALL_DATA               2
#define age1960_QUES_CALL_RADIO              4
#define age1960_QUES_CALL_IMM                8
#define age1960_QUES_CALL_CHAN              16
#define age1960_QUES_CALL_HANDOVER          32
#define age1960_QUES_CALL_NO_RESP           64
#define age1960_QUES_CALL_FRAMES           128
#define age1960_QUES_CALL_ID               256
#define age1960_QUES_CALL_AVAIL9           512
#define age1960_QUES_CALL_AVAIL10         1024
#define age1960_QUES_CALL_AVAIL11         2048
#define age1960_QUES_CALL_AVAIL12         4096
#define age1960_QUES_CALL_AVAIL13         8192
#define age1960_QUES_CALL_AVAIL14        16384
#define age1960_QUES_CALL_IT15           32768

#define age1960_OPER_CALL_AVAIL0             1
#define age1960_OPER_CALL_IDLE               2
#define age1960_OPER_CALL_CONNECTED          4
#define age1960_OPER_CALL_ALERTING           8
#define age1960_OPER_CALL_BCH_CHANGING      16
#define age1960_OPER_CALL_TCH_CHANGING      32
#define age1960_OPER_CALL_CNT_CHANGING      64
#define age1960_OPER_CALL_BS_ORIGINATING   128
#define age1960_OPER_CALL_BS_DISCONNECTING 256
#define age1960_OPER_CALL_AVAIL9           512
#define age1960_OPER_CALL_AVAIL10         1024
#define age1960_OPER_CALL_AVAIL11         2048
#define age1960_OPER_CALL_AVAIL12         4096
#define age1960_OPER_CALL_AVAIL13         8192
#define age1960_OPER_CALL_AVAIL14        16384
#define age1960_OPER_CALL_AVAIL15        32768

#define age1960_QUES_HW_AVAIL0               1
#define age1960_QUES_HW_AVAIL1               2
#define age1960_QUES_HW_AVAIL2               4
#define age1960_QUES_HW_AVAIL3               8
#define age1960_QUES_HW_PWRUP_ST_FAILED     16
#define age1960_QUES_HW_AVAIL5              32
#define age1960_QUES_HW_AVAIL6              64
#define age1960_QUES_HW_AVAIL7             128
#define age1960_QUES_HW_AVAIL8             256
#define age1960_QUES_HW_AVAIL9             512
#define age1960_QUES_HW_AVAIL10           1024
#define age1960_QUES_HW_AVAIL11           2048
#define age1960_QUES_HW_AVAIL12           4096
#define age1960_QUES_HW_AVAIL13           8192
#define age1960_QUES_HW_AVAIL14          16384
#define age1960_QUES_HW_AVAIL15          32768

    /***************************************************/
    /*  Constants used by function age1960_timeOut	   */
    /***************************************************/

#define age1960_TIMEOUT_MAX         120000L
#define age1960_TIMEOUT_MIN         0L

    /***************************************************/
    /*  Required plug and play functions from VPP-3.1  */
    /***************************************************/

    ViStatus _VI_FUNC age1960_init
    (ViRsrc     resourceName,
     ViBoolean  IDQuery,
     ViBoolean  resetDevice,
     ViPSession instrumentHandle);

    ViStatus _VI_FUNC age1960_close
    (ViSession  instrumentHandle);

    ViStatus _VI_FUNC age1960_reset
    (ViSession  instrumentHandle);

    ViStatus _VI_FUNC age1960_self_test
    (ViSession  instrumentHandle,
     ViPInt16   selfTestResult,
     ViPString  selfTestMessage);

    ViStatus _VI_FUNC age1960_error_query
    (ViSession  instrumentHandle,
     ViPInt32   errorCode,
     ViPString  errorMessage);

    ViStatus _VI_FUNC age1960_error_message
    (ViSession  instrumentHandle,
     ViStatus   statusCode,
     ViPString  message);

    ViStatus _VI_FUNC age1960_revision_query
    (ViSession  instrumentHandle,
     ViPString  instrumentDriverRevision,
     ViPString  firmwareRevision);


    /***************************************************/
    /*  Agilent standard utility functions             */
    /***************************************************/

    ViStatus _VI_FUNC age1960_timeOut
    (ViSession  instrumentHandle,
     ViInt32    setTimeOut);

    ViStatus _VI_FUNC age1960_timeOut_Q
    (ViSession  instrumentHandle,
     ViPInt32   timeOut);

    ViStatus _VI_FUNC age1960_errorQueryDetect
    (ViSession  instrumentHandle,
     ViBoolean  setErrorQueryDetect);

    ViStatus _VI_FUNC age1960_errorQueryDetect_Q
    (ViSession  instrumentHandle,
     ViPBoolean errorQueryDetect);

    ViStatus _VI_FUNC age1960_dcl
    (ViSession  instrumentHandle);

    ViStatus _VI_FUNC age1960_opc_Q
    (ViSession  instrumentHandle,
     ViPBoolean instrumentReady);


    /********************************************************************/
    /*  Agilent standard status functions (modified for multi-registers */
    /********************************************************************/

    ViStatus _VI_FUNC age1960_readStatusByte_Q
    (ViSession  instrumentHandle,
     ViPInt16   statusByte);

    ViStatus _VI_FUNC age1960_stdEvent_Q
    (ViSession  instrumentHandle,
     ViPInt32   stdEventRegister);

    ViStatus _VI_FUNC age1960_operEvent_Q
    (ViSession  instrumentHandle,
     ViInt16    operationName,
     ViPInt32   operationEventRegister);

    ViStatus _VI_FUNC age1960_operCond_Q
    (ViSession  instrumentHandle,
     ViInt16    operationName,
     ViPInt32   operationConditionRegister );

    ViStatus _VI_FUNC age1960_quesEvent_Q
    (ViSession  instrumentHandle,
     ViInt16    questionableName,
     ViPInt32   questionableEventRegister );

    ViStatus _VI_FUNC age1960_quesCond_Q
    (ViSession  instrumentHandle,
     ViInt16    questionableName,
     ViPInt32   questionableConditionRegister );

    ViStatus _VI_FUNC age1960_quesRegisterEnable
    (ViSession  instrumentHandle,
     ViInt16    questionableName,
     ViInt32    quesPosTransition,
     ViInt32    quesNegTransition,
     ViInt32    quesEnableBits);

    ViStatus _VI_FUNC age1960_operRegisterEnable
    (ViSession  instrumentHandle,
     ViInt16    operationName,
     ViInt32    operPosTransition,
     ViInt32    operNegTransition,
     ViInt32    operEnableBits);

    ViStatus _VI_FUNC age1960_stdEventEnable
    (ViSession  instrumentHandle,
     ViInt32    stdEnableBits);

    /***************************************************/
    /*  Agilent standard command passthrough functions */
    /***************************************************/

    ViStatus _VI_FUNC age1960_cmd
    (ViSession  instrumentHandle,
     ViString   sendStringCommand);

    ViStatus _VI_FUNC age1960_cmdString_Q
    (ViSession  instrumentHandle,
     ViString   queryStringCommand,
     ViInt32    stringSize,
     ViPString  stringResult);

    ViStatus _VI_FUNC age1960_cmdInt
    (ViSession  instrumentHandle,
     ViString   sendIntegerCommand,
     ViInt32    sendInteger);

    ViStatus _VI_FUNC age1960_cmdInt16_Q
    (ViSession  instrumentHandle,
     ViString   queryI16Command,
     ViPInt16   i16Result);

    ViStatus _VI_FUNC age1960_cmdInt32_Q
    (ViSession  instrumentHandle,
     ViString   queryI32Command,
     ViPInt32   i32Result);

    ViStatus _VI_FUNC age1960_cmdInt16Arr_Q
    (ViSession  instrumentHandle,
     ViString   queryI16ArrayCommand,
     ViInt32    i16ArraySize,
     ViInt16 _VI_FAR i16ArrayResult[],
     ViPInt32   i16ArrayCount);

    ViStatus _VI_FUNC age1960_cmdInt32Arr_Q
    (ViSession  instrumentHandle,
     ViString   queryI32ArrayCommand,
     ViInt32    i32ArraySize,
     ViInt32 _VI_FAR i32ArrayResult[],
     ViPInt32   i32ArrayCount);

    ViStatus _VI_FUNC age1960_cmdReal
    (ViSession  instrumentHandle,
     ViString   sendRealCommand,
     ViReal64   sendReal);

    ViStatus _VI_FUNC age1960_cmdReal64_Q
    (ViSession  instrumentHandle,
     ViString   queryRealCommand,
     ViPReal64  realResult);

    ViStatus _VI_FUNC age1960_cmdReal64Arr_Q
    (ViSession  instrumentHandle,
     ViString   realArrayCommand,
     ViInt32    realArraySize,
     ViReal64 _VI_FAR realArrayResult[],
     ViPInt32   realArrayCount);


    /* End of Agilent standard declarations */
    /*---------------------------------------------------------------------------*/


    /*****************************************************************************/
    /*  INSTRUMENT SPECIFIC SECTION                                              */
    /*  Constants and function prototypes for instrument specific functions.     */
    /*****************************************************************************/

    /***************************************************/
    /*         Instrument specific constants           */
    /***************************************************/

#define age1960_INSTR_ERROR_ARRAY_TOO_SMALL   	(_VI_ERROR+0x3FFC0D2AL) /* 0xBFFC0D2A */
#define age1960_INSTR_CANNOT_ORIG_NOW         	(_VI_ERROR+0x3FFC0D2BL) /* 0xBFFC0D2B */
#define age1960_INSTR_ERROR_CALL_FAILED       	(_VI_ERROR+0x3FFC0D2CL) /* 0xBFFC0D2C */
#define age1960_INSTR_ERROR_CPTO              	(_VI_ERROR+0x3FFC0D2DL) /* 0xBFFC0D2D */
#define age1960_INSTR_ERROR_INITTO            	(_VI_ERROR+0x3FFC0D2EL) /* 0xBFFC0D2E */
#define age1960_INSTR_ERROR_ORFS_MEAS_COUNT   	(_VI_ERROR+0x3FFC0D2FL) /* 0xBFFC0D2F */
#define age1960_INSTR_ERROR_ARRAY_TOO_LARGE   	(_VI_ERROR+0x3FFC0D30L) /* 0xBFFC0D30 */
#define age1960_INSTR_ERROR_AUTORANGE_FAILED  	(_VI_ERROR+0x3FFC0D31L) /* 0xBFFC0D31 */
#define age1960_INSTR_CANNOT_CHNGBAND_NOW     	(_VI_ERROR+0x3FFC0D32L) /* 0xBFFC0D32 */
#define age1960_INSTR_TOO_MANY_NONCONCURRENT  	(_VI_ERROR+0x3FFC0D33L) /* 0xBFFC0D33 */
#define age1960_INSTR_UNDEFINED_DATA          	(_VI_ERROR+0x3FFC0D34L) /* 0xBFFC0D34 */
#define age1960_INSTR_TA_NOT_AVAILABLE        	(_VI_ERROR+0x3FFC0D35L) /* 0xBFFC0D35 */
#define age1960_INSTR_TA_NOT_SWITCHABLE       	(_VI_ERROR+0x3FFC0D36L) /* 0xBFFC0D36 */
#define age1960_INSTR_TA_NOT_LICENSE          	(_VI_ERROR+0x3FFC0D37L) /* 0xBFFC0D37 */
#define age1960_INSTR_ERROR_NSUP_FUNC         	(_VI_ERROR+0x3FFC0D38L) /* 0xBFFC0D38 */
#define age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED  (_VI_ERROR+0X3FFC0D39L) /* 0XBFFC0D39 */

#define age1960_NOT_A_NUMBER                    9.91E+37	/* IEEE not a number */

#define age1960_NO_RESULT_AVAIL                 1			/* No result available */

#define age1960_INTEGRITY_MIN                   0			/* the min integrity value that can be returned */
#define age1960_INTEGRITY_MAX                  23			/* the max integrity value that can be returned */

#define age1960_ARRAY_MIN                       1			/* min array size is always 1    */
#define age1960_ARRAY_MAX                     500			/* max array size for instrument specific functions is 500 */
#define age1960_CP_TIMEOUT_MIN                  0.0			/* min seconds for a call processing fcn to complete */
#define age1960_CP_TIMEOUT_MAX                120.0			/* max seconds for call processing function to complete */

#define age1960_INIT_TIMEOUT_MIN                0.1			/* min seconds for a meas fcn to complete */
#define age1960_INIT_TIMEOUT_MAX              120.0			/* max seconds for a meas fcn to complete */

#define age1960_MEAS_TIMEOUT_MIN                0.1			/*  min seconds for a measurement timeout */
#define age1960_MEAS_TIMEOUT_MAX              999.9			/* max seconds for a measurement timeout */

#define age1960_MAV_TIMEOUT_MIN                 1			/* min seconds for HP-IB buffer to clear */
#define age1960_MAV_TIMEOUT_MAX                30			/* max seconds for HP-IB buffer to clear */

#define age1960_NUMBER_OF_MEAS_FCNS            10			/* 10 measurement fcns*/

#define age1960_WAIT                           -2			/* Wait for measurement function to complete */
#define age1960_NONEINIT                       -1			/* No measurement function initiated */

#define age1960_AAUDIO                          0			/* Analog Audio Meas Fcn */
#define age1960_BER                             1			/* Bit Error Meas Fcn */
#define age1960_DAUDIO                          2			/* Decoded Audio Meas Fcn */
#define age1960_FBER                            3			/* Fast Bit Error Meas Fcn */
#define age1960_ORFS                            4			/* ORFSpectrum Meas Fcn */
#define age1960_PFER                            5			/* Phase Freq Error Meas Fcn */
#define age1960_PVTIME                          6			/* Power vs Time Meas Fcn */
#define age1960_TXPOWER                         7			/* TX Power Meas Fcn */
#define age1960_DPOWER                          8			/* Dunamic Power Meas Fcn */
#define age1960_IQTUNING                        9			/* IQ Tuning Meas Fcn */

#define age1960_TEST_MODE                       0			/* Test operating mode */
#define age1960_ACELL_MODE                      1			/* Active Cell operating mode */

#define age1960_UNKNOWN_BAND                   -1			/* No band set */
#define age1960_CURRENT_BAND                    0			/* Current band */
#define age1960_PGSM_BAND                       1			/* PGSM band */
#define age1960_EGSM_BAND                       2			/* EGSM band */
#define age1960_DCS1800_BAND                    3			/* DCS 1800 band */
#define age1960_PCS1900_BAND                    4			/* PCS 1900 band */
#define age1960_GSM850_BAND                     5			/* GSM 850 band */

#define age1960_PGSM_MIN_CH                     1			/* PGSM min channel No. */
#define age1960_PGSM_MAX_CH                   124			/* PGSM max channel No. */
#define age1960_EGSM_MIN_CH_1                   0			/* EGSM min 1 channel No. */
#define age1960_EGSM_MAX_CH_1                 124			/* EGSM max 1 channel No. */
#define age1960_EGSM_MIN_CH_2                 975			/* EGSM min 2 channel No. */
#define age1960_EGSM_MAX_CH_2                1023			/* EGSM max 2 channel No. */
#define age1960_DCS1800_MIN_CH                512			/* DCS 1800 min channel No. */
#define age1960_DCS1800_MAX_CH                885			/* DCS 1800 max channel No. */
#define age1960_PCS1900_MIN_CH                512			/* PCS 1900 min channel No. */
#define age1960_PCS1900_MAX_CH                810			/* PCS 1900 max channel No. */
#define age1960_GSM850_MIN_CH				  128	    	/* GSM 850 min channel No. */
#define age1960_GSM850_MAX_CH				  251	    	/* GSM 850 max channel No. */

#define age1960_PGSM_MIN_TXL                    0			/* PGSM min TX Level */
#define age1960_PGSM_MAX_TXL                   31			/* PGSM max TX Level */
#define age1960_EGSM_MIN_TXL                    0			/* EGSM min TX Level */
#define age1960_EGSM_MAX_TXL                   31			/* EGSM max TX Level */
#define age1960_DCS1800_MIN_TXL                 0			/* DCS 1800 min TX Level */
#define age1960_DCS1800_MAX_TXL                31			/* DCS 1800 max TX Level */

#define age1960_PCS1900_MIN_TXL_1               0			/* PCS 1900 Rng 1 min TX Level */
#define age1960_PCS1900_MAX_TXL_1              15			/* PCS 1900 Rng 1 max TX Level */
#define age1960_PCS1900_MIN_TXL_2              30			/* PCS 1900 Rng 2 min TX Level */
#define age1960_PCS1900_MAX_TXL_2              31			/* PCS 1900 Rng 2 max TX Level */

#define age1960_GSM850_MIN_TXL                  0			/* GSM 850 min TX Level */
#define age1960_GSM850_MAX_TXL                 31			/* GSM 850 max TX Level */

#define age1960_TADVANCE_MIN                    0			/* Timing Advance minimum */
#define age1960_TADVANCE_MAX                   63			/* Timing Advance maxinum */

#define age1960_SACCHREADINGS_MIN               1			/* Min number of SACCH readings to be made */
#define age1960_SACCHREADINGS_MAX            1000			/* Max number of SACCH readings to be made */

#define age1960_TRIG_DELAY_MIN           -0.00231			/* min trigger delay in seconds */
#define age1960_TRIG_DELAY_MAX           +0.00231			/* max trigger delay in seconds */

#define age1960_TIMESLOT_MIN                    3			/* Timeslot minimum */
#define age1960_TIMESLOT_MAX                    5			/* Timeslot maximum */

#define age1960_EXPECTED_POWER_MIN            -60.0			/* Expected Power minimum */
#define age1960_EXPECTED_POWER_MAX            +53.0			/* Expected Power maximum */

#define age1960_BA_TABLE_PTS_MIN                0			/* BA Table minimum pts */
#define age1960_BA_TABLE_PTS_MAX               16			/* BA Table maxinum pts */

#define age1960_MEAS_COUNT_MIN                  0			/* minimum number of measurements that can be made */
#define age1960_MEAS_COUNT_MAX                999			/* maximum number of measurements that can be made */

#define age1960_FBER_COUNT_MIN                  1			/* minimum number of bits that can be tested for FBER*/
#define age1960_FBER_COUNT_MAX             999000			/* maximum number of bits that can be tested for FBER*/

#define age1960_BER_COUNT_MIN                   1			/* minimum number of bits that can be tested for BER*/
#define age1960_BER_COUNT_MAX              999000			/* maximum number of bits that can be tested for BER*/

#define age1960_FBER_DELAY_MIN                  0			/* minimum FBER frame delay */
#define age1960_FBER_DELAY_MAX                 26			/* maximum FBER frame delay */

#define age1960_BER_DELAY_MIN                   1			/* minimum BER frame delay */
#define age1960_BER_DELAY_MAX                  15			/* maximum BER frame delay */

#define age1960_TSC0_BURST                      0			/* TSC0 expected burst */
#define age1960_TSC1_BURST                      1			/* TSC1 expected burst */
#define age1960_TSC2_BURST                      2			/* TSC2 expected burst */
#define age1960_TSC3_BURST                      3			/* TSC3 expected burst */
#define age1960_TSC4_BURST                      4			/* TSC4 expected burst */
#define age1960_TSC5_BURST                      5			/* TSC5 expected burst */
#define age1960_TSC6_BURST                      6			/* TSC6 expected burst */
#define age1960_TSC7_BURST                      7			/* TSC7 expected burst */
#define age1960_RACH_BURST                      8			/* RACH expected burst */

#define age1960_CELL_POWER_MIN               -127.0			/* minimum cell power */
#define age1960_CELL_POWER_MAX                -10.0			/* maximum cell power */

#define age1960_CELL_1                          1			/* serving cell 1 */
#define age1960_CELL_2                          2			/* serving cell 2 */

#define age1960_MAN_RCV_CONTROL                 0			/* Manual receiver control */
#define age1960_AUTO_RCV_CONTROL                1			/* Auto receiver control */

#define age1960_TRIG_AUTO                       0			/* Auto trigger source */
#define age1960_TRIG_PROT                       1			/* Protocol trigger source */
#define age1960_TRIG_RF_RISE                    2			/* RF Rise trigger source */
#define age1960_TRIG_IMMEDIATE                  3			/* Immediate trigger source */

#define age1960_BSYNC_MID                       0			/* Midample burst sync */
#define age1960_BSYNC_AMPL                      1			/* Amplitude burst sync */
#define age1960_BSYNC_NONE                      2			/* No burst sync */

#define age1960_PVT_POINTS_MIN                  0			/* minimum number of pvt offsets */
#define age1960_PVT_POINTS_MAX                  12			/* maximum number of pvt offsets */

#define age1960_PVT_OFFSET_MIN          -0.000050			/* minimum pvt offset time in seconds */
#define age1960_PVT_OFFSET_MAX           0.000590			/* maximum pvt offset time in seconds */

#define age1960_ORFSSW_POINTS_MIN               0			/* minimum number of ORFS switching frequencies */
#define age1960_ORFSSW_POINTS_MAX               8			/* maximum number of ORFS switching frequencies */

#define age1960_ORFSMOD_POINTS_MIN              0			/* minimum number of ORFS modulation frequencies */
#define age1960_ORFSMOD_POINTS_MAX             22			/* maximum number of ORFS modulation frequencies */

#define age1960_ORFSFREQ_MIN_1           -1800000.0			/* minimum ORFS Frequency in Hz, range 1 */
#define age1960_ORFSFREQ_MAX_1                -10.0			/* maximum ORFS Frequency in Hz, range 1 */
#define age1960_ORFSFREQ_MIN_2                 10.0			/* minimum ORFS Frequency in Hz, range 2 */
#define age1960_ORFSFREQ_MAX_2            1800000.0			/* maximum ORFS Frequency in Hz, range 2 */

#define age1960_EXP_VOLT_MIN                  0.010			/* minimum expected audio measurement range in volts peak */
#define age1960_EXP_VOLT_MAX                   20.0			/* maximum expected audio measurement range in volts peak */

#define age1960_AAUDFLT_FREQ_MIN              200.0			/* minimum bandpass analog audio frequency in Hz */
#define age1960_AAUDFLT_FREQ_MAX             8000.0			/* maximum bandpass analog audio frequency in Hz */

#define age1960_DAUDFLT_FREQ_MIN              200.0			/* minimum bandpass decoded audio frequency in Hz */
#define age1960_DAUDFLT_FREQ_MAX             3600.0			/* maximum bandpass decoded audio frequency in Hz */

#define age1960_AMPOFFSET_ITEMS_MIN             0			/* min items sent of zero turns off amplitude offset */
#define age1960_AMPOFFSET_ITEMS_MAX            20			/* max items sent */

#define age1960_OFFSET_FREQ_MIN       292500000.0			/* Offset Frequency minimum */
#define age1960_OFFSET_FREQ_MAX      2700000000.0			/* Offset Frequency maximum */

#define age1960_PATH_GAIN_MIN              -100.0			/* minimum path offset (gain) in dB */
#define age1960_PATH_GAIN_MAX              +100.0			/* maximum path offset (gain) in dB */

#define age1960_AUTORANGE_TRIES_MIN             1			/* minimum autorange tries */
#define age1960_AUTORANGE_TRIES_MAX           100			/* maximum autorange tries */

#define age1960_EXPECTED_PWR_MIN              -60.0			/* minimum expected power setting */
#define age1960_EXPECTED_PWR_MAX               53.0			/* maximum expected power setting */

#define age1960_BER_TYPE_IA                     0			/* Type IA BER Measurement */
#define age1960_BER_TYPE_II                     1			/* Type II BER Measurement */
#define age1960_BER_TYPE_IB                     2			/* Type IB BER Measurement */
#define age1960_BER_RES_TYPE_IA                 3			/* Residual Type IA BER Measurement */
#define age1960_BER_RES_TYPE_II                 4			/* Residual Type II BER Measurement */
#define age1960_BER_RES_TYPE_IB                 5			/* Residual Type IB BER Measurement */

#define age1960_CONN_STATE_UNKNOWN			   -1      		/* Connection status UNKNOWN */
#define age1960_CALL_CONNECTED                  0			/* Call at the the connected state */
#define age1960_CALL_ALERTING                   1			/* Call at the alerting state */
#define age1960_CALL_IDLE                       2			/* Call at the idle state */

#define age1960_WAITFOR_NMRR                    0			/* Wait for a specific NMRR state */
#define age1960_WAITFOR_CALL                    1			/* Wait for call connected state */
#define age1960_WAITFOR_MEAS                    2			/* Wait for a any NMRR state */

#define age1960_HOLDOFF_TIME_MIN                0.0			/* min seconds for BER and FBER to holdoff the measurement */
#define age1960_HOLDOFF_TIME_MAX                5.0			/* max seconds for BER and FBER to holdoff the measurement */

#define age1960_PVTPLOT_ARRAY_MIN            2220			/* min number of points from PVT plot, for now fixed */
#define age1960_PVTPLOT_ARRAY_MAX            5000			/* Max number of points from PVT plot, for now fixed */

#define age1960_IQ_REF_NEG67K                   0			/* All zeros (-67.7) */
#define age1960_IQ_REF_ZEROK                    1			/* Carrier  (0kHz)  */
#define age1960_IQ_REF_PLUS67K                  2			/* All ones (+67.7) */
#define age1960_IQ_REF_AUTO                     3			/* Function will reference to the highest level  */

#define age1960_IQ_ARRAY_MIN                    9			/* The minimum array size for the IQ tuning results */
#define age1960_IQ_ARRAY_MAX				  500			/* The maximum array size for the IQ tuning results */

#define age1960_LOOPBACK_OFF                    0			/* Turn Loopback off  */
#define age1960_LOOPBACK_A                      1			/* Loopback to type A */
#define age1960_LOOPBACK_B                      2			/* Loopback to type B */
#define age1960_LOOPBACK_C                      3			/* Loopback to type C */

#define age1960_FR_SPEECH						0			/* Channel mode setting - Full rate Speech */
#define age1960_EFR_SPEECH						1			/* Channel mode setting -  Enhanced Full rate Speech */

#define age1960_EXP_DIFF_MIN				  -30.0			/* minimum Expected difference for dynamic power */
#define age1960_EXP_DIFF_MAX				   30.0			/* maximum Expected difference for dynamic power */

#define age1960_DPOW_COUNT_MIN					1			/* minimum count for dynamic power */
#define age1960_DPOW_COUNT_MAX				  999			/* maximum count for dynamic power */

#define age1960_DPOW_ARRAY_MIN					1			/* minimum number of points allowed for Dynamic power measurement */
#define age1960_DPOW_ARRAY_MAX				 2000			/* maximum number of points allowed for Dynmaic power measurement */

#define age1960_IQT_SPURFREQ_MIN_1			  -13.0E6
#define age1960_IQT_SPURFREQ_MAX_1			   -1.0E6
#define age1960_IQT_SPURFREQ_MIN_2			   1.0E6
#define age1960_IQT_SPURFREQ_MAX_2		  	   13.0E6

#define age1960_TA_SWITCH_MAX_TIME			  240			/* switching timeout value */

#define age1960_FUND_FREQ_MIN				  100.000000	/* Audio analyzer minimum fundamental frequency */
#define age1960_FUND_FREQ_MAX			    10000.000000	/* Audio analyzer maximum fundamental frequency */

#define age1960_FILTER_NONE						0			/* Audio analyzer filter type NONE */
#define age1960_FILTER_BANDPASS					1			/* Audio analyzer Tunable BandPass filter */
#define age1960_FILTER_CMESSAGE					2			/* Audio analyzer C_Message filter type */
#define age1960_FILTER_50_15KHZ					3			/* Audio analyzer BandPass filter 50 to 15khz */
#define age1960_FILTER_300_15KHZ				4			/* Audio analyzer BandPass filter 300 to 15KHZ */

#define age1960_BPF_FREQ_MIN				200.000000		/* Audio analyzer BandPass filter minimum center frequency */
#define age1960_BPF_AFAN_FREQ_MAX		  20000.000000		/* Audio analyzer BandPass filter maximum center frequency */

#define age1960_DET_RMS							0			/* Audio analyzer RMS detector */
#define age1960_DET_PPEAK						1			/* Audio analyzer Positive Peak detector */

#define age1960_EXPANDOR_LVL_MIN			  0.00			/* Audio analyzer RMS detector */
#define age1960_EXPANDOR_LVL_MAX			 10.00			/* Audio analyzer Positive Peak detector */

#define age1960_AUTORNG_AFANALYZER				0			/* Audio Frequency Analyzer Meas Fcn */
#define age1960_AUTORNG_TXPOWER					1			/* TX Power Meas Fcn */

#define age1960_CIFT_CONF_LEVEL_MIN				80.0		/* PFER CIFT confidence level minimum */
#define age1960_CIFT_CONF_LEVEL_MAX				99.9		/* PFER CIFT confidence level maximum */

#define age1960_PFER_PEAK_PHASE_LIMIT_MIN		0.0			/* PFER CIFT peak phase error limit minimum */
#define age1960_PFER_PEAK_PHASE_LIMIT_MAX		20.0		/* PFER CIFT peak phase error limit maximum */

#define age1960_PFER_RMS_PHASE_LIMIT_MIN		0.0			/* PFER CIFT rms phase error limit minimum */
#define age1960_PFER_RMS_PHASE_LIMIT_MAX		5.0			/* PFER CIFT rms phase error limit maximum */

#define age1960_PFER_FREQ_ERROR_LIMIT_MIN		0.01		/* PFER CIFT frequency error limit minimum */
#define age1960_PFER_FREQ_ERROR_LIMIT_MAX		0.10		/* PFER CIFT frequency error limit minimum */

#define age1960_GPRS_FORMAT					"GPRS"			/* GPRS TA format name */
#define age1960_GSM_FORMAT					"GSM"			/* GSM TA format name */
#define age1960_AMPS_IS136_FORMAT			"AMPS\IS136"	/* AMPS/IS136 TA format name */
#define age1960_WCDMA_FORMAT				"WCDMA"     	/* WCDMA TA format name */

#define age1960_GPRS_TA_SUFFIX				":GPRS"     	/* SCPI suffix for GPRS TAP SCPI */
#define age1960_GSM_TA_SUFFIX				":GSM"      	/* SCPI suffix for GSM TAP SCPI */
#define age1960_IS136_TA_SUFFIX				":DIG136"   	/* SCPI suffix for AMPS/IS136 TAP (Digital Control Channel Only) */
#define age1960_AMPS_IS136_TA_SUFFIX		":TA136"    	/* SCPI suffix for AMPS/IS136 TAP SCPI */
#define age1960_WCDMA_TA_SUFFIX				":WCDMA"    	/* SCPI suffix for WCDMA TAP SCPI */
#define age1960_C2K_TA_SUFFIX				":C2K"			/* SCPI suffix for C2K SCPI */

#define age1960_PVT_MASK_NARROW					0			/* Power Vs Time PCS ETSI Limit Mask - Narrow */
#define age1960_PVT_MASK_RELAXED				1			/* Power Vs Time PCS ETSI Limit Mask - Relaxed */

#define age1960_ORFS_LIMIT_ETSI					0			/* ORFS measurement Limit Source ETSI setting */
#define age1960_ORFS_LIMIT_MAN					1			/* ORFS measurement Limit Source Manual setting */

#define age1960_ORFSSW_LIM_POINTS_MIN			1			/* ORFS Switching Limits minimum number of points */
#define age1960_ORFSSW_LIM_POINTS_MAX			8			/* ORFS Switching Limits maximum number of points */

#define age1960_ORFSMOD_LIM_POINTS_MIN			1			/* ORFS Modulation Limits minimum number of points */
#define age1960_ORFSMOD_LIM_POINTS_MAX			22			/* ORFS Modulation Limits maximum number of points */

#define age1960_ORFSSW_LIMIT_MIN		      -200.0		/* ORFS Switching Limits minimum acceptable setting */
#define age1960_ORFSSW_LIMIT_MAX			   100.0		/* ORFS Switching Limits maximum acceptable setting */

#define age1960_ORFSMOD_LIMIT_MIN			  -200.0		/* ORFS Modulation Limits minimum acceptable setting */
#define age1960_ORFSMOD_LIMIT_MAX			   100.0		/* ORFS Modulation Limits maximum acceptable setting */


    /***************************************************/
    /*  Tracing functions for surround                 */
    /***************************************************/
#define  age1960_TRAC_STDERR       0
#define  age1960_TRAC_EVENTLOG     1
#define  age1960_TRAC_DEBUGSTRING  2
#define  age1960_TRAC_CALLBACK     3
#define  age1960_TRAC_NONE         0
#define  age1960_TRAC_ERROR        1
#define  age1960_TRAC_ALL          2

    ViStatus _VI_FUNC age1960_diagTrac  (
        ViSession vi,
        ViInt16 destination,
        ViInt16 level);

#ifdef INSTR_CALLBACKS
    ViStatus _VI_FUNC age1960_diagTracCallback  (
        ViSession vi,
        ViAddr function,
        ViAddr handle);

#endif /* INSTR_CALLBACKS */
    ViStatus _VI_FUNC age1960_diagTrac_Q  (
        ViSession vi,
        ViPInt16 destination,
        ViPInt16 level);


    /***************************************************/
    /*  Instrument specific functions                  */
    /***************************************************/

    ViStatus _VI_FUNC age1960_cmdIntArr
    (ViSession  instrumentHandle,
     ViString   sendIntegerArrayCommand,
     ViInt32    sendIntegerArraySize,
     ViInt32    sendIntegerArray[]);

    ViStatus _VI_FUNC age1960_cmdRealArr
    (ViSession  instrumentHandle,
     ViString   sendRealArrayCommand,
     ViInt32    sendRealArraySize,
     ViReal64   sendRealArray[]);

    ViStatus _VI_FUNC age1960_partialPreset
    (ViSession  instrumentHandle);

    ViStatus _VI_FUNC age1960_readTXPower_Q
    (ViSession  instrumentHandle,
     ViBoolean  autoRange,
     ViPInt32   TXPowerIntegrity,
     ViPReal64  TXPower);

    ViStatus _VI_FUNC age1960_readPowerVsTime_Q
    (ViSession  instrumentHandle,
     ViInt32    pvsTimeArraySize,
     ViPInt32   pvsTimeIntegrity,
     ViPReal64  pvsTimeMaskFail,
     ViPReal64  pvsTimeTXPower,
     ViReal64   _VI_FAR pvsTimeArray[],
     ViPInt32   pvsTimeCount);

    ViStatus _VI_FUNC age1960_readIQTuning_Q
    (ViSession  instrumentHandle,
     ViInt32    IQArraySize,
     ViPInt32   IQIntegrity,
     ViPReal64  IQSpurFreq,
     ViPReal64  IQSpurLevel,
     ViReal64   _VI_FAR IQResultsArray[],
     ViReal64   _VI_FAR IQFreqsArray[],
     ViPInt32   IQTuningCount);

    ViStatus _VI_FUNC age1960_readPFER_Q
    (ViSession instrumentHandle,
     ViPInt32   PFERIntegrity,
     ViPReal64  PFERRMSError,
     ViPReal64  PFERPeakError,
     ViPReal64  PFERFreqError);

    ViStatus _VI_FUNC age1960_readORFS_Q
    (ViSession  instrumentHandle,
     ViInt32    ORFSSwArraySize,
     ViInt32    ORFSModArraySize,
     ViPInt32   ORFSIntegrity,
     ViPReal64  ORFSTXPower,
     ViPReal64  ORFSBWPower,
     ViReal64   _VI_FAR ORFSSwArray[],
     ViReal64   _VI_FAR ORFSModArray[],
     ViPInt32   ORFSSwCount,
     ViPInt32   ORFSModCount);

    ViStatus _VI_FUNC age1960_readFBER_Q
    (ViSession  instrumentHandle,
     ViPInt32   FBERIntegrity,
     ViPReal64  FBERBitsTested,
     ViPReal64  FBERRatio,
     ViPReal64  FBERCount);

    ViStatus _VI_FUNC age1960_readBER_Q
    (ViSession  instrumentHandle,
     ViPInt32   BERIntegrity,
     ViPReal64  TypeIaBERBitsTested,
     ViPReal64  TypeIaBERRatio,
     ViPReal64  TypeIaBERCount,
     ViPReal64  TypeIbBERBitsTested,
     ViPReal64  TypeIbBERRatio,
     ViPReal64  TypeIbBERCount,
     ViPReal64  TypeIIBERBitsTested,
     ViPReal64  TypeIIBERRatio,
     ViPReal64  TypeIIBERCount);

    ViStatus _VI_FUNC age1960_readAAudio_Q
    (ViSession  instrumentHandle,
     ViPInt32   AAudioIntegrity,
     ViPReal64  AAudioResult);

    ViStatus _VI_FUNC age1960_readDAudio_Q
    (ViSession  instrumentHandle,
     ViPInt32   DAudioIntegrity,
     ViPReal64  DAudioResult);

    ViStatus _VI_FUNC age1960_readDynPower_Q
    (ViSession  instrumentHandle,
     ViInt32    dynPowerArraySize,
     ViInt32    _VI_FAR dynPowerIntegrity[],
     ViReal64   _VI_FAR dynPowerArray[],
     ViPInt32   dynPowerCount);

    ViStatus _VI_FUNC age1960_initMeasFcn
    (ViSession  instrumentHandle,
     ViInt16    measFcn);

    ViStatus _VI_FUNC age1960_initMeasFcns
    (ViSession  instrumentHandle,
     ViInt32    measFcnsToInit,
     ViInt16    _VI_FAR measFcnsArray[]);

    ViStatus _VI_FUNC age1960_fetchTXPower_Q
    (ViSession instrumentHandle,
     ViPInt32   TXPowerIntegrity,
     ViPReal64  TXPower);

    ViStatus _VI_FUNC age1960_fetchPowerVsTime_Q
    (ViSession instrumentHandle,
     ViInt32    pvsTimeArraySize,
     ViPInt32   pvsTimeIntegrity,
     ViPReal64  pvsTimeMaskFail,
     ViPReal64  pvsTimeTXPower,
     ViReal64   _VI_FAR pvsTimeArray[],
     ViPInt32   pvsTimeCount);

    ViStatus _VI_FUNC age1960_fetchDynPower_Q
    (ViSession  instrumentHandle,
     ViInt32    dynPowerArraySize,
     ViInt32    _VI_FAR dynPowerIntegrity[],
     ViReal64   _VI_FAR dynPowerArray[],
     ViPInt32   dynPowerCount);

    ViStatus _VI_FUNC age1960_fetchIQTuning_Q
    (ViSession  instrumentHandle,
     ViInt32    IQArraySize,
     ViPInt32   IQIntegrity,
     ViPReal64   IQSpurFreq,
     ViPReal64   IQSpurLevel,
     ViReal64   _VI_FAR IQResultsArray[],
     ViReal64   _VI_FAR IQFreqsArray[],
     ViPInt32   IQTuningCount);

    ViStatus _VI_FUNC age1960_fetchPFER_Q
    (ViSession instrumentHandle,
     ViPInt32   PFERIntegrity,
     ViPReal64  PFERRMSError,
     ViPReal64  PFERPeakError,
     ViPReal64  PFERFreqError);

    ViStatus _VI_FUNC age1960_fetchORFS_Q
    (ViSession  instrumentHandle,
     ViInt32    ORFSSwArraySize,
     ViInt32    ORFSModArraySize,
     ViPInt32   ORFSIntegrity,
     ViPReal64  ORFSTXPower,
     ViPReal64  ORFSBWPower,
     ViReal64   _VI_FAR ORFSSwArray[],
     ViReal64   _VI_FAR ORFSModArray[],
     ViPInt32   ORFSSwCount,
     ViPInt32   ORFSModCount);

    ViStatus _VI_FUNC age1960_fetchFBER_Q
    (ViSession  instrumentHandle,
     ViPInt32   FBERIntegrity,
     ViPReal64  FBERBitsTested,
     ViPReal64  FBERRatio,
     ViPReal64  FBERCount);

    ViStatus _VI_FUNC age1960_fetchBER_Q
    (ViSession  instrumentHandle,
     ViPInt32   BERIntegrity,
     ViPReal64  TypeIaBERBitsTested,
     ViPReal64  TypeIaBERRatio,
     ViPReal64  TypeIaBERCount,
     ViPReal64  TypeIbBERBitsTested,
     ViPReal64  TypeIbBERRatio,
     ViPReal64  TypeIbBERCount,
     ViPReal64  TypeIIBERBitsTested,
     ViPReal64  TypeIIBERRatio,
     ViPReal64  TypeIIBERCount);

    ViStatus _VI_FUNC age1960_fetchAAudio_Q
    (ViSession  instrumentHandle,
     ViPInt32   AAudioIntegrity,
     ViPReal64  AAudioResult);

    ViStatus _VI_FUNC age1960_fetchDAudio_Q
    (ViSession  instrumentHandle,
     ViPInt32   DAudioIntegrity,
     ViPReal64  DAudioResult);

    ViStatus _VI_FUNC age1960_waitForInitDone_Q
    (ViSession  instrumentHandle,
     ViReal64   initDoneTimeOut,
     ViPInt16   measFcnDone);

    ViStatus _VI_FUNC age1960_originateCall
    (ViSession  instrumentHandle,
     ViInt16    completion,
     ViReal64   originateTimeOut);

    ViStatus _VI_FUNC age1960_endCall
    (ViSession  instrumentHandle,
     ViBoolean  endSeq);

    ViStatus _VI_FUNC age1960_TCHARFCN
    (ViSession  instrumentHandle,
     ViInt32	TCHARFCN,
     ViInt16	TCHARFCNBand,
     ViBoolean  TCHARFCNSeq);

    ViStatus _VI_FUNC age1960_manualChan
    (ViSession  instrumentHandle,
     ViInt32    manualChan,
     ViInt16    manualChanBand);

    ViStatus _VI_FUNC age1960_expectedPower
    (ViSession  instrumentHandle,
     ViReal64   expectedPower,
     ViInt16    expectedPowerBand);

    ViStatus _VI_FUNC age1960_confRcvControl
    (ViSession  instrumentHandle,
     ViInt16    rcvControlMode);

    ViStatus _VI_FUNC age1960_MSTXLevel
    (ViSession  instrumentHandle,
     ViInt32    MSTXLevel,
     ViInt16    MSTXLevelBand,
     ViBoolean  MSTXLevelSeq);

    ViStatus _VI_FUNC age1960_TCHTimeslot
    (ViSession  instrumentHandle,
     ViInt32    TCHTimeslot,
     ViBoolean  TCHTimeslotSeq);

    ViStatus _VI_FUNC age1960_MSTimeAdvance
    (ViSession  instrumentHandle,
     ViInt32    MSTimingAdvance,
     ViBoolean  MSTimingAdvanceSeq);

    ViStatus _VI_FUNC age1960_BCHARFCN
    (ViSession  instrumentHandle,
     ViInt16    BCHARFCNCell,
     ViInt32    BCHARFCN,
     ViInt16    BCHARFCNBand,
     ViBoolean  BCHARFCNSeq);

    ViStatus _VI_FUNC age1960_cellBand
    (ViSession  instrumentHandle,
     ViInt16    servingCellBandCell,
     ViInt16    servingCellBand);

    ViStatus _VI_FUNC age1960_TCHBand
    (ViSession  instrumentHandle,
     ViInt16    TCHBand);

    ViStatus _VI_FUNC age1960_manualBand
    (ViSession  instrumentHandle,
     ViInt16    manaulBand);

    ViStatus _VI_FUNC age1960_operatingMode
    (ViSession  instrumentHandle,
     ViInt16    operatingMode);

    ViStatus _VI_FUNC age1960_BATable
    (ViSession  instrumentHandle,
     ViInt16    BATableCell,
     ViInt32    BATableArraySize,
     ViInt32    _VI_FAR BATableArray[],
     ViInt16    BATableBand);

    ViStatus _VI_FUNC age1960_burstType
    (ViSession  instrumentHandle,
     ViInt16    burstType);

    ViStatus _VI_FUNC age1960_amplitudeOffset
    (ViSession  instrumentHandle,
     ViReal64    _VI_FAR frequencyArray[],
     ViReal64    _VI_FAR offsetArray[],
     ViInt32     ampOffsetItems);

    ViStatus _VI_FUNC age1960_cellPower
    (ViSession  instrumentHandle,
     ViInt16    cellPowerCell,
     ViReal64   cellPower,
     ViReal64   cellPowerDelta);

    ViStatus _VI_FUNC age1960_channelMode
    (ViSession	instrumentHandle,
     ViInt16	channelMode);

    ViStatus _VI_FUNC age1960_FACCHSignaling
    (ViSession	instrumentHandle,
     ViBoolean	facchState);

    ViStatus _VI_FUNC age1960_integrityMessage
    (ViSession  instrumentHandle,
     ViInt32    integrityCode,
     ViPString  integrityMessage);

    ViStatus _VI_FUNC age1960_measurementTimeout
    (ViSession  instrumentHandle,
     ViInt16    timeoutMeasFcn,
     ViReal64   timeoutSec,
     ViBoolean  timeoutOnOff);

    ViStatus _VI_FUNC age1960_confTXPower
    (ViSession  instrumentHandle,
     ViInt32    TXPowerMeasCount,
     ViInt16    TXPowerTrigSource,
     ViReal64   TXPowerTrigDelay,
     ViBoolean  TXPowerTrigQual,
     ViReal64   TXPowerMaxTime);

    ViStatus _VI_FUNC age1960_confDynPower
    (ViSession  instrumentHandle,
     ViInt32    DynPowerMeasCount,
     ViReal64   DynPowerExpMaxDiff,
     ViReal64   DynPowerMaxTime);

    ViStatus _VI_FUNC age1960_confPowerVsTime
    (ViSession  instrumentHandle,
     ViInt32    pvsTimeMeasCount,
     ViInt16    pvsTimeTrigSource,
     ViReal64   pvsTimeTrigDelay,
     ViInt16    pvsTimeSync,
     ViInt32    pvsTimePoints,
     ViReal64   pvsTimeOffsets[],
     ViReal64   pvsTimeMaxTime);

    ViStatus _VI_FUNC age1960_confPFER
    (ViSession  instrumentHandle,
     ViInt32    PFERMeasCount,
     ViInt16    PFERTrigSource,
     ViReal64   PFERTrigDelay,
     ViBoolean  PFERTrigQual,
     ViInt16    PFERSync,
     ViReal64   PFERMaxTime);

    ViStatus _VI_FUNC age1960_confORFS
    (ViSession  instrumentHandle,
     ViInt32    ORFSSwMeasCount,
     ViInt32    ORFSModMeasCount,
     ViInt16    ORFSTrigSource,
     ViReal64   ORFSTrigDelay,
     ViInt32    ORFSSwPoints,
     ViReal64   ORFSSwFreqs[],
     ViInt32    ORFSModPoints,
     ViReal64   ORFSModFreqs[],
     ViReal64   ORFSMaxTime);

    ViStatus _VI_FUNC age1960_confFastBER
    (ViSession  instrumentHandle,
     ViInt32    FBERMeasCount,
     ViInt32    FBERManualDelay,
     ViBoolean  FBERAutoDelay,
     ViBoolean  FBERSignallingControl,
     ViReal64   FBERHoldoffTime,
     ViReal64   FBERMaxTime);

    ViStatus _VI_FUNC age1960_confBER
    (ViSession  instrumentHandle,
     ViInt32    BERMeasCount,
     ViInt16    BERMeasType,
     ViInt32    BERManualDelay,
     ViBoolean  BERAutoDelay,
     ViBoolean  BERSignallingControl,
     ViReal64   BERHoldoffTime,
     ViReal64   BERMaxTime);

    ViStatus _VI_FUNC age1960_confAAudio
    (ViSession  instrumentHandle,
     ViInt32    AAudioMeasCount,
     ViReal64   AAudioExpVolt,
     ViReal64   AAudioFiltFreq,
     ViBoolean  AAudioFiltState,
     ViReal64   AAudioMaxTime);

    ViStatus _VI_FUNC age1960_confDAudio
    (ViSession  instrumentHandle,
     ViInt32    DAudioMeasCount,
     ViReal64   DAudioFiltFreq,
     ViBoolean  DAudioFiltState,
     ViReal64   DAudioMaxTime);

    ViStatus _VI_FUNC age1960_confIQTuning
    (ViSession  instrumentHandle,
     ViInt32    IQTMeasCount,
     ViInt16    IQTTrigSource,
     ViReal64   IQTTrigDelay,
     ViInt16    IQTRefOffsetFrequency,
     ViBoolean  IQTSpurFreqState,
     ViReal64   IQTSpurFrequency,
     ViReal64   IQTMaxTime);

    ViStatus _VI_FUNC age1960_confAutoRange
    (ViSession instrumentHandle,
     ViInt16 measFcnToUse,
     ViInt32 tries);

    ViStatus _VI_FUNC age1960_confForMSOriginate
    (ViSession  instrumentHandle);

    ViStatus _VI_FUNC age1960_waitForConnect
    (ViSession  instrumentHandle,
     ViReal64   connectTimeOut);

    ViStatus _VI_FUNC age1960_getCorrectionFreq_Q
    (ViSession  instrumentHandle,
     ViInt16    cell,
     ViPReal64  downlinkFreq,
     ViPReal64  uplinkFreq);

    ViStatus _VI_FUNC age1960_getPresentFreq_Q
    (ViSession  instrumentHandle,
     ViPReal64  downlinkFreq,
     ViPReal64  uplinkFreq,
     ViPReal64  measurementFreq);

    ViStatus _VI_FUNC age1960_getApproxOffset_Q
    (ViSession  instrumentHandle,
     ViReal64   frequency,
     ViPReal64  offset);

    ViStatus _VI_FUNC age1960_UUTLevel_Q
    (ViSession instrumentHandle,
     ViInt16 cell,
     ViPReal64 level);

    ViStatus _VI_FUNC age1960_SACCHReport_Q
    (ViSession instrumentHandle,
     ViBoolean  waitForNewSACCH,
     ViInt32    numberOfReadings,
     ViPReal64  RXLevel,
     ViPReal64  TXLevel,
     ViPReal64  RXQuality,
     ViPReal64  TimingAdvance);

    ViStatus _VI_FUNC age1960_waitForClearHPIB
    (ViSession instrumentHandle,
     ViInt32  timeOut);

    ViStatus _VI_FUNC age1960_useSRQMAV
    (ViSession instrumentHandle,
     ViBoolean useSRQMAV);

    ViStatus _VI_FUNC age1960_useSRQMAV_Q
    (ViSession   instrumentHandle,
     ViPBoolean  useSRQMAV);

    ViStatus _VI_FUNC age1960_PowerVsTimePlot_Q
    (ViSession   instrumentHandle,
     ViInt32     pvsTimePlotArraySize,
     ViPInt32    pvsTimePlotIntegrity,
     ViPReal64   pvsTimePlotMaskFail,
     ViPReal64   pvsTimePlotPower,
     ViPReal64   pvsTimeStartTime,
     ViPReal64   pvsTimeIncrement,
     ViReal64    _VI_FAR pvsTimePlotArray[]);

    ViStatus _VI_FUNC age1960_loopback
    (ViSession 	instrumentHandle,
     ViInt16   	loopbackMode);

    ViStatus _VI_FUNC age1960_currentTA
    (ViSession  instrumentHandle,
     ViString   TAName,
     ViString   TARevision);

    ViStatus _VI_FUNC age1960_currentTA_Q
    (ViSession  instrumentHandle,
     ViPString  TANameQuery,
     ViPString  TARevisionQuery);

    ViStatus _VI_FUNC age1960_currentTAModel_Q
    (ViSession  instrumentHandle,
     ViPString  TAModel);

    ViStatus _VI_FUNC age1960_currentActiveTAP
    (ViSession  instrumentHandle,
     ViString   TAName);

    ViStatus _VI_FUNC age1960_currentActiveTAP_Q
    (ViSession  instrumentHandle,
     ViPString  TANameQuery);

    ViStatus _VI_FUNC age1960_confAFAN
    (ViSession 	instrumentHandle,
     ViInt32 	AFANMeasCount,
     ViBoolean 	AFANSinadDist,
     ViReal64 	AFANFundFreq,
     ViInt16 	AFANFilterType,
     ViReal64 	AFANBPFCntFreq,
     ViInt16 	AFANDetType,
     ViReal64 	AFANPeakVolt,
     ViBoolean 	AFANDeemphasis,
     ViReal64 	AFANExpandorLevel,
     ViReal64 	AFANMaxTime);

    ViStatus _VI_FUNC age1960_fetchAFAN_Q
    (ViSession 	instrumentHandle,
     ViPInt32 	AFANIntegrity,
     ViPReal64 	AFANAvgAudioLevel,
     ViPReal64 	AFANAvgSINAD,
     ViPReal64 	AFANAvgDist);

    ViStatus _VI_FUNC age1960_readAFAN_Q
    (ViSession 	instrumentHandle,
     ViBoolean 	AFANAutoRange,
     ViPInt32 	AFANIntegrity,
     ViPReal64 	AFANAvgAudioLevel,
     ViPReal64 	AFANAvgSINAD,
     ViPReal64 	AFANAvgDist);

    ViStatus _VI_FUNC age1960_confPFER_CIFT
    (ViSession 	instrumentHandle,
     ViBoolean  PFERCIFTState,
     ViReal64   PFERConfidenceLevel,
     ViReal64   PFERPeakPhaseErrorMaxLimit,
     ViReal64   PFERRmsPhaseErrorMaxLimit,
     ViReal64   PFERFrequencyErrorMaxLimit);

    ViStatus _VI_FUNC age1960_confPVTMask
    (ViSession  instrumentHandle,
     ViInt16    PVTMaskForPCS);

    ViStatus _VI_FUNC age1960_confORFSLimitSource
    (ViSession  instrumentHandle,
     ViInt16    limitSource);

    ViStatus _VI_FUNC age1960_confORFSSwitchLimits
    (ViSession  instrumentHandle,
     ViInt16    measurementBand,
     ViInt32    switchingOffsetPoints,
     ViReal64   _VI_FAR switchingLimitsArray[]);

    ViStatus _VI_FUNC age1960_confORFSModLimits
    (ViSession  instrumentHandle,
     ViInt16    measurementBand,
     ViInt32    modulationOffsetPoints,
     ViReal64   _VI_FAR modulationLimitsArray[]);

    ViStatus _VI_FUNC age1960_fetchORFSSummaryPassFail_Q
    (ViSession  instrumentHandle,
     ViPReal64  overallPFSummary,
     ViPReal64  switchingPFSummary,
     ViPReal64  modulationPFSummary);

    ViStatus _VI_FUNC age1960_burstTimingError_Q
    (ViSession  instrumentHandle,
     ViPReal64 	burstTimingError);


    /* End of instrument specific declarations */
    /*---------------------------------------------------------------------------*/

    /* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif /* age1960_INCLUDE */

