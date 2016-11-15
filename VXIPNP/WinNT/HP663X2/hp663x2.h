/*****************************************************************************/
/*  $Header: /VXIPnP/Eagle/hp663x2/hp663x2.h 3     5/22/00 10:19a Rickc $  */
/*  Copyright 1998-2000 Agilent Technologies, Inc.                           */
/*---------------------------------------------------------------------------*/
/*  Driver for hp663x2, Dynamic Measurement dc Sources                       */
/*  Driver Version: A.01.00                                                  */
/*****************************************************************************/

#ifndef hp663x2_INCLUDE
#define hp663x2_INCLUDE
#include "vpptype.h"

/* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

    /*****************************************************************************/
    /*  STANDARD SECTION                                                         */
    /*  Constants and function prototypes for HP standard functions.             */
    /*****************************************************************************/

    /***************************************************/
    /*  Standard constant error conditions returned    */
    /*  by driver functions.                           */
    /*    HP Common Error numbers start at BFFC0D00    */
    /*    The parameter errors extend the number of    */
    /*      errors over the eight defined in VPP 3.4   */
    /***************************************************/

#define HP663X2_INSTR_ERROR_NULL_PTR      (_VI_ERROR+0x3FFC0D02L) /* 0xBFFC0D02 */
#define HP663X2_INSTR_ERROR_RESET_FAILED  (_VI_ERROR+0x3FFC0D03L) /* 0xBFFC0D03 */
#define HP663X2_INSTR_ERROR_UNEXPECTED    (_VI_ERROR+0x3FFC0D04L) /* 0xBFFC0D04 */
#define HP663X2_INSTR_ERROR_INV_SESSION   (_VI_ERROR+0x3FFC0D05L) /* 0xBFFC0D05 */
#define HP663X2_INSTR_ERROR_LOOKUP        (_VI_ERROR+0x3FFC0D06L) /* 0xBFFC0D06 */
#define HP663X2_INSTR_ERROR_DETECTED      (_VI_ERROR+0x3FFC0D07L) /* 0xBFFC0D07 */
#define HP663X2_INSTR_NO_LAST_COMMA       (_VI_ERROR+0x3FFC0D08L) /* 0xBFFC0D08 */

#define HP663X2_INSTR_ERROR_PARAMETER9    (_VI_ERROR+0x3FFC0D20L) /* 0xBFFC0D20 */
#define HP663X2_INSTR_ERROR_PARAMETER10   (_VI_ERROR+0x3FFC0D21L) /* 0xBFFC0D21 */
#define HP663X2_INSTR_ERROR_PARAMETER11   (_VI_ERROR+0x3FFC0D22L) /* 0xBFFC0D22 */
#define HP663X2_INSTR_ERROR_PARAMETER12   (_VI_ERROR+0x3FFC0D23L) /* 0xBFFC0D23 */
#define HP663X2_INSTR_ERROR_PARAMETER13   (_VI_ERROR+0x3FFC0D24L) /* 0xBFFC0D24 */
#define HP663X2_INSTR_ERROR_PARAMETER14   (_VI_ERROR+0x3FFC0D25L) /* 0xBFFC0D25 */
#define HP663X2_INSTR_ERROR_PARAMETER15   (_VI_ERROR+0x3FFC0D26L) /* 0xBFFC0D26 */
#define HP663X2_INSTR_ERROR_PARAMETER16   (_VI_ERROR+0x3FFC0D27L) /* 0xBFFC0D27 */
#define HP663X2_INSTR_ERROR_PARAMETER17   (_VI_ERROR+0x3FFC0D28L) /* 0xBFFC0D28 */
#define HP663X2_INSTR_ERROR_PARAMETER18   (_VI_ERROR+0x3FFC0D29L) /* 0xBFFC0D29 */

#define HP663X2_INSTR_ERROR_MEAS_TMO	  (_VI_ERROR+0x3FFC0D2AL) /* 0xBFFC0D2A */
#define HP663X2_INSTR_ERROR_PORT_CONFIG	  (_VI_ERROR+0x3FFC0D2BL) /* 0xBFFC0D2B */
#define HP663X2_INSTR_ERROR_GET_LIMIT	  (_VI_ERROR+0x3FFC0D2CL) /* 0xBFFC0D2C */
#define HP663X2_INSTR_ERROR_NSUP_FUNC	  (_VI_ERROR+0x3FFC0D2DL) /* 0xBFFC0D2D */
#define HP663X2_INSTR_ERROR_NSUP_PARAM	  (_VI_ERROR+0x3FFC0D2EL) /* 0xBFFC0D2E */

    /***************************************************/
    /*  Constants used by system status functions      */
    /*    These defines are bit numbers which define   */
    /*    the operation and questionable registers.    */
    /*    They are instrument specific.                */
    /***************************************************/

