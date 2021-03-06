s��        ��   � ��  X   �   ����                               age1960     age1960                                       � � ��ViInt16     � ��ViPInt16     
�  	ViInt16 []  �  � ��ViInt32     � ��ViPInt32     
�  	ViInt32 []  � � ��ViReal64     	� 	��ViPReal64     �  
ViReal64 []     � ��ViString     	� 	��ViBoolean     
� 
��ViPBoolean     � 	 ViBoolean []     � ��ViRsrc     	� 	��ViSession     � ��ViStatus     � ��ViPChar     	�  ViChar []  $    age1960 Instrument Driver Revision A.05.00 
Copyright �1999-2002 Agilent Technologies

This driver is to be used with the 8960 Series 10 E5515A/B/C that contains the E1960A GSM TA Revision A.09.xx or higher. This driver also supports the E1985A Fast Swithing TA Revision A.03.xx or higher.
     A    Application Functions allow high level control of the test set.     m    Call Processing function will allow the control of the base station emulator functions within the test set.     R    Link Control will allow the configuration of the base station emulator settings.     R    Call control will allow the mobile station or base station origination of calls.     P    System Utilities will perform general functions that apply to the test system.     N    Measure class will Configure, Read, Initiate, and Fetch measurement results.     ^    Configure Measurements set the desired measurement conditions for the measurement functions.     �    Read functions initiate and retrive measured results in a sequential method.  The measurement functions should be Configured before the Read is used.     �    Initiate functions start the desired measurements in a concurrent method.  The measurement functions should be Configured before they are Initiated.     �    Fetch functions retrive measured results in a concurrent method.  The measurement functions should be Configured and Initiated before they are Fetched.     �    Utility functions perform several standard P&P tasks and a variety of Agilent utility tasks.  They also include Agilent's instrument command passthrough functions which allow generic access to the instrument's command set.
     �    Agilent's instrument command passthrough functions.

These functions pass commands directly to the instrument, thus allowing generic access to the instrument's command set.
     �    The initialize function initializes the software connection to the instrument and optionally verifies that instrument is in the system. In addition, it may perform any necessary actions to place the instrument in its reset state.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     �    [INPUT PARAMETER]

The instrument's VISA resource description.

The default for this instrument is "GPIB0::14::INSTR". If you change the GPIB address of this instrument, you must change the default resource description as well.
     �    [INPUT PARAMETER]

If VI_TRUE, perform a revision query to verify that the instrument is the one expected by the driver.

If VI_FALSE, do not perform a revision query.
     �    [INPUT PARAMETER]

If VI_TRUE, perform a reset operation while initializing the instrument.

If VI_FALSE, do not perform a reset operation.
     �    [OUTPUT PARAMETER]

The instrument's VISA session handle. This is VI_NULL if an error occurred during the initialization process.
    

o���  �    Status                            � 9   �  �    Resource Name                     � 4# �       ID Query                          � 4� �       Reset Device                      +
 5 �  �    Instrument Handle                  	           "GPIB0::14::INSTR"  % Do Query VI_TRUE Skip Query VI_FALSE  # Reset VI_TRUE Don't Reset VI_FALSE    	            -    This function will set the BA Table ARFCNs.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of BA table array. This value MUST be equal to or less than the number of array elements defined for BATableArray[].

age1960_BA_TABLE_PTS_MIN    0
age1960_BA_TABLE_PTS_MAX   16     z    [INPUT PARAMETER]

The cell to set the BA table ARFCNs for.


age1960_CELL_1   1    Cell 1
age1960_CELL_2   2    Cell 2
    �    [INPUT PARAMETER]

The integer array that contains the BA Table ARFCNs. Each value in this array will be checked against the following values for the given Cell band:

PGSM MIN Channel    = age1960_PGSM_MIN_CH          1
PGSM MAX Channel    = age1960_PGSM_MAX_CH        124
EGSM MIN Channel 1  = age1960_EGSM_MIN_CH_1        0
EGSM MAX Channel 1  = age1960_EGSM_MAX_CH_1      124
EGSM MIN Channel 2  = age1960_EGSM_MIN_CH_2      975
EGSM MAX Channel 2  = age1960_EGSM_MAX_CH_2     1023
DCS1800 MIN Channel = age1960_DCS1800_MIN_CH     512
DCS1800 MAX Channel = age1960_DCS1800_MAX_CH     885
PCS1900 MIN Channel = age1960_DCS1800_MIN_CH     512
PCS1900 MAX Channel = age1960_DCS1800_MAX_CH     810
    �    [INPUT PARAMETER]

The Cell band to append to the command. If Current Band is used, the BA Table will be set for the Cell band the instrument is presently configured for.


