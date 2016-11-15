/****************************************************************************
 *                       Keithley 230X Series DC Power Supplies
 *---------------------------------------------------------------------------
 *   Copyright (c) National Instruments 2000.  All Rights Reserved.
 *---------------------------------------------------------------------------
 *
 * Title:    ke230x.h
 * Purpose:  Keithley 2302 and 2306 Series DC Power Supplies Instrument Driver
 *           instrument driver declarations.
 *
 ****************************************************************************/

#ifndef __KE230X_HEADER
#define __KE230X_HEADER

#include <ivi.h>
#include <ividcpwr.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

    /****************************************************************************
     *----------------- Instrument Driver Revision Information -----------------*
     ****************************************************************************/
#define KE230X_MAJOR_VERSION                2     /* Instrument driver major version   */
#define KE230X_MINOR_VERSION                0     /* Instrument driver minor version   */

#define KE230X_CLASS_SPEC_MAJOR_VERSION     2     /* Class specification major version */
#define KE230X_CLASS_SPEC_MINOR_VERSION     0     /* Class specification minor version */

#define KE230X_SUPPORTED_INSTRUMENT_MODELS  "KE2302,KE2306"
#define KE230X_DRIVER_VENDOR                "Keithley Instruments"
#define KE230X_DRIVER_DESCRIPTION           "Keithley 2302 and 2306 DC Power Supplies"

    /****************************************************************************
     *------------------------------ Useful Macros -----------------------------*
     ****************************************************************************/

    /*- Defined values for rangeType parameter of function -----------------*/
    /*- ConfigureOutputRange -----------------------------------------------*/
#define KE230X_VAL_RANGE_CURRENT               IVIDCPWR_VAL_RANGE_CURRENT
#define KE230X_VAL_RANGE_VOLTAGE               IVIDCPWR_VAL_RANGE_VOLTAGE

    /*- Defined values for outputState parameter of function ---------------*/
    /*- QueryOutputState ---------------------------------------------------*/
#define KE230X_VAL_OUTPUT_CONSTANT_CURRENT     IVIDCPWR_VAL_OUTPUT_CONSTANT_CURRENT
#define KE230X_VAL_OUTPUT_OVER_VOLTAGE         IVIDCPWR_VAL_OUTPUT_OVER_VOLTAGE
#define KE230X_VAL_OUTPUT_OVER_CURRENT         IVIDCPWR_VAL_OUTPUT_OVER_CURRENT

    /*- Defined values for measurementType parameter of function -----------*/
    /*- Measure ------------------------------------------------------------*/
#define KE230X_VAL_MEASURE_CURRENT             IVIDCPWR_VAL_MEASURE_CURRENT
#define KE230X_VAL_MEASURE_VOLTAGE             IVIDCPWR_VAL_MEASURE_VOLTAGE

#define KE230X_VAL_MEASURE_DVM                (IVIDCPWR_VAL_OUTPUT_STATE_SPECIFIC_EXT_BASE + 0L)
#define KE230X_VAL_MEASURE_PCURR              (IVIDCPWR_VAL_OUTPUT_STATE_SPECIFIC_EXT_BASE + 1L)
#define KE230X_VAL_MEASURE_LINT               (IVIDCPWR_VAL_OUTPUT_STATE_SPECIFIC_EXT_BASE + 2L)

#define KE230X_VAL_MAX_TIME_INFINITE 		VI_TMO_INFINITE
#define KE230X_VAL_MAX_TIME_IMMEDIATE 		VI_TMO_IMMEDIATE
    /****************************************************************************
     *---------------------------- Attribute Defines ---------------------------*
     ****************************************************************************/

    /*- IVI Inherent Instrument Attributes ---------------------------------*/

    /* User Options */
#define KE230X_ATTR_RANGE_CHECK                   IVI_ATTR_RANGE_CHECK                    /* ViBoolean */
#define KE230X_ATTR_QUERY_INSTR_STATUS            IVI_ATTR_QUERY_INSTR_STATUS             /* ViBoolean */
#define KE230X_ATTR_CACHE                         IVI_ATTR_CACHE                          /* ViBoolean */
#define KE230X_ATTR_SIMULATE                      IVI_ATTR_SIMULATE                       /* ViBoolean */
#define KE230X_ATTR_RECORD_COERCIONS              IVI_ATTR_RECORD_COERCIONS               /* ViBoolean */

    /* Instrument Capabilities */