#define	HP663X2_STB_QUES			0x0008
#define HP663X2_STB_MAV				0x0010
#define	HP663X2_STB_ESB				0x0020
#define	HP663X2_STB_MSS				0x0040
#define	HP663X2_STB_RQS				0x0040
#define	HP663X2_STB_OPER			0x0080

#define	HP663X2_SRE_QUES			((ViInt16)8)
#define	HP663X2_SRE_MAV				((ViInt16)16)
#define	HP663X2_SRE_ESB				((ViInt16)32)
#define	HP663X2_SRE_MSS				((ViInt16)64)
#define	HP663X2_SRE_OPER			((ViInt16)128)

#define	HP663X2_OPER_CAL			0x0001
#define	HP663X2_OPER_WTG			0x0020
#define	HP663X2_OPER_CV				0x0100
#define	HP663X2_OPER_CC_POS			0x0400
#define	HP663X2_OPER_CC_NEG			0x0800

#define	HP663X2_QUES_OV				0x0001
#define	HP663X2_QUES_OCP			0x0002
#define	HP663X2_QUES_FUSE			0x0004
#define	HP663X2_QUES_OT				0x0010
#define	HP663X2_QUES_RI				0x0200
#define	HP663X2_QUES_UNREG			0x0400
#define	HP663X2_QUES_OVLD			0x4000

#define	HP663X2_ESR_OPC				0x0001
#define	HP663X2_ESR_QYE				0x0004
#define	HP663X2_ESR_DDE				0x0008
#define	HP663X2_ESR_EXE				0x0010
#define	HP663X2_ESR_CME				0x0020
#define	HP663X2_ESR_PON				0x0080

#define	HP663X2_REG_ESE				0
#define	HP663X2_REG_SRE				1
#define	HP663X2_REG_OPER			2
#define	HP663X2_REG_QUES			3

#define	HP663X2_CMDSTRING_MAX		256

#define	HP663X2_CMDSTRING_Q_MIN		2
#define	HP663X2_CMDSTRING_Q_MAX		32767L


    /***************************************************/
    /*  Constants used by function hp663x2_timeOut      */
    /***************************************************/