age1960_CURRENT_BAND    0    Current Band
age1960_PGSM_BAND       1    PGSM Band
age1960_EGSM_BAND       2    EGSM Band
age1960_DCS1800_BAND    3    DCS 1800 Band
age1960_PCS1900_BAND    4    PCS 1900 Band
age1960_GSM850_BAND     5    GSM 850 Band
    qg���  �    Status                            P :  �  �    Instrument Handle                 � 2� �      BA Table Array Size               � 2 E �      BA Table Cell                      { U �       BA Table Array                    � {r �      BA Table Band                      	           0                                    ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2                   �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND    '    This function will set the BCH ARFCN.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [INPUT PARAMETER]

The BCH ARFCN number to be set. This number will be checked against the following values for the given Cell band:

PGSM MIN Channel      age1960_PGSM_MIN_CH          1
PGSM MAX Channel      age1960_PGSM_MAX_CH        124
EGSM MIN Channel 1    age1960_EGSM_MIN_CH_1        0
EGSM MAX Channel 1    age1960_EGSM_MAX_CH_1      124
EGSM MIN Channel 2    age1960_EGSM_MIN_CH_2      975
EGSM MAX Channel 2    age1960_EGSM_MAX_CH_2     1023
DCS1800 MIN Channel   age1960_DCS1800_MIN_CH     512
DCS1800 MAX Channel   age1960_DCS1800_MAX_CH     885
PCS1900 MIN Channel   age1960_DCS1800_MIN_CH     512
PCS1900 MAX Channel   age1960_DCS1800_MAX_CH     810
GSM850 MIN Channel    age1960_GSM850_MIN_CH      128
GSM850 MIN Channel    age1960_GSM850_MAX_CH      251

    �    [INPUT PARAMETER]

The Cell band to append to the command. If Current Band is used, the BCH ARFCN will be set for the Cell band the instrument is presently configured for.


age1960_CURRENT_BAND     0     Current Band
age1960_PGSM_BAND        1     PGSM Band
age1960_EGSM_BAND        2     EGSM Band
age1960_DCS1800_BAND     3     DCS 1800 Band
age1960_PCS1900_BAND     4     PCS 1900 Band
age1960_GSM850_BAND      5     GSM 850 Band
     y    [INPUT PARAMETER]

The cell to set the BCH ARFCNs for.


age1960_CELL_1     1    Cell 1
age1960_CELL_2     2    Cell 2
     �    [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the BCH ARFCN has been changed. If set to No Wait, this function will send the command to set the BCH ARFCN and return.    a���  �    Status                            � ,  �  �    Instrument Handle                 [ ?� �      BCH ARFCN                          h � 7 �      BCH ARFCN Band                    "$ > 7 �      BCH ARFCN Cell                    "� �u �       BCH ARFCN Seq                      	           0      �                             �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND               ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2  ,  Wait for Completed VI_TRUE No Wait VI_FALSE    �    This function will set the burst type to be used. This parameter is used when Operating Mode = Test mode or Cell Activated State = Off.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [INPUT PARAMETER]

The Burst Type to set.


age1960_TSC0_BURST     0     TSC0 Burst
age1960_TSC1_BURST     1     TSC1 Burst
age1960_TSC2_BURST     2     TSC2 Burst
age1960_TSC3_BURST     3     TSC3 Burst
age1960_TSC4_BURST     4     TSC4 Burst
age1960_TSC5_BURST     5     TSC5 Burst
age1960_TSC6_BURST     6     TSC6 Burst
age1960_TSC7_BURST     7     TSC7 Burst
age1960_RACH_BURST     8     RACH Burst    &�
s���  �    Status                            (� "  �  �    Instrument Handle                 ) @ � �      Burst Type                         	           0           	  TSC0 Burst age1960_TSC0_BURST TSC1 Burst age1960_TSC1_BURST TSC2 Burst age1960_TSC2_BURST TSC3 Burst age1960_TSC3_BURST TSC4 Burst age1960_TSC4_BURST TSC5 Burst age1960_TSC5_BURST TSC6 Burst age1960_TSC6_BURST TSC7 Burst age1960_TSC7_BURST RACH Burst age1960_RACH_BURST    H    This function sets the Cell band in the instrument for the given cell.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [INPUT PARAMETER]

The Serving Cell Band to set.


age1960_PGSM_BAND       1     PGSM Band
age1960_EGSM_BAND       2     EGSM Band
age1960_DCS1800_BAND    3     DCS 1800 Band
age1960_PCS1900_BAND    4     PCS 1900 Band
age1960_GSM850_BAND     5     GSM850 Band
     �    [INPUT PARAMETER]

The serving cell to set the Cell Band for.


age1960_CELL_1     1     Cell 1
age1960_CELL_2     2     Cell 2
    ,�\���  �    Status                            .� C  �  �    Instrument Handle                 .� Wg �      Serving Cell Band                 0 W N �      Serving Cell Band Cell             	           0               �PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND               ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2   &    This function will set the instrument's cell output power level in dBm. This function will also allow for the correction of any differences bewteen uplink path offset that was set with the age1960_amplitudeOffset() function and downlink path offset requirements when a single offset is used.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     {    [INPUT PARAMETER]

The cell to set the Cell Power for.


age1960_CELL_1     1     Cell 1
age1960_CELL_2     2     Cell 2
     �    [INPUT PARAMETER]

The cell power in dBm to be set at the UUT interface. 

 age1960_CELL_POWER_MIN    -127
 age1960_CELL_POWER_MAX     -10
        [INPUT PARAMETER]

The delta value that will be added to the Cell Power setting to correct for differences between the downlink and uplink path losses if only a single value is set with age1960_amplitudeOffset() function. Older versions of firmware used only a single path loss correction value and this parameter was needed to correct for differences between uplink frequency and downlink frequency path losses. Newer versions of firmware allow up to 20 frequencies and associated path losses to be entered, thereby negating the need for this parameter. If you are using more than one entry in the RF IN/OUT Amplitude offset table than this parameter should always have a value of 0.
 
        
Cell Power Delta_MIN    -75.000000 
Cell Power Delta_MAX    +75.000000   


    3�\���  �    Status                            5q .  �  �    Instrument Handle                 5� L 4 �      Cell Power Cell                   6T L � �      Cell Power                        6� L� �      Cell Power Delta                   	           0               ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2 ?�      �$      �_�     �I                ?�      @R�     �R�                          ]    This function will set the Expected Power level (in dBm) that the instrument will receive.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The expected power in dBm to be set. 

 age1960_EXPECTED_POWER_MIN    -60.0
 age1960_EXPECTED_POWER_MAX    +53.0    �    [INPUT PARAMETER]

The manual band to append to the command. If Current Band is used, the expected power will be set for the manual band the instrument is presently configured for.


age1960_CURRENT_BAND     0     Current Band
age1960_PGSM_BAND        1     PGSM Band
age1960_EGSM_BAND        2     EGSM Band
age1960_DCS1800_BAND     3     DCS 1800 Band
age1960_PCS1900_BAND     4     PCS 1900 Band
age1960_GSM850_BAND      5     GSM 850 Band
    <R���  �    Status                            =� H  �  �    Instrument Handle                 >F ? n �      Expected Power                    >� <] �      Expected Power Band                	           0 ?�      @J�     �N      @*                              �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM850 Band age1960_GSM850_BAND       This function will set the loopback mode to be used. This function should always be use to set the loopback, since the driver will use the last value set via this function when the driver needs to turn off a measurement function that automatically controls loopback (BERR, FBER).
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The loopback mode to set.


age1960_LOOPBACK_OFF     0     Off
age1960_LOOPBACK_A       1     Type A
age1960_LOOPBACK_B       2     Type B
age1960_LOOPBACK_C       3     Type C
    C�
s���  �    Status                            Ex "  �  �    Instrument Handle                 E� M � � d    Loopback Mode                      	           0               gOff age1960_LOOPBACK_OFF Type A age1960_LOOPBACK_A Type B age1960_LOOPBACK_B Type C age1960_LOOPBACK_C    7    This function sets the manual band in the instrument.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [INPUT PARAMETER]

The manual band to set.


age1960_PGSM_BAND        1     PGSM Band
age1960_EGSM_BAND        2     EGSM Band
age1960_DCS1800_BAND     3     DCS 1800 Band
age1960_PCS1900_BAND     4     PCS 1900 Band
age1960_GSM850_BAND      5     GSM 850 Band
    Hs���  �    Status                            I� "  �  �    Instrument Handle                 JJ ? � �      Manual  Band                       	           0               �PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND    q    This function will set the manual channel (which is available when the receiver control is in the manual mode).    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [INPUT PARAMETER]

The manual channel number to be set.  This number will be checked against the following values for the given manual band:

PGSM MIN Channel         age1960_PGSM_MIN_CH        1 
PGSM MAX Channel         age1960_PGSM_MAX_CH        124 
EGSM MIN Channel 1       age1960_EGSM_MIN_CH_1      0 
EGSM MAX Channel 1       age1960_EGSM_MAX_CH_1      124 
EGSM MIN Channel 2       age1960_EGSM_MIN_CH_2      975 
EGSM MAX Channel 2       age1960_EGSM_MAX_CH_2      1023
DCS1800 MIN Channel      age1960_DCS1800_MIN_CH     512 
DCS1800 MAX Channel      age1960_DCS1800_MAX_CH     885 
PCS1900 MIN Channel      age1960_DCS1800_MIN_CH     512 
PCS1900 MAX Channel      age1960_DCS1800_MAX_CH     810    �    [INPUT PARAMETER]

The manual band to append to the command.  If Current Band is used, the manual channel  will be set for the manual band the instrument is presently configured for.


age1960_CURRENT_BAND     0     Current Band
age1960_PGSM_BAND        1     PGSM Band
age1960_EGSM_BAND        2     EGSM Band
age1960_DCS1800_BAND     3     DCS 1800 Band
age1960_PCS1900_BAND     4     PCS 1900 Band
age1960_GSM850_BAND      5     GSM 850 Band
    M0h���  �    Status                            O A  �  �    Instrument Handle                 Oo ; g �      Manual Chan                       R; 7s �      Manual Chan Band                   	           0      �                             �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND    /    This function will set the MS Timing Advance.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The MS Timing Advance number to be set. This number will be checked against the following values:

 age1960_TADVANCE_MIN     0
 age1960_TADVANCE_MAX    63
     �    [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the MS Timing Advance has been changed. If set to No Wait, this function will send the MS Timing Advance command and return.    Vi���  �    Status                            W� "  �  �    Instrument Handle                 XD @ < �      MS Timing Advance                 X� <} �       MS Timing Advance Seq              	           0       ?                 ,  Wait for Completed VI_TRUE No Wait VI_FALSE    )    This function will set the MS TX Level.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    I    [INPUT PARAMETER]

The MS TX number to be set.  This number will be checked against the following values for the given traffic band:

PGSM MIN MS TX Level     age1960_PGSM_MIN_TXL           0
PGSM MAX MS TX Level     age1960_PGSM_MAX_TXL           31
EGSM MIN MS TX Level     age1960_EGSM_MIN_TXL           0 
EGSM MAX MS TX Level     age1960_EGSM_MAX_TXL           31
DCS1800 MIN MS TX Level  age1960_DCS1800_MIN_TXL        0
DCS1800 MAX MS TX Level  age1960_DCS1800_MAX_TXL        31
PCS1900 MIN MS TX Leve   age1960_PCS1900_MIN_TXL_1      0
PCS1900 MAX MS TX Level  age1960_PCS1900_MAX_TXL_1      15
PCS1900 MIN MS TX Level  age1960_PCS1900_MIN_TXL_2      30
PCS1900 MAX MS TX Level  age1960_PCS1900_MAX_TXL_2      31
GSM850 MIN MS TX Level   age1960_GSM850_MIN_TXL         0
GSM850 MAX MS TX Level   age1960_GSM850_MAX_TXL         31
      �    [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the level has been changed. If set to No Wait, this function will send the change MS TX Level command and return.    �    [INPUT PARAMETER]

The traffic band to append to the command. If Current Band is used, the MS TX level will be set for the traffic band the instrument is presently configured for.


age1960_CURRENT_BAND   0     Current Band
age1960_PGSM_BAND      1     PGSM Band
age1960_EGSM_BAND      2     EGSM Band
age1960_DCS1800_BAND   3     DCS 1800 Band
age1960_PCS1900_BAND   4     PCS 1900 Band
age1960_GSM850_BAND    5     PCS 1900 Band
    [Kp���  �    Status                            ]* "  �  �    Instrument Handle                 ]� A ' �      MS TX Level                       `� =� �       MS TX Level Seq                   a� A � �      MS TX Level Band                   	           0                        ,  Wait for Completed VI_TRUE No Wait VI_FALSE               �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND    �    This function will set the instrument to either the Active Cell operating mode for testing using call processing or to the Test Mode operating mode for testing with a test bus.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The operating mode to set.


age1960_ACELL_MODE   1    Active Cell
age1960_TEST_MODE    0    Test Mode
    fgs���  �    Status                            hF "  �  �    Instrument Handle                 h� C � �      Operating Mode                     	           0               ;Active Cell age1960_ACELL_MODE Test Mode age1960_TEST_MODE    o    This function will define whether the Base Station Emulator is in Manual or Automatic receiver control mode.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The measurement receiver control mode.

 age1960_MAN_RCV_CONTROL    0    Manual
 age1960_AUTO_RCV_CONTROL   1    Auto    j�s���  �    Status                            lz "  �  �    Instrument Handle                 l� 9 � �      Receiver Control                   	           0              =Manual age1960_MAN_RCV_CONTROL Auto age1960_AUTO_RCV_CONTROL    *    This function will set the TCH Timeslot.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().     �    [INPUT PARAMETER]

The Timeslot number to be set. This number will be checked against the following values:

 age1960_TIMESLOT_MIN       3
 age1960_TIMESLOT_MAX       5     �    [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the timeslot has been changed. If set to No Wait, this function will send the change timeslot command and return.    n�Y���  �    Status                            py G  �  �    Instrument Handle                 p� @ m �      TCH Timeslot                      q� ;m �       TCH Timeslot Seq                   	           0                      ,  Wait for Completed VI_TRUE No Wait VI_FALSE    '    This function will set the TCH ARFCN.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().        [INPUT PARAMETER]

The TCH ARFCN number to be set.  This number will be
checked against the following values for the given
TCH band:

PGSM MIN Channel       age1960_PGSM_MIN_CH          1
PGSM MAX Channel       age1960_PGSM_MAX_CH        124
EGSM MIN Channel 1     age1960_EGSM_MIN_CH_1        0
EGSM MAX Channel 1     age1960_EGSM_MAX_CH_1      124
EGSM MIN Channel 2     age1960_EGSM_MIN_CH_2      975
EGSM MAX Channel 2     age1960_EGSM_MAX_CH_2     1023
DCS1800 MIN Channel    age1960_DCS1800_MIN_CH     512
DCS1800 MAX Channel    age1960_DCS1800_MAX_CH     885
PCS1900 MIN Channel    age1960_DCS1800_MIN_CH     512
PCS1900 MAX Channel    age1960_DCS1800_MAX_CH     810
GSM850 MIN Channel     age1960_GSM850_MIN_CH      128
GSM850 MIN Channel     age1960_GSM850_MAX_CH      251    �    [INPUT PARAMETER]

The TCH band to append to the command. If Current Band is used, the TCH ARFCN will be set for the TCH band the instrument is presently configured for.


age1960_CURRENT_BAND   0    Current Band
age1960_PGSM_BAND      1    PGSM Band
age1960_EGSM_BAND      2    EGSM Band
age1960_DCS1800_BAND   3    DCS 1800 Band
age1960_PCS1900_BAND   4    PCS 1900 Band
age1960_GSM850_BAND    5    GSM 850 Band
    "    [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the TCH ARFCN has been changed. If set to No Wait, this function will send the command to set the TCH ARFCN and return.


VI_TRUE    1   Wait for Completed
VI_FALSE   0   No Wait
    s�s���  �    Status                            u�
 "  �  �    Instrument Handle                 v
 G C �      TCH ARFCN                         y! G � �      TCH ARFCN Band                    z� B� �       TCH ARFCN Seq                      	           0      �                             �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND  ,  Wait for Completed VI_TRUE No Wait VI_FALSE    8    This function sets the traffic band in the instrument.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().     �    [INPUT PARAMETER]

The traffic band to set.


age1960_PGSM_BAND      1     PGSM Band
age1960_EGSM_BAND      2     EGSM Band
age1960_DCS1800_BAND   3     DCS 1800 Band
age1960_PCS1900_BAND   4     PCS 1900 Band
age1960_GSM850_BAND    5     GSM 850 Band
    ~cs���  �    Status                            �B "  �  �    Instrument Handle                 �� > � �      TCH Band                           	           0               �PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND    �    This function sets the channel mode for either Full Rate speech (FR Speech) or Enhanced Full Rate Speech (EFR Speech). The phone being tested has to be able to support EFR. The default is FR.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The channel mode to set.


age1960_FR_SPEECH    0    Full Rate Speech
age1960_EFR_SPEECH   1    Enhanced Full Rate Speech
    ��i���  �    Status                            �� "  �  �    Instrument Handle                 � C � �      Channel Mode                       	           0               :FR Speech age1960_FR_SPEECH EFR Speech age1960_EFR_SPEECH       This function sets the TX Level FACCH Signaling parameter.

When TX Level FACCH Signaling is set to on, the base station emulator uses both a FACCH (Fast Associated  Control CHannel) channel assignment and an update to the SACCH (Slow Associated Control CHannel) header to signal the mobile to change to a new power level.

When TX Level FACCH Signaling is set to off, the base station emulator uses only an update to the SACCH header to signal the mobile to change to a new power level. A FACCH channel assignment message is not sent. This setting is useful if you want to update the SACCH header's TX Level field without performing a channel assignment.

The setting of TX Level FACCH Signaling can be changed in either of the test set's two operating modes, Active Cell or Test mode.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     p    [INPUT PARAMETER]

Toggle for the TX Level FACCH Signaling function.


VI_TRUE    1    On
VI_FALSE   0    Off
    ��k���  �    Status                            �� "  �  �    Instrument Handle                 �� S �       Set FACCH Signaling                	           0   On 1 Off 0   �    This query returns the burst timing error report which provides a coarse indication of the difference between the actual and expected burst reception times.  The error is calculated on a burst by burst basis from the adaptive equalization filter coefficients.  The reporting period for burst timing error is approximately 0.5 seconds, and the value returned is the peak timing error occurring during the period.  Note that this report is not based on a demodulated midamble position.      �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1964_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1964_init().
     7    [OUTPUT PARAMETER]

The returned burst timing error.
    �"k���  �    Status                            � "  �  �    Instrument Handle                 �c 7 � �       Burst Timing Error                 	           0    	          M    This function will verify that the instrument is in the correct state to originate a call to the mobile station. If the instrument state is correct, this function will originate a call and wait until the instrument indicates that the call is connected or in the alerting state. If the timeout value has been exceeded before the call is in the desired state, an error is returned. If a call is already connected, this function will return with no error status set. If the timeout value is set to 0, this function will just originate the call and not wait until it is in the desired state.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The maximum time in seconds this function will try to originate a call to the mobile station.

 age1960_CP_TIMEOUT_MIN    0.0
 age1960_CP_TIMEOUT_MAX  120.0     �    [INPUT PARAMETER]

The desired completion state for the call origination.


age1960_CALL_CONNECTED   0    Call Connected State
age1960_CALL_ALERTING    1    Call Alerting State
    ��s���  �    Status                            �� "  �  �    Instrument Handle                 �� x � �      Originate Time Out                �� 2 � � �    Completion                         	           0 ?�      @^              @$                              VCall Connected State age1960_CALL_CONNECTED Call Alerting State age1960_CALL_ALERTING   W    This function will verify that the instrument is in the correct state to end the call with the mobile station. If the instrument state is correct, this function will end the call and if desired wait until the instrument indicates that the call is not connected. If a call is not connected, this function will return with no error status set.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [INPUT PARAMETER]

If set to Wait for Completed, this function will wait until the instrument indicates that the call has been disconnected. If set to No Wait, this function will send the CALL:END command and return.


VI_TRUE    1    Wait for Completed
VI_FALSE   0    No Wait
    �:s���  �    Status                            � "  �  �    Instrument Handle                 �y T � �       End Seq                            	           0  ,  Wait for Completed VI_TRUE No Wait VI_FALSE    �    This function will configure the instrument to wait for a MS originated call. After this function is called, the age1960_waitForConnect() function should be used to wait for the call to be connected.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �Is���  �    Status                            �( "  �  �    Instrument Handle                  	           0   �    This function will wait until the instrument indicates that a call is connected or until the timeout period ends. This function should be used for MS originated calls after age1960_confForMSOriginate() has been called. If the instrument indicates that the call is already CONNECTED, this function will return with VI_SUCCESS. If the instrument indicated that it is not in the proper state for a call, this function will return with the errStatus set. After the call has been connected, this function will re-check the call status state to verify that it is CONNECTED.  If it is not CONNECTED, this function will return an error condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().     �    [INPUT PARAMETER]

The maximum time in seconds this function will wait for a call from the mobile station.

 age1960_CP_TIMEOUT_MIN    0
 age1960_CP_TIMEOUT_MAX  120    ��s���  �    Status                            �j "  �  �    Instrument Handle                 �� = � �      Connect Time Out                   	           0 ?�      @^              @4                  �    This function allows the user to change the TA loaded in the instrument to another one currently resident on the hard disk. This could be a later or earlier version of GSM or a different TA (e.g. AMPS/136). This function is only available on A.04.01 and beyond for GSM. A.03.04 does not support TA switching, however you can switch from A.04.01 to A.03.04, but not the other way.

Select the desired TA and the TA's revision. If the selected TA is currently loaded with the desired revision, then nothing is done. If the TARevision is "Latest" or "", then the latest revision for the desired TA is loaded. If the TA or the revision does not exist, then an error is returned.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     \    [INPUT PARAMETER]

The name of the TA to load. This is the name followed by "Mobile Test".     �    [INPUT PARAMETER]

The revision number of the TA to load. If TA Revision is set to "Latest" or "", then latest TA on the instrument disk will be loaded.    ��o���  �    Status                            �� "  �  �    Instrument Handle                 �> ; k � ^    TA Name                           ��  � �  �    TA Revision                        	           0    "GSM Mobile Test"    ""    |    Read the current TA name, and revision. This function will remove the set of quotation marks around the two string values.     0    [OUTPUT PARAMETER]

The name of the TA loaded.     ;    [OUTPUT PARAMETER]

The revision number of the TA loaded.     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1962_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1962_error_message() function to get a textual description of errors and warnings.
    �� > g � ^    TA Name Query                     �  � � �  �    TA Revision Query                 �C &  �  �    Instrument  Handle                ��	v���  �    Status                             	            	            0    	           j    Read the current TA model. This function will remove the set of quotation marks around the model number.     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1962_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1962_error_message() function to get a textual description of errors and warnings.
     8    [OUTPUT PARAMETER]

The model number of the TA loaded.    �� :  �  �    Instrument  Handle                �Q[���  �    Status                            �2 Z � �      TA Model                           0    	           	           S    This function is meant to work with the E1985A Fast Switching TA. The function fast switches the current active Test Application Partition (TAP) to the TAP specified. If the current Test Application is E1960A and the TAP specified is GSM, the function returns without doing anything.  If the TA is E1960A and the TAP specified is something other than GSM (e.g. AMPS/136), then the error age1960_INSTR_TA_NOT_LICENSE is used to indicate an error condition. If the TA is not E1960A and does not support the command SYST:APPL:CAT:FORM?, then a UUT error will occur. The error status is returned.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1962_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1962_error_message() function to get a textual description of errors and warnings.
    Q    [INPUT PARAMETER]

The name of the specified Test Application Partition (TAP) to switch to.

The following constants have been defined for user convenience.

age1960_GPRS_FORMAT                 "GPRS"
age1960_GSM_FORMAT                  "GSM"
age1960_AMPS_IS136_FORMAT           "AMPS\IS136"
age1960_WCDMA_FORMAT                "WCDMA"    �� $  �  �    Instrument  Handle                ��t���  �    Status                            �� H � �  �    TAP Name                           0    	           "GSM"       This function is meant to work with the E1985A Fast Switching TA. The function returns the currently active Test Application Partition (TAP) name. If the Test Application does not support SYST:APPL:FORMAT:NAME? command, a UUT error will occur resulting in a time out being returned. 
    J    [OUTPUT PARAMETER]

The name of the current active Test Application Partition (TAP).

The following constants have been defined for user convenience.

age1960_GPRS_FORMAT                 "GPRS"
age1960_GSM_FORMAT                  "GSM"
age1960_AMPS_IS136_FORMAT           "AMPS\IS136"
age1960_WCDMA_FORMAT                "WCDMA"    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1962_error_message() function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1962_init().    � F � �      TAP Name Query                    �Sq���  �    Status                            �4 !  �  �    Instrument  Handle                 	            	           0    H    This function will convert an integrity code to the integrity message.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     P    [OUTPUT PARAMETER]

The integrity message. This is limited to 256 characters.
     p    [INPUT PARAMETER]

The integrity code to be converted.

age1960_INTEGRITY_MIN   0
age1960_INTEGRITY_MAX  23


    şo���  �    Status                            ǀ "  �  �    Instrument  Handle                �� � # � �    Integrity Message                 �8 & � �      Integrity Code                     	           0    	                                 P    This function will perform a partial preset by sending the SYST:PRESet3 command, and set all measurements to single trigger mode by sending the SET:CONT:OFF command. This function should be used to quickly place the instrument in a known state without changing any of the "SETup" parameters except Trigger Arm, which is set to Single.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �	k���  �    Status                            �� "  �  �    Instrument Handle                  	           0    �    This function will determine the present downlink and uplink frequencies. These frequencies could then be used to find path loss factors. This function was used in earlier versions of the GSM TA. Newer versions will use age1960_getPresentFreq_Q().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     }    [INPUT PARAMETER]

The cell to return the current frequencies.


age1960_CELL_1   1    Cell 1
age1960_CELL_2   2    Cell 2
     Y    [OUTPUT PARAMETER]

The current downlink frequency in Hz that the test set is tuned to.     W    [OUTPUT PARAMETER]

The current uplink frequency in Hz that the test set is tuned to.    ��o���  �    Status                            Я "  �  �    Instrument  Handle                � N ( �      Cell                              є F � �  x    Downlink Freq                     �� F� �  x    Uplink Freq                        	           0               ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2    	           	           l    This function will determine the present downlink, uplink and measurement frequencies of the instrument.      �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     Y    [OUTPUT PARAMETER]

The current downlink frequency in Hz that the test set is tuned to.     W    [OUTPUT PARAMETER]

The current uplink frequency in Hz that the test set is tuned to.     �    [OUTPUT PARAMETER]

The current measurement frequency in Hz that the test set is tuned to. If the measurement frequency control is set to Auto, then the measurement frequency will be the same as the uplink frequency.
    �5o���  �    Status                            � "  �  �    Instrument  Handle                �v D $ �  x    Downlink Freq                     �� D � �  x    Uplink Freq                       �6 D� �  x    Measurement Freq                   	           0    	           	           	           ]    This function will return the current Cell Power level in dBm that is set for a given cell.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     z    [INPUT PARAMETER]

The cell to set query the UUT level for.


age1960_CELL_1   1    Cell 1
age1960_CELL_2   2    Cell 2
     :    [OUTPUT PARAMETER]

The current cell power level in dBm.    ټ^���  �    Status                            ۝ *  �  �    Instrument  Handle                �� 9 5 �      Cell                              � 1m �  x    Level                              	           0               ,Cell 1 age1960_CELL_1 Cell 2 age1960_CELL_2    	          @    This function will get the closest offset value from the RF In/Out amplitude offset table based on the frequency. The offset value returned will be an interpolated value if there is no exact frequency match in the offset table. The amplitude offsets can be set using the age1960_amplitudeOffset() function.

In earlier versions of the GSM TA only 1 offset value could be set and therefore only one value was ever returned. Beginning with E1960A TA version A.07.xx and E1985A TA version A.01.xx the offset table can include up to 20 frequencies and associated offset values.
     �    [OUTPUT PARAMETER]

Amplitude offset from instrument in dB. This may be interpolated from the closest two frequencies' offsets.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1962_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     �    [INPUT PARAMETER]

Desired frequency offset to retrieve in Hz.

 age1960_OFFSET_FREQ_MIN    292500000.0
 age1960_OFFSET_FREQ_MAX   2700000000.0
    �5 F\ �  x    Offset                            � K  �  �    Instrument  Handle                � �M���  �    Status                            �� N q �      Frequency                          	           0    	        A.��    A��`   A�o2    A�ׄ                E    This function will fill the RF IN/OUT Amplitude Offset table.  This function will automatically set the state of the frequency correction to ON. The offset will be used to adjust the downlink, uplink and measurement frequencies.  When operating at frequencies not specified in the array, but within the boundary defined by the lowest frequency and the highest frequency, linear interpolation will be used to define offset values.  When operating at frequencies outside the boundaries defined by the lowest frequency and the highest frequency, the offset associated with the closest frequency will be used.                    

In earlier versions of the GSM TA only 1 offset value could be set. Beginning with E1960A TA version A.07.xx and E1985A TA version A.01.xx the offset table can include up to 20 frequencies and associated offset values.

WARNING!  If the offset values are large, it may cause the receiver to be over ranged, resulting in measurements being unavailable or invalid. Turning on the automatic logging using age1960_errorQueryDetect() will alert you to this problem.  

    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The frequency array of reals to send in the comma separated form. The frequencies are in Hz. This number will be checked against the following values:

age1960_OFFSET_FREQ_MIN     292500000.0 
age1960_OFFSET_FREQ_MAX    2700000000.0      �    [INPUT PARAMETER]

The offset array of reals to send in the comma separated form.  The offsets are in dB. This number will be checked against the following values:

age1960_PATH_GAIN_MIN    -100.0 
age1960_PATH_GAIN_MAX    +100.0    c    [INPUT PARAMETER]

The number of items to send to the instrument. This number MUST be equal to or less than the actual frequency and offset array sizes. If set to 0 the STATe of the amplitude offsets table will be set to OFF. This number will be checked against the following values:

age1960_AMPOFFSET_ITEMS_MIN     0 
age1960_AMPOFFSET_ITEMS_MAX    20    �o���  �    Status                            �� "  �  �    Instrument  Handle                �C / 3 �       Frequency Array                   �I . � �       Offset Array                      �8 2� �      Amp Offset Items                   	           0                                v    This function will set a flag that will make the driver use SRQ and MAV techniques to keep from hanging the bus. These techniques are only required if mulitple test systems are being run from the same controller. The flag is set to VI_FALSE in the age1960_init function. Therefore, this function does not need to be called unless it is desired to set the use SRQ MAV flag.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Set to true if the driver functions should use SRQ MAV techniques.


VI_TRUE    1   Use SRQ MAV
VI_FALSE   0   Do not use SRQ MAV
    �Zo���  �    Status                            �; "  �  �    Instrument  Handle                � E  �       use SRQ MAV                        	           0    On VI_TRUE Off VI_FALSE    ;    This function will query how the use SRQ MAV flag is set.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     F    [OUTPUT PARAMETER]

The current condition of the use SRQ MAV flag.

    �Ho���  �    Status                            �) "  �  �    Instrument  Handle                �� D � �       Level                              	           0    	            �    This function will perform a SYSTem:SYNChronized? to ensure that all the existing commands in the Test Set's GPIB buffer have been acted upon.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The maximum time out in seconds to wait for the existing information in the Test Set's GPIB buffer to be acted upon.


age1960_MAV_TIMEOUT_MIN    1
age1960_MAV_TIMEOUT_MAX    30
    �+o���  �    Status                            � "  �  �    Instrument  Handle                �l ^ � �      Time Out                           	           0                           This function will perform a SETup:AAUDio for the given parameters. The :CONTinuous state will be set to OFF (Trigger Arm = Single) by this function. The :COUNt:STATe will be set to the ON condition if the AAudioMeasCount value is >0. Otherwise, this state will be set of OFF. The :TIMeout:STATe will be set to the ON condition. 

This function supports earlier versions of the E1960A TA. Greater functionality can be achieved using the age1960_confAFAN() function if your version of the E1960A TA will support it (A.07xx or later). 
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9

     �    [INPUT PARAMETER]

The number of multi measurements to be made.  If this value is set to zero then the multi measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     /    [INPUT PARAMETER]

The bandpass filter state.     �    [INPUT PARAMETER]

The expected voltage level for the signal in volts peak.
 
 age1960_EXP_VOLT_MIN     0.01
 age1960_EXP_VOLT_MIN     20.0     �    [INPUT PARAMETER]

The desired bandpass filter frequency in Hz.
 
 age1960_AAUDFLT_FREQ_MIN     200
 age1960_AAUDFLT_FREQ_MAX    8000
    �"m���  �    Status                            "  �  �    Instrument Handle                a �F �      AAudio Max Time                  H > < �      AAudio Meas Count                ' � � �       AAudio Filt State                ^ > � �      AAudio Exp Volt                  � >� �      AAudio Filt Freq                   	           0 ?�      @�?33333?�������@                     �                   On VI_TRUE Off VI_FALSE ?�      @4      ?�z�G�{@4                ?�      @�@     @i      @�@                 �    This function will perform a SETup:AAUDio for the given parameters. The :CONTinuous state will be set to OFF (Trigger Arm = Single) by this function. The :COUNt:STATe will be set to the ON condition if the AFANMeasCount value is >0. Otherwise, this state will be set to OFF. The :TIMeout:STATe will be set to the ON condition. 

Note: This function adds functionality that was made available in version A.07.xx of the E1960A TA. If backward compatibility is required, use the age1960_confAAudio() function.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1961_error_message() function to get a textual description of errors and warnings.
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1961_init().     �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

age1960_MEAS_TIMEOUT_MIN     1
age1960_MEAS_TIMEOUT_MAX     999.9

     �    [INPUT PARAMETER]

The number of multi-measurements to be made. If this value is set to zero then the multi-measurement function will be turned off.

age1960_MEAS_COUNT_MIN     0
age1960_MEAS_COUNT_MAX     999
     E    [INPUT PARAMETER]

Set the SINAD and distortion state to On or Off.     �    [INPUT PARAMETER]

The expected voltage level for the signal in volts peak.
 
age1960_EXP_VOLT_MIN      0.001
age1960_EXP_VOLT_MAX     20.0     ?    [INPUT PARAMETER]

Set the de-emphasis state to On or Off.        �    [INPUT PARAMETER]

The fundamental frequency for the SINAD and Distortion measurement.

age1960_FUND_FREQ_MIN       100.0
age1960_FUND_FREQ_MAX     10000.0
        [INPUT PARAMETER]

The filter type to be used.

age1960_FILTER_NONE        0    None
age1960_FILTER_BANDPASS    1    Tunable Band Pass
age1960_FILTER_CMESSAGE    2    C_Message
age1960_FILTER_50_15KHZ    3    Band Pass 50 - 15kHz
age1960_FILTER_300_15KHZ   4    Band Pass 300 - 15kHz
     �    [INPUT PARAMETER]

The center frequency for the tunable bandpass filter if it is on.

age1960_BPF_FREQ_MIN            200.0
age1960_BPF_AFAN_FREQ_MAX     20000.0     z    [INPUT PARAMETER]

The detector type to be used.  

age1960_DET_RMS     0    RMS
age1960_DET_PPEAK   1    Positive PEAK
     �    [INPUT PARAMETER]

The OFF will be set via a level of 0.  Other levels will set to the desired level. 

age1960_EXPANDOR_LVL_MIN      0.0
age1960_EXPANDOR_LVL_MAX     10.0   	�m���  �    Status                           � "  �  �    Instrument Handle                � �� 
�      AFAN Max Time                    � ' , �      AFAN Meas Count                  � & � �       AFAN Sinad Dist                  � t� �      AFAN Peak Volt                   y � ? �       AFAN Deemphasis                  � ' �      AFAN Fund Freq                   f 'w � �    AFAN Filter Type                 � t , �      AFAN BPF Cnt Freq                7 t � �      AFAN Det Type                    � � � 	�      AFAN Expandor Level                	           0 ?�      @�?33333?�      @                     �                   On VI_TRUE Off VI_FALSE ?�      @4      ?PbM���@4                   On VI_TRUE Off VI_FALSE @Y      @È     @Y      @�@                             �None age1960_FILTER_NONE Tunable Band Pass age1960_FILTER_BANDPASS C_Message age1960_FILTER_CMESSAGE Band Pass 50 - 15kHz age1960_FILTER_50_15KHZ Band Pass 300 - 15kHz age1960_FILTER_300_15KHZ @Y      @ӈ     @i      @�@                             4RMS age1960_DET_RMS Positive PEAK age1960_DET_PPEAK ?�      @$                                   �    This function will perform a SETup:BERRor for the given parameters. The :CONTinuous state will be set to OFF (single) by this function. The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     {    [INPUT PARAMETER]

The number of bits to be measured.

 age1960_BER_COUNT_MIN         1
 age1960_BER_COUNT_MAX    999000
    J    [INPUT PARAMETER]

The number of frames to delay for the uplink and downlink to correlate. This value will only have an effect if the BERAutoDelay is set to Off i.e. use manual delay.  If BERAutoDelay is On, the number of frames to delay is determined automatically.

 age1960_BER_DELAY_MIN      1
 age1960_BER_DELAY_MAX     15
     z    [INPUT PARAMETER]

Selects whether or not to automatically detect the loopback delay or to use the user specified delay.    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN      0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
    B    [INPUT PARAMETER]

The type of bit error rate measurement to be made.


age1960_BER_TYPE_IA      0   Type IA
age1960_BER_TYPE_II      1   Type II
age1960_BER_TYPE_IB      2   Type IB
age1960_BER_RES_TYPE_IA  3   Residual Type IA
age1960_BER_RES_TYPE_II  4   Residual Type II
age1960_BER_RES_TYPE_IB  5   Residual Type IB     m    [INPUT PARAMETER]

Selects whether or not to automatically deselect loopback before signaling is performed.     �    [INPUT PARAMETER]

The time in seconds that the instrument will wait after signaling before the Bit Error Rate measurement is started. Zero (0) will turn this feature off.

 age1960_HOLDOFF_TIME_MIN      0
 age1960_HOLDOFF_TIME_MAX      5   �g���  �    Status                           � ,  �  �    Instrument Handle                6 : ; �      BER Meas Count                   � 9� �      BER Manual Delay                  � K �       BER Auto Delay                   � � 8 �      BER Max Time                     u ; � �      BER Meas Type                    � � � �       BER Signalling Control            4 �� �      BER Holdoff Time                   	           0     >X     '                              On VI_TRUE Off VI_FALSE ?�      @�?33333?�������@                             �Type IA  age1960_BER_TYPE_IA Type II age1960_BER_TYPE_II Type IB age1960_BER_TYPE_IB Residual Type IA age1960_BER_RES_TYPE_IA Residual Type II age1960_BER_RES_TYPE_II Residual Type IB age1960_BER_RES_TYPE_IB   On VI_TRUE Off VI_FALSE ?�      @              ?�                  ;    This function will perform a SETup:DAUDio for the given parameters.  The :CONTinuous state will be set to OFF (single) by this function. The :COUNt:STATe will be set to the ON condition if the DAudioMeasCount value is >0 otherwise this state will be set of OFF. The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi-measurements to be made. If it is set to zero, then the multi-measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     :    [INPUT PARAMETER]

Set the bandpass filter to On or Off.     �    [INPUT PARAMETER]

The desired bandpass filter frequency in Hz.
 
 age1960_DAUDFLT_FREQ_MIN     200
 age1960_DAUDFLT_FREQ_MAX    3600
    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN      0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
   %�m���  �    Status                           '� "  �  �    Instrument Handle                (5 2 > �      DAudio Meas Count                ) -� �       DAudio Filt State                )N 2 � �      DAudio Filt Freq                 )� � = �      DAudio Max Time                    	           0      �                   On VI_TRUE Off VI_FALSE ?�      @�      @i      @�@               ?�      @�?33333?�������@                      The Dynamic Power measurement performs a series of consecutive power measurements on a mobile station returning a power measurement and an integrity value for each burst measured. Dynamic Power is only available via the test set's remote user interface. Dynamic Power is not an ETSI specified measurement.
The signal is measured at the RF IN/OUT port. 

This function will perform a SETup:DPOWer for the given parameters. The :CONTinuous state will be set to OFF (single) by this function. The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     u    [INPUT PARAMETER]

The number of bits to be measured.


age1960_DPOW_COUNT_MIN    1
age1960_DPOW_COUNT_MAX    999

    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
        [INPUT PARAMETER]

Sets the Expected Maximum Difference from Previous Measurement parameter for the Dynamic Power measurement. The units dB are optional.

The Expected Maximum Difference from Previous Measurement parameter is used with the measured transmit power from the previous burst to set the maximum RF power that the base station emulator is expecting the mobile to transmit in the next burst. Negative numbers then effectively means the it is a step down in power.

Range is from -30 to +30 dB, Resolution 0.01dB
   /�m���  �    Status                           1� "  �  �    Instrument Handle                2 L 9 �      Dyn Power Meas Count             2� K� �      Dyn Power Max Time               4f K � �      Dyn Power Exp Max Diff             	           0      �              ?�      @�?33333?�������@                ?�      @>      �>      @                   �    This function will perform a SETup:FBERror for the given parameters. The :CONTinuous state will be set to OFF (single) by this function. The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     }    [INPUT PARAMETER]

The number of bits to be measured.

 age1960_FBER_COUNT_MIN         1
 age1960_FBER_COUNT_MAX    999000
     �    [INPUT PARAMETER]

The number of frames to wait for the uplink and downlink to correlate. This value will only have an effect if the FBERAutoDelay is set to false i.e. use manual delay.

 age1960_FBER_DELAY_MIN      0
 age1960_FBER_DELAY_MAX     26
     z    [INPUT PARAMETER]

Selects whether or not to automatically detect the loopback delay or to use the user specified delay.    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
     m    [INPUT PARAMETER]

Selects whether or not to automatically deselect loopback before signaling is performed.     �    [INPUT PARAMETER]

The time in seconds that the instrument will wait after signaling before the Bit Error Rate measurement is started.  Zero (0) will turn this feature off.

 age1960_HOLDOFF_TIME_MIN      0
 age1960_HOLDOFF_TIME_MAX      5   8�m���  �    Status                           :� "  �  �    Instrument Handle                ; 6 ? �      FBER Meas Count                  ;� 6 � �      FBER Manual Delay                <� 1� �       FBER Auto Delay                  = � �      FBER Max Time                    >� z R �       FBER Signalling Control          ?k  � �      FBER Holdoff Time                  	           0     >X     '                                On VI_TRUE Off VI_FALSE ?�      @�?33333?�������@                  On VI_TRUE Off VI_FALSE ?�      @              ?�                  ;    This function will perform a SETup:IQT for the given parameters.  The :CONTinuous state will be set to OFF (single) by this function.  The :COUNt:STATe will be set to the ON condition if the TXPowerMeasCount value is >0 otherwise this state will be set of OFF.  The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi-measurements to be made. If it is set to zero then the multi-measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     �    [INPUT PARAMETER]

The type of trigger source.

age1960_TRIG_RF_RISE     2    RF Rise
age1960_TRIG_IMMEDIATE   3    Immediate
     �    [INPUT PARAMETER]

The trigger delay time in seconds.

 age1960_TRIG_DELAY_MIN    -0.00231
 age1960_TRIG_DELAY_MAX    +0.00231
     H    [INPUT PARAMETER]

This sets the spur frequency measurement on or off.    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
     �    [INPUT PARAMETER]

This sets the IQT reference offset frequency in which the dB levels returned are relative to.


age1960_IQ_REF_NEG67K    -67 kHz
age1960_IQ_REF_ZERO67K     0 kHz
age1960_IQ_REF_PLUS67K   +67 kHz
age1960_IQ_REF_AUTO        Auto
     �    [INPUT PARAMETER]

This sets the IQT spur offset frequency from the carrier in Hz. The range can be from -13 to -1 MHz or +1 to 13 MHz.


age1960_IQT_SPURFREQ_MIN_1    -13E06
age1960_IQT_SPURFREQ_MAX_2    +13E06
   Dm���  �    Status                           E� "  �  �    Instrument Handle                FY ( 5 �      IQT Meas Count                   G/ ( � �      IQT Trig Source                  G� (� �      IQT Trig Delay                   H@ w �       IQT Spur Freq State              H� � 8 �      IQT Max Time                     Jv x 5 �      IQT Ref Offset Frequency         Kv w� �      IQT Spur Frequency (Hz)            	           0      �                              >RF Rise age1960_TRIG_RF_RISE Immediate age1960_TRIG_IMMEDIATE ?6��C-?b�k΅3��b�k΅3�                     On VI_TRUE Off VI_FALSE ?�      @�?33333?�������@                             q-67 kHz age1960_IQ_REF_NEG67K 0 kHz age1960_IQ_REF_ZEROK +67 kHz age1960_IQ_REF_PLUS67K Auto age1960_IQ_REF_AUTO A.��    Ah˨    �h˨    Ac�                    This function will perform a SETup:ORFSpectrum for the given parameters.  The :CONTinuous state will be set to OFF (single) by this function.  The :COUNt:STATe will be set to the ON condition if the ORFSModMeasCount value or the ORFSSwMeasCount value is >0.  If the ORFSModMeasCount value and the ORFSSwCount are both 0, the :COUNt:STATe will be set to OFF.  If only one of the ORFSModMeasCount or ORFSSwMeasCount values is 0, an error message will be generated.  The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi measurements to be made for switching.  If it is set to zero then the multi measurement function will be turned off. 


age1960_MEAS_COUNT_MIN     0
age1960_MEAS_COUNT_MAX     999

     �    [INPUT PARAMETER]

The number of multi-measurements to be made for modulation. If it is set to zero then the multi-measurement function will be turned off.      �    [INPUT PARAMETER]

The number switching frequency points to be set that are contained in the array ORFSModFreqs. If it is set to zero, no switching points will be measured.

 age1960_ORFSSW_POINTS_MIN      0
 age1960_ORFSSW_POINTS_MAX      8     �    [INPUT PARAMETER]

The number modulation frequency points to be set that are contained in the array ORFSModFreqs. If it is set to zero, no modulation points will be measured.

 age1960_ORFSMOD_POINTS_MIN      0
 age1960_ORFSMOD_POINTS_MAX     22        [INPUT PARAMETER]

The array of switching frequency offset points to be test in Hz.

MIN 1   age1960_ORFSFREQ_MIN_1   -1800000.0
MAX 1   age1960_ORFSFREQ_MAX_1        -10.0
MIN 2   age1960_ORFSFREQ_MIN_2         10.0
MAX 2   age1960_ORFSFREQ_MAX_2    1800000.0
        [INPUT PARAMETER]

The array of modulation frequency offset points to be test in Hz.

MIN 1 = age1960_ORFSFREQ_MIN_1   -1800000.0
MAX 1 = age1960_ORFSFREQ_MAX_1        -10.0
MIN 2 = age1960_ORFSFREQ_MIN_2         10.0
MAX 2 = age1960_ORFSFREQ_MAX_2    1800000.0
     �    [INPUT PARAMETER]

The type of trigger source.

age1960_TRIG_AUTO        0    Auto
age1960_TRIG_PROT        1    Protocol
age1960_TRIG_RF_RISE     2    RF Rise
age1960_TRIG_IMMEDIATE   3    Immediate
     �    [INPUT PARAMETER]

The trigger delay time in seconds.

 age1960_TRIG_DELAY_MIN    -0.00231
 age1960_TRIG_DELAY_MAX    +0.00231
    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN      0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
   Q�v���  �    Status                           S� "  �  �    Instrument Handle                T 6 ? �      ORFS Sw Meas Count               T� 6 � �      ORFS Mod Meas Count              U� � � �      ORFS Sw Points                   V� � A �      ORFS Mod Points                  W� �� �       ORFS Sw Freqs                    X� � � �       ORFS Mod Freqs                   Y� 6� �      ORFS Trig Source                 Z� � @ �      ORFS Trig Delay                  [ �� 	�      ORFS Max Time                      	           0      �                     �                                                                                pAuto age1960_TRIG_AUTO Protocol age1960_TRIG_PROT RF Rise age1960_TRIG_RF_RISE Immediate age1960_TRIG_IMMEDIATE ?6��C-?b�k΅3��b�k΅3�                  ?�      @�?33333?�������@                  �    This function configures the Output RF Spectrum measurement Limit Source field. When set to ETSI the limit mask values are not user settable and the default limit values are used. When set to Manual, the the Modulation Limits and Switching Limits values used to determine whether or not a measurement passed are user settable for each offset. To set the user defined values, use the functions age1964_confORFSSwitchLimits() and age1964_confORFSModLimits().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1964_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1964_init().
     .    [INPUT PARAMETER]

The limit source setting.   bZm���  �    Status                           d9   �  �    Instrument Handle                d� G � �      Limit Source                       	           0               ;ETSI age1960_ORFS_LIMIT_ETSI Manual age1960_ORFS_LIMIT_MAN   �    This function configures the Output RF Spectrum measurement Switching Limits pass/fail mask. The limit values are setup for the individual measurement band selected, or for the current measurement band. Note: the limit values set by this function will only be used when the ORFS measurement Limit Source setting is set to Manual (use the function age1960_ORFSLimitSource() for changing Limit Source field setting).
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of switching Limit points to be set that are contained in the array SwitchingLimitsArray. 

age1960_ORFSSW_LIM_POINTS_MIN      1
age1960_ORFSSW_LIM_POINTS_MAX      8
        [INPUT PARAMETER]

The array of switching limit values in dBm. The array must be diminsioned with at least as many elements as offset points you wish to pass. Note: if you send less than all 8 possible points , only those values for offset points sent will be modified. Example: If you wish to setup 2 offset points, select the band you wish the values to apply to, set Switching Offset Points to 2, have an array diminsioned with at least 2 elements, and assign array values for the first 2 elements of the array. When sent the first 2 switching offsets will change. The current values for offsets points 3 to 8 will not be altered.

The array values are checked against the following limits:
age1960_ORFSSW_LIMIT_MIN       -200.0
age1960_ORFSSW_LIMIT_MAX        100.0
     �    [INPUT PARAMETER]

The cell band for which the Switching limits are applied. If current band is selected, the limits set for the cell band the instrument is presently configured for are used.
   grm���  �    Status                           iQ 0  �  �    Instrument Handle                i� [ � �      Switching Offset Points          j [} �       Switching Limits Array           m� [ 5 �      Measurement Band                   	           0                                       �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND   �    This function configures the Output RF Spectrum measurement Modulation Limits pass/fail mask. The limit values are setup for the individual measurement band selected, or for the current measurement band. Note: the limit values set by this function will only be used when the ORFS measurement Limit Source setting is set to Manual (use the function age1960_ORFSLimitSource() for changing Limit Source field setting).
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of modulation limit points to be set that are contained in the array ModulationLimitsArray. 

age1960_ORFSMOD_LIM_POINTS_MIN       1
age1960_ORFSMOD_LIM_POINTS_MAX      22
    
    [INPUT PARAMETER]

The array of modulation limit values in dB. The array must be diminsioned with at least as many elements as offset points you wish to pass. Note: if you send less than all 22 possible points , only those values for offset points sent will be modified. Example: If you wish to setup 2 offset points, select the band you wish the values to apply to, set Modulation Offset Points to 2, have an array diminsioned with at least 2 elements, and assign array values for the first 2 elements of the array. When sent the first 2 modulation offsets will change. The current values for offsets points 3 to 22 will not be altered.

The array values are checked against the following limits:
age1960_ORFSMOD_LIMIT_MIN       -200.0
age1960_ORFSMOD_LIMIT_MAX        100.0
     �    [INPUT PARAMETER]

The cell band for which the Modulation limits are applied. If current band is selected, the limits set for the cell band the instrument is presently configured for are used.
   rm���  �    Status                           s� +  �  �    Instrument Handle                tC U � �      Modulation Offset Points         u U� �       Modulation Limits Array          x) U - �      Measurement Band                   	           0                                       �Current Band age1960_CURRENT_BAND PGSM Band age1960_PGSM_BAND EGSM Band age1960_EGSM_BAND DCS 1800 Band age1960_DCS1800_BAND PCS 1900 Band age1960_PCS1900_BAND GSM 850 Band age1960_GSM850_BAND   =    This function will perform a SETup:PFERror for the given parameters.  The :CONTinuous state will be set to OFF (single) by this function.  The :COUNt:STATe will be set to the ON condition if the PFERMeasCount  value is >0 otherwise this state will be set of OFF.  The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi-measurements to be made. If it is set to zero then the multi-measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     �    [INPUT PARAMETER]

The type of trigger source.

age1960_TRIG_AUTO        0    Auto
age1960_TRIG_PROT        1    Protocol
age1960_TRIG_RF_RISE     2    RF Rise
age1960_TRIG_IMMEDIATE   3    Immediate
     �    [INPUT PARAMETER]

The trigger delay time in seconds.

 age1960_TRIG_DELAY_MIN    -0.00231
 age1960_TRIG_DELAY_MAX    +0.00231
     �    [INPUT PARAMETER]

The type of burst synchronization.

age1960_BSYNC_MID    0    Midamble
age1960_BSYNC_AMPL   1    Amplitude
age1960_BSYNC_NONE   2    None
    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
     +    [INPUT PARAMETER]

The trigger qualifier.   |>m���  �    Status                           ~ "  �  �    Instrument Handle                ~} " 8 �      PFER Meas Count                  S " � �      PFER Trig Source                 �% "� �      PFER Trig Delay                  �� p � �      PFER Sync                        �U t� �      PFER Max Time                    �; l D �       PFER Trig Qual                     	           0      �                              pAuto age1960_TRIG_AUTO Protocol age1960_TRIG_PROT RF Rise age1960_TRIG_RF_RISE Immediate age1960_TRIG_IMMEDIATE ?6��C-?b�k΅3��b�k΅3�                                PMidamble age1960_BSYNC_MID Amplitude age1960_BSYNC_AMPL None age1960_BSYNC_NONE ?�      @�?33333?�������@                   On VI_TRUE Off VI_FALSE    �    This function will perform a SETup:PFERror for the Confidence Intervals for Fast Termination (CIFT) parameters.  CIFT uses statistical means to determine whether a test will pass or fail without having to complete the whole test.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Selects Confidence Intervals for Fast Termination (CIFT) state.

When set to the On state, the PFER multi measurement count state will also be set to On.
     �    [INPUT PARAMETER]

Selects the required confidence level (%).

 age1960_CIFT_CONF_LEVEL_MIN    80.0
 age1960_CIFT_CONF_LEVEL_MAX    99.9
     �    [INPUT PARAMETER]

The peak phase error maximum limit (deg).

 age1960_PFER_PEAK_PHASE_LIMIT_MIN     0.0
 age1960_PFER_PEAK_PHASE_LIMIT_MAX    20.0     �    [INPUT PARAMETER]

The RMS phase error maximum limit (deg).

 age1960_PFER_RMS_PHASE_LIMIT_MIN    0.0
 age1960_PFER_RMS_PHASE_LIMIT_MAX    5.0
     �    [INPUT PARAMETER]

The frequency error maximum limit (ppm).

 age1960_PFER_FREQ_ERROR_LIMIT_MIN     0.01
 age1960_PFER_FREQ_ERROR_LIMIT_MAX     0.10
   �}m���  �    Status                           �\ "  �  �    Instrument Handle                �� < � �       PFER CIFT State                  �s A. �      PFER Confidence Level            � � = �      PFER Peak Phase Error Max Limit  �� � � �      PFER RMS Phase Error Max Limit   �< �� �      PFER Frequency Error Max Limit     	           0    On VI_TRUE Off VI_FALSE ?�      @X�\(�@T      @X�               ?�������@4              @4                ?�������@              @                ?�z�G�{?�������?�z�G�{?�������            >    This function will perform a SETup:PVTime for the given parameters.  The :CONTinuous state will be set to OFF (single) by this function.  The :COUNt:STATe will be set to the ON condition if the pvsTimeMeasCount value is >0 otherwise this state will be set of OFF.  The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi measurements to be made.  If it is set to zero then the multi measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     �    [INPUT PARAMETER]

The type of trigger source.


age1960_TRIG_AUTO        0    Auto
age1960_TRIG_PROT        1    Protocol
age1960_TRIG_RF_RISE     2    RF Rise
age1960_TRIG_IMMEDIATE   3    Immediate
     �    [INPUT PARAMETER]

The trigger delay time in seconds.

 age1960_TRIG_DELAY_MIN    -0.00231
 age1960_TRIG_DELAY_MAX    +0.00231
     �    [INPUT PARAMETER]

The type of burst synchronization.


age1960_BSYNC_MID    0   Midamble
age1960_BSYNC_AMPL   1   Amplitude
age1960_BSYNC_NONE   2   None
     �    [INPUT PARAMETER]

The number offset points to be set that are contained in the array pvsTimeOffsets.

 age1960_PVT_POINTS_MIN      0
 age1960_PVT_POINTS_MAX     12     �    [INPUT PARAMETER]

The array of power vs time offset points to be test in seconds.

 age1960_PVT_OFFSET_MIN   -0.000050
 age1960_PVT_OFFSET_MAX   +0.000593
    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
   �lm���  �    Status                           �K   �  �    Instrument Handle                �� ' > �      P vs Time Meas Count             �� ( � �      P vs Time Trig Source            �U %� �      P vs Time Trig Delay             �� x $ �      P vs Time Sync                   �� v � �      P vs Time Points                 �1 v� �       P vs Time Offsets                �� � ? �      P vs Time Max Time                 	           0      �                              pAuto age1960_TRIG_AUTO Protocol age1960_TRIG_PROT RF Rise age1960_TRIG_RF_RISE Immediate age1960_TRIG_IMMEDIATE ?6��C-?b�k΅3��b�k΅3�                                PMidamble age1960_BSYNC_MID Amplitude age1960_BSYNC_AMPL None age1960_BSYNC_NONE                          ?�      @�?33333?�������@                      This function is used to set the Power Vs. Time measurement PCS ETSI Limit field. The field is only available on the Power Vs Time measurement setup screen when the instrument cell band is PCS, and only affects measurements when the PCS band is selected. This function is provided because there is an inconsistency in the GSM specifications that determine the mask specification for the PCS band. The inconsistency centers around the -18 to -10us limit for the PCS band. In one specification the limit is -6dB, in another specification the limit ranges from -6dB to -1dB depending on the transmitted power level. The current limit in the PvT measurement is -6dB. In order to allow users to test vs the other specification, an option is provided that allows the selection of this relaxed mask specification. 

The "Narrow" setting chooses an upper limit of -6dB for the -18 to -10us section.

The "Relaxed" setting chooses a limit of -6dB or -4dB (Tx Level 11), -2dB (Tx Level 12), -1dB (Tx Level 13, 14 and 15) for the -18 to -10us section.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     5    [INPUT PARAMETER]

The PCS ETSI Limit mask setting.   �+r���  �    Status                           �
 "  �  �    Instrument Handle                �j D � �      PVT Mask For PCS                   	           0               @Narrow age1960_PVT_MASK_NARROW Relaxed age1960_PVT_MASK_RELAXED   =    This function will perform a SETup:TXPower for the given parameters. The :CONTinuous state will be set to OFF (single) by this function.  The :COUNt:STATe will be set to the ON condition if the TXPowerMeasCount value is >0 otherwise this state will be set of OFF. The :TIMeout:STATe will be set to the ON condition.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of multi-measurements to be made. If it is set to zero then the multi-measurement function will be turned off.

 age1960_MEAS_COUNT_MIN      0
 age1960_MEAS_COUNT_MAX    999
     �    [INPUT PARAMETER]

The type of trigger source.

age1960_TRIG_AUTO        0    Auto
age1960_TRIG_PROT        1    Protocol
age1960_TRIG_RF_RISE     2    RF Rise
age1960_TRIG_IMMEDIATE   3    Immediate
     �    [INPUT PARAMETER]

The trigger delay time in seconds.

 age1960_TRIG_DELAY_MIN    -0.00231
 age1960_TRIG_DELAY_MAX    +0.00231
     +    [INPUT PARAMETER]

The trigger qualifier.    �    [INPUT PARAMETER]

The maximum time in seconds that the instrument will wait for the measurement to complete.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
   ��m���  �    Status                           �� "  �  �    Instrument Handle                �* ( 5 �      TX Power Meas Count              �  ( � �      TX Power Trig Source             �� (� �      TX Power Trig Delay              �[ � ? �       TX Power Trig Qual               �� � � �      TX Power Max Time                  	           0      �                              pAuto age1960_TRIG_AUTO Protocol age1960_TRIG_PROT RF Rise age1960_TRIG_RF_RISE Immediate age1960_TRIG_IMMEDIATE ?6��C-?b�k΅3��b�k΅3�                     On VI_TRUE Off VI_FALSE ?�      @�?33333?�������@                      This function will perform a READ:TXPower? and set the expected power setting to the returned TX power value until the integrity indicates no overrange or no underrange conditions. For the analog analyzer range this function will set the expected voltage to 2x the current reading for an overrange condition and .5 the reading for an underrange condition and repeat this action until the range is correct or the try counter has been passed.

WARNING!  The receiver may be overranged or underranged due to the combined expected power setting and the amplitude offset for the current frequency setting.  Turning on the automatic logging using age1960_errorQueryDetect() will alert you to this problem. Overranging or underranging the receiver may result in measurements being unavailable or invalid.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of times to try to set the range correctly. 
   
 age1960_AUTORANGE_TRIES_MIN    1
 age1960_AUTORANGE_TRIES_MAX  100     �    [INPUT PARAMETER]

The measurement function to use for autoranging.


age1960_AUTORNG_AFANALYZER  0  Audio Frequency Analyzer Meas Fcn
age1960_AUTORNG_TXPOWER     1  TX Power Meas Fcn
   �&l���  �    Status                           � 6  �  �    Instrument Handle                �e =� �      Tries                            � = L � �    Meas Fcn To Use                    	           0       d                           hAudio Frequency Analyzer Meas Fcn age1960_AUTORNG_AFANALYZER TX Power Meas Fcn  age1960_AUTORNG_TXPOWER   �    This function will set the measurement timeout value.  Note: that this timeout value can also be set using the age1960_conf< measurement function> driver.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [INPUT PARAMETER]

The timeout value in seconds for the desired measurement function.

If using A.03.04 firmware the minimum timeout that can be set is 1 second. Also the resolution is 1 second. The instrument will round the timeout value sent, but will beep at you if it is set between 0.1s and less than 0.5s. 
A.04.01 or higher will allow settings of 0.1 to 999.9s in 0.1s steps.

 age1960_MEAS_TIMEOUT_MIN    0.1
 age1960_MEAS_TIMEOUT_MAX    999.9
    n    [INPUT PARAMETER]

Set to On if the measurement function's timeout value should be used. If Off, the measurement function will never timeout from the instrument, it will timeout from the driver timeout value if the measurement function does not complete.  Note: It is best to always use this timeout set to On, so the measurement function will handle the timeout.
    �    [INPUT PARAMETER]

The measurement function to set the timeout value for.


age1960_AAUDIO    0    Analog Audio
age1960_BER       1    Bit Error
age1960_DAUDIO    2    Decoded Audio
age1960_FBER      3    Fast Bit Error
age1960_ORFS      4    Output RF Spectrum
age1960_PFER      5    Phase Freq Error
age1960_PVTIME    6    Power vs Time
age1960_TXPOWER   7    TX Power
age1960_DPOWER    8    Dynamic Power
age1960_IQTUNING  9    IQ Tuning
   �s���  �    Status                           ��   �  �    Instrument Handle                �B 1* �      Timeout Sec                      � ,� �       Timeout On Off                   �� 1  � �    Timeout Meas Fcn                   	           0 ?�      @�?33333?�������?�                  On VI_TRUE Off VI_FALSE            
  Analog Audio age1960_AAUDIO Bit Error age1960_BER Decoded Audio age1960_DAUDIO Fast Bit Error age1960_FBER Output RF Spectrum age1960_ORFS Phase Freq Error age1960_PFER Power vs Time age1960_PVTIME TX Power age1960_TXPOWER Dyn Power age1960_DPOWER IQ Tuning age1960_IQTUNING    �    This function performs a READ:AAUDio and returns the following measurement results:

1) Measurement Integrity
2) Analog Audio     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [OUTPUT PARAMETER]

The integrity response from the instrument. This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
13 = Unidentified Error
     Q    [OUTPUT PARAMETER]

The analog audio response in volts RMS from the instrument.   �Ss���  �    Status                           �2 "  �  �    Instrument Handle                Ē A L �       AAudio Integrity                 ŵ @� �       AAudio Result                      	           0    	            	          �    This function will perform a READ:AAUDio and return:

1) Measurement Integrity 
2) Average Audio Level 
3) Average SINAD 
4) Average Distortion (if available).

Invalid measurement results will result in a value of 9.91 E+37. 

Note: This function adds functionality that was not available in earlier versions of the E1960A TA. If backward compatibility is required, use the age1960_readAAudio() function.     �    [INPUT PARAMETER]

Set to True if this functions should try to auto range the instrument by resetting the AFAN expected peak voltage given the current reading for AFAN level.         [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
13 = Unidentified Error
     a    [OUTPUT PARAMETER]

The returned value from the instrument for the average audio level result.
     ^    [OUTPUT PARAMETER]

The returned value from the instrument for the average SINAD result.         a    [OUTPUT PARAMETER]

The returned value from the instrument for average distortion result.     
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1961_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1961_error_message function to get a textual description of errors and warnings.
   Ȫ ! �       AFAN Auto Range                  �c e B �       AFAN Integrity                   ʇ eo �  n    AFAN Avg Audio Level             �� � B �       AFAN Avg SINAD                   �V �o �  n    AFAN Avg Dist                    ˿	 '  �  �    Instrument Handle                �[���  �    Status                             On VI_TRUE Off VI_FALSE    	            	           	           	           0    	          	�    This function performs a READ:BERRor:FULL. It is either Residual type  or non residual depending on what is set by the Configure Bit Error. In the results returned below it just mentions the type, however if one of the residual types is set then all the residual types are returned instead on of the non-residual.
The following measurement results are returned:

1) Measurement Integrity
2) Type Ia Bits Tested
3) Type Ia Bit Error Ratio
4) Type Ia Bit Error Count
5) Type Ib Bits Tested
6) Type Ib Bit Error Ratio
7) Type Ib Bit Error Count
8) Type II Bits Tested
9) Type II Bit Error Ratio
10) Type II Bit Error Count

It will also turn off all measurement functions that cannot run           concurrently with BER.

The RX BER Meas is used to measure the performance of the receiver of the MS. A downlink TCH with PRBS data is generated at a known level, and the MS receives it, loops it back to its transmitter, and returns the PRBS data to the BS. A comparison is then made in the BS between the data sent and the data received, and the BER is calculated.
There are 3 commonly used types of bit error measurement in GSM:

1) BER using MS loopback with signaling of erased speech frames. In this case the comparison is made between the required class of bits within the 260 bits of speech data presented to the channel coder of the BS and the 260 bits of speech data received by the channel decoder of the BS. The mobile will indicate in the speech frame if the downlink frame was received and was found to have CRC errors which would cause the speech frame to be erased. The mobile indicates this on the uplink by setting all bits in the uplink speech frame to 0.

Referred to from now on as "BER with FE" or �Residual BER� (loopback type A)

2) BER using MS loopback without signaling of erased speech frames. In this case the comparison is made between the required class of bits within the 260 bits of speech data presented to the channel coder of the BS and the 260 bits of speech data received by the channel decoder of the BS. The mobile does not provide any indication of whether or not the downlink speech frame was erased.

Referred to from now on as "BER without FE" or "Non-Residual BER" (loopback type B);

3) BER using MS burst-by-burst loopback. In this case the comparison is made between the 114 bits of interleaved data presented to layer 1 in the BS and the 114 bits of interleaved data received by layer 1 in the BS.

Referred to from now on as "Fast Bit Errors" (loopback type C).    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout        [OUTPUT PARAMETER]

This result represents the number of type Ia (residual or non-residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The type Ia (residual or non-residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The type Ia (residual or non residual set in configure Bit Error) bit error count response from the instrument.
        [OUTPUT PARAMETER]

This result represents the number of  type Ib (residual or non residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The type Ib (residual or non residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The  type Ib (residual or non residual set in configure Bit Error) bit error count response from the instrument.
        [OUTPUT PARAMETER]

This result represents the number of  type II (residual or non residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The type II (residual or non residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The  type II (residual or non residual set in configure Bit Error) bit error count response from the instrument.
   ٟs���  �    Status                           �~ "  �  �    Instrument Handle                �� *  �       BER Integrity                    ܘ * � �       Type Ia BER Bits Tested          ݴ * �       Type Ia BER Ratio                �L *� �       Type Ia BER Count                �� y Y �       Type Ib BER Bits Tested          �� y � �       Type Ib BER Ratio                �� yg �       Type Ib BER Count                � � Y �       Type II BER Bits Tested          �; � � 	�       Type II BER Ratio                �� �g 
�       Type II BER Count                  	           0    	            	           	           	           	           	           	           	           	           	          y    This function performs a READ:DAUDio and returns the following measurement results:

1) Measurement Integrity
2) Decoded Audio 

It will also turn off all measurement functions that cannot run          concurrently with DAUDIO.If this function turns off BERR or FBER, this function will reset the loopback control to the value that was set via the age1960_loopback function.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = PCM Full Scale Warning
13 = Unidentified Error
     \    [OUTPUT PARAMETER]

The decoded audio response in percent full scale from the instrument.
   ��s���  �    Status                           �� "  �  �    Instrument Handle                �' 9 > �       DAudio Integrity                 �F 9� �       DAudio Result                      	           0    	            	          �    This function performs a read of the Dynamic Power results. This function will return the following measurement results:

1) Measurement Integrity an array of N integers representing the integrity of each burst measured. N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function (Dyn Power Meas Count).

2) The Average TX  power of each of the N bursts.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of the dynamic Power result array. The size must be equal to or greater than the number of points defined by SETup:DPOWer:COUNt:NUMBer.

 age1960_DPOW_ARRAY_MIN   1
 age1960_DPOW_ARRAY_MAX   2000    ,    [OUTPUT PARAMETER]

The integrity response array from the instrument. It has N elements, where N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function. Each element returned contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
10 = Signal Too Noisy
11 = Sync Not Found
13 = Unidentified Error
     �    [OUTPUT PARAMETER]

The Dynamic Power Results Array. It has N elements, where N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function.

Range: -100 to +100dBm and 9.91 E+37 (NAN).     P    [OUTPUT PARAMETER]

The number of points measured for the dynamic power array.   �;s���  �    Status                           �    �  �    Instrument Handle                �z 2 � �      Dyn Power Array Size             �_ � . �       Dyn Power Integrity              � � � �       Dyn Power Results Array          �{ �� �       Dyn Power Count                    	           0      �      
           	            	            	               This function performs a READ:FBERror? and returns the following measurement results:

1) Measurement Integrity
2) Fast Bits Tested
3) Fast Bit Error Ratio
4) Fast Bit Error Count

It will also turn off all measurement functions that cannot run           concurrently with FBER.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
15 = Questionable for Phase 1 Mobile
     W    [OUTPUT PARAMETER]

The fast bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

This result represents the number of bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.
     M    [OUTPUT PARAMETER]

The fast bit error count response from the instrument.
   �}
U���  �    Status                           �\ 3  �  �    Instrument Handle                �� . N �       FBER Integrity                   �� � N �       FBER Ratio                       �� .p �       FBER Bits Tested                 �� �p �       FBER Count                         	           0    	            	           	           	          u    This function performs a fetch of the IQ Tuning results. This function will return the following measurement results:

1) Measurement Integrity
2) The the user settable spur frequency.
3) The user settable spur reference level.
4) Measured IQ results
5) Offset Frequency (kHz) at which the IQ results were measured
6) IQ measurement Count (the number of offsets measured)    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

The integrity response from the instrument. This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
10 = Signal Too Noisy
11 = Sync Not Found
13 = Unidentified Error
     {    [INPUT PARAMETER]

Size of the IQ Results and the IQ Freqs arrays.

 age1960_IQ_ARRAY_MIN   9
 age1960_IQ_ARRAY_MAX   500     ,    [OUTPUT PARAMETER]

The IQ Results Array.
     R    [OUTPUT PARAMETER]

The frequencies in kHz that the IQ results were measured at.     R    [OUTPUT PARAMETER]

The number of IQ results measured. This should always be 9.
     i    [OUTPUT PARAMETER]

This sets the spur frequency in Hz at which an additional measurement can be made.
     Q    [OUTPUT PARAMETER]

This is the relative level in dB's at the spur frequency.

   �+g���  �    Status                           
 !  �  �    Instrument Handle                j k F �       IQT Integrity                     . � �      IQT Array Size                   � � F �       IQT Results Array                � � � �       IQT Freqs Array                  # �� �       IQT Tuning Count                 } k � �       IQT Spur Freq                    � k� �       IQT Spur Level                     	           0    	              �   	   
           	            	            	            	           	          q    This function performs a READ:ORFS and returns the following measurement results:

1) Measurement Integrity
2) TX Power
3) 30 kHz Bandwidth Power (if there are points defined by SETup:ORFS:MODulation:FREQuency else this will return NAN)
4) An array of ORFS switching results at the points defined by SETup:ORFS:SWITching:FREQuency
5) An array of ORFS modulation results at the points defined by SETup:ORFS:MODulation:FREQuency
6) The number of points returned in the switching results array
7) The number of points returned in the modulation results array

This is a narrow band measurement of the modulation spectrum of the MS�s transmitter. It is made in two parts, the first being the spectrum due to the GMSK modulation measured over the useful part of the burst, excluding the midamble, and the second being the spectrum due to switching transients, which is measured across the entire time slot, plus and minus approximately 10 bits of margin. The majority of the switching energy measured is contained in the rising and falling edges of the burst.
The measurement can be made on the following types of input signal:
 
1) Normal (TCH) bursts. (the shorter RACH bursts are not supported)

2) GMSK modulated signals without burst modulation (these signals will have no measurable spectrum due to switching transients)
This measurement is made from the RF IN/OUT port on the front panel.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of the ORFS switching result array. The size must be equal to or greater than the number of points defined by SETup:ORFS:SWITching:FREQuency:POINts.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500    Y    [OUTPUT PARAMETER]

The integrity response from the instrument. This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
10 = Signal Too Noisy
12 = Oven Out of Range
13 = Unidentified Error
     O    [OUTPUT PARAMETER]

The ORFS TX power response in watts from the instrument.
     �    [OUTPUT PARAMETER]

The 30 kHz bandwidth power response from the instrument. If 
SETup:ORFS:MODulation:FREQuency:POINTs? returns 0, indicating that no Modulation measurement was made, this value will return  NAN.
    #    [OUTPUT PARAMETER]

The ORFS switching array response from the instrument that represents results measured at the specified SETUP:ORFS:SWITching:FREQuency. If 
SETup:ORFS:SWITching:FREQuency:POINts? returns 0, indicating that no Switching measurement was made, this value will return NAN.
     Q    [OUTPUT PARAMETER]

The number of points measured for the ORFS switching array.     �    [INPUT PARAMETER]

Size of the ORFS modulation result array. The size must be equal to or greater than the number of points defined by SETup:ORFS:MODulation:FREQuency:POINts.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500     R    [OUTPUT PARAMETER]

The number of points measured for the ORFS modulation array.    &    [OUTPUT PARAMETER]

The ORFS modulation array response from the instrument that represents results measured at the specified SETUP:ORFS:MODulation:FREQuency.  If 
SETup:ORFS:MODulation:FREQuency:POINts? returns 0 indicating that no Modulation measurement was made, this value will return NAN
   
s���  �    Status                           � "  �  �    Instrument Handle                L  � �      ORFS Sw Array Size               3 a  �       ORFS Integrity                   � a � �       ORFS TX Power                    � a� �       ORFS BW Power                    � �  �       ORFS Sw Array                    � �* �       ORFS Sw Count                    N c �      ORFS Mod Array Size              7 �� 	�       ORFS Mod Count                   � � � �       ORFS Mod Array                     	           0      �                 	            	           	           	            	              �                 	            	           P    This function performs a READ:PFER and returns the following measurement results:

1) Measurement Integrity
2) Maximum Peak Phase Error
3) Maximum RMS Phase Error
4) Worst Frequency Error

This is a narrow band (<200kHz) measurement of the GMSK modulation quality and frequency accuracy of the MS transmitter, measured across the useful part of the TDMA burst.
This measurement is made from the RF IN/OUT port on the front panel.

The measurement can be made on three types of signals:
1) Normal (TCH) bursts
2) Access (RACH) bursts
3) Non-burst modulated signals
 
In all cases, there must be 0.3GMSK modulated data on the signal. For signals without GMSK modulation, it is only possible to make a valid measurement by offsetting the Expected Frequency by +/- 67.7083 kHz to make the signal look like it is modulated with all zeros or all ones.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
10 = Signal Too Noisy
11 = Sync Not Found
13 = Unidentified Error
     [    [OUTPUT PARAMETER]

The maximum RMS phase error response in degrees from the instrument.
     \    [OUTPUT PARAMETER]

The maximum peak phase error response in degrees from the instrument.
     T    [OUTPUT PARAMETER]

The worst frequency error response in Hz from the instrument.
   �U���  �    Status                           �   �  �    Instrument Handle                0 3 : �       PFER Integrity                    � 3p �       PFER RMS Error                   !< � : �       PFER Peak Error                  !� �p �       PFER Freq Error                    	           0    	            	           	           	          �    This function performs a READ:PVT and returns the following measurement results:

1) Measurement Integrity
2) Power Mask Pass Fail indication (1 = pass 0 = fail)
3) Average Power
4) Peak Plus Power
5) Peak Minus Power
6) An array of power vs time results at the points defined by            SETup:PVT:TIME:POINTs?
7) The number of points returned in the above array

This Power vs Time measurement is a narrow band measurement of the transmitted carrier power of the MS as it varies across the TDMA burst. Included is a broadband Carrier Power Measurement of the average power across the burst which is made from the same physical burst as the trace measurement. This Carrier Power Measurement can be midamble synchronized as defined by GSM 11.10. The measurement can be made on three types of signals:

1) Normal (TCH) bursts

2) Access (RACH) bursts

3) Non-pulsed (CW) signals - (corner case)
   The trace will be flat, but the power will be correct.

This measurement will implement TrigQualPower = On.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
11 = Sync Not Found
12 = Oven Out of Range
13 = Unidentified Error
     �    [INPUT PARAMETER]

Size of the Power vs Time array. The size MUST be equal to or greater than the number of points defined by SETup:PVT:TIME:POINts

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500     i    [OUTPUT PARAMETER]

The average of the power in watts as measured across the useful part of the burst.
     �    [OUTPUT PARAMETER]

The power vs time array response from the instrument that represents results measured at the specified SETUP:PVT:TIME:POINts.
     Q    [OUTPUT PARAMETER]

The number of points measured for the power vs time array.
    S    [OUTPUT PARAMETER]

The power vs time mask pass fail response from the instrument.

0 = results were within the specified mask.
1 = results were NOT within the specified mask.

The pass fail result is based on a point by point comparison with the Power Vs. Time Meas mask. The mask is as defined in "ETSI GSM 05.05 Ver. 4.21.0 Annex B".
   'sr���  �    Status                           )R !  �  �    Instrument Handle                )�  � �       P vs Time Integrity              +\ ' 3 �      P vs Time Array Size             ,* x ) �       P vs Time TX Power               ,� x � �       P vs Time Array                  -7 x� �       P vs Time Count                  -� � �       P vs Time Mask Fail                	           0    	              �                 	           	            	            	          �    This function will perform a FETCH:PVT:MTR? to obtain data for use in plotting power vs time results. This function will return the Integrity, a pass/fail flag, the TX power measurement, the plot start time (usec), the time increment for each point (usec) and the array of 2220 points for the power vs time plot. This function will read the instrument settings for the required parameters for the FETCH:PVT:MTR? query.

Note: This function is only designed to work when the instrument is either in Test operating mode, or Cell operating mode and a call is CONNECTED. If neither of these conditions is met the function will return age1960_INSTR_ERROR_FUNC_NOT_SUPPORTED as the error status.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

The integrity response from the instrument. This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
11 = Sync Not Found
12 = Oven Out of Range
13 = Unidentified Error
         [INPUT PARAMETER]

Size of the Power vs Time Plot array.

 age1960_PVTPLOT_ARRAY_MIN   2220
 age1960_PVTPLOT_ARRAY_MAX   5000     i    [OUTPUT PARAMETER]

The average of the power in watts as measured across the useful part of the burst.
     �    [OUTPUT PARAMETER]

The power vs time array response from the instrument that represents results measured at the 2220 offset times.    S    [OUTPUT PARAMETER]

The power vs time mask pass fail response from the instrument.

0 = results were within the specified mask.
1 = results were NOT within the specified mask.

The pass fail result is based on a point by point comparison with the Power Vs. Time Meas mask. The mask is as defined in "ETSI GSM 05.05 Ver. 4.21.0 Annex B".
     ^    [OUTPUT PARAMETER]

The increment time in usec for the points in the P vs Time Plot Array.

     ^    [OUTPUT PARAMETER]

The start time in usec for the first point in the P vs Time Plot Array.
   3�s���  �    Status                           5� '  �  �    Instrument Handle                5� Z B �       P vs Time Plot Integrity         7� " M �      P vs Time Plot Array Size        8" X� �       P vs Time Plot Power             8� �� �       P vs Time Plot Array             9  Y � �       P vs Time Plot Mask Fail         :{ � � �       P vs Time Increment              :� � B �       P vs Time Start Time               	           0    	              �  �  �           	           	            	           	           	              This function performs a READ:TXP and returns the following measurement results:

1) Measurement Integrity
2) TX output power

It will also range the input of the instrument to match the TX power reading if the auto range parameter is set to On. This is a broadband measurement of the peak transmitted carrier power of the MS. The measurement can be made on three types of signals:

1) Normal (TCH) bursts - Average in watts across the useful part of the normal burst (147 bit periods)

2)Access (RACH) bursts - Average in watts across the useful part of the access burst (87 bit periods)

3)Non burst modulated signals - Average in watts of 147 bit periods starting from near the trigger point (which is undefined)

This measurement is made from the RF IN/OUT port on the front panel.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     Q    [OUTPUT PARAMETER]

The TX output power response in watts from the instrument.
    D    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
12 = Oven Out of Range
13 = Unidentified Error
     �    [INPUT PARAMETER]

Set to true if this functions should try to auto range the instrument by resetting the expected power setting from the value read for TX Power.   @�m���  �    Status                           B� "  �  �    Instrument Handle                B� 5� �       TX Power                         CG 5 � �       TX Power Integrity               D� 8 ] �       Auto Range                         	           0    	           	            On VI_TRUE Off VI_FALSE    @    This function will read the mobile reported SACCH information.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     ;    [OUTPUT PARAMETER]

The mobile SACCH reported RX Quality.     �    [INPUT PARAMETER]

Set to true if this functions should use the :NEW node to wait for a new SACCH report from the mobile.

Note: It may take up to 3 NEW SACCH reports before the mobile response is up to date with the test set.
        [INPUT PARAMETER]

The number of SACCH reports to be read before the final report values are returned.

Note: It may take up to 3 NEW SACCH reports before the mobile response is up to date with the test set.


age1960_SACCHREADINGS_MIN    1
age1960_SACCHREADINGS_MAX    1000
     9    [OUTPUT PARAMETER]

The mobile SACCH reported RX Level.     ?    [OUTPUT PARAMETER]

The mobile SACCH reported Timing Advance.     9    [OUTPUT PARAMETER]

The mobile SACCH reported TX Level.   F�K���  �    Status                           H�   �  �    Instrument Handle                I � 4 �       RX Quality                       IV , Q �       Wait For New SACCH               JC 0q �      Number Of Readings               K` p 4 �       RX Level                         K� �f �       Timing Advance                   K� pf �       TX Level                           	           0    	          On VI_TRUE Off VI_FALSE      �                 	           	           	          T    This function initiate the desired measurement function. It will also turn off all measurement functions that cannot run concurrently if the new measurement function cannot run concurrently. If this function turns off BERR or FBER, this function will reset the loopback control to the value that was set via the age1960_loopback function.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [INPUT PARAMETER]

The measurement function to initiate.

age1960_AAUDIO     0   Analog Audio
age1960_BER        1   Bit Error
age1960_DAUDIO     2   Decoded Audio
age1960_FBER       3   Fast Bit Error
age1960_ORFS       4   Output RF Spectrum
age1960_PFER       5   Phase Freq Error
age1960_PVTIME     6   Power vs Time
age1960_TXPOWER    7   TX Power
age1960_DPOWER     8   Dynamic Power
age1960_IQTUNING   9   IQ Tuning
   O�
s���  �    Status                           Q   �  �    Instrument Handle                Q� D � � �    Meas Fcn                           	           0            
  Analog Audio age1960_AAUDIO Bit Error age1960_BER Decoded Audio age1960_DAUDIO Fast Bit Error age1960_FBER Output RF Spectrum age1960_ORFS Phase Freq Error age1960_PFER Power vs Time age1960_PVTIME TX Power age1960_TXPOWER Dynamic Power age1960_DPOWER IQ Tuning age1960_IQTUNING   X    This function will initiate a group of measurement functions. It will also turn off all measurement functions that cannot run concurrently if the new measurement function cannot run concurrently. If this function turns off BERR or FBER, this function will reset the loopback control to the value that was set via the age1960_loopback function    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The number of measurement functions to initiate at one time. The number of functions to initiate is checked against the age1960_NUMBER_OF_MEAS_FCNS value.

age1960_NUMBER_OF_MEAS_FCNS = 10    "    [INPUT PARAMETER]

The array measurement function to initiate.  The measurement functions available are as follows:

Analog Audio  (AAUD)     0  age1960_AAUDIO
Bit Error  (BER)         1  age1960_BER 
Decoded Audio  (DAUD)    2  age1960_DAUDIO
Fast Bit Error  (FBER)   3  age1960_FBER
ORF Spectrum (ORFS)      4  age1960_ORFS 
Phase Freq  (PFER)       5  age1960_PFER 
Power vs  (PVT)          6  age1960_PVTIME 
TX Power  (TXP)          7  age1960_TXPOWER
Dynamic Power  (DPOW)    8  age1960_DPOWER
IQ Tuning  (IQT)         9  age1960_IQTUNING   V�w���  �    Status                           X�   �  �    Instrument Handle                Y F / �      Meas Fcns To Init                Y� Fw �       Meas Fcns Array                    	           0       
                     �    This function will query the instrument until an initiated measurement function is complete (or none are initiated). The routine stops trying after the Init Done Timeout value is reached.                                                                  �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    x    [OUTPUT PARAMETER]

The measurement function that has completed.  The measurement functions available are as follows:

Wait for Init          -2   age1960_WAIT
None are Initiated     -1   age1960_NONEINIT
Analog Audio  (AAUD)    0   age1960_AAUDIO
Bit Error (BER)         1   age1960_BER
Decoded Audio (DAUD)    2   age1960_DAUDIO
Fast Bit Error (FBER)   3   age1960_BER
ORF Spectrum (ORFS)     4   age1960_ORFS
Phase Freq (PFER)       5   age1960_PFER
Power vs Time (PVT)     6   age1960_PVTIME
TX Power (TXP)          7   age1960_TXPOWER
Dynamic Power (DPOW)    8   age1960_DPOWER 
IQ Tuning (IQT)         9   age1960_IQTUNING

     �    [INPUT PARAMETER]

The maximum time (in seconds) this function will wait for a measurement function to complete.

 age1960_INIT_TIMEOUT_MIN  0.1
 age1960_INIT_TIMEOUT_MAX  120   ^s���  �    Status                           _� "  �  �    Instrument Handle                `J � � �       Meas Fcn Done                    b� 9 � �      Init DoneTime Out                  	           0    	         ?�      @^      ?�������@                  P    This function performs a FETCh:AAUDio and returns the following measurement results:

1) Measurement Integrity
2) Analog Audio 

This function supports earlier versions of the E1960A TA. Greater functionality can be achieved using the age1960_fetchfAFAN() function if your version of the E1960A TA will support it (A.07xx or later). 
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
13 = Unidentified Error
     Q    [OUTPUT PARAMETER]

The analog audio response in volts RMS from the instrument.   e�S���  �    Status                           g� "  �  �    Instrument Handle                h6 8 @ �       AAudio Integrity                 iZ 4n �       AAudio Result                      	           0    	            	              This function will perform a FETCH:AAUDio and return:

1) Measurement Integrity 
2) Average Audio Level 
3) Average SINAD 
4) Average Distortion (if available).

