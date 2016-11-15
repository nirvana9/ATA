'*****************************************************************************
'*  $Header:: /VXIPnP/Eagle/hp663x2/hp663x2.bas 1     5/09/00 11:19a Rkinn $ *
'*  Copyright 1998-2000 Agilent Technologies, Inc.                           *
'*---------------------------------------------------------------------------*
'*  Driver for HP663X2 Dynamic Measurement DC Source                         *
'*****************************************************************************

'*****************************************************************************
'*  STANDARD SECTION                                                         *
'*  Constants and function prototypes for HP standard functions.             *
'*****************************************************************************

'        ***************************************************
'        *  Standard constant error conditions returned    *
'        *  by driver functions.                           *
'        *    HP Common Error numbers start at BFFC0D00    *
'        *    The parameter errors extend the number of    *
'        *      errors over the eight defined in VPP 3.4   *
'        ***************************************************

Global Const HP663X2_INSTR_ERROR_NULL_PTR = &HBFFC0D02
Global Const HP663X2_INSTR_ERROR_RESET_FAILED = &HBFFC0D03
Global Const HP663X2_INSTR_ERROR_UNEXPECTED = &HBFFC0D04
Global Const HP663X2_INSTR_ERROR_INV_SESSION = &HBFFC0D05
Global Const HP663X2_INSTR_ERROR_LOOKUP = &HBFFC0D06
Global Const HP663X2_INSTR_ERROR_DETECTED = &HBFFC0D07

Global Const HP663X2_INSTR_ERROR_PARAMETER9 = &HBFFC0D20
Global Const HP663X2_INSTR_ERROR_PARAMETER10 = &HBFFC0D21
Global Const HP663X2_INSTR_ERROR_PARAMETER11 = &HBFFC0D22
Global Const HP663X2_INSTR_ERROR_PARAMETER12 = &HBFFC0D23
Global Const HP663X2_INSTR_ERROR_PARAMETER13 = &HBFFC0D24
Global Const HP663X2_INSTR_ERROR_PARAMETER14 = &HBFFC0D25
Global Const HP663X2_INSTR_ERROR_PARAMETER15 = &HBFFC0D26
Global Const HP663X2_INSTR_ERROR_PARAMETER16 = &HBFFC0D27
Global Const HP663X2_INSTR_ERROR_PARAMETER17 = &HBFFC0D28
Global Const HP663X2_INSTR_ERROR_PARAMETER18 = &HBFFC0D29

'        ***************************************************
'        *  Constants used by system status functions      *
'        *    These defines are bit numbers which define   *
'        *    the operation and questionable registers.    *
'        *    They are instrument specific.                *
'        ***************************************************

Global Const HP663X2_STB_QUES = &H8
Global Const HP663X2_STB_MAV = &H10
Global Const HP663X2_STB_ESB = &H20
Global Const HP663X2_STB_MSS = &H40
Global Const HP663X2_STB_RQS = &H40
Global Const HP663X2_STB_OPER = &H80

Global Const HP663X2_SRE_QUES = &H8
Global Const HP663X2_SRE_MAV = &H10
Global Const HP663X2_SRE_ESB = &H20
Global Const HP663X2_SRE_OPER = &H80

Global Const HP663X2_OPER_CAL = &H1
Global Const HP663X2_OPER_WTG = &H20
Global Const HP663X2_OPER_CV = &H100
Global Const HP663X2_OPER_CC_POS = &H400
Global Const HP663X2_OPER_CC_NEG = &H800

Global Const HP663X2_QUES_OV = &H1
Global Const HP663X2_QUES_OCP = &H2
Global Const HP663X2_QUES_FUSE = &H4
Global Const HP663X2_QUES_OT = &H10
Global Const HP663X2_QUES_RI = &H200
Global Const HP663X2_QUES_UNREG = &H400
Global Const HP663X2_QUES_OVLD = &H4000

