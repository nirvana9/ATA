Attribute VB_Name = "Module1"
'*****************************************************************************
'*  age1960.bas                                                              *
'*  Copyright (C) 1999 - 2002 Agilent Technologies                           *
'*---------------------------------------------------------------------------*
'*  Driver for age1960, GSM Mobile Test for 8960 Series 10 E5515             *
'*  Driver Version: A.05.00                                                  *
'*****************************************************************************

'*****************************************************************************
'*  STANDARD SECTION                                                         *
'*  Constants and function prototypes for Agilent standard functions.        *
'*****************************************************************************

'        ***************************************************
'        *  Standard constant used by driver functions     *
'        *  that are defined in visa32.bas                 *
'        *  If you don't include visa32.bas, uncomment the *
'        *  following constants                            *
'        ***************************************************

'Global Const VI_SUCCESS = 0
'Global Const VI_TRUE = 1
'Global Const VI_FALSE = 0
'Global Const VI_NULL = 0
'
'        ***************************************************
'        *  Standard constant error conditions returned    *
'        *  by driver functions.                           *
'        *  Agilent Common Error numbers start at BFFC0D00 *
'        *  The parameter errors extend the number of      *
'        *  errors over the eight defined in VPP 3.4       *
'        ***************************************************

Global Const age1960_INSTR_ERROR_NULL_PTR = &HBFFC0D02
Global Const age1960_INSTR_ERROR_RESET_FAILED = &HBFFC0D03
Global Const age1960_INSTR_ERROR_UNEXPECTED = &HBFFC0D04
Global Const age1960_INSTR_ERROR_INV_SESSION = &HBFFC0D05
Global Const age1960_INSTR_ERROR_LOOKUP = &HBFFC0D06
Global Const age1960_INSTR_ERROR_DETECTED = &HBFFC0D07
Global Const age1960_INSTR_NO_LAST_COMMA = &HBFFC0D08

Global Const age1960_INSTR_ERROR_PARAMETER9 = &HBFFC0D20
Global Const age1960_INSTR_ERROR_PARAMETER10 = &HBFFC0D21
Global Const age1960_INSTR_ERROR_PARAMETER11 = &HBFFC0D22
Global Const age1960_INSTR_ERROR_PARAMETER12 = &HBFFC0D23
Global Const age1960_INSTR_ERROR_PARAMETER13 = &HBFFC0D24
Global Const age1960_INSTR_ERROR_PARAMETER14 = &HBFFC0D25
Global Const age1960_INSTR_ERROR_PARAMETER15 = &HBFFC0D26
Global Const age1960_INSTR_ERROR_PARAMETER16 = &HBFFC0D27
Global Const age1960_INSTR_ERROR_PARAMETER17 = &HBFFC0D28
Global Const age1960_INSTR_ERROR_PARAMETER18 = &HBFFC0D29

Global Const age1960_CMDSTRING_Q_MIN = 2
Global Const age1960_CMDSTRING_Q_MAX = 32767
Global Const age1960_CMDINT_MIN = -2147483647
Global Const age1960_CMDINT_MAX = 2147483647
Global Const age1960_CMDREAL_MIN = -1E+300
Global Const age1960_CMDREAL_MAX = 1E+300

'        ***************************************************
'        *  Constants used by system status functions      *
'        *    These defines are bit numbers which define   *
'        *    the operation and questionable registers.    *
'        *    They are instrument specific.                *
'        ***************************************************

Global Const age1960_QUES_SUMMARY = 0
Global Const age1960_QUES_CALL = 1
Global Const age1960_QUES_ERROR = 2
Global Const age1960_QUES_HARDWARE = 3
Global Const age1960_QUES_CALL_GSM = 4
Global Const age1960_QUES_ERROR_GSM = 5

Global Const age1960_OPER_SUMMARY = 0
Global Const age1960_OPER_NMRR = 1
Global Const age1960_OPER_CALL = 2
Global Const age1960_OPER_NMRR_GSM = 3
Global Const age1960_OPER_CALL_GSM = 4

'        ****************************************************
'        *  Constants used by system status functions       *
'        *    These defines are bit numbers which define    *
'        *    the operation and questionable registers.     *
'        *    They are instrument specific.                 *
'        ****************************************************

Global Const age1960_REGISTER_MIN = 0
Global Const age1960_REGISTER_MAX = 32767

Global Const age1960_STD_REGISTER_MIN = 0
Global Const age1960_STD_REGISTER_MAX = 255

Global Const age1960_ESE_OPC = 1
Global Const age1960_ESE_AVAIL1 = 2
Global Const age1960_ESE_QUERY_ERR = 4
Global Const age1960_ESE_DEVICE_ERR = 8
Global Const age1960_ESE_EXEC_ERR = 16
Global Const age1960_ESE_CMD_ERR = 32
Global Const age1960_ESE_AVAIL6 = 64
Global Const age1960_ESE_PWR_ON = 128

Global Const age1960_STB_OPC = 1
Global Const age1960_STB_AVAIL1 = 2
Global Const age1960_STB_ERR_QUEUE = 4
Global Const age1960_STB_QUES_SUM = 8
Global Const age1960_STB_MAV = 16
Global Const age1960_STB_ESE_SUM = 32
Global Const age1960_STB_RQS = 64
Global Const age1960_STB_OPER_SUM = 128

Global Const age1960_QUES_SUM_AVAIL0 = 1
Global Const age1960_QUES_SUM_ERRORS = 2
Global Const age1960_QUES_SUM_AVAIL2 = 4
Global Const age1960_QUES_SUM_AVAIL3 = 8
Global Const age1960_QUES_SUM_AVAIL4 = 16
Global Const age1960_QUES_SUM_AVAIL5 = 32
Global Const age1960_QUES_SUM_AVAIL6 = 64
Global Const age1960_QUES_SUM_AVAIL7 = 128
Global Const age1960_QUES_SUM_AVAIL8 = 256
Global Const age1960_QUES_SUM_AVAIL9 = 512
Global Const age1960_QUES_SUM_CALL = 1024
Global Const age1960_QUES_SUM_HW = 2048
Global Const age1960_QUES_SUM_AVAIL12 = 4096
Global Const age1960_QUES_SUM_AVAIL13 = 8192
Global Const age1960_QUES_SUM_AVAIL14 = 16384
Global Const age1960_QUES_SUM_AVAIL15 = 32768

Global Const age1960_OPER_SUM_AVAIL0 = 1
Global Const age1960_OPER_SUM_AVAIL1 = 2
Global Const age1960_OPER_SUM_AVAIL2 = 4
Global Const age1960_OPER_SUM_AVAIL3 = 8
Global Const age1960_OPER_SUM_AVAIL4 = 16
Global Const age1960_OPER_SUM_AVAIL5 = 32
Global Const age1960_OPER_SUM_AVAIL6 = 64
Global Const age1960_OPER_SUM_AVAIL7 = 128
Global Const age1960_OPER_SUM_AVAIL8 = 256
Global Const age1960_OPER_SUM_NMRR = 512
Global Const age1960_OPER_SUM_CALL = 1024
Global Const age1960_OPER_SUM_AVAIL11 = 2048
Global Const age1960_OPER_SUM_SYSSYNC = 4096
Global Const age1960_OPER_SUM_AVAIL13 = 8192
Global Const age1960_OPER_SUM_AVAIL14 = 16384
Global Const age1960_OPER_SUM_AVAIL15 = 32768