Initialize the measurement with age1960_initMeasFcn() or age1960_initMeasFcns() using age1960_AAUDIO.

Invalid measurement results will result in a value of 9.91 E+37.  

Note: This function adds functionality that was not available in earlier versions of the E1960A TA. If backward compatibility is required, use the age1960_fetchAAudio() function.        [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
13 = Unidentified Error
     a    [OUTPUT PARAMETER]

The returned value from the instrument for the average audio level result.
     ^    [OUTPUT PARAMETER]

The returned value from the instrument for the average SINAD result.         e    [OUTPUT PARAMETER]

The returned value from the instrument for the average distortion result.     
     W    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1961_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1961_error_message function to get a textual description of errors and warnings.
   l� F \ �       AFAN Integrity                   m� Ft �       AFAN Avg Audio Level             nF � \ �       AFAN Avg SINAD                   n� �t �       AFAN Avg Dist                    o C  �  �    Instrument Handle                oxZ���  �    Status                             	            	           	           	           0    	          	�    This function performs a FETCh:BERRor:FULL. It is either Residual type  or non-residual depending on what is set by the Configure Bit Error. In the results returned below it just mentions the type, however if one of the residual types is set then all the residual types are returned instead on of the non-residual.
The following measurement results are returned:

1) Measurement Integrity
2) Type Ia Bits Tested
3) Type Ia Bit Error Ratio
4) Type Ia Bit Error Count
5) Type Ib Bits Tested
6) Type Ib Bit Error Ratio
7) Type Ib Bit Error Count
8) Type II Bits Tested
9) Type II Bit Error Ratio
10) Type II Bit Error Count