Global Const HP663X2_ESR_OPC = &H1
Global Const HP663X2_ESR_QYE = &H4
Global Const HP663X2_ESR_DDE = &H8
Global Const HP663X2_ESR_EXE = &H10
Global Const HP663X2_ESR_CME = &H20
Global Const HP663X2_ESR_PON = &H80

Global Const HP663X2_REG_ESE = 0
Global Const HP663X2_REG_SRE = 1
Global Const HP663X2_REG_OPER = 2
Global Const HP663X2_REG_QUES = 3

Global Const HP663X2_CMDSTRING_MAX = 256

Global Const HP663X2_CMDSTRING_Q_MIN = 2
Global Const HP663X2_CMDSTRING_Q_MAX = 32767&

'        ***************************************************
'        *  Constants used by function hp663x2_timeOut      *
'        ***************************************************

Global Const HP663X2_TIMEOUT_MAX = 2147483647
Global Const HP663X2_TIMEOUT_MIN = 1&

'        ***************************************************
'        *  Required plug and play functions from VPP-3.1  *
'        ***************************************************

Declare Function hp663x2_init Lib "hp663x2_32.dll" _
        (ByVal resourceName As String, _
         ByVal IDQuery As Integer, _
         ByVal resetDevice As Integer, _
               instrumentHandle As Long) As Long

Declare Function hp663x2_close Lib "hp663x2_32.dll" _
        (ByVal instrumentHandle As Long) As Long

Declare Function hp663x2_reset Lib "hp663x2_32.dll" _
        (ByVal instrumentHandle As Long) As Long

Declare Function hp663x2_self_test Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           selfTestResult As Integer, _
     ByVal selfTestMessage As String) As Long

Declare Function hp663x2_error_query Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           errorCode As Long, _
     ByVal errorMessage As String) As Long

Declare Function hp663x2_error_message Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal statusCode As Long, _
     ByVal message As String) As Long

Declare Function hp663x2_revision_query Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal instrumentDriverRevision As String, _
     ByVal firmwareRevision As String) As Long


'        ***************************************************
'        *  HP standard utility functions                  *
'        ***************************************************

Declare Function hp663x2_timeOut Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal setTimeOut As Long) As Long

Declare Function hp663x2_timeOut_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           timeOut As Long) As Long

Declare Function hp663x2_errorQueryDetect Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal setErrorQueryDetect As Integer) As Long

Declare Function hp663x2_errorQueryDetect_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           errorQueryDetect As Integer) As Long

Declare Function hp663x2_dcl Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long) As Long

Declare Function hp663x2_opc_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           instrumentReady As Integer) As Long


'        ***************************************************
'        *  HP standard status functions                   *
'        ***************************************************

Declare Function hp663x2_readStatusByte_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           statusByte As Integer) As Long

Declare Function hp663x2_operEvent_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           operationEventRegister As Long) As Long

Declare Function hp663x2_operCond_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           operationConditionRegister As Long) As Long

Declare Function hp663x2_quesEvent_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           questionableEventRegister As Long) As Long

Declare Function hp663x2_quesCond_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           questionableConditionRegister As Long) As Long


'        ***************************************************
'        *  HP standard command passthrough functions      *
'        ***************************************************

Declare Function hp663x2_cmd Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sendStringCommand As String) As Long

Declare Function hp663x2_cmdString_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryStringCommand As String, _
     ByVal stringSize As Long, _
     ByVal stringResult As String) As Long

Declare Function hp663x2_cmdInt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sendIntegerCommand As String, _
     ByVal sendInteger As Long) As Long

Declare Function hp663x2_cmdInt16_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryI16Command As String, _
           i16Result As Integer) As Long



'* End of HP standard declarations */
'*---------------------------------------------------------------------------*


'*****************************************************************************
'*  INSTRUMENT SPECIFIC SECTION                                              *
'*  Constants and function prototypes for instrument specific functions.     *
'*****************************************************************************