#define KE230X_ATTR_NUM_CHANNELS                  IVI_ATTR_NUM_CHANNELS                   /* ViInt32,  read-only  */

    /* Driver Information  */
#define KE230X_ATTR_SPECIFIC_DRIVER_PREFIX        IVI_ATTR_SPECIFIC_DRIVER_PREFIX         /* ViString, read-only  */
#define KE230X_ATTR_SUPPORTED_INSTRUMENT_MODELS   IVI_ATTR_SUPPORTED_INSTRUMENT_MODELS    /* ViString, read-only  */
#define KE230X_ATTR_GROUP_CAPABILITIES            IVI_ATTR_GROUP_CAPABILITIES             /* ViString, read-only  */
#define KE230X_ATTR_INSTRUMENT_MANUFACTURER       IVI_ATTR_INSTRUMENT_MANUFACTURER        /* ViString, read-only  */
#define KE230X_ATTR_INSTRUMENT_MODEL              IVI_ATTR_INSTRUMENT_MODEL               /* ViString, read-only  */
#define KE230X_ATTR_INSTRUMENT_FIRMWARE_REVISION  IVI_ATTR_INSTRUMENT_FIRMWARE_REVISION   /* ViString, read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MAJOR_VERSION  /* ViInt32,  read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_MINOR_VERSION  /* ViInt32,  read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_REVISION      IVI_ATTR_SPECIFIC_DRIVER_REVISION       /* ViString, read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_VENDOR        IVI_ATTR_SPECIFIC_DRIVER_VENDOR         /* ViString, read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_DESCRIPTION   IVI_ATTR_SPECIFIC_DRIVER_DESCRIPTION    /* ViString, read-only  */
#define KE230X_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MAJOR_VERSION /* ViInt32, read-only */
#define KE230X_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION IVI_ATTR_SPECIFIC_DRIVER_CLASS_SPEC_MINOR_VERSION /* ViInt32, read-only */

    /* Error Info */
#define KE230X_ATTR_PRIMARY_ERROR                 IVI_ATTR_PRIMARY_ERROR                  /* ViInt32   */
#define KE230X_ATTR_SECONDARY_ERROR               IVI_ATTR_SECONDARY_ERROR                /* ViInt32   */
#define KE230X_ATTR_ERROR_ELABORATION             IVI_ATTR_ERROR_ELABORATION              /* ViString  */

    /* Advanced Session Information */
#define KE230X_ATTR_LOGICAL_NAME                  IVI_ATTR_LOGICAL_NAME                   /* ViString, read-only  */
#define KE230X_ATTR_RESOURCE_DESCRIPTOR           IVI_ATTR_RESOURCE_DESCRIPTOR            /* ViString, read-only  */
#define KE230X_ATTR_IO_SESSION_TYPE               IVI_ATTR_IO_SESSION_TYPE                /* ViString, read-only  */
#define KE230X_ATTR_IO_SESSION                    IVI_ATTR_IO_SESSION                     /* ViSession, read-only */


    /*- Instrument-Specific Attributes -------------------------------------*/

    /*- Basic Instrument Operations ----------------------------------------*/
#define KE230X_ATTR_VOLTAGE_LEVEL             IVIDCPWR_ATTR_VOLTAGE_LEVEL           /* ViReal64,  multi-channel, volts */
#define KE230X_ATTR_OVP_ENABLED               IVIDCPWR_ATTR_OVP_ENABLED             /* ViBoolean, multi-channel        */
#define KE230X_ATTR_OVP_LIMIT                 IVIDCPWR_ATTR_OVP_LIMIT
#define KE230X_ATTR_CURRENT_LIMIT_BEHAVIOR    IVIDCPWR_ATTR_CURRENT_LIMIT_BEHAVIOR  /* ViInt32,   multi-channel        */
#define KE230X_ATTR_CURRENT_LIMIT             IVIDCPWR_ATTR_CURRENT_LIMIT           /* ViReal64,  multi-channel, amps  */
#define KE230X_ATTR_OUTPUT_ENABLED            IVIDCPWR_ATTR_OUTPUT_ENABLED          /* ViBoolean, multi-channel        */

    /*- Additional Output Settings -----------------------------------------*/
#define KE230X_ATTR_VPT_CLAMP_ENABLED (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 2L)
#define KE230X_ATTR_VPT_OFFSET              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 3L)
#define KE230X_ATTR_OUTPUT_BANDWIDTH        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 7L)
#define KE230X_ATTR_OUTPUT_IMPEDANCE        (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 8L)

    /*- Relay Output -------------------------------------------------------*/