It will also turn off all measurement functions that cannot run           concurrently with BER.

The RX BER Meas is used to measure the performance of the receiver of the MS. A downlink TCH with PRBS data is generated at a known level, and the MS receives it, loops it back to its transmitter, and returns the PRBS data to the BS. A comparison is then made in the BS between the data sent and the data received, and the BER is calculated.
There are 3 commonly used types of bit error measurement in GSM:

1) BER using MS loopback with signaling of erased speech frames. In this case the comparison is made between the required class of bits within the 260 bits of speech data presented to the channel coder of the BS and the 260 bits of speech data received by the channel decoder of the BS. The mobile will indicate in the speech frame if the downlink frame was received and was found to have CRC errors which would cause the speech frame to be erased. The mobile indicates this on the uplink by setting all bits in the uplink speech frame to 0.

Referred to from now on as "BER with FE" or �Residual BER� (loopback type A)

2) BER using MS loopback without signaling of erased speech frames. In this case the comparison is made between the required class of bits within the 260 bits of speech data presented to the channel coder of the BS and the 260 bits of speech data received by the channel decoder of the BS. The mobile does not provide any indication of whether or not the downlink speech frame was erased.

Referred to from now on as "BER without FE" or "Non-Residual BER" (loopback type B);

3) BER using MS burst-by-burst loopback. In this case the comparison is made between the 114 bits of interleaved data presented to layer 1 in the BS and the 114 bits of interleaved data received by layer 1 in the BS.