'        ***************************************************
'        *  Instrument specific error numbers              *
'        ***************************************************
Global Const HP663X2_INSTR_ERROR_NVRAM_RD0_CS = 1
Global Const HP663X2_INSTR_ERROR_NVRAM_CONFIG_CS = 2
Global Const HP663X2_INSTR_ERROR_NVRAM_CAL_CS = 3
Global Const HP663X2_INSTR_ERROR_NVRAM_STATE_CS = 4
Global Const HP663X2_INSTR_ERROR_NVRAM_RST_CS = 5
Global Const HP663X2_INSTR_ERROR_RAM_SELFTEST = 10
Global Const HP663X2_INSTR_ERROR_DAC_SELFTEST1 = 11
Global Const HP663X2_INSTR_ERROR_DAC_SELFTEST2 = 12
Global Const HP663X2_INSTR_ERROR_DAC_SELFTEST3 = 13
Global Const HP663X2_INSTR_ERROR_DAC_SELFTEST4 = 14
Global Const HP663X2_INSTR_ERROR_OVDAC_SELFTEST = 15
Global Const HP663X2_INSTR_ERROR_DIGIO_SELFTEST = 80
Global Const HP663X2_INSTR_ERROR_INGUARD_RXBUF_OVR = 213
Global Const HP663X2_INSTR_ERROR_RS232_FRAMING = 216
Global Const HP663X2_INSTR_ERROR_RS232_PARITY = 217
Global Const HP663X2_INSTR_ERROR_RS232_RX_OVR = 218
Global Const HP663X2_INSTR_ERROR_FP_UART_OVR = 220
Global Const HP663X2_INSTR_ERROR_FP_UART_FRAMING = 221
Global Const HP663X2_INSTR_ERROR_FP_UART_PARITY = 222
Global Const HP663X2_INSTR_ERROR_FP_RXBUF_OVR = 223
Global Const HP663X2_INSTR_ERROR_FP_TIMEOUT = 224
Global Const HP663X2_INSTR_ERROR_CAL_SWITCH = 401
Global Const HP663X2_INSTR_ERROR_CAL_PASSWORD = 402
Global Const HP663X2_INSTR_ERROR_CAL_DISABLED = 403
Global Const HP663X2_INSTR_ERROR_CAL_RB_CONST = 404
Global Const HP663X2_INSTR_ERROR_CAL_PROG_CONST = 405
Global Const HP663X2_INSTR_ERROR_CAL_CMD_SEQUENCE = 406
Global Const HP663X2_INSTR_ERROR_CVCC_STATUS = 407
Global Const HP663X2_INSTR_ERROR_ALC_NOT_NORMAL = 408
Global Const HP663X2_INSTR_ERROR_TOO_MANY_SWE_POINTS = 601
Global Const HP663X2_INSTR_ERROR_RS232_CMD_ONLY = 602
Global Const HP663X2_INSTR_ERROR_INCOMPATIBLE_FETCH = 603
Global Const HP663X2_INSTR_ERROR_MEAS_OVERRANGE = 604

Global Const HP663X2_DRV_ERROR_DIGIO_WRONG_MODE = 800

'        /***************************************************/
'        /*  Instrument specific constants                  */
'        /***************************************************/

Global Const HP663X2_VOLTAGE = 0
Global Const HP663X2_CURRENT = 1

Global Const HP663X2_DISP_NORMAL = 0
Global Const HP663X2_DISP_TEXT = 1

Global Const HP663X2_MEAS_DC = 0
Global Const HP663X2_MEAS_ACDC = 1
Global Const HP663X2_MEAS_HIGH = 2
Global Const HP663X2_MEAS_LOW = 3
Global Const HP663X2_MEAS_MAX = 4
Global Const HP663X2_MEAS_MIN = 5