#define KE230X_ATTR_RELAY_1_CONNECTED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 9L)
#define KE230X_ATTR_RELAY_2_CONNECTED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 10L)
#define KE230X_ATTR_RELAY_3_CONNECTED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 11L)
#define KE230X_ATTR_RELAY_4_CONNECTED       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 12L)

    /*- Measure -------------------------------------------------------*/
#define KE230X_ATTR_MEAS_FUNCTION           (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 13L)
#define KE230X_ATTR_MEAS_INTEGRATION_RATE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 14L)
#define KE230X_ATTR_MEAS_AVERAGE_COUNT      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 15L)

    /*- Pulse Current -------------------------------------------------------*/
#define KE230X_ATTR_MEAS_PCURR_AVERAGE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 16L)
#define KE230X_ATTR_MEAS_PCURR_TRIGGER_POLARITY (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 17L)
#define KE230X_ATTR_MEAS_PCURR_INT_TIME_LOW (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 18L)
#define KE230X_ATTR_MEAS_PCURR_INT_TIME_HIGH (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 40L)
#define KE230X_ATTR_MEAS_PCURR_INT_TIME_AVERAGE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 19L)
#define KE230X_ATTR_MEAS_PCURR_STEP_ENABLED (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 20L)
#define KE230X_ATTR_MEAS_PCURR_STEP_UP (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 22L)
#define KE230X_ATTR_MEAS_PCURR_STEP_TIME    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 23L)
#define KE230X_ATTR_MEAS_PCURR_STEP1_TIMEOUT (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 24L)
#define KE230X_ATTR_MEAS_PCURR_STEP_TIMEOUT (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 25L)
#define KE230X_ATTR_MEAS_PCURR_STEP_DELAY   (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 26L)
#define KE230X_ATTR_MEAS_PCURR_STEP_TLEV_RANGE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 27L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_ENABLED (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 28L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TLEV_RANGE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 30L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_DELAY (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 31L)

    /*- Long Integration -------------------------------------------------------*/
#define KE230X_ATTR_MEAS_LINT_TRIGGER_EDGE  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 32L)
#define KE230X_ATTR_MEAS_LINT_TIME  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 33L)
#define KE230X_ATTR_MEAS_LINT_TLEV_RANGE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 35L)
#define KE230X_ATTR_MEAS_LINT_TIMEOUT       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 36L)
#define KE230X_ATTR_MEAS_LINT_PULSE_SEARCH  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 37L)
#define KE230X_ATTR_MEAS_LINT_FAST_MODE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 38L)
#define KE230X_ATTR_MEAS_LINT_DETECT_PULSE_ONLY (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 39L)
#define KE230X_ATTR_MEAS_LINT_TIME_AUTO (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 41L)

#define KE230X_ATTR_ID_QUERY_RESPONSE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 1L)     /* ViString (Read Only) */
#define KE230X_ATTR_MEAS_LINT_TRIGGER_LEVEL_AMP (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 42L)
#define KE230X_ATTR_MEAS_LINT_TRIGGER_LEVEL_ONE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 43L)
#define KE230X_ATTR_MEAS_LINT_TRIGGER_LEVEL_MILL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 44L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL_MILL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 45L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL_AMP (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 46L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL_ONE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 47L)
#define KE230X_ATTR_IO_TIMEOUT              (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 48L)
#define KE230X_ATTR_SOUR_MAX_CURRENT_RANGE  (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 49L)
#define KE230X_ATTR_MEAS_PCURR_TIMEOUT (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 50L)
#define KE230X_ATTR_MEAS_PCURR_FAST_MODE (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 51L)
#define KE230X_ATTR_MEAS_PCURR_PULSE_SEARCH (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 52L)
#define KE230X_ATTR_MEAS_PCURR_DETECT_PULSE_ONLY (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 53L)
#define KE230X_ATTR_CURRENT_AUTO_RANGE      (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 54L)
#define KE230X_ATTR_MEAS_LINT_TRIGGER_LEVEL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 55L)
#define KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 56L)
#define KE230X_ATTR_MAX_VOLTAGE_LEVEL       (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 57L)
#define KE230X_ATTR_MEAS_PCURR_STEP_DOWN    (IVI_SPECIFIC_PUBLIC_ATTR_BASE + 58L)

    /****************************************************************************
     *------------------------ Attribute Value Defines -------------------------*
     ****************************************************************************/
    /*- Defined values for attribute KE230X_ATTR_CURRENT_LIMIT_BEHAVIOR ---*/