Referred to from now on as "Fast Bit Errors" (loopback type C).    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

The integrity response from the instrument. This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout        [OUTPUT PARAMETER]

This result represents the number of type Ia (residual or non-residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The  type Ia (residual or non-residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The  type Ia (residual or non-residual set in configure Bit Error) bit error count response from the instrument.
        [OUTPUT PARAMETER]

This result represents the number of  type Ib (residual or non-residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The  type Ib (residual or non-residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The  type Ib (residual or non-residual set in configure Bit Error) bit error count response from the instrument.
        [OUTPUT PARAMETER]

This result represents the number of  type II (residual or non-residual set in configure Bit Error) bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.     �    [OUTPUT PARAMETER]

The type II (residual or non-residual set in configure Bit Error) bit error ratio response in percent from the instrument.     �    [OUTPUT PARAMETER]

The  type II (residual or non-residual set in configure Bit Error) bit error count response from the instrument.
   |�s���  �    Status                           ~� "  �  �    Instrument Handle                ~� )  �       BER Integrity                    � ) � �       Type Ia BER Bits Tested          �� ) �       Type Ia BER Ratio                �W )� �       Type Ia BER Count                �� x ^ �       Type Ib BER Bits Tested          � w � �       Type Ib BER Ratio                �� wx �       Type Ib BER Count                �+ � ^ �       Type II BER Bits Tested          �H � � 	�       Type II BER Ratio                �� �x 
�       Type II BER Count                  	           0    	            	           	           	           	           	           	           	           	           	           �    This function performs a FETCh:DADio and returns the following measurement results:

1) Measurement Integrity
2) Decoded Audio     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
        [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = PCM Full Scale Warning
13 = Unidentified Error
     \    [OUTPUT PARAMETER]

The decoded audio response in percent full scale from the instrument.
   ��s���  �    Status                           �� "  �  �    Instrument Handle                �< 3 I �       DAudio Integrity                 �[ 3� �       DAudio Result                      	           0    	            	          �    This function performs a fetch of the Dynamic Power results. This function will return the following measurement results:

1) Measurement Integrity an array of N integers representing the    integrity of each burst measured. N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function (Dyn Power Meas Count).

2) The Average TX  power of each of the N bursts.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of the dynamic Power result array.  The size must be equal to or greater than the number of points defined by SETup:DPOWer:COUNt:NUMBer.


age1960_DPOW_ARRAY_MIN       1
age1960_DPOW_ARRAY_MAX    2000
    .    [OUTPUT PARAMETER]

The integrity response array from the instrument.  It has N elements, where N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function.  Each element returned contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
10 = Signal Too Noisy
11 = Sync Not Found
13 = Unidentified Error
     �    [OUTPUT PARAMETER]

The Dynamic Power Results Array. It has N elements, where N is equal to the number of bursts being counted. N is set in the Configure Dynamic Power function.

Range: -100 to +100dBm and 9.91 E+37 (NAN).     P    [OUTPUT PARAMETER]

The number of points measured for the dynamic power array.   �Ts���  �    Status                           �3    �  �    Instrument Handle                �� < � �      Dyn Power Array Size             �~ � 9 �       Dyn Power Integrity              �� � � �       Dyn Power Results Array          �� �� �       Dyn Power Count                    	           0      �      
           	            	            	            �    This function performs a FETCH:FBERror? and returns the following measurement results:

1) Measurement Integrity
2) Fast Bits Tested
3) Fast Bit Error Ratio
4) Fast Bit Error Count    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
15 = Questionable for Phase 1 Mobile     �    [OUTPUT PARAMETER]