Global Const age1960_QUES_ERR_AVAIL0 = 1
Global Const age1960_QUES_ERR_100 = 2
Global Const age1960_QUES_ERR_200 = 4
Global Const age1960_QUES_ERR_300 = 8
Global Const age1960_QUES_ERR_400 = 16
Global Const age1960_QUES_ERR_500 = 32
Global Const age1960_QUES_ERR_600 = 64
Global Const age1960_QUES_ERR_700 = 128
Global Const age1960_QUES_ERR_800 = 256
Global Const age1960_QUES_ERR_900 = 512
Global Const age1960_QUES_ERR_AVAIL10 = 1024
Global Const age1960_QUES_ERR_AVAIL11 = 2048
Global Const age1960_QUES_ERR_AVAIL12 = 4096
Global Const age1960_QUES_ERR_AVAIL13 = 8192
Global Const age1960_QUES_ERR_AVAIL14 = 6384
Global Const age1960_QUES_ERR_AVAIL15 = 32768

Global Const age1960_OPER_NMRR_AVAIL0 = 1
Global Const age1960_OPER_NMRR_TXP = 2
Global Const age1960_OPER_NMRR_PVT = 4
Global Const age1960_OPER_NMRR_PFER = 8
Global Const age1960_OPER_NMRR_ORFS = 16
Global Const age1960_OPER_NMRR_AAUD = 32
Global Const age1960_OPER_NMRR_DAUD = 64
Global Const age1960_OPER_NMRR_FBER = 128
Global Const age1960_OPER_NMRR_BER = 256
Global Const age1960_OPER_NMRR_IQT = 512
Global Const age1960_OPER_NMRR_DPOW = 1024
Global Const age1960_OPER_NMRR_AVAIL11 = 2048
Global Const age1960_OPER_NMRR_AVAIL12 = 4096
Global Const age1960_OPER_NMRR_AVAIL13 = 8192
Global Const age1960_OPER_NMRR_AVAIL14 = 16384
Global Const age1960_OPER_NMRR_AVAIL15 = 32768

Global Const age1960_QUES_CALL_AVAIL0 = 1
Global Const age1960_QUES_CALL_DATA = 2
Global Const age1960_QUES_CALL_RADIO = 4
Global Const age1960_QUES_CALL_IMM = 8
Global Const age1960_QUES_CALL_CHAN = 16
Global Const age1960_QUES_CALL_HANDOVER = 32
Global Const age1960_QUES_CALL_NO_RESP = 64
Global Const age1960_QUES_CALL_FRAMES = 128
Global Const age1960_QUES_CALL_ID = 256
Global Const age1960_QUES_CALL_AVAIL9 = 512
Global Const age1960_QUES_CALL_AVAIL10 = 1024
Global Const age1960_QUES_CALL_AVAIL11 = 2048
Global Const age1960_QUES_CALL_AVAIL12 = 4096
Global Const age1960_QUES_CALL_AVAIL13 = 8192
Global Const age1960_QUES_CALL_AVAIL14 = 16384
Global Const age1960_QUES_CALL_IT15 = 32768

Global Const age1960_OPER_CALL_AVAIL0 = 1
Global Const age1960_OPER_CALL_IDLE = 2
Global Const age1960_OPER_CALL_CONNECTED = 4
Global Const age1960_OPER_CALL_ALERTING = 8
Global Const age1960_OPER_CALL_BCH_CHANGING = 16
Global Const age1960_OPER_CALL_TCH_CHANGING = 32
Global Const age1960_OPER_CALL_CNT_CHANGING = 64
Global Const age1960_OPER_CALL_BS_ORIGINATING = 128
Global Const age1960_OPER_CALL_BS_DISCONNECTING = 256
Global Const age1960_OPER_CALL_AVAIL9 = 512
Global Const age1960_OPER_CALL_AVAIL10 = 1024
Global Const age1960_OPER_CALL_AVAIL11 = 2048
Global Const age1960_OPER_CALL_AVAIL12 = 4096
Global Const age1960_OPER_CALL_AVAIL13 = 8192
Global Const age1960_OPER_CALL_AVAIL14 = 16384
Global Const age1960_OPER_CALL_AVAIL15 = 32768

Global Const age1960_QUES_HW_AVAIL0 = 1
Global Const age1960_QUES_HW_AVAIL1 = 2
Global Const age1960_QUES_HW_AVAIL2 = 4
Global Const age1960_QUES_HW_AVAIL3 = 8
Global Const age1960_QUES_HW_PWRUP_ST_FAILED = 16
Global Const age1960_QUES_HW_AVAIL5 = 32
Global Const age1960_QUES_HW_AVAIL6 = 64
Global Const age1960_QUES_HW_AVAIL7 = 128
Global Const age1960_QUES_HW_AVAIL8 = 256
Global Const age1960_QUES_HW_AVAIL9 = 512
Global Const age1960_QUES_HW_AVAIL10 = 1024
Global Const age1960_QUES_HW_AVAIL11 = 2048
Global Const age1960_QUES_HW_AVAIL12 = 4096
Global Const age1960_QUES_HW_AVAIL13 = 8192
Global Const age1960_QUES_HW_AVAIL14 = 16384
Global Const age1960_QUES_HW_AVAIL15 = 32768

'        ***************************************************
'        *  Constants used by function age1960_timeOut     *
'        ***************************************************

Global Const age1960_TIMEOUT_MAX = 120000
Global Const age1960_TIMEOUT_MIN = 0&

'        ***************************************************
'        *  Required plug and play functions from VPP-3.1  *
'        ***************************************************

Declare Function age1960_init Lib "age1960_32.dll" _
   (ByVal resourceName As String, _
    ByVal IDQuery As Integer, _
    ByVal resetDevice As Integer, _
    ByRef instrumentHandle As Long) As Long

Declare Function age1960_close Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long) As Long

Declare Function age1960_reset Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long) As Long

Declare Function age1960_self_test Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef selfTestResult As Integer, _
    ByVal selfTestMessage As String) As Long

Declare Function age1960_error_query Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef errorCode As Long, _
    ByVal errorMessage As String) As Long

Declare Function age1960_error_message Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal statusCode As Long, _
    ByVal message As String) As Long

Declare Function age1960_revision_query Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal instrumentDriverRevision As String, _
    ByVal firmwareRevision As String) As Long

'        ***************************************************
'        *  Agilent standard utility functions             *
'        ***************************************************

Declare Function age1960_timeOut Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal setTimeOut As Long) As Long

Declare Function age1960_timeOut_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef timeout As Long) As Long

Declare Function age1960_errorQueryDetect Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal setErrorQueryDetect As Integer) As Long

Declare Function age1960_errorQueryDetect_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef errorQueryDetect As Integer) As Long

Declare Function age1960_dcl Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long) As Long

Declare Function age1960_opc_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef instrumentReady As Integer) As Long


'        ********************************************************************
'        *  Agilent standard status functions (modified for multi-registers)*
'        ********************************************************************

Declare Function age1960_readStatusByte_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef statusByte As Integer) As Long

Declare Function age1960_stdEvent_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByRef stdEventRegister As Long) As Long

Declare Function age1960_operEvent_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal operationName As Integer, _
    ByRef operationEventRegister As Long) As Long