#define KE230X_VAL_CURRENT_REGULATE           IVIDCPWR_VAL_CURRENT_REGULATE
#define KE230X_VAL_CURRENT_TRIP               IVIDCPWR_VAL_CURRENT_TRIP

    /* Instrument specific attribute value definitions */

#define KE230X_VAL_KE2302                                           0
#define KE230X_VAL_KE2306                                           5

#define KE230X_VAL_LOW                                              0
#define KE230X_VAL_HIGH                                             1
#define KE230X_VAL_AVERAGE                                          2

#define KE230X_VAL_FALLING                                          0
#define KE230X_VAL_RISING                                           1
#define KE230X_VAL_NEITHER                                          2


#define KE230X_VAL_100_MILLI_AMP									0
#define KE230X_VAL_ONE_AMP											1
#define KE230X_VAL_FIVE_AMP											2

#define KE230X_VAL_MEAS_MODE_PCUR									0
#define KE230X_VAL_MEAS_MODE_LINT									1

#define KE230X_VAL_MAX_SAMPLE_COUNT									5000
#define KE230X_VAL_MAX_PCUR_STEP_COUNT								20

#define KE230X_VAL_AUTO_RANGE										-1

    /****************************************************************************
     *---------------- Instrument Driver Function Declarations -----------------*
     ****************************************************************************/

    /*- Init and Close Functions -------------------------------------------*/
    ViStatus _VI_FUNC  ke230x_init (ViRsrc resourceName, ViBoolean IDQuery,
                                    ViBoolean resetDevice, ViSession *vi);

    ViStatus _VI_FUNC  ke230x_InitWithOptions (ViRsrc resourceName, ViBoolean IDQuery,
            ViBoolean resetDevice, ViString optionString,
            ViSession *newVi);

    ViStatus _VI_FUNC  ke230x_close (ViSession vi);

    /*- Coercion Info Functions --------------------------------------------*/
    ViStatus _VI_FUNC  ke230x_GetNextCoercionRecord (ViSession vi,
            ViInt32 bufferSize,
            ViChar record[]);

    /*- Locking Functions --------------------------------------------------*/
    ViStatus _VI_FUNC  ke230x_LockSession (ViSession vi, ViBoolean *callerHasLock);
    ViStatus _VI_FUNC  ke230x_UnlockSession (ViSession vi, ViBoolean *callerHasLock);


    /*- Basic Instrument Operation -----------------------------------------*/
    ViStatus _VI_FUNC ke230x_ConfigureOutputEnabled (ViSession vi,
            ViConstString channelName,
            ViBoolean enabled);

    ViStatus _VI_FUNC ke230x_ConfigureOutputRange (ViSession vi,
            ViConstString channelName,
            ViInt32 rangeType,
            ViReal64 range);

    ViStatus _VI_FUNC ke230x_ConfigureCurrentLimit (ViSession vi,
            ViConstString channelName,
            ViInt32 behavior,
            ViReal64 limit);
    ViStatus _VI_FUNC ke230x_ConfigureOVP (ViSession vi,
                                           ViConstString channelName,
                                           ViBoolean enabled,
                                           ViReal64 limit);

    ViStatus _VI_FUNC ke230x_ConfigureVoltageLevel (ViSession vi,
            ViConstString channelName,
            ViReal64 level);

    ViStatus _VI_FUNC ke230x_ConfigureVoltageProtection (ViSession vi,
            ViConstString channelName,
            ViBoolean clamp,
            ViReal64 offset);

    ViStatus _VI_FUNC ke230x_ConfigureOutputBandwidth (ViSession vi,
            ViConstString channelName,
            ViInt32 bandwidth);

    ViStatus _VI_FUNC ke230x_ConfigureChannelOneImpedance (ViSession vi,
            ViReal64 impedance);

    ViStatus _VI_FUNC ke230x_ConfigureOutputRelays (ViSession instrumentHandle,
            ViInt32 relay, ViBoolean state);


    ViStatus _VI_FUNC ke230x_QueryOutputState (ViSession vi,
            ViConstString channelName,
            ViInt32 outputState,
            ViBoolean* inState);

    ViStatus _VI_FUNC ke230x_QueryMaxCurrentLimit (ViSession vi,
            ViConstString channelName,
            ViReal64 voltageLevel,
            ViReal64* maxCurrentLimit);

    ViStatus _VI_FUNC ke230x_QueryMaxVoltageLevel (ViSession vi,
            ViConstString channelName,
            ViReal64 currentLimit,
            ViReal64* maxVoltageLevel);

    ViStatus _VI_FUNC ke230x_ResetOutputProtection (ViSession vi,
            ViConstString channelName);

    /*- Measuring ----------------------------------------------------------*/

    ViStatus _VI_FUNC ke230x_Measure (ViSession vi,
                                      ViConstString channelName,
                                      ViInt32 measurementType,
                                      ViReal64* measurement);

    ViStatus _VI_FUNC ke230x_ConfigureBasicMeasurement (ViSession instrumentHandle,
            ViChar channelName[],
            ViInt32 function,
            ViReal64 integrationRate,
            ViInt32 averageCount);




    ViStatus _VI_FUNC ke230x_ConfigurePcurMeasurement (ViSession instrumentHandle,
            ViConstString channelName,
            ViInt32 averageCount,
            ViReal64 integrationTime,
            ViInt32 triggerPolarity,
            ViInt32 triggerRange,
            ViReal64 triggerLevel,
            ViReal64 triggerDelay,
            ViReal64 timeout);




    ViStatus _VI_FUNC ke230x_ConfigureAutoTime (ViSession instrumentHandle,
            ViConstString channelName,
            ViInt32 measurement);

    ViStatus _VI_FUNC ke230x_ConfigurePcurDigitizer (ViSession instrumentHandle,
            ViConstString channelName,
            ViInt32 numberOfSamples,
            ViInt32 triggerPolarity,
            ViInt32 triggerRange,
            ViReal64 triggerLevel,
            ViReal64 triggerDelay);

    ViStatus _VI_FUNC ke230x_ConfigurePcurStep (ViSession vi,
            ViConstString channelName,
            ViInt32 numberOfSteps,
            ViInt32 numberOfUpSteps,
            ViReal64 integrationTime,
            ViReal64 firstStepTimeout,
            ViReal64 stepTimeout,
            ViReal64 triggerLevelRange,
            ViReal64 triggerLevel[],
            ViReal64 triggerDelay);

    ViStatus _VI_FUNC ke230x_ConfigureLintMeasurement (ViSession instrumentHandle,
            ViConstString channelName,
            ViReal64 integrationTime,
            ViInt32 measureTriggerRange,
            ViReal64 measureTriggerLevel,
            ViInt32 measureTriggerEdge,
            ViReal64 timeout);

    ViStatus _VI_FUNC ke230x_ConfigureFastSearchDetect (ViSession instrumentHandle,
            ViChar channelName[],
            ViBoolean enableFastMode,
            ViBoolean enablePulseSearch,
            ViBoolean enableDetectPulseMode,
            ViInt32 measurement);

    ViStatus _VI_FUNC ke230x_GetActualPointCount (ViSession vi,
            ViConstString channelName,
            ViInt32 *pointsRead);

    ViStatus _VI_FUNC ke230x_ReadMeasurement (ViSession vi,
            ViConstString channelName,
            ViInt32 maxTime,
            ViReal64 *measurement);

    ViStatus _VI_FUNC ke230x_FetchMeasurement (ViSession vi,
            ViConstString channel,
            ViReal64 *measurement);

    ViStatus _VI_FUNC ke230x_ReadMultiPointMeasurement (ViSession vi,
            ViConstString channelName,
            ViInt32 maxTime,
            ViInt32 measurementArraySize,
            ViReal64 measurementArray[],
            ViInt32 *pointsRead);

    ViStatus _VI_FUNC ke230x_FetchMultiPointMeasurement (ViSession vi,
            ViConstString channelName,
            ViInt32 measurementArraySize,
            ViReal64 measurementArray[],
            ViInt32 *pointsRead);

    ViStatus _VI_FUNC ke230x_SendMeasSWTrigger (ViSession instrumentHandle,
            ViConstString channelName);

    ViStatus _VI_FUNC ke230x_MeasurementComplete (ViSession vi,
            ViConstString channelName,
            ViBoolean *complete);

    /*- Error Functions ----------------------------------------------------*/
    ViStatus _VI_FUNC  ke230x_error_query (ViSession vi, ViInt32 *errorCode,
                                           ViChar errorMessage[]);