Global Const HP663X2_TRIG_SRC_BUS = 0
Global Const HP663X2_TRIG_SRC_INT = 1

Global Const HP663X2_TRIG_COUNT_MAX = 100
Global Const HP663X2_TRIG_COUNT_MIN = 1

Global Const HP663X2_TRIG_SLOPE_EITHER = 0
Global Const HP663X2_TRIG_SLOPE_POS = 1
Global Const HP663X2_TRIG_SLOPE_NEG = 2

Global Const HP663X2_RELAY_POLARITY_NORMAL = 0
Global Const HP663X2_RELAY_POLARITY_REVERSE = 1

Global Const HP663X2_RI_OFF = 0
Global Const HP663X2_RI_LATCHING = 1
Global Const HP663X2_RI_LIVE = 2

Global Const HP663X2_DFI_SRC_PREV = 0
Global Const HP663X2_DFI_SRC_QUES = 1
Global Const HP663X2_DFI_SRC_OPER = 2
Global Const HP663X2_DFI_SRC_ESB = 3
Global Const HP663X2_DFI_SRC_RQS = 4
Global Const HP663X2_DFI_SRC_OFF = 5

Global Const HP663X2_DIG_OUT_MAX = 7
Global Const HP663X2_DIG_OUT_MIN = 0

Global Const HP663X2_PON_STATE_RST = 0
Global Const HP663X2_PON_STATE_RCL0 = 1

Global Const HP663X2_SENSE_FUNC_VOLT = 0
Global Const HP663X2_SENSE_FUNC_CURR = 1

Global Const HP663X2_CURR_DET_ACDC = 0
Global Const HP663X2_CURR_DET_DC = 1

Global Const HP663X2_CURR_IMM = 0
Global Const HP663X2_CURR_TRIG = 1

Global Const HP663X2_CURR_Q = 0
Global Const HP663X2_CURR_Q_TRIG = 1
Global Const HP663X2_CURR_Q_MAX = 2
Global Const HP663X2_CURR_Q_MIN = 3

Global Const HP663X2_VOLT_IMM = 0
Global Const HP663X2_VOLT_TRIG = 1
Global Const HP663X2_VOLT_OVP = 2
Global Const HP663X2_VOLT_OVP_MAX = 3
Global Const HP663X2_VOLT_OVP_MIN = 4

Global Const HP663X2_VOLT_Q = 0
Global Const HP663X2_VOLT_Q_MAX = 1
Global Const HP663X2_VOLT_Q_MIN = 2
Global Const HP663X2_VOLT_Q_TRIG = 3
Global Const HP663X2_VOLT_Q_TRIG_MAX = 4
Global Const HP663X2_VOLT_Q_TRIG_MIN = 5
Global Const HP663X2_VOLT_Q_OVP = 6
Global Const HP663X2_VOLT_Q_OVP_MAX = 7
Global Const HP663X2_VOLT_Q_OVP_MIN = 8

Global Const HP663X2_ARM_OUTPUT_ONCE = 0
Global Const HP663X2_ARM_OUTPUT_CONTINUOUS = 1
Global Const HP663X2_ARM_ACQUISITION = 2
Global Const HP663X2_ARM_VOLT_ACQUISITION = 3
Global Const HP663X2_ARM_CURR_ACQUISITION = 4

Global Const HP663X2_TRIG_OUTPUT = 0
Global Const HP663X2_TRIG_ACQUISITION = 1

Global Const HP663X2_SET_VOLT = 0
Global Const HP663X2_SET_CURR = 1

Global Const HP663X2_OUTP_STATUS_OFF = 0
Global Const HP663X2_OUTP_STATUS_CV = 1
Global Const HP663X2_OUTP_STATUS_CC = 2
Global Const HP663X2_OUTP_STATUS_UNREG = 3
Global Const HP663X2_OUTP_STATUS_PROT = 4
Global Const HP663X2_OUTP_STATUS_UNKNOWN = 5