Declare Function age1960_operCond_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal operationName As Integer, _
    ByRef operationConditionRegister As Long) As Long

Declare Function age1960_quesEvent_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal questionableName As Integer, _
    ByRef questionableEventRegister As Long) As Long

Declare Function age1960_quesCond_Q Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal questionableName As Integer, _
    ByRef questionableConditionRegister As Long) As Long

Declare Function age1960_quesRegisterEnable Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal questionableName As Integer, _
    ByVal quesPosTransition As Long, _
    ByVal quesNegTransition As Long, _
    ByVal quesEnableBits As Long) As Long

Declare Function age1960_operRegisterEnable Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal operationName As Integer, _
    ByVal operPosTransition As Long, _
    ByVal operNegTransition As Long, _
    ByVal operEnableBits As Long) As Long

Declare Function age1960_stdEventEnable Lib "age1960_32.dll" _
   (ByVal instrumentHandle As Long, _
    ByVal stdEnableBits As Long) As Long

'        ***************************************************
'        *  Agilent standard command passthrough functions *
'        ***************************************************

Declare Function age1960_cmd Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sendStringCommand As String) As Long

Declare Function age1960_cmdString_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryStringCommand As String, _
     ByVal stringSize As Long, _
     ByVal stringResult As String) As Long

Declare Function age1960_cmdInt Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sendIntegerCommand As String, _
     ByVal sendInteger As Long) As Long

Declare Function age1960_cmdInt16_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryI16Command As String, _
     ByRef i16Result As Integer) As Long

Declare Function age1960_cmdInt32_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryI32Command As String, _
     ByRef i32Result As Long) As Long

Declare Function age1960_cmdInt16Arr_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryI16ArrayCommand As String, _
     ByVal i16ArraySize As Long, _
     ByRef i16ArrayResult As Integer, _
     ByRef i16ArrayCount As Long) As Long

Declare Function age1960_cmdInt32Arr_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryI32ArrayCommand As String, _
     ByVal i32ArraySize As Long, _
     ByRef i32ArrayResult As Long, _
     ByRef i32ArrayCount As Long) As Long

Declare Function age1960_cmdReal Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal sendRealCommand As String, _
     ByVal sendReal As Double) As Long

Declare Function age1960_cmdReal64_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal queryRealCommand As String, _
     ByRef realResult As Double) As Long

Declare Function age1960_cmdReal64Arr_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
     ByVal realArrayCommand As String, _
     ByVal realArraySize As Long, _
     ByRef realArrayResult As Double, _
     ByRef realArrayCount As Long) As Long


'* End of Agilent standard declarations
'*---------------------------------------------------------------------------*


'*****************************************************************************
'*  INSTRUMENT SPECIFIC SECTION                                              *
'*  Constants and function prototypes for instrument specific functions.     *
'*****************************************************************************

'        ***************************************************
'        *  Instrument specific constants                  *
'        ***************************************************

Global Const age1960_INSTR_ERROR_ARRAY_TOO_SMALL = &HBFFC0D2A
Global Const age1960_INSTR_CANNOT_ORIG_NOW = &HBFFC0D2B
Global Const age1960_INSTR_ERROR_CALL_FAILED = &HBFFC0D2C
Global Const age1960_INSTR_ERROR_CPTO = &HBFFC0D2D
Global Const age1960_INSTR_ERROR_INITTO = &HBFFC0D2E
Global Const age1960_INSTR_ERROR_ORFS_MEAS_COUNT = &HBFFC0D2F
Global Const age1960_INSTR_ERROR_ARRAY_TOO_LARGE = &HBFFC0D30
Global Const age1960_INSTR_ERROR_AUTORANGE_FAILED = &HBFFC0D31
Global Const age1960_INSTR_CANNOT_CHNGBAND_NOW = &HBFFC0D32
Global Const age1960_INSTR_TOO_MANY_NONCONCURRENT = &HBFFC0D33
Global Const age1960_INSTR_UNDEFINED_DATA = &HBFFC0D34
Global Const age1960_INSTR_TA_NOT_AVAILABLE = &HBFFC0D35
Global Const age1960_INSTR_TA_NOT_SWITCHABLE = &HBFFC0D36
Global Const age1960_INSTR_TA_NOT_LICENSE = &HBFFC0D37
Global Const age1960_INSTR_ERROR_NSUP_FUNC = &HBFFC0D38
Global Const age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED = &HBFFC0D39

Global Const age1960_NOT_A_NUMBER = 9.91E+37            ' IEEE not a number

Global Const age1960_NO_RESULT_AVAIL = 1                ' No result available

Global Const age1960_INTEGRITY_MIN = 0                  ' the min integrity value that can be returned
Global Const age1960_INTEGRITY_MAX = 23                 ' the max integrity value that can be returned

Global Const age1960_ARRAY_MIN = 1                      ' min array size is always 1
Global Const age1960_ARRAY_MAX = 500                    ' max array size for instrument specific functions is 500
Global Const age1960_CP_TIMEOUT_MIN = 0#                ' min seconds for a call processing fcn to complete
Global Const age1960_CP_TIMEOUT_MAX = 120#              ' max seconds for call processing function to complete

Global Const age1960_INIT_TIMEOUT_MIN = 0.1             ' min seconds for a meas fcn to complete
Global Const age1960_INIT_TIMEOUT_MAX = 120#            ' max seconds for a meas fcn to complete

Global Const age1960_MEAS_TIMEOUT_MIN = 0.1             '  min seconds for a measurement timeout
Global Const age1960_MEAS_TIMEOUT_MAX = 999.9            ' max seconds for a measurement timeout

Global Const age1960_MAV_TIMEOUT_MIN = 1                ' min seconds for HP-IB buffer to clear
Global Const age1960_MAV_TIMEOUT_MAX = 30               ' max seconds for HP-IB buffer to clear

Global Const age1960_NUMBER_OF_MEAS_FCNS = 10           ' 10 measurement fcns

Global Const age1960_WAIT = -2                          ' Wait for measurement function to complete
Global Const age1960_NONEINIT = -1                      ' No measurement function initiated

Global Const age1960_AAUDIO = 0                         ' Analog Audio Meas Fcn
Global Const age1960_BER = 1                            ' Bit Error Meas Fcn
Global Const age1960_DAUDIO = 2                         ' Decoded Audio Meas Fcn
Global Const age1960_FBER = 3                           ' Fast Bit Error Meas Fcn
Global Const age1960_ORFS = 4                           ' ORFSpectrum Meas Fcn
Global Const age1960_PFER = 5                           ' Phase Freq Error Meas Fcn
Global Const age1960_PVTIME = 6                         ' Power vs Time Meas Fcn
Global Const age1960_TXPOWER = 7                        ' TX Power Meas Fcn
Global Const age1960_DPOWER = 8                         ' Dunamic Power Meas Fcn
Global Const age1960_IQTUNING = 9                       ' IQ Tuning Meas Fcn

Global Const age1960_TEST_MODE = 0                      ' Test operating mode
Global Const age1960_ACELL_MODE = 1                     ' Active Cell operating mode