#define HP663X2_TIMEOUT_MAX         2147483647L
#define HP663X2_TIMEOUT_MIN         0L

    /***************************************************/
    /*  Required plug and play functions from VPP-3.1  */
    /***************************************************/

    ViStatus _VI_FUNC hp663x2_init (ViRsrc resourceName,
                                    ViBoolean idQuery,
                                    ViBoolean resetDevice,
                                    ViPSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_reset (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_self_test (ViSession instrumentHandle,
                                         ViPInt16 testResult,
                                         ViChar _VI_FAR testMessage[]);

    ViStatus _VI_FUNC hp663x2_error_message (ViSession instrumentHandle,
            ViStatus statusCode,
            ViChar _VI_FAR statusMessage[]);

    ViStatus _VI_FUNC hp663x2_error_query (ViSession instrumentHandle,
                                           ViPInt32	 errorCode,
                                           ViChar	 _VI_FAR errorMessage[]);

    ViStatus _VI_FUNC hp663x2_revision_query (ViSession instrumentHandle,
            ViChar	_VI_FAR instrumentDriverRevision[],
            ViChar	_VI_FAR firmwareRevision[] );

    ViStatus _VI_FUNC hp663x2_close (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_errorQueryDetect (ViSession instrumentHandle,
            ViBoolean errorQueryDetect);

    ViStatus _VI_FUNC hp663x2_errorQueryDetect_Q (ViSession instrumentHandle,
            ViPBoolean errorQueryDetect);

    /***************************************************/
    /*  HP standard utility functions                  */
    /***************************************************/

    ViStatus _VI_FUNC hp663x2_timeOut (ViSession instrumentHandle, ViInt32 timeOut);

    ViStatus _VI_FUNC hp663x2_timeOut_Q (ViSession instrumentHandle, ViInt32 *timeOut);

    ViStatus _VI_FUNC hp663x2_dcl (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_trg (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_opc_Q (ViSession instrumentHandle,
                                     ViPBoolean	instrumentReady);

    /***************************************************/
    /*  HP standard status functions                   */
    /***************************************************/

    ViStatus _VI_FUNC hp663x2_readStatusByte_Q (ViSession instrumentHandle,
            ViInt16 *statusByte);

    ViStatus _VI_FUNC hp663x2_operEvent_Q (ViSession instrumentHandle,
                                           ViInt32 *operationEventRegister);

    ViStatus _VI_FUNC hp663x2_operCond_Q (ViSession instrumentHandle,
                                          ViInt32 *operationConditionRegister);

    ViStatus _VI_FUNC hp663x2_quesEvent_Q (ViSession instrumentHandle,
                                           ViInt32 *questionableEventRegister);

    ViStatus _VI_FUNC hp663x2_quesCond_Q (ViSession instrumentHandle,
                                          ViInt32 *questionableConditionRegister);

    /***************************************************/
    /*  HP standard command passthrough functions      */
    /***************************************************/

    ViStatus _VI_FUNC hp663x2_cmd (ViSession	instrumentHandle,
                                   ViString		stringCommand);

    ViStatus _VI_FUNC hp663x2_cmdString_Q (ViSession	instrumentHandle,
                                           ViString		queryStringCommand,
                                           ViInt32		stringSize,
                                           ViChar		_VI_FAR stringResult[]);

    ViStatus _VI_FUNC hp663x2_cmdInt(ViSession  instrumentHandle,
                                     ViString   sendIntegerCommand,
                                     ViInt32    sendInteger );

    ViStatus _VI_FUNC hp663x2_cmdInt16_Q(ViSession  instrumentHandle,
                                         ViString   queryI16Command,
                                         ViPInt16   i16Result);

    /* End of HP standard declarations */
    /*---------------------------------------------------------------------------*/


    /*****************************************************************************/
    /*  INSTRUMENT SPECIFIC SECTION                                              */
    /*  Constants and function prototypes for instrument specific functions.     */
    /*****************************************************************************/

    /***************************************************/
    /*  Instrument specific error numbers              */
    /***************************************************/

#define	HP663X2_INSTR_ERROR_NVRAM_RD0_CS		1
#define	HP663X2_INSTR_ERROR_NVRAM_CONFIG_CS		2
#define	HP663X2_INSTR_ERROR_NVRAM_CAL_CS		3
#define	HP663X2_INSTR_ERROR_NVRAM_STATE_CS		4
#define	HP663X2_INSTR_ERROR_NVRAM_RST_CS		5
#define	HP663X2_INSTR_ERROR_RAM_SELFTEST		10
#define	HP663X2_INSTR_ERROR_DAC_SELFTEST1		11
#define	HP663X2_INSTR_ERROR_DAC_SELFTEST2		12
#define	HP663X2_INSTR_ERROR_DAC_SELFTEST3		13
#define	HP663X2_INSTR_ERROR_DAC_SELFTEST4		14
#define	HP663X2_INSTR_ERROR_OVDAC_SELFTEST		15
#define	HP663X2_INSTR_ERROR_DIGIO_SELFTEST		80
#define	HP663X2_INSTR_ERROR_INGUARD_RXBUF_OVR	213
#define	HP663X2_INSTR_ERROR_RS232_FRAMING		216
#define	HP663X2_INSTR_ERROR_RS232_PARITY		217
#define	HP663X2_INSTR_ERROR_RS232_RX_OVR		218
#define	HP663X2_INSTR_ERROR_FP_UART_OVR			220
#define	HP663X2_INSTR_ERROR_FP_UART_FRAMING		221
#define	HP663X2_INSTR_ERROR_FP_UART_PARITY		222
#define	HP663X2_INSTR_ERROR_FP_RXBUF_OVR		223
#define	HP663X2_INSTR_ERROR_FP_TIMEOUT			224
#define	HP663X2_INSTR_ERROR_CAL_SWITCH			401
#define	HP663X2_INSTR_ERROR_CAL_PASSWORD		402
#define	HP663X2_INSTR_ERROR_CAL_DISABLED		403
#define	HP663X2_INSTR_ERROR_CAL_RB_CONST		404
#define	HP663X2_INSTR_ERROR_CAL_PROG_CONST		405
#define	HP663X2_INSTR_ERROR_CAL_CMD_SEQUENCE	406
#define	HP663X2_INSTR_ERROR_CVCC_STATUS			407
#define	HP663X2_INSTR_ERROR_ALC_NOT_NORMAL		408
#define	HP663X2_INSTR_ERROR_TOO_MANY_SWE_POINTS 601
#define	HP663X2_INSTR_ERROR_RS232_CMD_ONLY		602
#define	HP663X2_INSTR_ERROR_INCOMPATIBLE_FETCH	603
#define	HP663X2_INSTR_ERROR_MEAS_OVERRANGE		604


    /***************************************************/
    /*  Instrument specific constants                  */
    /***************************************************/

#define	HP663X2_DEFAULT_MEAS_TIMEOUT	5000 /* milliseconds */

#define	HP663X2_VOLTAGE				0
#define	HP663X2_CURRENT				1

#define	HP663X2_DISP_NORMAL			0
#define	HP663X2_DISP_TEXT			1

#define	HP663X2_MEAS_DC				0
#define	HP663X2_MEAS_ACDC			1
#define	HP663X2_MEAS_HIGH			2
#define	HP663X2_MEAS_LOW			3
#define	HP663X2_MEAS_MAX			4
#define	HP663X2_MEAS_MIN			5

#define	HP663X2_TRIG_SRC_BUS		0
#define	HP663X2_TRIG_SRC_INT		1

#define	HP663X2_TRIG_COUNT_MAX		100
#define	HP663X2_TRIG_COUNT_MIN		1

#define	HP663X2_TRIG_SLOPE_EITHER    0
#define	HP663X2_TRIG_SLOPE_POS       1
#define	HP663X2_TRIG_SLOPE_NEG       2

#define	HP663X2_RELAY_POLARITY_NORMAL     0
#define	HP663X2_RELAY_POLARITY_REVERSE    1

#define	HP663X2_RI_OFF				0
#define	HP663X2_RI_LATCHING			1
#define	HP663X2_RI_LIVE				2

#define	HP663X2_DFI_SRC_PREV		0
#define	HP663X2_DFI_SRC_QUES		1
#define	HP663X2_DFI_SRC_OPER		2
#define	HP663X2_DFI_SRC_ESB			3
#define	HP663X2_DFI_SRC_RQS			4
#define	HP663X2_DFI_SRC_OFF			5

#define	HP663X2_DIG_OUT_MAX			7
#define HP663X2_DIG_OUT_MIN			0

#define	HP663X2_PON_STATE_RST		0
#define	HP663X2_PON_STATE_RCL0		1

#define	HP663X2_SWEEP_INT_MAX		390e-6
#define	HP663X2_SWEEP_INT_MIN		15.6e-6

#define	HP663X2_SWEEP_SIZE_MIN		1
#define	HP663X2_SWEEP_SIZE_MAX		4096

#define	HP663X2_SWEEP_OFFSET_MIN	-4095
#define	HP663X2_SWEEP_OFFSET_MAX	2000000000

#define	HP663X2_SENSE_FUNC_VOLT		0
#define	HP663X2_SENSE_FUNC_CURR		1

#define	HP663X2_CURR_DET_ACDC		0
#define	HP663X2_CURR_DET_DC			1

#define	HP663X2_CURR_IMM            0
#define	HP663X2_CURR_TRIG           1

#define	HP663X2_CURR_Q              0
#define	HP663X2_CURR_Q_TRIG         1
#define	HP663X2_CURR_Q_MAX          2
#define	HP663X2_CURR_Q_MIN          3

#define	HP663X2_RES_IMM		        0
#define	HP663X2_RES_TRIG            1

#define	HP663X2_RES_Q               0
#define	HP663X2_RES_Q_TRIG          1
#define	HP663X2_RES_Q_MAX           2
#define	HP663X2_RES_Q_MIN           3
#define HP663X2_RES_Q_TRIG_MAX		4
#define	HP663X2_RES_Q_TRIG_MIN		5

#define	HP663X2_VOLT_IMM			0
#define	HP663X2_VOLT_TRIG			1
#define	HP663X2_VOLT_OVP			2
#define	HP663X2_VOLT_OVP_MAX		3
#define	HP663X2_VOLT_OVP_MIN		4

#define	HP663X2_VOLT_Q				0
#define	HP663X2_VOLT_Q_MAX          1
#define	HP663X2_VOLT_Q_MIN			2
#define	HP663X2_VOLT_Q_TRIG         3
#define	HP663X2_VOLT_Q_TRIG_MAX     4
#define	HP663X2_VOLT_Q_TRIG_MIN     5
#define	HP663X2_VOLT_Q_OVP			6
#define	HP663X2_VOLT_Q_OVP_MAX      7
#define	HP663X2_VOLT_Q_OVP_MIN      8

#define	HP663X2_ARM_OUTPUT_ONCE			0
#define	HP663X2_ARM_OUTPUT_CONTINUOUS	1
#define	HP663X2_ARM_OUTPUT_CONTINUOUS_OFF		2
#define	HP663X2_ARM_ACQUISITION			3
#define	HP663X2_ARM_VOLT_ACQUISITION    4
#define	HP663X2_ARM_CURR_ACQUISITION    5

#define	HP663X2_TRIG_OUTPUT			0
#define	HP663X2_TRIG_ACQUISITION	1

#define	HP663X2_SET_VOLT			0
#define	HP663X2_SET_CURR			1

#define	HP663X2_OUTP_STATUS_OFF		0
#define	HP663X2_OUTP_STATUS_CV		1
#define	HP663X2_OUTP_STATUS_CC		2
#define	HP663X2_OUTP_STATUS_UNREG	3
#define	HP663X2_OUTP_STATUS_PROT	4
#define	HP663X2_OUTP_STATUS_UNKNOWN	5

#define	HP663X2_SENSE_WIND_RECT		0
#define	HP663X2_SENSE_WIND_HANN		1

#if 0
    /* these are read from the unit at init time */
    /*
    #define	HP663X2_VOLT_TRIG_MAX		20.0
    #define	HP663X2_VOLT_MAX			20.0
    #define	HP663X2_CURR_TRIG_MAX		2.0
    #define	HP663X2_CURR_MAX			2.0
    #define	HP663X2_VOLT_TRIG_MIN		0
    #define	HP663X2_VOLT_MIN			0
    #define	HP663X2_CURR_TRIG_MIN		0
    #define	HP663X2_CURR_MIN			0
    */
#endif

#define	HP663X2_ACQ_COUNT_MIN		1
#define	HP663X2_ACQ_COUNT_MAX		100

#define	HP663X2_HW_SETTLING_MIN		0
#define	HP663X2_HW_SETTLING_MAX		30000		/* 30 seconds */

#define	HP663X2_OCP_DELAY_MIN		0
#define	HP663X2_OCP_DELAY_MAX		2147480000

    /********* added by dawn - 6/28/96 **********/
    /* these times may be modified after more testing */
#define HP663X2_CLOCK_W_DISPLAY_ON		55
#define HP663X2_CLOCK_W_DISPLAY_OFF		15
#define HP663X2_CLOCK_HW_SETTLE			50
#define HP663X2_CLOCK_RESET_INST		100
#define HP663X2_CLOCK_SELF_TEST			200

    /***************************************************/
    /*  Instrument specific functions                  */
    /***************************************************/

    ViStatus _VI_FUNC hp663x2_rippleRiDfi (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_readOutputStatus (ViSession instrumentHandle,
            ViPInt16 outputStatus);

    ViStatus _VI_FUNC hp663x2_outputVoltCurr(ViSession instrumentHandle,
            ViReal64 voltLev,
            ViReal64 currLev);

    ViStatus _VI_FUNC hp663x2_setMeasure (ViSession instrumentHandle,
                                          ViInt16	outputParam,
                                          ViReal64	outputLev,
                                          ViInt16	settlingTime,
                                          ViPReal64	voltQLev,
                                          ViPReal64 currQLev,
                                          ViPInt16	outputStatus);

    ViStatus _VI_FUNC hp663x2_doDelay (ViSession	instrumentHandle,
                                       ViInt16		milliSeconds);

    ViStatus _VI_FUNC hp663x2_setDisplay (ViSession instrumentHandle,
                                          ViBoolean dispState,
                                          ViInt16 dispMode,
                                          ViChar _VI_FAR messageText[]);

    ViStatus _VI_FUNC hp663x2_getDisplay (ViSession instrumentHandle,
                                          ViPBoolean dispState,
                                          ViPInt16 dispMode,
                                          ViChar _VI_FAR messageText[]);

    ViStatus _VI_FUNC hp663x2_measureVolt (ViSession instrumentHandle,
                                           ViInt16 measurement,
                                           ViPReal64 voltResult);

    ViStatus _VI_FUNC hp663x2_measureCurr (ViSession instrumentHandle,
                                           ViInt16 measurement,
                                           ViPReal64 currResult);

    ViStatus _VI_FUNC hp663x2_measureVoltArray (ViSession instrumentHandle,
            ViInt32 numReadings,
            ViReal64 _VI_FAR voltArray []);

    ViStatus _VI_FUNC hp663x2_measureCurrArray (ViSession instrumentHandle,
            ViInt32 numReadings,
            ViReal64 _VI_FAR currArray []);

    ViStatus _VI_FUNC hp663x2_fetchVolt (ViSession instrumentHandle,
                                         ViInt16 measurement,
                                         ViPReal64 voltResult);

    ViStatus _VI_FUNC hp663x2_fetchCurr (ViSession instrumentHandle,
                                         ViInt16 measurement,
                                         ViPReal64 currResult);

    ViStatus _VI_FUNC hp663x2_fetchVoltArray (ViSession instrumentHandle,
            ViInt32 numReadings,
            ViReal64 _VI_FAR voltArray []);

    ViStatus _VI_FUNC hp663x2_fetchCurrArray (ViSession instrumentHandle,
            ViInt32 numReadings,
            ViReal64 _VI_FAR currArray []);

    ViStatus _VI_FUNC hp663x2_setVoltAcqTrig (ViSession instrumentHandle,
            ViInt16 acqTrigSource,
            ViInt16 acqTrigVoltCount,
            ViInt16 acqTrigVoltSlope,
            ViReal64 acqTrigVoltLev,
            ViReal64 acqTrigVoltHyst);

    ViStatus _VI_FUNC hp663x2_getVoltAcqTrig (ViSession instrumentHandle,
            ViPInt16 acqTrigSource,
            ViPInt16 acqTrigVoltCount,
            ViPInt16 acqTrigVoltSlope,
            ViPReal64 acqTrigVoltLev,
            ViPReal64 acqTrigVoltHyst);

    ViStatus _VI_FUNC hp663x2_setCurrAcqTrig (ViSession instrumentHandle,
            ViInt16 acqTrigSource,
            ViInt16 acqTrigCurrCount,
            ViInt16 acqTrigCurrSlope,
            ViReal64 acqTrigCurrLev,
            ViReal64 acqTrigCurrHyst);

    ViStatus _VI_FUNC hp663x2_getCurrAcqTrig (ViSession instrumentHandle,
            ViPInt16 acqTrigSource,
            ViPInt16 acqTrigCurrCount,
            ViPInt16 acqTrigCurrSlope,
            ViPReal64 acqTrigCurrLev,
            ViPReal64 acqTrigCurrHyst);

    ViStatus _VI_FUNC hp663x2_setSweepParams (ViSession instrumentHandle,
            ViReal64 sampleInterval,
            ViInt32 sweepSize,
            ViInt32 sweepOffset);

    ViStatus _VI_FUNC hp663x2_getSweepParams (ViSession instrumentHandle,
            ViPReal64 sampleInterval,
            ViPInt32 sweepSize,
            ViPInt32 sweepOffset);

    ViStatus _VI_FUNC hp663x2_setMeasTimeout (ViSession instrumentHandle,
            ViInt32 measTimeout);

    ViStatus _VI_FUNC hp663x2_getMeasTimeout (ViSession instrumentHandle,
            ViPInt32 measTimeout);

    ViStatus _VI_FUNC hp663x2_clearOutpProt (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_setOutpRelayPolarity (ViSession instrumentHandle,
            ViInt16 relayPolarity);

    ViStatus _VI_FUNC hp663x2_getOutpRelayPolarity (ViSession instrumentHandle,
            ViPInt16 relayPolarity);

    ViStatus _VI_FUNC hp663x2_setOutpState (ViSession instrumentHandle,
                                            ViBoolean outputState,
                                            ViBoolean switchRelay);

    ViStatus _VI_FUNC hp663x2_getOutpState (ViSession instrumentHandle,
                                            ViPBoolean outputState);

    ViStatus _VI_FUNC hp663x2_setPonState (ViSession instrumentHandle,
                                           ViInt16 ponState);

    ViStatus _VI_FUNC hp663x2_getPonState (ViSession instrumentHandle,
                                           ViPInt16 ponState);

    ViStatus _VI_FUNC hp663x2_setOutpRelay (ViSession instrumentHandle,
                                            ViBoolean relayState);

    ViStatus _VI_FUNC hp663x2_getOutpRelay (ViSession instrumentHandle,
                                            ViPBoolean relayState);

    ViStatus _VI_FUNC hp663x2_setRiDfi (ViSession instrumentHandle,
                                        ViInt16 riMode,
                                        ViBoolean dfiState,
                                        ViInt16 dfiSourceBit);

    ViStatus _VI_FUNC hp663x2_getRiDfi (ViSession instrumentHandle,
                                        ViPInt16 riMode,
                                        ViPBoolean dfiState,
                                        ViPInt16 dfiSourceBit);

    ViStatus _VI_FUNC hp663x2_setOutpTrigSource (ViSession instrumentHandle,
            ViInt16 outpTrigSource);

    ViStatus _VI_FUNC hp663x2_getOutpTrigSource (ViSession instrumentHandle,
            ViPInt16 outpTrigSource);

    ViStatus _VI_FUNC hp663x2_getAcqSenseFunc (ViSession instrumentHandle,
            ViPInt16 senseFunction);

    ViStatus _VI_FUNC hp663x2_setCurrSenseParams (ViSession instrumentHandle,
            ViReal64 currSenseRange,
            ViInt16 currSenseDetector);

    ViStatus _VI_FUNC hp663x2_getCurrSenseParams (ViSession instrumentHandle,
            ViPReal64 currSenseRange,
            ViPInt16 currSenseDetector);

    ViStatus _VI_FUNC hp663x2_setWindowType (ViSession instrumentHandle,
            ViInt16 windowType);

    ViStatus _VI_FUNC hp663x2_getWindowType (ViSession instrumentHandle,
            ViPInt16 windowType);

    ViStatus _VI_FUNC hp663x2_setCurr (ViSession instrumentHandle,
                                       ViInt16 currParam,
                                       ViReal64 currLev);

    ViStatus _VI_FUNC hp663x2_getCurr (ViSession instrumentHandle,
                                       ViInt16 currQParam,
                                       ViPReal64 currLev);

    ViStatus _VI_FUNC hp663x2_setVolt (ViSession instrumentHandle,
                                       ViInt16 voltParam,
                                       ViReal64 voltLev);

    ViStatus _VI_FUNC hp663x2_getVolt (ViSession instrumentHandle,
                                       ViInt16 voltQParam,
                                       ViPReal64 voltLev);

    ViStatus _VI_FUNC hp663x2_setRes (ViSession instrumentHandle,
                                      ViInt16 resParam,
                                      ViReal64 resLev);

    ViStatus _VI_FUNC hp663x2_getRes(ViSession instrumentHandle,
                                     ViInt16 resQParam,
                                     ViPReal64 resLev);


    ViStatus _VI_FUNC hp663x2_setOcpParams (ViSession instrumentHandle,
                                            ViBoolean ocpEnable,
                                            ViReal64 ccDelay);

    ViStatus _VI_FUNC hp663x2_getOcpParams (ViSession instrumentHandle,
                                            ViPBoolean ocpEnable,
                                            ViPReal64 ccDelay);

    ViStatus _VI_FUNC hp663x2_setDigio (ViSession instrumentHandle, ViInt16 digOutData);

    ViStatus _VI_FUNC hp663x2_getDigio (ViSession instrumentHandle,
                                        ViPInt16 digInData);

    ViStatus _VI_FUNC hp663x2_getVoltAlcBandwidth (ViSession instrumentHandle,
            ViPInt32 alcBandwidth);

    ViStatus _VI_FUNC hp663x2_abort (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_arm (ViSession instrumentHandle, ViInt16 trigSystem);

    ViStatus _VI_FUNC hp663x2_trigger (ViSession instrumentHandle, ViInt16 subSystem);

    ViStatus _VI_FUNC hp663x2_trg (ViSession instrumentHandle);

    ViStatus _VI_FUNC hp663x2_stdEvent_Q (ViSession instrumentHandle,
                                          ViPInt16 stdEventStatus);

    ViStatus _VI_FUNC hp663x2_setEnableReg (ViSession instrumentHandle,
                                            ViInt16 enableRegister,
                                            ViInt16 enableMask);

    ViStatus _VI_FUNC hp663x2_getEnableReg (ViSession instrumentHandle,
                                            ViInt16 enableRegister,
                                            ViPInt16 enableMask);

    /***************************************************/
    /*  New constants/functions as of Rev. A.01.00     */
    /***************************************************/

#define HP663X2_QUES_FP				0x0008

    /***************************************************/
    /*  DVM                                            */
    /***************************************************/

#define HP663X2_ARM_DVM_ACQUISITION	6

#define HP663X2_DVM					2

    ViStatus _VI_FUNC hp663x2_fetchDvm (ViSession instrumentHandle,
                                        ViInt16 measurement,
                                        ViPReal64 voltResult);

    ViStatus _VI_FUNC hp663x2_getDvmAcqTrig (ViSession instrumentHandle,
            ViPInt16 acqTrigSource,
            ViPInt16 acqTrigVoltCount,
            ViPInt16 acqTrigVoltSlope,
            ViPReal64 acqTrigVoltLev,
            ViPReal64 acqTrigVoltHyst);

    ViStatus _VI_FUNC hp663x2_measureDvm (ViSession instrumentHandle,
                                          ViInt16 measurement,
                                          ViPReal64 voltResult);

    ViStatus _VI_FUNC hp663x2_setDvmAcqTrig (ViSession instrumentHandle,
            ViInt16 acqTrigSource,
            ViInt16 acqTrigVoltCount,
            ViInt16 acqTrigVoltSlope,
            ViReal64 acqTrigVoltLev,
            ViReal64 acqTrigVoltHyst);

    /***************************************************/
    /*  Second output                                  */
    /***************************************************/

#define HP663X2_QUES_UNREG2			0x0100
#define HP663X2_QUES_OCP2			0x1000

#define HP663X2_OPER_CV2			0x0200
#define HP663X2_OPER_CC2			0x1000

#define	HP663X2_CURR2_IMM			0
#define	HP663X2_CURR2_TRIG			1

#define	HP663X2_CURR2_Q             0
#define	HP663X2_CURR2_Q_MAX         1
#define	HP663X2_CURR2_Q_MIN         2
#define	HP663X2_CURR2_Q_TRIG        3
#define HP663X2_CURR2_Q_TRIG_MAX	4
#define HP663X2_CURR2_Q_TRIG_MIN	5

#define	HP663X2_VOLT2_IMM			0
#define	HP663X2_VOLT2_TRIG			1

#define	HP663X2_VOLT2_Q				0
#define	HP663X2_VOLT2_Q_MAX         1
#define	HP663X2_VOLT2_Q_MIN			2
#define	HP663X2_VOLT2_Q_TRIG        3
#define	HP663X2_VOLT2_Q_TRIG_MAX    4
#define	HP663X2_VOLT2_Q_TRIG_MIN    5

    ViStatus _VI_FUNC hp663x2_getCurr2 (ViSession instrumentHandle,
                                        ViInt16 currQParam,
                                        ViPReal64 currLev);

    ViStatus _VI_FUNC hp663x2_getDisplayChannel (ViSession instrumentHandle,
            ViPInt32 dispChannel);

    ViStatus _VI_FUNC hp663x2_getSenseProt (ViSession instrumentHandle,
                                            ViPBoolean senseProtState);

    ViStatus _VI_FUNC hp663x2_getVolt2 (ViSession instrumentHandle,
                                        ViInt16 voltQParam,
                                        ViPReal64 voltLev);

    ViStatus _VI_FUNC hp663x2_measureCurr2 (ViSession instrumentHandle,
                                            ViInt16 measurement,
                                            ViPReal64 currResult);

    ViStatus _VI_FUNC hp663x2_measureVolt2 (ViSession instrumentHandle,
                                            ViInt16 measurement,
                                            ViPReal64 voltResult);

    ViStatus _VI_FUNC hp663x2_outputVoltCurr2 (ViSession instrumentHandle,
            ViReal64 voltLev,
            ViReal64 currLev);

    ViStatus _VI_FUNC hp663x2_readOutputStatus2 (ViSession instrumentHandle,
            ViPInt16 outputStatus);

    ViStatus _VI_FUNC hp663x2_setCurr2 (ViSession instrumentHandle,
                                        ViInt16 currParam,
                                        ViReal64 currLev);

    ViStatus _VI_FUNC hp663x2_setDisplayChannel (ViSession instrumentHandle,
            ViInt32 dispChannel);

    ViStatus _VI_FUNC hp663x2_setMeasure2 (ViSession instrumentHandle,
                                           ViInt16 outputParam,
                                           ViReal64 outputLev,
                                           ViInt16 settlingTime,
                                           ViPReal64 voltQLev,
                                           ViPReal64 currQLev,
                                           ViPInt16 outputStatus);

    ViStatus _VI_FUNC hp663x2_setSenseProt (ViSession instrumentHandle,
                                            ViBoolean senseProtState);

    ViStatus _VI_FUNC hp663x2_setVolt2 (ViSession instrumentHandle,
                                        ViInt16 voltParam,
                                        ViReal64 voltLev);

    /* End of instrument specific declarations */

    /*---------------------------------------------------------------------------*/

    /* Used for "C" externs in C++ */
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif /* hp663x2_INCLUDE */