Global Const HP663X2_VOLT_TRIG_MIN = 0
Global Const HP663X2_VOLT_MIN = 0
Global Const HP663X2_CURR_TRIG_MIN = 0
Global Const HP663X2_CURR_MIN = 0

Global Const HP663X2_SENSE_WIND_RECT = 0
Global Const HP663X2_SENSE_WIND_HANN = 1

Global Const HP663X2_ACQ_COUNT_MIN = 1
Global Const HP663X2_ACQ_COUNT_MAX = 100

'        ***************************************************
'        *  Instrument specific functions                  *
'        ***************************************************
Declare Function hp663x2_outputVoltCurr Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal voltLev As Double, _
     ByVal currLev As Double) As Long

Declare Function hp663x2_readOutputStatus Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           outputStatus As Integer) As Long

Declare Function hp663x2_rippleRiDfi Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long) As Long

Declare Function hp663x2_setMeasure Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal outputParam As Integer, _
     ByVal outputLev As Double, _
     ByVal settlingTime As Integer, _
           voltQlev As Double, _
           currQlev As Double, _
           outputStatus As Integer) As Long

Declare Function hp663x2_setDisplay Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal dispState As Integer, _
     ByVal dispMode As Integer, _
     ByVal messageText As String) As Long

Declare Function hp663x2_getDisplay Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           dispState As Integer, _
           dispMode As Integer, _
     ByVal messageText As String) As Long

Declare Function hp663x2_measureVolt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal measurement As Integer, _
           measResult As Double) As Long

Declare Function hp663x2_measureCurr Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal measurement As Integer, _
           measResult As Double) As Long

Declare Function hp663x2_measureVoltArray Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal numReadings As Integer, _
           waveformArray As Double) As Long

Declare Function hp663x2_measureCurrArray Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal numReadings As Integer, _
           waveformArray As Double) As Long

Declare Function hp663x2_fetchVolt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal measurement As Integer, _
           fetchResult As Double) As Long

Declare Function hp663x2_fetchCurr Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal measurement As Integer, _
           fetchResult As Double) As Long

Declare Function hp663x2_fetchVoltArray Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal numReadings As Integer, _
           waveformArray As Double) As Long

Declare Function hp663x2_fetchCurrArray Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal numReadings As Integer, _
           waveformArray As Double) As Long

Declare Function hp663x2_setVoltAcqTrig Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal acqTrigSource As Integer, _
     ByVal acqTrigVoltCount As Integer, _
     ByVal acqTrigVoltSlope As Integer, _
     ByVal acqTrigVoltLev As Double, _
     ByVal acqTrigVoltHyst As Double) As Long

Declare Function hp663x2_getVoltAcqTrig Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           acqTrigSource As Integer, _
           acqTrigVoltCount As Integer, _
           acqTrigVoltSlope As Integer, _
           acqTrigVoltLev As Double, _
           acqTrigVoltHyst As Double) As Long

Declare Function hp663x2_setCurrAcqTrig Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal acqTrigSource As Integer, _
     ByVal acqTrigCurrCount As Integer, _
     ByVal acqTrigCurrSlope As Integer, _
     ByVal acqTrigCurrLev As Double, _
     ByVal acqTrigCurrHyst As Double) As Long

Declare Function hp663x2_getCurrAcqTrig Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           acqTrigSource As Integer, _
           acqTrigCurrCount As Integer, _
           acqTrigCurrSlope As Integer, _
           acqTrigCurrLev As Double, _
           acqTrigCurrHyst As Double) As Long

Declare Function hp663x2_setSweepParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sampleInterval As Double, _
     ByVal sweepSize As Long, _
     ByVal sweepOffset As Long) As Long

Declare Function hp663x2_getSweepParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           sampleInterval As Double, _
           sweepSize As Long, _
           sweepOffset As Long) As Long