Global Const age1960_UNKNOWN_BAND = -1                  ' No band set
Global Const age1960_CURRENT_BAND = 0                   ' Current band
Global Const age1960_PGSM_BAND = 1                      ' PGSM band
Global Const age1960_EGSM_BAND = 2                      ' EGSM band
Global Const age1960_DCS1800_BAND = 3                   ' DCS 1800 band
Global Const age1960_PCS1900_BAND = 4                   ' PCS 1900 band
Global Const age1960_GSM850_BAND = 5                    ' GSM 850 band

Global Const age1960_PGSM_MIN_CH = 1                    ' PGSM min channel No.
Global Const age1960_PGSM_MAX_CH = 124                  ' PGSM max channel No.
Global Const age1960_EGSM_MIN_CH_1 = 0                  ' EGSM min 1 channel No.
Global Const age1960_EGSM_MAX_CH_1 = 124                ' EGSM max 1 channel No.
Global Const age1960_EGSM_MIN_CH_2 = 975                ' EGSM min 2 channel No.
Global Const age1960_EGSM_MAX_CH_2 = 1023               ' EGSM max 2 channel No.
Global Const age1960_DCS1800_MIN_CH = 512               ' DCS 1800 min channel No.
Global Const age1960_DCS1800_MAX_CH = 885               ' DCS 1800 max channel No.
Global Const age1960_PCS1900_MIN_CH = 512               ' PCS 1900 min channel No.
Global Const age1960_PCS1900_MAX_CH = 810               ' PCS 1900 max channel No.
Global Const age1960_GSM850_MIN_CH = 128                ' GSM 850 min channel No.
Global Const age1960_GSM850_MAX_CH = 251                ' GSM 850 max channel No.

Global Const age1960_PGSM_MIN_TXL = 0                   ' PGSM min TX Level
Global Const age1960_PGSM_MAX_TXL = 31                  ' PGSM max TX Level
Global Const age1960_EGSM_MIN_TXL = 0                   ' EGSM min TX Level
Global Const age1960_EGSM_MAX_TXL = 31                  ' EGSM max TX Level
Global Const age1960_DCS1800_MIN_TXL = 0                ' DCS 1800 min TX Level
Global Const age1960_DCS1800_MAX_TXL = 31               ' DCS 1800 max TX Level

Global Const age1960_PCS1900_MIN_TXL_1 = 0              ' PCS 1900 Rng 1 min TX Level
Global Const age1960_PCS1900_MAX_TXL_1 = 15             ' PCS 1900 Rng 1 max TX Level
Global Const age1960_PCS1900_MIN_TXL_2 = 30             ' PCS 1900 Rng 2 min TX Level
Global Const age1960_PCS1900_MAX_TXL_2 = 31             ' PCS 1900 Rng 2 max TX Level

Global Const age1960_GSM850_MIN_TXL = 0                 ' GSM 850 min TX Level
Global Const age1960_GSM850_MAX_TXL = 31                ' GSM 850 max TX Level

Global Const age1960_TADVANCE_MIN = 0                   ' Timing Advance minimum
Global Const age1960_TADVANCE_MAX = 63                  ' Timing Advance maxinum

Global Const age1960_SACCHREADINGS_MIN = 1              ' Min number of SACCH readings to be made
Global Const age1960_SACCHREADINGS_MAX = 1000           ' Max number of SACCH readings to be made

Global Const age1960_TRIG_DELAY_MIN = -0.00231          ' min trigger delay in seconds
Global Const age1960_TRIG_DELAY_MAX = 0.00231           ' max trigger delay in seconds

Global Const age1960_TIMESLOT_MIN = 3                   ' Timeslot minimum
Global Const age1960_TIMESLOT_MAX = 5                   ' Timeslot maximum

Global Const age1960_EXPECTED_POWER_MIN = -60#          ' Expected Power minimum
Global Const age1960_EXPECTED_POWER_MAX = 53#           ' Expected Power maximum

Global Const age1960_BA_TABLE_PTS_MIN = 0               ' BA Table minimum pts
Global Const age1960_BA_TABLE_PTS_MAX = 16              ' BA Table maxinum pts

Global Const age1960_MEAS_COUNT_MIN = 0                 ' minimum number of measurements that can be made
Global Const age1960_MEAS_COUNT_MAX = 999               ' maximum number of measurements that can be made

Global Const age1960_FBER_COUNT_MIN = 1                 ' minimum number of bits that can be tested for FBER
Global Const age1960_FBER_COUNT_MAX = 999000            ' maximum number of bits that can be tested for FBER

Global Const age1960_BER_COUNT_MIN = 1                  ' minimum number of bits that can be tested for BER
Global Const age1960_BER_COUNT_MAX = 999000             ' maximum number of bits that can be tested for BER

Global Const age1960_FBER_DELAY_MIN = 0                 ' minimum FBER frame delay
Global Const age1960_FBER_DELAY_MAX = 26                ' maximum FBER frame delay

Global Const age1960_BER_DELAY_MIN = 1                  ' minimum BER frame delay
Global Const age1960_BER_DELAY_MAX = 15                 ' maximum BER frame delay

Global Const age1960_TSC0_BURST = 0                     ' TSC0 expected burst
Global Const age1960_TSC1_BURST = 1                     ' TSC1 expected burst
Global Const age1960_TSC2_BURST = 2                     ' TSC2 expected burst
Global Const age1960_TSC3_BURST = 3                     ' TSC3 expected burst
Global Const age1960_TSC4_BURST = 4                     ' TSC4 expected burst
Global Const age1960_TSC5_BURST = 5                     ' TSC5 expected burst
Global Const age1960_TSC6_BURST = 6                     ' TSC6 expected burst
Global Const age1960_TSC7_BURST = 7                     ' TSC7 expected burst
Global Const age1960_RACH_BURST = 8                     ' RACH expected burst

Global Const age1960_CELL_POWER_MIN = -127#             ' minimum cell power
Global Const age1960_CELL_POWER_MAX = -10#              ' maximum cell power

Global Const age1960_CELL_1 = 1                         ' serving cell 1
Global Const age1960_CELL_2 = 2                         ' serving cell 2

Global Const age1960_MAN_RCV_CONTROL = 0                ' Manual receiver control
Global Const age1960_AUTO_RCV_CONTROL = 1               ' Auto receiver control

Global Const age1960_TRIG_AUTO = 0                      ' Auto trigger source
Global Const age1960_TRIG_PROT = 1                      ' Protocol trigger source
Global Const age1960_TRIG_RF_RISE = 2                   ' RF Rise trigger source
Global Const age1960_TRIG_IMMEDIATE = 3                 ' Immediate trigger source

Global Const age1960_BSYNC_MID = 0                      ' Midample burst sync
Global Const age1960_BSYNC_AMPL = 1                     ' Amplitude burst sync
Global Const age1960_BSYNC_NONE = 2                     ' No burst sync

Global Const age1960_PVT_POINTS_MIN = 0                 ' minimum number of pvt offsets
Global Const age1960_PVT_POINTS_MAX = 12                ' maximum number of pvt offsets

Global Const age1960_PVT_OFFSET_MIN = -0.00005          ' minimum pvt offset time in seconds
Global Const age1960_PVT_OFFSET_MAX = 0.00059           ' maximum pvt offset time in seconds

Global Const age1960_ORFSSW_POINTS_MIN = 0              ' minimum number of ORFS switching frequencies
Global Const age1960_ORFSSW_POINTS_MAX = 8              ' maximum number of ORFS switching frequencies