// obsolute inherent functions
    ViStatus _VI_FUNC  ke230x_GetErrorInfo (ViSession vi, ViStatus *primaryError,
                                            ViStatus *secondaryError,
                                            ViChar errorElaboration[256]);
    ViStatus _VI_FUNC  ke230x_ClearErrorInfo (ViSession vi);



    ViStatus _VI_FUNC  ke230x_error_message (ViSession vi, ViStatus errorCode,
            ViChar errorMessage[256]);



    ViStatus _VI_FUNC  ke230x_GetError (ViSession vi,
                                        ViStatus *errorCode,
                                        ViInt32 bufferSize,
                                        ViChar description[]);
    ViStatus _VI_FUNC  ke230x_ClearError (ViSession vi);

    /*- Channel Info Functions ---------------------------------------------*/
    ViStatus _VI_FUNC ke230x_GetChannelName (ViSession vi, ViInt32 index,
            ViInt32 bufferSize,
            ViChar name[]);

    /*- Interchangeability Checking Functions ------------------------------*/
    ViStatus _VI_FUNC ke230x_GetNextInterchangeWarning (ViSession vi,
            ViInt32 bufferSize,
            ViChar warnString[]);
    ViStatus _VI_FUNC ke230x_ResetInterchangeCheck (ViSession vi);
    ViStatus _VI_FUNC ke230x_ClearInterchangeWarnings (ViSession vi);


    /*- Utility Functions --------------------------------------------------*/
    ViStatus _VI_FUNC  ke230x_reset (ViSession vi);
    ViStatus _VI_FUNC  ke230x_self_test (ViSession vi, ViInt16 *selfTestResult,
                                         ViChar selfTestMessage[]);

    ViStatus _VI_FUNC  ke230x_ResetWithDefaults (ViSession vi);
    ViStatus _VI_FUNC  ke230x_Disable (ViSession vi);
    ViStatus _VI_FUNC  ke230x_InvalidateAllAttributes (ViSession vi);

    ViStatus _VI_FUNC  ke230x_revision_query (ViSession vi,
            ViChar instrumentDriverRevision[],
            ViChar firmwareRevision[]);
    ViStatus _VI_FUNC  ke230x_WriteInstrData (ViSession vi, ViConstString writeBuffer);
    ViStatus _VI_FUNC  ke230x_ReadInstrData  (ViSession vi, ViInt32 numBytes,
            ViChar rdBuf[], ViInt32 *bytesRead);

    /*- Set, Get, and Check Attribute Functions ----------------------------*/
    ViStatus _VI_FUNC  ke230x_GetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 *value);
    ViStatus _VI_FUNC  ke230x_GetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 *value);
    ViStatus _VI_FUNC  ke230x_GetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 bufSize, ViChar value[]);
    ViStatus _VI_FUNC  ke230x_GetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession *value);
    ViStatus _VI_FUNC  ke230x_GetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean *value);

    ViStatus _VI_FUNC  ke230x_SetAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
    ViStatus _VI_FUNC  ke230x_SetAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
    ViStatus _VI_FUNC  ke230x_SetAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);
    ViStatus _VI_FUNC  ke230x_SetAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
    ViStatus _VI_FUNC  ke230x_SetAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

    ViStatus _VI_FUNC  ke230x_CheckAttributeViInt32 (ViSession vi, ViConstString channelName, ViAttr attribute, ViInt32 value);
    ViStatus _VI_FUNC  ke230x_CheckAttributeViReal64 (ViSession vi, ViConstString channelName, ViAttr attribute, ViReal64 value);
    ViStatus _VI_FUNC  ke230x_CheckAttributeViString (ViSession vi, ViConstString channelName, ViAttr attribute, ViConstString value);
    ViStatus _VI_FUNC  ke230x_CheckAttributeViSession (ViSession vi, ViConstString channelName, ViAttr attribute, ViSession value);
    ViStatus _VI_FUNC  ke230x_CheckAttributeViBoolean (ViSession vi, ViConstString channelName, ViAttr attribute, ViBoolean value);

    /*********************************************************
        Functions reserved for class driver use only.
        End-users should not call these functions.
     *********************************************************/
    ViStatus _VI_FUNC  ke230x_IviInit (ViRsrc resourceName, ViBoolean IDQuery,
                                       ViBoolean reset, ViSession vi);
    ViStatus _VI_FUNC  ke230x_IviClose (ViSession vi);

    /****************************************************************************
     *------------------------ Error And Completion Codes ----------------------*
     ****************************************************************************/
#define KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED  (IVI_SPECIFIC_ERROR_BASE + 1)
#define KE230X_ERROR_MAX_TIME_EXCEEDED            (IVI_SPECIFIC_ERROR_BASE + 2)

    /****************************************************************************
     *---------------------------- End Include File ----------------------------*
     ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
#endif /* __KE230X_HEADER */