Declare Function hp663x2_setMeasTimeout Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal milliSeconds As Long) As Long

Declare Function hp663x2_getMeasTimeout Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           milliSeconds As Long) As Long

Declare Function hp663x2_clearOutpProt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long) As Long

Declare Function hp663x2_setOutpRelayPolarity Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal relayPolarity As Integer) As Long

Declare Function hp663x2_getOutpRelayPolarity Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           relayPolarity As Integer) As Long

Declare Function hp663x2_setOutpState Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal outputState As Integer, _
     ByVal switchRelay As Integer) As Long

Declare Function hp663x2_getOutpState Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           outputState As Integer) As Long

Declare Function hp663x2_setPonState Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal ponState As Integer) As Long

Declare Function hp663x2_getPonState Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           ponState As Integer) As Long

Declare Function hp663x2_setOutpRelay Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal relayState As Integer) As Long

Declare Function hp663x2_getOutpRelay Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           relayState As Integer) As Long

Declare Function hp663x2_setRiDfi Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal riMode As Integer, _
     ByVal dfiState As Integer, _
     ByVal dfiSourceBit As Integer) As Long

Declare Function hp663x2_getRiDfi Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           riMode As Integer, _
           dfiState As Integer, _
           dfiSourceBit As Integer) As Long

Declare Function hp663x2_setOutpTrigSource Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal outpTrigSource As Integer) As Long

Declare Function hp663x2_getOutpTrigSource Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           outpTrigSource As Integer) As Long

Declare Function hp663x2_getAcqSenseFunc Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           senseFunction As Integer) As Long

Declare Function hp663x2_setCurrSenseParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal currSenseRange As Double, _
     ByVal currSenseDetector As Integer) As Long

Declare Function hp663x2_getCurrSenseParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           currSenseRange As Double, _
           currSenseDetector As Integer) As Long

Declare Function hp663x2_setWindowType Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal windowType As Integer) As Long

Declare Function hp663x2_getWindowType Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           windowType As Integer) As Long

Declare Function hp663x2_setCurr Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal currQParam As Integer, _
     ByVal currLev As Double) As Long

Declare Function hp663x2_getCurr Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal currQParam As Integer, _
           currLev As Double) As Long

Declare Function hp663x2_setVolt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal voltQParam As Integer, _
     ByVal voltLev As Double) As Long

Declare Function hp663x2_getVolt Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal voltQParam As Integer, _
           voltLev As Double) As Long

Declare Function hp663x2_setOcpParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal ocpEnable As Integer, _
     ByVal ccDelay As Double) As Long

Declare Function hp663x2_getOcpParams Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           ocpEnable As Integer, _
           ccDelay As Double) As Long

Declare Function hp663x2_setDigio Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal digInData As Integer) As Long

Declare Function hp663x2_getDigio Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           digInData As Integer) As Long

Declare Function hp663x2_getVoltAlcBandwidth Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           alcBandwidth As Double) As Long

Declare Function hp663x2_arm Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal trigSystem As Integer) As Long

Declare Function hp663x2_trigger Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal subSystem As Integer) As Long

Declare Function hp663x2_stdEvent_Q Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
           stdEventStatus As Integer) As Long

Declare Function hp663x2_setEnableReg Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal enableRegister As Integer, _
     ByVal enableMask As Integer) As Long

Declare Function hp663x2_getEnableReg Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal enableRegister As Integer, _
           enableMask As Integer) As Long

Declare Function hp663x2_doDelay Lib "hp663x2_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal milliSeconds As Long) As Long
     
'        ***************************************************
'        *  New constants/functions as of Rev. A.01.00     *
'        ***************************************************

Global Const HP663X2_QUES_FP = &H0008

'        ***************************************************
'        *  DVM                                            *
'        ***************************************************

Global Const HP663X2_ARM_DVM_ACQUISITION = 6

Global Const HP663X2_DVM = 2

Global Const HP663X2_DVM_MIN = 0