Global Const age1960_ORFSMOD_POINTS_MIN = 0             ' minimum number of ORFS modulation frequencies
Global Const age1960_ORFSMOD_POINTS_MAX = 22            ' maximum number of ORFS modulation frequencies

Global Const age1960_ORFSFREQ_MIN_1 = -1800000#         ' minimum ORFS Frequency in Hz, range 1
Global Const age1960_ORFSFREQ_MAX_1 = -10#              ' maximum ORFS Frequency in Hz, range 1
Global Const age1960_ORFSFREQ_MIN_2 = 10#               ' minimum ORFS Frequency in Hz, range 2
Global Const age1960_ORFSFREQ_MAX_2 = 1800000#          ' maximum ORFS Frequency in Hz, range 2

Global Const age1960_EXP_VOLT_MIN = 0.01                ' minimum expected audio measurement range in volts peak
Global Const age1960_EXP_VOLT_MAX = 20#                 ' maximum expected audio measurement range in volts peak

Global Const age1960_AAUDFLT_FREQ_MIN = 200#            ' minimum bandpass analog audio frequency in Hz
Global Const age1960_AAUDFLT_FREQ_MAX = 8000#           ' maximum bandpass analog audio frequency in Hz

Global Const age1960_DAUDFLT_FREQ_MIN = 200#            ' minimum bandpass decoded audio frequency in Hz
Global Const age1960_DAUDFLT_FREQ_MAX = 3600#           ' maximum bandpass decoded audio frequency in Hz

Global Const age1960_AMPOFFSET_ITEMS_MIN = 0            ' min items sent of zero turns off amplitude offset
Global Const age1960_AMPOFFSET_ITEMS_MAX = 20           ' max items sent '

Global Const age1960_OFFSET_FREQ_MIN = 292500000#       ' Offset Frequency minimum '
Global Const age1960_OFFSET_FREQ_MAX = 2700000000#      ' Offset Frequency maximum '

Global Const age1960_PATH_GAIN_MIN = -100#              ' minimum path offset (gain) in dB '
Global Const age1960_PATH_GAIN_MAX = 100#               ' maximum path offset (gain) in dB '

Global Const age1960_AUTORANGE_TRIES_MIN = 1            ' minimum autorange tries
Global Const age1960_AUTORANGE_TRIES_MAX = 100          ' maximum autorange tries

Global Const age1960_EXPECTED_PWR_MIN = -60#            ' minimum expected power setting
Global Const age1960_EXPECTED_PWR_MAX = 53#             ' maximum expected power setting

Global Const age1960_BER_TYPE_IA = 0                    ' Type IA BER Measurement
Global Const age1960_BER_TYPE_II = 1                    ' Type II BER Measurement
Global Const age1960_BER_TYPE_IB = 2                    ' Type IB BER Measurement
Global Const age1960_BER_RES_TYPE_IA = 3                ' Residual Type IA BER Measurement
Global Const age1960_BER_RES_TYPE_II = 4                ' Residual Type II BER Measurement
Global Const age1960_BER_RES_TYPE_IB = 5                ' Residual Type IB BER Measurement

Global Const age1960_CONN_STATE_UNKNOWN = -1            ' Connection status UNKNOWN
Global Const age1960_CALL_CONNECTED = 0                 ' Call at the the connected state
Global Const age1960_CALL_ALERTING = 1                  ' Call at the alerting state
Global Const age1960_CALL_IDLE = 2                      ' Call at the idle state

Global Const age1960_WAITFOR_NMRR = 0                   ' Wait for a specific NMRR state
Global Const age1960_WAITFOR_CALL = 1                   ' Wait for call connected state
Global Const age1960_WAITFOR_MEAS = 2                   ' Wait for a any NMRR state

Global Const age1960_HOLDOFF_TIME_MIN = 0#              ' min seconds for BER and FBER to holdoff the measurement
Global Const age1960_HOLDOFF_TIME_MAX = 5#              ' max seconds for BER and FBER to holdoff the measurement

Global Const age1960_PVTPLOT_ARRAY_MIN = 2220           ' min number of points from PVT plot, for now fixed
Global Const age1960_PVTPLOT_ARRAY_MAX = 5000           ' Max number of points from PVT plot, for now fixed

Global Const age1960_IQ_REF_NEG67K = 0                  ' All zeros (-67.7)
Global Const age1960_IQ_REF_ZEROK = 1                   ' Carrier  (0kHz)
Global Const age1960_IQ_REF_PLUS67K = 2                 ' All ones (+67.7)
Global Const age1960_IQ_REF_AUTO = 3                    ' Function will reference to the highest level

Global Const age1960_IQ_ARRAY_MIN = 9                   ' The minimum array size for the IQ tuning results
Global Const age1960_IQ_ARRAY_MAX = 500                 ' The maximum array size for the IQ tuning results

Global Const age1960_LOOPBACK_OFF = 0                   ' Turn Loopback off
Global Const age1960_LOOPBACK_A = 1                     ' Loopback to type A
Global Const age1960_LOOPBACK_B = 2                     ' Loopback to type B
Global Const age1960_LOOPBACK_C = 3                     ' Loopback to type C

Global Const age1960_FR_SPEECH = 0                      ' Channel mode setting - Full rate Speech
Global Const age1960_EFR_SPEECH = 1                     ' Channel mode setting -  Enhanced Full rate Speech

Global Const age1960_EXP_DIFF_MIN = -30#                ' minimum Expected difference for dynamic power
Global Const age1960_EXP_DIFF_MAX = 30#                 ' maximum Expected difference for dynamic power

Global Const age1960_DPOW_COUNT_MIN = 1                 ' minimum count for dynamic power
Global Const age1960_DPOW_COUNT_MAX = 999               ' maximum count for dynamic power

Global Const age1960_DPOW_ARRAY_MIN = 1                 ' minimum number of points allowed for Dynamic power measurement
Global Const age1960_DPOW_ARRAY_MAX = 2000              ' maximum number of points allowed for Dynmaic power measurement

Global Const age1960_IQT_SPURFREQ_MIN_1 = -13000000#
Global Const age1960_IQT_SPURFREQ_MAX_1 = -1000000#
Global Const age1960_IQT_SPURFREQ_MIN_2 = 1000000#
Global Const age1960_IQT_SPURFREQ_MAX_2 = 13000000#

Global Const age1960_TA_SWITCH_MAX_TIME = 240           ' TA switching timeout value

Global Const age1960_FUND_FREQ_MIN = 100#               ' Audio analyzer minimum fundamental frequency */
Global Const age1960_FUND_FREQ_MAX = 10000#             ' Audio analyzer maximum fundamental frequency */

Global Const age1960_FILTER_NONE = 0                    ' Audio analyzer filter type NONE */
Global Const age1960_FILTER_BANDPASS = 1                ' Audio analyzer Tunable BandPass filter */
Global Const age1960_FILTER_CMESSAGE = 2                ' Audio analyzer C_Message filter type */
Global Const age1960_FILTER_50_15KHZ = 3                ' Audio analyzer BandPass filter 50 to 15khz */
Global Const age1960_FILTER_300_15KHZ = 4               ' Audio analyzer BandPass filter 300 to 15KHZ */

Global Const age1960_BPF_FREQ_MIN = 200#                ' Audio analyzer BandPass filter minimum center frequency */
Global Const age1960_BPF_AFAN_FREQ_MAX = 20000#         ' Audio analyzer BandPass filter maximum center frequency */