This result represents the number of bits actually tested. Note that the number actually tested will usually exceed the number requested due to the quantization effects of the frame structure.
     W    [OUTPUT PARAMETER]

The fast bit error ratio response in percent from the instrument.     M    [OUTPUT PARAMETER]

The fast bit error count response from the instrument.
   �<[���  �    Status                           � :  �  �    Instrument Handle                �{ 2 U �       FBER Integrity                   �Z 0v �       FBER Bits Tested                 �9 � U �       FBER Ratio                       �� �v �       FBER Count                         	           0    	            	           	           	          v    This function performs a fetch of the IQ Tuning results. This function will return the following measurement results:

1) Measurement Integrity
2) The the user settable spur frequency.
3) The user settable spur reference level.
4) Measured IQ results
5) Offset Frequency (kHz) at which the IQ results were measured
6) IQ measurement Count (the number of offsets measured)
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    Z    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 10 = Signal Too Noisy
 11 = Sync Not Found
 13 = Unidentified Error
     {    [INPUT PARAMETER]

Size of the IQ Results and the IQ Freqs arrays.

 age1960_IQ_ARRAY_MIN   9
 age1960_IQ_ARRAY_MAX   500     ,    [OUTPUT PARAMETER]

The IQ Results Array.
     R    [OUTPUT PARAMETER]

The frequencies in kHz that the IQ results were measured at.     S    [OUTPUT PARAMETER]

The number of IQ results measured.  This should always be 9.
     i    [OUTPUT PARAMETER]

This sets the spur frequency in Hz at which an additional measurement can be made.
     Q    [OUTPUT PARAMETER]

This is the relative level in dB's at the spur frequency.

   ��t���  �    Status                           �� !  �  �    Instrument Handle                �) o B �       IQT Integrity                    �� - � �      IQT Array Size                   � � D �       IQT Results Array                �B � � �       IQT Freqs Array                  �� �y �       IQT Tuning Count                 �� r � �       IQT Spur Freq                    �h py �       IQT Spur Level                     	           0    	              �   	   
           	            	            	            	           	          r    This function performs a FETCh:ORFS and returns the following measurement results:

1) Measurement Integrity
2) TX Power
3) 30 kHz Bandwidth Power (if there are points defined by SETup:ORFS:MODulation:FREQuency else this will return NAN)
4) An array of ORFS switching results at the points defined by SETup:ORFS:SWITching:FREQuency
5) An array of ORFS modulation results at the points defined by SETup:ORFS:MODulation:FREQuency
6) The number of points returned in the switching results array
7) The number of points returned in the modulation results array

This is a narrow band measurement of the modulation spectrum of the MS�s transmitter. It is made in two parts, the first being the spectrum due to the GMSK modulation measured over the useful part of the burst, excluding the midamble, and the second being the spectrum due to switching transients, which is measured across the entire time slot, plus and minus approximately 10 bits of margin. The majority of the switching energy measured is contained in the rising and falling edges of the burst.
The measurement can be made on the following types of input signal:
 
1) Normal (TCH) bursts. (the shorter RACH bursts are not supported)

2) GMSK modulated signals without burst modulation (these signals will have no measurable spectrum due to switching transients)
This measurement is made from the RF IN/OUT port on the front panel.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of the ORFS switching result array.  The size must be equal to or greater than the number of points defined by SETup:ORFS:SWITching:FREQuency:POINts.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500     �    [INPUT PARAMETER]

Size of the ORFS modulation result array.  The size must be equal to or greater than the number of points defined by SETup:ORFS:MODulation:FREQuency:POINts.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500    Y    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
10 = Signal Too Noisy
12 = Oven Out of Range
13 = Unidentified Error     O    [OUTPUT PARAMETER]

The ORFS TX power response in watts from the instrument.
     �    [OUTPUT PARAMETER]

The 30 kHz bandwidth power response from the instrument. If 
SETup:ORFS:MODulation:FREQuency:POINTs? returns 0, indicating that no Modulation measurement was made, this value will return  NAN.
    #    [OUTPUT PARAMETER]

The ORFS switching array response from the instrument that represents results measured at the specified SETUP:ORFS:SWITching:FREQuency. If 
SETup:ORFS:SWITching:FREQuency:POINts? returns 0, indicating that no Switching measurement was made, this value will return NAN.
    &    [OUTPUT PARAMETER]

The ORFS modulation array response from the instrument that represents results measured at the specified SETUP:ORFS:MODulation:FREQuency.  If 
SETup:ORFS:MODulation:FREQuency:POINts? returns 0 indicating that no Modulation measurement was made, this value will return NAN
     Q    [OUTPUT PARAMETER]

The number of points measured for the ORFS switching array.     R    [OUTPUT PARAMETER]