Declare Function hp663x2_fetchDvm Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal measurement As Integer, _
	       voltResult As Double) As Long

Declare Function hp663x2_getDvmAcqTrig Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	       acqTrigSource As Integer, _
	       acqTrigVoltCount As Integer, _
	       acqTrigVoltSlope As Integer, _
	       acqTrigVoltLev As Double, _
	       acqTrigVoltHyst As Double) As Long

Declare Function hp663x2_measureDvm Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal measurement As Integer, _
	       voltResult As Double) As Long

Declare Function hp663x2_setDvmAcqTrig Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal acqTrigSource As Integer, _
	 ByVal acqTrigVoltCount As Integer, _
	 ByVal acqTrigVoltSlope As Integer, _
	 ByVal acqTrigVoltLev As Double, _
	 ByVal acqTrigVoltHyst As Double) As Long

'        ***************************************************/
'        *  Second output                                  */
'        ***************************************************/

Global Const HP663X2_QUES_UNREG2 = &H0100
Global Const HP663X2_QUES_OCP2 = &H1000

Global Const HP663X2_OPER_CV2 = &H0200
Global Const HP663X2_OPER_CC2 = &H1000

Global Const HP663X2_CURR2_IMM = 0
Global Const HP663X2_CURR2_TRIG = 1

Global Const HP663X2_CURR2_Q = 0
Global Const HP663X2_CURR2_Q_MAX = 1
Global Const HP663X2_CURR2_Q_MIN = 2
Global Const HP663X2_CURR2_Q_TRIG = 3
Global Const HP663X2_CURR2_Q_TRIG_MAX = 4
Global Const HP663X2_CURR2_Q_TRIG_MIN = 5

Global Const HP663X2_VOLT2_IMM = 0
Global Const HP663X2_VOLT2_TRIG = 1

Global Const HP663X2_VOLT2_Q = 0
Global Const HP663X2_VOLT2_Q_MAX = 1
Global Const HP663X2_VOLT2_Q_MIN = 2
Global Const HP663X2_VOLT2_Q_TRIG = 3
Global Const HP663X2_VOLT2_Q_TRIG_MAX = 4
Global Const HP663X2_VOLT2_Q_TRIG_MIN = 5

Declare Function hp663x2_getCurr2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal currQParam As Integer, _
	       currLev As Double) As Long

Declare Function hp663x2_getDisplayChannel Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	       dispChannel As Long) As Long

Declare Function hp663x2_getSenseProt Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	       senseProtState As Integer) As Long

Declare Function hp663x2_getVolt2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal voltQParam As Integer, _
	       voltLev As Double) As Long

Declare Function hp663x2_measureCurr2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal measurement As Integer, _
	       currResult As Double) As Long

Declare Function hp663x2_measureVolt2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal measurement As Integer, _
	       voltResult As Double) As Long

Declare Function hp663x2_outputVoltCurr2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal voltLev As Double, _
	 ByVal currLev As Double) As Long

Declare Function hp663x2_readOutputStatus2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	       outputStatus As Integer) As Long

Declare Function hp663x2_setCurr2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal currParam As Integer, _
	 ByVal currLev As Double) As Long

Declare Function hp663x2_setDisplayChannel Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal dispChannel As Long) As Long

Declare Function hp663x2_setMeasure2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal outputParam As Integer, _
	 ByVal outputLev As Double, _
	 ByVal settlingTime As Integer, _
	       voltQLev As Double, _
	       currQLev As Double, _
	       outputStatus As Integer) As Long

Declare Function hp663x2_setSenseProt Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal senseProtState As Integer) As Long

Declare Function hp663x2_setVolt2 Lib "hp663x2_32.dll" _
	(ByVal instrumentHandle As Long, _
	 ByVal voltParam As Integer, _
	 ByVal voltLev As Double) As Long


'* End of instrument specific declarations *
'*---------------------------------------------------------------------------*