Global Const age1960_DET_RMS = 0                        ' Audio analyzer RMS detector
Global Const age1960_DET_PPEAK = 1                      ' Audio analyzer Positive Peak detector

Global Const age1960_EXPANDOR_LVL_MIN = 0#              ' Audio analyzer RMS detector
Global Const age1960_EXPANDOR_LVL_MAX = 10#             ' Audio analyzer Positive Peak detector

Global Const age1960_AUTORNG_AFANALYZER = 0             ' Audio Frequency Analyzer Meas Fcn
Global Const age1960_AUTORNG_TXPOWER = 1                ' TX Power Meas Fcn

Global Const age1960_CIFT_CONF_LEVEL_MIN = 80#          ' PFER CIFT confidence level minimum
Global Const age1960_CIFT_CONF_LEVEL_MAX = 99.9         ' PFER CIFT confidence level maximum

Global Const age1960_PFER_PEAK_PHASE_LIMIT_MIN = 0#     ' PFER CIFT peak phase error limit minimum
Global Const age1960_PFER_PEAK_PHASE_LIMIT_MAX = 20#    ' PFER CIFT peak phase error limit maximum

Global Const age1960_PFER_RMS_PHASE_LIMIT_MIN = 0#      ' PFER CIFT rms phase error limit minimum
Global Const age1960_PFER_RMS_PHASE_LIMIT_MAX = 5#      ' PFER CIFT rms phase error limit maximum

Global Const age1960_PFER_FREQ_ERROR_LIMIT_MIN = 0.01   ' PFER CIFT frequency error limit minimum
Global Const age1960_PFER_FREQ_ERROR_LIMIT_MAX = 0.1    ' PFER CIFT frequency error limit minimum

Global Const age1960_GPRS_FORMAT = "GPRS"               ' GPRS TA format name
Global Const age1960_GSM_FORMAT = "GSM"                 ' GSM TA format name
Global Const age1960_AMPS_IS136_FORMAT = "AMPS\IS136"   ' AMPS/IS136 TA format name
Global Const age1960_WCDMA_FORMAT = "WCDMA"             ' WCDMA TA format name

Global Const age1960_GPRS_TA_SUFFIX = ":GPRS"           ' SCPI suffix for GPRS TAP SCPI
Global Const age1960_GSM_TA_SUFFIX = ":GSM"             ' SCPI suffix for GSM TAP SCPI
Global Const age1960_IS136_TA_SUFFIX = ":DIG136"        ' SCPI suffix for AMPS/IS136 TAP (Digital Control Channel Only)
Global Const age1960_AMPS_IS136_TA_SUFFIX = ":TA136"    ' SCPI suffix for AMPS/IS136 TAP SCPI
Global Const age1960_WCDMA_TA_SUFFIX = ":WCDMA"         ' SCPI suffix for WCDMA TAP SCPI
Global Const age1960_C2K_TA_SUFFIX = ":C2K"             ' SCPI suffix for C2K SCPI

Global Const age1960_PVT_MASK_NARROW = 0                ' Power Vs Time PCS ETSI Limit Mask - Narrow
Global Const age1960_PVT_MASK_RELAXED = 1               ' Power Vs Time PCS ETSI Limit Mask - Relaxed

Global Const age1960_ORFS_LIMIT_ETSI = 0                ' ORFS measurement Limit Source ETSI setting
Global Const age1960_ORFS_LIMIT_MAN = 1                 ' ORFS measurement Limit Source Manual setting
 
Global Const age1960_ORFSSW_LIM_POINTS_MIN = 1          ' ORFS Switching Limits minimum number of points
Global Const age1960_ORFSSW_LIM_POINTS_MAX = 8          ' ORFS Switching Limits maximum number of points

Global Const age1960_ORFSMOD_LIM_POINTS_MIN = 1         ' ORFS Modulation Limits minimum number of points
Global Const age1960_ORFSMOD_LIM_POINTS_MAX = 22        ' ORFS Modulation Limits maximum number of points

Global Const age1960_ORFSSW_LIMIT_MIN = -200#           ' ORFS Switching Limits minimum acceptable setting
Global Const age1960_ORFSSW_LIMIT_MAX = 100#            ' ORFS Switching Limits maximum acceptable setting

Global Const age1960_ORFSMOD_LIMIT_MIN = -200#          ' ORFS Modulation Limits minimum acceptable setting
Global Const age1960_ORFSMOD_LIMIT_MAX = 100#           ' ORFS Modulation Limits maximum acceptable setting

'        ***************************************************
'        *  Instrument specific functions                  *
'        ***************************************************

'* End of instrument specific declarations *
'*---------------------------------------------------------------------------*

Declare Function age1960_cmdIntArr Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal sendIntegerArrayCommand As String, _
    ByVal sendIntegerArraySize As Long, _
    ByRef sendIntegerArray As Long) As Long

Declare Function age1960_cmdRealArr Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal sendRealArrayCommand As String, _
    ByVal sendRealArraySize As Long, _
    ByRef sendRealArray As Double) As Long

Declare Function age1960_partialPreset Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long) As Long

Declare Function age1960_readTXPower_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal autoRange As Integer, _
    ByRef TXPowerIntegrity As Long, _
    ByRef TXPower As Double) As Long

Declare Function age1960_readPowerVsTime_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal pvsTimeArraySize As Long, _
    ByRef pvsTimeIntegrity As Long, _
    ByRef pvsTimeMaskFail As Double, _
    ByRef pvsTimeTXPower As Double, _
    ByRef pvsTimeArray As Double, _
    ByRef pvsTimeCount As Long) As Long

Declare Function age1960_readIQTuning_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal IQArraySize As Long, _
    ByRef IQIntegrity As Long, _
    ByRef IQSpurFreq As Double, _
    ByRef IQSpurLevel As Double, _
    ByRef IQResultsArray As Double, _
    ByRef IQFreqsArray As Double, _
    ByRef IQTuningCount As Long) As Long

Declare Function age1960_readPFER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef PFERIntegrity As Long, _
    ByRef PFERRMSError As Double, _
    ByRef PFERPeakError As Double, _
    ByRef PFERFreqError As Double) As Long

Declare Function age1960_readORFS_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal ORFSSwArraySize As Long, _
    ByVal ORFSModArraySize As Long, _
    ByRef ORFSIntegrity As Long, _
    ByRef ORFSTXPower As Double, _
    ByRef ORFSBWPower As Double, _
    ByRef ORFSSwArray As Double, _
    ByRef ORFSModArray As Double, _
    ByRef ORFSSwCount As Long, _
    ByRef ORFSModCount As Long) As Long
           
Declare Function age1960_readFBER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef FBERIntegrity As Long, _
    ByRef FBERBitsTested As Double, _
    ByRef FBERRatio As Double, _
    ByRef FBERCount As Double) As Long

Declare Function age1960_readBER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef BERIntegrity As Long, _
    ByRef TypeIaBERBitsTested As Double, _
    ByRef TypeIaBERRatio As Double, _
    ByRef TypeIaBERCount As Double, _
    ByRef TypeIbBERBitsTested As Double, _
    ByRef TypeIbBERRatio As Double, _
    ByRef TypeIbBERCount As Double, _
    ByRef TypeIIBERBitsTested As Double, _
    ByRef TypeIIBERRatio As Double, _
    ByRef TypeIIBERCount As Double) As Long