The number of points measured for the ORFS modulation array.   ��s���  �    Status                           �g "  �  �    Instrument Handle                �� ' � �      ORFS Sw Array Size               �� 'c �      ORFS Mod Array Size              �� k  �       ORFS Integrity                   �� k � �       ORFS TX Power                    �Q k� �       ORFS BW Power                    �0 �  �       ORFS Sw Array                    �[ � � �       ORFS Mod Array                   �� �& �       ORFS Sw Count                    �� �� 	�       ORFS Mod Count                     	           0      �                   �                 	            	           	           	            	            	            	           �    This function returns the Output RF Spectrum measurement pass/fail summary results when those results are evaluated against the switching and modulation limits masks. The following values are returned:

1) The summary pass fail result when switching AND modulation limits masks are evaluated.

2) The summary pass/fail result when only the switching limits mask is evaluated.

3) The summary pass/fail result when only the modulation limits mask is evaluated.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

The switching Pass/Fail result when switching mask limits are evaluated. A NAN will be returned if the measurement does not complete or if no switching offsets are enabled.

0      = passed.
1      = failed.
NAN    = undefined.
    �    [OUTPUT PARAMETER]

The overall Pass/Fail result when both switching limits AND modulation limits masks are evaluated. A NAN will be returned if the measurement does not complete, or neither switching or modulation offset limits were enabled. If no ORFS due to switching offsets are enabled, the measurement will pass if all ORFS due to modulation results that were enabled passed (and vice-versa).

0      = passed.
1      = failed.
NAN    = undefined.
     �    [OUTPUT PARAMETER]

The modulation Pass/Fail result when modulation mask limits are evaluated. A NAN will be returned if the measurement does not complete or if no modulation offsets are enabled.

0      = passed.
1      = failed.
NAN    = undefined.
   ��
[���  �    Status                           �� 6  �  �    Instrument Handle                �+ I � �       Switching PF Summary             �- I D �       Overall PF Summary               �� Iz �       Modulation PF Summary              	           0    	           	           	          Q    This function performs a FETCh:PFER and returns the following measurement results:

1) Measurement Integrity
2) Maximum Peak Phase Error
3) Maximum RMS Phase Error
4) Worst Frequency Error

This is a narrow band (<200kHz) measurement of the GMSK modulation quality and frequency accuracy of the MS transmitter, measured across the useful part of the TDMA burst.
This measurement is made from the RF IN/OUT port on the front panel.

The measurement can be made on three types of signals:
1) Normal (TCH) bursts
2) Access (RACH) bursts
3) Non-burst modulated signals
 
In all cases, there must be 0.3GMSK modulated data on the signal. For signals without GMSK modulation, it is only possible to make a valid measurement by offsetting the Expected Frequency by +/- 67.7083 kHz to make the signal look like it is modulated with all zeros or all ones.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
10 = Signal Too Noisy
11 = Sync Not Found
13 = Unidentified Error
     \    [OUTPUT PARAMETER]

The maximum peak phase error response in degrees from the instrument.
     [    [OUTPUT PARAMETER]

The maximum RMS phase error response in degrees from the instrument.
     T    [OUTPUT PARAMETER]

The worst frequency error response in Hz from the instrument.
   ǙV���  �    Status                           �x ;  �  �    Instrument Handle                �� 9 V �       PFER Integrity                   ˁ � V �       PFER Peak Error                  �� 7q �       PFER RMS Error                   �H �q �       PFER Freq Error                    	           0    	            	           	           	          �    This function performs a FETCh:PVT and returns the following measurement results:

1) Measurement Integrity
2) Power Mask Pass Fail indication (1 = pass 0 = fail)
3) Average Power
4) Peak Plus Power
5) Peak Minus Power
6) An array of power vs time results at the points defined by  SETup:PVT:TIME:POINTs?
7) The number of points returned in the above array

This Power vs Time measurement is a narrow band measurement of the transmitted carrier power of the MS as it varies across the TDMA burst. Included is a broadband Carrier Power Measurement of the average power across the burst which is made from the same physical burst as the trace measurement. This Carrier Power Measurement can be midamble synchronized as defined by GSM 11.10. The measurement can be made on three types of signals:
1) Normal (TCH) bursts

2) Access (RACH) bursts

3) Non-pulsed (CW) signals - (corner case)

   The trace will be flat, but the power will be correct.

This measurement will implement TrigQualPower = On.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

Size of the Power vs Time array.  The size must be equal to or greater than the number of points defined by SETup:PVT:TIME:POINts

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500    �    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
 8 = Trig/Fall Early [Trig Too Early]
 9 = Trig/Rise Late [Trig Too Late]
11 = Sync Not Found
12 = Oven Out of Range
13 = Unidentified Error    S    [OUTPUT PARAMETER]

The power vs time mask pass fail response from the instrument.

0 = results were within the specified mask.
1 = results were NOT within the specified mask.

The pass fail result is based on a point by point comparison with the Power Vs. Time Meas mask. The mask is as defined in "ETSI GSM 05.05 Ver. 4.21.0 Annex B".
     i    [OUTPUT PARAMETER]

The average of the power in watts as measured across the useful part of the burst.
     �    [OUTPUT PARAMETER]

The power vs time array response from the instrument that represents results measured at the specified SETUP:PVT:TIME:POINts.
     Q    [OUTPUT PARAMETER]

The number of points measured for the power vs time array.
   �|���  �    Status                           ��   �  �    Instrument Handle                �Q 3 ; �      P vs Time Array Size             �  + � �       P vs Time Integrity              �� +� �       P vs Time Mask Fail              �$ � 1 �       P vs Time TX Power               ؕ � � �       P vs Time Array                  �1 �� �       P vs Time Count                    	           0      �                 	            	           	           	            	           �    This function performs a FETCh:TXP and returns the following measurement results:

1) Measurement Integrity
2) TX output power

This is a broadband measurement of the peak transmitted carrier power of the MS. The measurement can be made on three types of signals:

1) Normal (TCH) bursts - Average in watts across the useful part of the normal burst (147 bit periods)

2)Access (RACH) bursts - Average in watts across the useful part of the access burst (87 bit periods)

3)Non burst modulated signals - Average in watts of 147 bit periods starting from near the trigger point (which is undefined)

This measurement is made from the RF IN/OUT port on the front panel.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    C    [OUTPUT PARAMETER]

The integrity response from the instrument.  This integer contains the following information:

 0 = Normal
 1 = No Result Available
 2 = Measurement Timeout
 3 = Hardware Not Installed
 4 = Hardware Error
 5 = Over Range
 6 = Under Range
 7 = Burst Short
12 = Oven Out of Range
13 = Unidentified Error     Q    [OUTPUT PARAMETER]

The TX output power response in watts from the instrument.
   �;s���  �    Status                           � "  �  �    Instrument Handle                �z 5 @ �       TX Power Integrity               �� 1� �       TX Power                           	           0    	            	          �    The reset function places the instrument in a default state. Before issuing this function, it may be necessary to send a device clear to ensure that the instrument can execute a reset.  A device clear can be issued by invoking age1960_dcl().

For the states affected by the reset command, see the instrument manual.

Reset does not affect the status system.

To quickly place the instrument in a default state without resetting the complete instrument see age1960_partialPreset().      �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
   �k���  �    Status                           �� "  �  �    Instrument Handle                  	           0   0    The self-test function causes the instrument to perform a self-test and returns the result of that self-test.  This is used to verify that an instrument is operating properly.  A failure may indicate a potential hardware problem.

For a list of possible self-test errors, consult the instrument manual.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
         [OUTPUT PARAMETER]

Numeric result from self-test operation.

 0  = no error (test passed)
-1 = error occured (test failed).
     T    [OUTPUT PARAMETER]

Self-test status message.  This is limited to 256 characters.
   ��o���  �    Status                           �� "  �  �    Instrument  Handle               �< 6 � �  �    Self Test Result                 �� � " � �    Self Test Message                  	           0    	            	            r    This function translates the error value returned from an instrument driver function to a user-readable string.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     l    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().  It may be VI_NULL.      P    [INPUT PARAMETER]

The error return value from an instrument driver function.
     P    [OUTPUT PARAMETER]

Error message string.  This is limited to 256 characters.
   �	q���  �    Status                           �w	 "  �  �    Instrument Handle                �� 4 � �  �    Status Code                      �C � " � �    Message                            	           0    0    	           �    This function returns the error numbers and corresponding error messages in the error queue of an instrument. See the manual for a listing of the instrument error numbers and messages.

Instrument errors may occur when a user attempts to place the instrument in an invalid state such as sending an invalid sequence of coupled commands.

Instrument errors can be detected by polling. Automatic polling can be accomplished by using the age1960_errorQueryDetect() function.

    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     /    [OUTPUT PARAMETER]

Instrument's error code.
     U    [OUTPUT PARAMETER]

Instrument's error message. This is limited to 256 characters.
   �sn���  �    Status                           �T "  �  �    Instrument Handle                �� . � �  �    Error Code                       �� � " � �    Error Message                      	           0    	            	            m    This function returns the revision of the instrument driver and the firmware of the instrument being used.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     U    [OUTPUT PARAMETER]

Instrument driver revision. This is limited to 256 characters.
     W    [OUTPUT PARAMETER]

Instrument firmware revision. This is limited to 256 characters.
   ��q���  �    Status                           �� "  �  �    Instrument Handle                �� / � � ,    Instrument Driver Revision       �X � � � ,    Firmware Revision                  	           0    	            	               The timeout function sets the timeout value for driver I/O transactions in milliseconds. The timeout period may vary on computer platforms.

The default timeout period varies by VISA implementation. For Agilent VISA, it is 2 seconds.

Some implementations of VISA will only accept a limited number of timeout values.  If you set the VISA timeout to a given value using one of those implementations, and then query the timeout, you may find that the query returns a different value than the one you set.  Agilent VISA sets the timeout to the precise value you specify, and returns that value when queried.

This timeout value should be longer than the timeout values used for the individual configuration measurement timeout values.  This will allow the instrument to handle measurement timeout.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

This value sets the I/O timeout for all functions in the driver. It is specified in milliseconds. 

 age1960_TIMEOUT_MIN   0
 age1960_TIMEOUT_MAX   120000   ��t���  �    Status                            � "  �  �    Instrument Handle                 B � �      Set Time Out                       	           0     ��      �           �    The timeout query function returns the timeout value for driver I/O transactions in milliseconds. The timeout period may vary on computer platforms.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

This is the timeout value presently set for all the I/O functions in the driver. It is specified in milliseconds. 
   6r���  �    Status                            "  �  �    Instrument Handle                w 2 � � �    Time Out                           	           0    	           +    This function enables or disables automatic instrument error checking.

When automatic instrument error querying is enabled, the driver will query the instrument for an error before returning from each driver function.  This adds the overhead of one instrument query to each driver function call.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

A boolean which enables/disables automatic instrument error querying.  

If VI_TRUE, enable automatic instrument error querying.

If VI_FALSE, disable automatic instrument error querying.
   �	t���  �    Status                           	�	 "  �  �    Instrument Handle                
8 . � �       Set Error Query Detect             	           0     Enable VI_TRUE Disable VI_FALSE   =    This function indicates if automatic instrument error detection is enabled or disabled. 

When automatic instrument error querying is enabled, the driver will query the instrument for an error before returning from each driver function.  This adds the overhead of one instrument query to each driver function call.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [OUTPUT PARAMETER]

A boolean which indicates if automatic instrument error querying is enabled or disabled.  

If VI_TRUE, automatic instrument error querying is enabled.

If VI_FALSE, automatic instrument error querying is disabled.
   )v���  �    Status                           
 "  �  �    Instrument Handle                j 4 � �  d    Error Query Detect                 	           0    	           �    This function sends a device clear (DCL) to the instrument.

A device clear will abort the current operation and enable the instrument to accept a new command or query.

This is particularly useful in situations where it is not possible to determine the instrument state. In this case, it is customary to send a device clear before issuing a new instrument driver function.  The device clear ensures that the instrument will be able to begin processing the new commands.

    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
   �
u���  �    Status                           � 1  �  �    Instrument Handle                  	           0    B    This function sends the *OPC? common command to the instrument.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
    �    [OUTPUT PARAMETER]

If VI_TRUE, all instrument operations are complete.

If VI_FALSE, instrument operations are still pending.

Since the E1960 has many operations that can be running at the same time it may be best to use some of the instrument's programming features that allow knowning when specific operations are completed. See the instrument manual for Test System Synchronization.

   u���  �    Status                           � "  �  �    Instrument Handle                D 0 � �       Instrument Ready                   	           0    	            )    This function queries the status byte.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     4    [OUTPUT PARAMETER]

The value of the status byte.
   �v���  �    Status                           � "  �  �    Instrument Handle                 / � �  d    Status Byte                        	           0    	            5    This function queries the standard event register.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     @    [OUTPUT PARAMETER]

The value of the standard event register.
   6t���  �    Status                             "  �  �    Instrument Handle                 w 6 � �  d    Std Event Register                 	           0    	            =    This function queries the desired operation event register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     R    [OUTPUT PARAMETER]

The value of the operation event register that was queried.
    :    [INPUT PARAMETER]

The name of the operational event register to query.

age1960_OPER_SUMMARY    0    Operational Summary
age1960_OPER_NMRR       1    Operational NMRR
age1960_OPER_CALL       2    Operational Call
age1960_OPER_NMRR_GSM   3    Operation NMRR GSM
age1960_OPER_CALL_GSM   4    Operational Call GSM
   !�t���  �    Status                           #� "  �  �    Instrument Handle                $ � � �  d    Operation Event Register         $[ H � � �    Operation Name                     	           0    	                       �Operation Summary age1960_OPER_SUMMARY Operation NMRR age1960_OPER_NMRR Operation Call age1960_OPER_CALL Operation NMRR GSM age1960_OPER_NMRR_GSM Operation Call GSM age1960_OPER_CALL_GSM    A    This function queries the desired operation condition register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     U    [OUTPUT PARAMETER]

The value of the operation condition register that was queried.    :    [INPUT PARAMETER]

The name of the operational event register to query.

age1960_OPER_SUMMARY    0    Operational Summary
age1960_OPER_NMRR       1    Operational NMRR
age1960_OPER_CALL       2    Operational Call
age1960_OPER_NMRR_GSM   3    Operation NMRR GSM
age1960_OPER_CALL_GSM   4    Operational Call GSM
   '�u���  �    Status                           )� "  �  �    Instrument Handle                )� � � �  d    Operation Condition Register     *? 5 � � �    Operation Name                     	           0    	                       �Operation Summary age1960_OPER_SUMMARY Operation NMRR age1960_OPER_NMRR Operation Call age1960_OPER_CALL Operation NMRR GSM age1960_OPER_NMRR_GSM Operation Call GSM age1960_OPER_CALL_GSM    @    This function queries the desired questionable event register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     T    [OUTPUT PARAMETER]

The value of the questionable event register that was queried.    r    [INPUT PARAMETER]

The name of the questionable event register to query.


age1960_QUES_SUMMARY    0   Questionable Summary
age1960_QUES_CALL       1   Questionable Call
age1960_QUES_ERROR      2   Questionable Error
age1960_QUES_HARDWARE   3   Questionable Hardware
age1960_QUES_CALL_GSM   4   Questionable Call GSM
age1960_QUES_ERROR_GSM  5   Questionable Error GSM
   -�u���  �    Status                           /e "  �  �    Instrument Handle                /� � � �  d    Questionable Event Register      0! A � � �    Questionable Name                  	           0    	                       �Questionable Summary age1960_QUES_SUMMARY Questionable Call age1960_QUES_CALL Questionable Error age1960_QUES_ERROR Questionable hardware age1960_QUES_HARDWARE Questionable Call GSM age1960_QUES_CALL_GSM Questionable Error GSM age1960_QUES_ERROR_GSM    D    This function queries the desired questionable condition register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     X    [OUTPUT PARAMETER]

The value of the questionable condition register that was queried.    w    [INPUT PARAMETER]

The name of the questionable event register to query.

age1960_QUES_SUMMARY     0   Questionable Summary
age1960_QUES_CALL        1   Questionable Call
age1960_QUES_ERROR       2   Questionable Error
age1960_QUES_HARDWARE    3   Questionable Hardware
age1960_QUES_CALL_GSM    4   Questionable Call GSM
age1960_QUES_ERROR_GSM   5   Questionable Error GSM
   3�u���  �    Status                           5�   �  �    Instrument Handle                6" � � �  d    Questionable Condition Register  6� : � � �    Questionable Name                  	           0    	                       �Questionable Summary age1960_QUES_SUMMARY Questionable Call age1960_QUES_CALL Questionable Error age1960_QUES_ERROR Questionable Hardware age1960_QUES_HARDWARE Questionable Call GSM age1960_QUES_CALL_GSM Questionable Error GSM age1960_QUES_ERROR_GSM    ;    This function enables the desired questionable register.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The desired questionable register bits (value) for a positive transition that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767     �    [INPUT PARAMETER]

The desired questionable register bits (value) for a negative transition that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767     �    [INPUT PARAMETER]

The desired questionable register bits (value) that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767    w    [INPUT PARAMETER]

The name of the questionable event register to query.

age1960_QUES_SUMMARY     0   Questionable Summary
age1960_QUES_CALL        1   Questionable Call
age1960_QUES_ERROR       2   Questionable Error
age1960_QUES_HARDWARE    3   Questionable Hardware
age1960_QUES_CALL_GSM    4   Questionable Call GSM
age1960_QUES_ERROR_GSM   5   Questionable Error GSM
   :>u���  �    Status                           < "  �  �    Instrument Handle                < � E �      Ques Pos Transition              =5 � � �      Ques NegTransition               =� �� �      Ques Enable Bits                 >� 5 � � �    Questionable Name                  	           0      �                     �                     �                              �Questionable Summary age1960_QUES_SUMMARY Questionable Call age1960_QUES_CALL Questionable Error age1960_QUES_ERROR Questionable Hardware age1960_QUES_HARDWARE Questionable Call GSM age1960_QUES_CALL_GSM Questionable Error GSM age1960_QUES_ERROR_GSM    7    This function enables the desired operation register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The desired operation register bits (value) for a positive transition that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767     �    [INPUT PARAMETER]

The desired operation register bits (value) for a negative transition that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767     �    [INPUT PARAMETER]

The desired operation register bits (value) that are to be enabled.

 age1960_REGISTER_MIN   0
 age1960_REGISTER_MAX   32767    5    [INPUT PARAMETER]

The name of the operational event register to query.

age1960_OPER_SUMMARY    0   Operational Summary
age1960_OPER_NMRR       1   Operational NMRR
age1960_OPER_CALL       2   Operational Call
age1960_OPER_NMRR_GSM   3   Operation NMRR GSM
age1960_OPER_CALL_GSM   4   Operational Call GSM
   B�u���  �    Status                           D� "  �  �    Instrument Handle                E# � D �      Oper Pos Transition              E� � � �      Oper NegTransition               F� �� �      Oper Enable Bits                 G" < � � �    Operation Name                     	           0      �                     �                     �                              �Operation Summary age1960_OPER_SUMMARY Operation NMRR age1960_OPER_NMRR Operation Call age1960_OPER_CALL Operation NMRR GSM age1960_OPER_NMRR_GSM Operation Call GSM age1960_OPER_CALL_GSM    5    This function enables the standard event register.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     �    [INPUT PARAMETER]

The standard event register bits (value) that are to be enabled.

  age1960_STD_REGISTER_MIN   0
  age1960_STD_REGISTER_MAX   255   J�u���  �    Status                           L� "  �  �    Instrument Handle                M; W � �      Std Enable Bits                    	           0       �                   l    This function passes an instrument command to the instrument.  The function does not expect any response.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     {    [INPUT PARAMETER]

The instrument command. It must be a NULL terminated C string and may not exceed 256 bytes in length.
   Os���  �    Status                           P� "  �  �    Instrument Handle                QS 1 " � �    Send String Command                	           0    ""   �    Passes the command string to the instrument. The function waits for a response which must be a string (character data).

A max of size-1 bytes will be read from the instrument into the string and the string will be NULL terminated (so its length can be found with strlen).

If more than size-1 bytes are generated by the instrument then remaining bytes will be discarded and the instrument's output buffer will be cleared.
     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     {    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.     z    [INPUT PARAMETER]

The length of result array in bytes. 

 age1960_CMDSTRING_Q_MIN   2
 age1960_CMDSTRING_Q_MAX   32767
     8    [OUTPUT PARAMETER]

The response from the instrument.
   T>o���  �    Status                           V "  �  �    Instrument Handle                V} 0 " � �    Query String Command             W  l � �      String Size                      W� � " � �    String Result                      	           0    " "      �                 	            �    This function passes the string in Send Integer Command followed by a space and then an integer.  Note that either a ViInt16 or 32 can be passed, as the ViInt16 will be promoted.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     �    [INPUT PARAMETER]

The integer to be appended to the command string. 

 age1960_CMDINT_MIN   -2147483647
 age1960_CMDINT_MAX    2147483647
   Y�t���  �    Status                           [� "  �  �    Instrument Handle                \ / " � �    Send Integer Command             \� s � �      Send Integer                       	           0    ""    ����                 t    Passes the command string to the instrument.  The function expects a 16-bit integer response from the instrument.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     8    [OUTPUT PARAMETER]

The response from the instrument.
   ^�q���  �    Status                           `x "  �  �    Instrument Handle                `� / " � �    Query I16 Command                a\ p � �       I16 Result                         	           0    ""    	            t    Passes the command string to the instrument.  The function expects a 32-bit integer response from the instrument.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     8    [OUTPUT PARAMETER]

The response from the instrument.
   cq���  �    Status                           d� "  �  �    Instrument Handle                eN . " � �    Query I32 Command                e� p � �  �    I32 Result                         	           0    ""    	            �    This function sends the string in the Send Integer Array Command followed by a space and then a list of comma separated integers that are contained in the Send Integer Array.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     e    [INPUT PARAMETER]

Size of the Send Integer Array.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500     �    [INPUT PARAMETER]

The array of integer to send in the comma separated form after the Send integer Array Command string is sent.
   g�t���  �    Status                           i� "  �  �    Instrument Handle                j  0 " � �    Send Integer Array Command       j� p E �      Send Integer Array Size          j� p� �       Send integer Array                 	           0    ""      �                     �    Passes the command string to the instrument.  The function expects the instrument to return a comma or semi-colon separated list of 16-bit integers.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     �    [INPUT PARAMETER]

The size (elements) of the result array passed into the function.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500
     8    [OUTPUT PARAMETER]

The response from the instrument.
     O    [OUTPUT PARAMETER]

The number of array elements returned by the instrument.
   mVs���  �    Status                           o5 "  �  �    Instrument Handle                o� / " � �    Query I16 Array Command          p r & �      I16 Array Size                   p� j � �  �    I16 Array Result                 p� jm �  �    I16 Array Count                    	           0    ""      �                 	            	            �    Passes the command string to the instrument.  The function expects the instrument to return a comma or semi-colon separated list of 32-bit integers.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     �    [INPUT PARAMETER]

The size (elements) of the result array passed into the function.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500
     8    [OUTPUT PARAMETER]

The response from the instrument.
     O    [OUTPUT PARAMETER]

The number of array elements returned by the instrument.
   scs���  �    Status                           uB "  �  �    Instrument Handle                u� / " � �    Query I32 Array Command          v& p & �      I32 Array Size                   v� h � �  �    I32 Array Result                 v� hc �  �    I32 Array Count                    	           0    ""      �                 	            	            �    This function passes the string in Send Real Command followed by a space and then a real. Note that either a ViReal32 or 64 can be passed as the ViReal32 will be promoted.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     �    [INPUT PARAMETER]

The real number to be appended to the command string. 

 age1960_CMDREAL_MIN    -1E+300
 age1960_CMDREAL_MAX     1E+300   y�t���  �    Status                           {f "  �  �    Instrument Handle                {� 0 " � �    Send Real Command                |J t � �      Send Real                          	           0    "" }s�k��;~7�<� u��7�<� u�                     q    Passes the command string to the instrument.  The function expects a 64-bit real response from the instrument.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     8    [OUTPUT PARAMETER]

The response from the instrument.
   ~mq���  �    Status                           �L "  �  �    Instrument Handle                �� . " � �    Query Real Command               �0 k � �       Real Result                        	           0    ""    	           �    This function sends the string in the Send Real Array Command followed by a space and then a list of comma separated reals that are contained in the Send Real Array.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     |    [INPUT PARAMETER]

The instrument command.  It must be a NULL terminated C string and may not exceed 256 bytes in length.
     b    [INPUT PARAMETER]

Size of the Send Real Array.

 age1960_ARRAY_MIN   1
 age1960_ARRAY_MAX   500     ~    [INPUT PARAMETER]

The array of reals to send in the comma separated form after the Send Real Array Command string is sent.
   �t���  �    Status                           �� "  �  �    Instrument Handle                �U 0 " � �    Send Real Array Command          �� o " �      Send Real Array Size             �C p� �       Send Real Array                    	           0    ""      �                     �    Passes the command string to the instrument.  The function expects the instrument to return a comma or semi-colon separated list of 64-bit reals.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
     {    [INPUT PARAMETER]

The instrument command. It must be a NULL terminated C string and may not exceed 256 bytes in length.
     U    [INPUT PARAMETER]

Size of result. 

 age1960_ARRAY_MIN  1
 age1960_ARRAY_MAX  500
     8    [OUTPUT PARAMETER]

The response from the instrument.
     O    [OUTPUT PARAMETER]

The number of array elements returned by the instrument.
   ��u���  �    Status                           �~ %  �  �    Instrument Handle                �� . % � �    Real Array Command               �a s / �      Real Array Size                  �� k � �  �    Real Array Result                �� k� �  �    Real Array Count                   	           0    ""      �                 	            	            �    The close function terminates the software connection to the instrument and de-allocates system resources. It is generally a good programming habit to close the instrument handle when the program is done using the instrument.     �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the age1960_error_message() function to get a textual description of errors and warnings.
     X    [INPUT PARAMETER]

The instrument's VISA session handle returned from age1960_init().
   ��
q���  �    Status                           �� "  �  �    Instrument Handle                  	           0 ����         	$  �     K-    init                            ����         <  x     K-    BATable                         ����         �  #�     K-    BCHARFCN                        ����         &3  *�     K-    burstType                       ����         ,n  0�     K-    cellBand                        ����         2d  9�     K-    cellPower                       ����         ;�  @�     K-    expectedPower                   ����         Bw  F�     K-    loopback                        ����         G�  KY     K-    manualBand                      ����         L�  T     K-    manualChan                      ����         U�  Y�     K-    MSTimeAdvance                   ����         [  c|     K-    MSTXLevel                       ����         e�  i*     K-    operatingMode                   ����         j$  ml     K-    confRcvControl                  ����         nh  rr     K-    TCHTimeslot                     ����         s�  {�     K-    TCHARFCN                        ����         ~#  ��     K-    TCHBand                         ����         �  ��     K-    channelMode                     ����         ��  �u     K-    FACCHSignaling                  ����         �3  ��     K-    burstTimingError_Q              ����         �^  �f     K-    originateCall                   ����         ��  ��     K-    endCall                         ����         �x  ��     K-    confForMSOriginate              ����         �  �x     K-    waitForConnect                  ����         �S  �D     K-    currentTA                       ����         �D  ��     K.    currentTA_Q                     ����         ��  �r     K.    currentTAModel_Q                ����         �.  �"     K.    currentActiveTAP                ����         ��  ē     K.    currentActiveTAP_Q              ����         �O  Ȱ     K-    integrityMessage                ����         ɸ  �Q     K-    partialPreset                   ����         ��  �T     K-    getCorrectionFreq_Q             ����         ��  �     K-    getPresentFreq_Q                ����         �W  ��     K-    UUTLevel_Q                      ����         ��  �     K.    getApproxOffset_Q               ����         �  �     K-    amplitudeOffset                 ����         ��  �:     K-    useSRQMAV                       ����         �  ��     K-    useSRQMAV_Q                     ����         ��  �;     K-    waitForClearHPIB                ����         � �     K-    confAAudio                      ����        � n     K-    confAFAN                        ����        2 !, 	    K-    confBER                         ����        $� +�     K-    confDAudio                      ����        -� 6z     Q-    confDynPower                    ����        8 @d     K-    confFastBER                     ����        B� LT 	    K-    confIQTuning                    ����        O� \�     K-    confORFS                        ����        `� d�     K-    confORFSLimitSource             ����        e� nU     K-    confORFSSwitchLimits            ����        pZ x�     K-    confORFSModLimits               ����        z� �n     K-    confPFER                        ����        �� ��     K-    confPFER_CIFT                   ����        �& �� 	    K-    confPowerVsTime                 ����        � ��     K-    confPVTMask                     ����        �� �t     K-    confTXPower                     ����        �� ��     K-    confAutoRange                   ����        �6 �I     K-    measurementTimeout              ����        �� �     K-    readAAudio_Q                    ����        � ��     K.    readAFAN_Q                      ����        �� �b     K-    readBER_Q                       ����        �g �     K-    readDAudio_Q                    ����        � ��     K-    readDynPower_Q                  ����        �] �/     K-    readFBER_Q                      ����        �� G 	    K-    readIQTuning_Q                  ����        � �     K-    readORFS_Q                      ����        � !�     K-    readPFER_Q                      ����        #{ .�     K-    readPowerVsTime_Q               ����        0� ;G 	    K-    PowerVsTimePlot_Q               ����        =� E?     K-    readTXPower_Q                   ����        F� L)     K-    SACCHReport_Q                   ����        ND S�     K-    initMeasFcn                     ����        Uf \     K-    initMeasFcns                    ����        ] c�     K-    waitForInitDone_Q               ����        d� i�     K-    fetchAAudio_Q                   ����        j� qW     K.    fetchAFAN_Q                     ����        r� �o     K-    fetchBER_Q                      ����        �t ��     K-    fetchDAudio_Q                   ����        �� ��     K-    fetchDynPower_Q                 ����        �~ ��     K-    fetchFBER_Q                     ����        �l �� 	    K-    fetchIQTuning_Q                 ����        � �<     K-    fetchORFS_Q                     ����        � �     K-    fetchORFSSummaryPassFail_Q      ����        �@ ̤     K-    fetchPFER_Q                     ����        �# ي     K-    fetchPowerVsTime_Q              ����        ۖ �     K-    fetchTXPower_Q                  ����        � �H     K-    reset                           ����        �� �     K-    self_test                       ����        � �     K-    error_message                   ����        � �H     K-    error_query                     ����        �E ��     K-    revision_query                  ����        �� �     K-    timeOut                         ����        �      K-    timeOut_Q                       ����        �      K-    errorQueryDetect                ����        � _     K-    errorQueryDetect_Q              ����         >     K-    dcl                             ����        � �     K-    opc_Q                           ����        � =     K-    readStatusByte_Q                ����        �  �     K-    stdEvent_Q                      ����        !{ %�     K-    operEvent_Q                     ����        'X +�     K-    operCond_Q                      ����        -< 1�     K-    quesEvent_Q                     ����        3� 8     K-    quesCond_Q                      ����        9� @     K-    quesRegisterEnable              ����        B� H_     K-    operRegisterEnable              ����        J� M�     K-    stdEventEnable                  ����        N� Q�     K-    cmd                             ����        R� W�     K-    cmdString_Q                     ����        Y ]     K-    cmdInt                          ����        ^ a�     K-    cmdInt16_Q                      ����        b� f     K-    cmdInt32_Q                      ����        g	 k|     K-    cmdIntArr                       ����        l� q@     K-    cmdInt16Arr_Q                   ����        r� wM     K-    cmdInt32Arr_Q                   ����        x� |�     K-    cmdReal                         ����        }� �p     K-    cmdReal64_Q                     ����        �g ��     K-    cmdRealArr                      ����        � �U     K-    cmdReal64Arr_Q                  ����        �� �     K-    close                                 �                                     DInitialize                           Application Functions               iCall Processing                     �Link Control                         DSet BA Table                         DSet BCH ARFCN                        DSet Burst Type                       DSet Cell Band                        DSet Cell Power                       DSet Expected Power                   DSet Loopback                         DSet Manual Band                      DSet Manual Channel                   DSet MS Time Advance                  DSet MS TX Level                      DSet Operating Mode                   DSet Receiver Control                 DSet TCH Timeslot                     DSet TCH ARFCN                        DSet TCH Band                         DSet Channel Mode                     DSet FACCH Signaling                  DQuery Burst Timing Error            8Call Control                         DOriginate Call To MS                 DEnd Call with MS                     DConfigure for MS Originate           DWait for Connect                    �System Utilities                     DPerform TA Switch                    DQuery Current TA                     DQuery Current TA Model               DSet Current Active TAP               DQuery Current Active TAP             DIntegrity Message                    DPartial Preset                       DQuery Correction Frequency           DQuery Present Frequency              DQuery UUT Level                      DQuery Amplitude Offset               DSet Amplitude Offset                 DSet Use SRQ MAV                      DQuery Use SRQ MAV                    DWait for Clear GPIB Bus             �Measure                             @Configure Measurement                DConfigure Analog Audio               DConfigure Audio Analyzer             DConfigure Bit Error                  DConfigure Decoded Audio              DConfigure Dynamic Power              DConfigure Fast Bit Error             DConfigure IQ Tuning                  DConfigure Output RF Spectrum         DConfigure ORFS Limit Source          DConfigure ORFS Switching Limit       DConfigure ORFS Modulation Limit      DConfigure Phase Freq Error           DConfigure Phase Freq Error CIFT      DConfigure Power vs Time              DConfigure Power vs Time Mask         DConfigure TX Power                   DConfigure Autorange                  DConfigure Measurement Timeout       �Read                                 DRead Analog Audio                    DRead Audio Analyzer                  DRead Bit Errors                      DRead Decoded Audio                   DRead Dynamic Power                   DRead Fast Bit Errors                 DRead IQ Tuning                       DRead Output RF Spectrum              DRead Phase Freq Error                DRead Power vs Time                   DRead Power vs Time Plot              DRead TX Power                        DRead SACCH Report                   EInitiate                             DInitiate a Measure Function          DInitiate Measure Functions           DQuery Initiate Done                 �Fetch                                DFetch Analog Audio                   DFetch Audio Analyzer                 DFetch Bit Errors                     DFetch Decoded Audio                  DFetch Dynamic Power                  DFetch Fast Bit Errors                DFetch IQ Tuning                      DFetch Output RF Spectrum             DFetch ORFS Pass Fail Summary         DFetch Phase Freq Error               DFetch Power vs Time                  DFetch TX Power                      �Utility                              DReset                                DSelf-Test                            DError Message                        DError Query                          DRevision Query                       DSet Timeout                          DQuery Timeout                        DSet Error Query Detect               DQuery Error Query Detect             DDevice Clear                         D*OPC?                                DRead Status Byte                     DStatus Standard Event                DStatus Operation Event               DStatus Operation Condition           DStatus Questionable Event            DStatus Questionable Condition        DStatus Questionable Enable           DStatus Operation Enable              DStatus Standard Event Enable        mPassthrough Functions                DSend String                          DQuery String                         DSend Integer                         DQuery Int16                          DQuery Int32                          DSend Integer Array                   DQuery Int16 Array                    DQuery Int32 Array                    DSend Real                            DQuery Real64                         DSend Real Array                      DQuery Real64 Array                   DClose                           