Declare Function age1960_readAAudio_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef AAudioIntegrity As Long, _
    ByRef AAudioResult As Double) As Long

Declare Function age1960_readDAudio_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef DAudioIntegrity As Long, _
    ByRef DAudioResult As Double) As Long

Declare Function age1960_readDynPower_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal dynPowerArraySize As Long, _
    ByRef dynPowerIntegrity As Long, _
    ByRef dynPowerArray As Double, _
    ByRef dynPowerCount As Long) As Long

Declare Function age1960_initMeasFcn Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal measFcn As Integer) As Long

Declare Function age1960_initMeasFcns Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal measFcnsToInit As Long, _
    ByRef measFcnsArray As Integer) As Long

Declare Function age1960_fetchTXPower_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef TXPowerIntegrity As Long, _
    ByRef TXPower As Double) As Long

Declare Function age1960_fetchPowerVsTime_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal pvsTimeArraySize As Long, _
    ByRef pvsTimeIntegrity As Long, _
    ByRef pvsTimeMaskFail As Double, _
    ByRef pvsTimeTXPower As Double, _
    ByRef pvsTimeArray As Double, _
    ByRef pvsTimeCount As Long) As Long

Declare Function age1960_fetchDynPower_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal dynPowerArraySize As Long, _
    ByRef dynPowerIntegrity As Long, _
    ByRef dynPowerArray As Double, _
    ByRef dynPowerCount As Long) As Long

Declare Function age1960_fetchIQTuning_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal IQArraySize As Long, _
    ByRef IQIntegrity As Long, _
    ByRef IQSpurFreq As Double, _
    ByRef IQSpurLevel As Double, _
    ByRef IQResultsArray As Double, _
    ByRef IQFreqsArray As Double, _
    ByRef IQTuningCount As Long) As Long

Declare Function age1960_fetchPFER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef PFERIntegrity As Long, _
    ByRef PFERRMSError As Double, _
    ByRef PFERPeakError As Double, _
    ByRef PFERFreqError As Double) As Long

Declare Function age1960_fetchORFS_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal ORFSSwArraySize As Long, _
    ByVal ORFSModArraySize As Long, _
    ByRef ORFSIntegrity As Long, _
    ByRef ORFSTXPower As Double, _
    ByRef ORFSBWPower As Double, _
    ByRef ORFSSwArray As Double, _
    ByRef ORFSModArray As Double, _
    ByRef ORFSSwCount As Long, _
    ByRef ORFSModCount As Long) As Long
           
Declare Function age1960_fetchFBER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef FBERIntegrity As Long, _
    ByRef FBERBitsTested As Double, _
    ByRef FBERRatio As Double, _
    ByRef FBERCount As Double) As Long

Declare Function age1960_fetchBER_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef BERIntegrity As Long, _
    ByRef TypeIaBERBitsTested As Double, _
    ByRef TypeIaBERRatio As Double, _
    ByRef TypeIaBERCount As Double, _
    ByRef TypeIbBERBitsTested As Double, _
    ByRef TypeIbBERRatio As Double, _
    ByRef TypeIbBERCount As Double, _
    ByRef TypeIIBERBitsTested As Double, _
    ByRef TypeIIBERRatio As Double, _
    ByRef TypeIIBERCount As Double) As Long
         
Declare Function age1960_fetchAAudio_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef AAudioIntegrity As Long, _
    ByRef AAudioResult As Double) As Long

Declare Function age1960_fetchDAudio_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef DAudioIntegrity As Long, _
    ByRef DAudioResult As Double) As Long

Declare Function age1960_waitForInitDone_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal InitDoneTimeOut As Double, _
    ByRef MeasFcnDone As Integer) As Long

Declare Function age1960_originateCall Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal completion As Integer, _
    ByVal originateTimeOut As Double) As Long

Declare Function age1960_endCall Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal endSeq As Integer) As Long

Declare Function age1960_TCHARFCN Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TCHARFCN As Long, _
    ByVal TCHARFCNBand As Integer, _
    ByVal TCHARFCNSeq As Integer) As Long

Declare Function age1960_manualChan Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal manualChan As Long, _
    ByVal manualChanBand As Integer) As Long

Declare Function age1960_expectedPower Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal expectedPower As Double, _
    ByVal expectedPowerBand As Integer) As Long

Declare Function age1960_confRcvControl Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal rcvControlMode As Integer) As Long

Declare Function age1960_MSTXLevel Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal MSTXLevel As Long, _
    ByVal MSTXLevelBand As Integer, _
    ByVal MSTXLevelSeq As Integer) As Long

Declare Function age1960_TCHTimeslot Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TCHTimeslot As Long, _
    ByVal TCHTimeslotSeq As Integer) As Long

Declare Function age1960_MSTimeAdvance Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal MSTimingAdvance As Long, _
    ByVal MSTimingAdvanceSeq As Integer) As Long

Declare Function age1960_BCHARFCN Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal BCHARFCNCell As Integer, _
    ByVal BCHARFCN As Long, _
    ByVal BCHARFCNBand As Integer, _
    ByVal BCHARFCNSeq As Integer) As Long

Declare Function age1960_cellBand Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal servingCellBandCell As Integer, _
    ByVal servingCellBand As Integer) As Long

Declare Function age1960_TCHBand Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TCHBand As Integer) As Long

Declare Function age1960_manualBand Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal manaulBand As Integer) As Long

Declare Function age1960_operatingMode Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal operatingMode As Integer) As Long

Declare Function age1960_BATable Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal BATableCell As Integer, _
    ByVal BATableArraySize As Long, _
    ByRef BATableArray As Long, _
    ByVal BATableBand As Integer) As Long

Declare Function age1960_burstType Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal burstType As Integer) As Long

Declare Function age1960_amplitudeOffset Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef frequencyArray As Double, _
    ByRef offsetArray As Double, _
    ByVal ampOffsetItems As Long) As Long

Declare Function age1960_cellPower Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal cellPowerCell As Integer, _
    ByVal cellPower As Double, _
    ByVal cellPowerDelta As Double) As Long

Declare Function age1960_channelMode Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal channelMode As Integer) As Long

Declare Function age1960_FACCHSignaling Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal facchState As Integer) As Long

Declare Function age1960_integrityMessage Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal integrityCode As Long, _
    ByVal integrityMessage As String) As Long

Declare Function age1960_measurementTimeout Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal timeoutMeasFcn As Integer, _
    ByVal timeoutSec As Double, _
    ByVal timeoutOnOff As Integer) As Long

Declare Function age1960_confTXPower Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TXPowerMeasCount As Long, _
    ByVal TXPowerTrigSource As Integer, _
    ByVal TXPowerTrigDelay As Double, _
    ByVal TXPowerTrigQual As Integer, _
    ByVal TXPowerMaxTime As Double) As Long

Declare Function age1960_confDynPower Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal DynPowerMeasCount As Long, _
    ByVal DynPowerExpMaxDiff As Double, _
    ByVal DynPowerMaxTime As Double) As Long

Declare Function age1960_confPowerVsTime Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal pvsTimeMeasCount As Long, _
    ByVal pvsTimeTrigSource As Integer, _
    ByVal pvsTimeTrigDelay As Double, _
    ByVal pvsTimeSync As Integer, _
    ByVal pvsTimePoints As Long, _
    ByRef pvsTimeOffsets As Double, _
    ByVal pvsTimeMaxTime As Double) As Long

Declare Function age1960_confPFER Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal PFERMeasCount As Long, _
    ByVal PFERTrigSource As Integer, _
    ByVal PFERTrigDelay As Double, _
    ByVal PFERTrigQual As Integer, _
    ByVal PFERSync As Integer, _
    ByVal PFERMaxTime As Double) As Long

Declare Function age1960_confORFS Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal ORFSSwMeasCount As Long, _
    ByVal ORFSModMeasCount As Long, _
    ByVal ORFSTrigSource As Integer, _
    ByVal ORFSTrigDelay As Double, _
    ByVal ORFSSwPoints As Long, _
    ByRef ORFSSwFreqs As Double, _
    ByVal ORFSModPoints As Long, _
    ByRef ORFSModFreqs As Double, _
    ByVal ORFSMaxTime As Double) As Long

Declare Function age1960_confFastBER Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal FBERMeasCount As Long, _
    ByVal FBERManualDelay As Long, _
    ByVal FBERAutoDelay As Integer, _
    ByVal FBERSignallingControl As Integer, _
    ByVal FBERHoldoffTime As Double, _
    ByVal FBERMaxTime As Double) As Long

Declare Function age1960_confBER Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal BERMeasCount As Long, _
    ByVal BERMeasType As Integer, _
    ByVal BERManualDelay As Long, _
    ByVal BERAutoDelay As Integer, _
    ByVal BERSignallingControl As Integer, _
    ByVal BERHoldoffTime As Double, _
    ByVal BERMaxTime As Double) As Long

Declare Function age1960_confAAudio Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal AAudioMeasCount As Long, _
    ByVal AAudioExpVolt As Double, _
    ByVal AAudioFiltFreq As Double, _
    ByVal AAudioFiltState As Integer, _
    ByVal AAudioMaxTime As Double) As Long

Declare Function age1960_confDAudio Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal DAudioMeasCount As Long, _
    ByVal DAudioFiltFreq As Double, _
    ByVal DAudioFiltState As Integer, _
    ByVal DAudioMaxTime As Double) As Long

Declare Function age1960_confIQTuning Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal IQTMeasCount As Long, _
    ByVal IQTTrigSource As Integer, _
    ByVal IQTTrigDelay As Double, _
    ByVal IQTRefOffsetFrequency As Integer, _
    ByVal IQTSpurFreqState As Integer, _
    ByVal IQTSpurFrequncy As Double, _
    ByVal IQTMaxTime As Double) As Long
    
Declare Function age1960_confAutoRange Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal measFcnToUse As Integer, _
    ByVal tries As Long) As Long

Declare Function age1960_confForMSOriginate Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long) As Long

Declare Function age1960_waitForConnect Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal connectTimeOut As Double) As Long

Declare Function age1960_getCorrectionFreq_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal cell As Integer, _
    ByRef downlinkFreq As Double, _
    ByRef uplinkFreq As Double) As Long

Declare Function age1960_getPresentFreq_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef downlinkFreq As Double, _
    ByRef uplinkFreq As Double, _
    ByRef measurementFreq As Double) As Long
    
Declare Function age1960_getApproxOffset_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal frequency As Double, _
    ByRef offset As Double) As Long

Declare Function age1960_UUTLevel_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal cell As Integer, _
    ByRef level As Double) As Long
    
Declare Function age1960_SACCHReport_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal waitForNewSACCH As Integer, _
    ByVal numberOfReadings As Long, _
    ByRef RXLevel As Double, _
    ByRef TXLevel As Double, _
    ByRef RXQuality As Double, _
    ByRef TimingAdvance As Double) As Long
    
Declare Function age1960_waitForClearHPIB Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal timeout As Long) As Long

Declare Function age1960_useSRQMAV Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal useSRQMAV As Integer) As Long
    
Declare Function age1960_useSRQMAV_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef useSRQMAV As Integer) As Long
    
Declare Function age1960_PowerVsTimePlot_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal pvsTimePlotArraySize As Long, _
    ByRef pvsTimePlotIntegrity As Long, _
    ByRef PvsTimePlotMaskFail As Double, _
    ByRef pvsTimePlotPower As Double, _
    ByRef pvsTimeStartTime As Double, _
    ByRef pvsTimeIncrement As Double, _
    ByRef pvsTimePlotArray As Double) As Long

Declare Function age1960_loopback Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal loopbackMode As Integer) As Long

Declare Function age1960_currentTA Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TAName As String, _
    ByVal TARevision As String) As Long
     
Declare Function age1960_currentTA_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TANameQuery As String, _
    ByVal TARevisionQuery As String) As Long
    
Declare Function age1960_currentTAModel_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TAModel As String) As Long

Declare Function age1960_currentActiveTAP Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TAName As String) As Long

Declare Function age1960_currentActiveTAP_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal TANameQuery As String) As Long

Declare Function age1960_confAFAN Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal AFANMeasCount As Long, _
    ByVal AFANSinadDist As Integer, _
    ByVal AFANFundFreq As Double, _
    ByVal AFANFilterType As Integer, _
    ByVal AFANBPFCntFreq As Double, _
    ByVal AFANDetType As Integer, _
    ByVal AFANPeakVolt As Double, _
    ByVal AFANDeemphasis As Integer, _
    ByVal AFANExpandorLevel As Double, _
    ByVal AFANMaxTime As Double) As Long

Declare Function age1960_fetchAFAN_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef AFANIntegrity As Long, _
    ByRef AFANAvgAudioLevel As Double, _
    ByRef AFANAvgSINAD As Double, _
    ByRef AFANAvgDist As Double) As Long

Declare Function age1960_readAFAN_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal AFANAutoRange As Integer, _
    ByRef AFANIntegrity As Long, _
    ByRef AFANAvgAudioLevel As Double, _
    ByRef AFANAvgSINAD As Double, _
    ByRef AFANAvgDist As Double) As Long

Declare Function age1960_confPFER_CIFT Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal PFERCIFTState As Integer, _
    ByVal PFERConfidenceLevel As Double, _
    ByVal PFERPeakPhaseErrorMaxLimit As Double, _
    ByVal PFERRmsPhaseErrorMaxLimit As Double, _
    ByVal PFERFrequencyErrorMaxLimit As Double) As Long
    
Declare Function age1960_confORFSLimitSource Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal limitSource As Integer) As Long

Declare Function age1960_confPVTMask Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal PVTMaskForPCS As Integer) As Long

Declare Function age1960_confORFSSwitchLimits Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal measurementBand As Integer, _
    ByVal switchingOffsetPoints As Long, _
    ByRef switchingLimitsArray As Double) As Long

Declare Function age1960_confORFSModLimits Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByVal measurementBand As Integer, _
    ByVal modulationOffsetPoints As Long, _
    ByRef modulationLimitsArray As Double) As Long

Declare Function age1960_fetchORFSSummaryPassFail_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef overallPFSummary As Double, _
    ByRef switchingPFSummary As Double, _
    ByRef modulationPFSummary As Double) As Long

Declare Function age1960_burstTimingError_Q Lib "age1960_32.dll" _
    (ByVal instrumentHandle As Long, _
    ByRef burstTimingError As Double) As Long


    
