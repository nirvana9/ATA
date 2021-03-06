s��        d�   r J�     �   ����                               hp663x2     HP663x2                                       � � ��ViInt16     � ��ViPInt16     
�  	ViInt16 []  �  � ��ViInt32     � ��ViPInt32     
�  	ViInt32 []  � � ��ViReal64     	� 	��ViPReal64     �  
ViReal64 []     � ��ViString     	� 	��ViBoolean     
� 
��ViPBoolean     � 	 ViBoolean []     � ��ViRsrc     	� 	��ViSession     � ��ViStatus     � ��ViPChar     	�  ViChar []   h    hpxxxx  Instrument Driver
Copyright @1997, Hewlett-Packard Company

Put Instrument Description Here.

     �    Utility functions perform several standard P&P tasks and a variety of HP utility tasks.  They also include HP's instrument command passthrough functions which allow generic access to the instrument's command set.
     �    HP's instrument command passthrough functions.

These functions pass commands directly to the instrument, thus allowing generic access to the instrument's command set.
     �    The initialize function initializes the software connection to the instrument and optionally verifies that instrument is in the system.  In addition, it may perform any necessary actions to place the instrument in its reset state.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The instrument's VISA resource description.

The default for this instrument is "GPIB0::5::INSTR".  If you change the HPIB address of this instrument, you must change the default resource description as well.     �    Perform an identity query to verify that the instrument is the one expected by the driver.


Macro Name  Value  Description
--------------------------------
VI_TRUE       1    Do Query
VI_FALSE      0    Skip Query     �    Preform instrument reset operation.

Macro Name  Value  Description
------------------------------
VI_TRUE       1    Reset
VI_FALSE      0    Don't Reset     p    The instrument's VISA session handle.  This is VI_NULL if an error occurred during the initialization process.    �
o���  �    Status                            � 7 .  �  �    Resource Name                     � 7# �       Id Query                          � 7� �       Reset Device                      	% � � �  �    Instrument Handle                  	           GPIB0::5::INSTR  % Do Query VI_TRUE Skip Query VI_FALSE  # Reset VI_TRUE Don't Reset VI_FALSE    	            9    This function queries the power source's output status.        The operating status of the power source.  The following values are returned:

Macro Name                Value  Description
----------------------------------------------------------------
HP663X2_OUTP_STATUS_OFF     0    Output is in the off state.
HP663X2_OUTP_STATUS_CV      1    Output is in Constant Voltage
                                 mode.  The CV bit is set.
HP663X2_OUTP_STATUS_CC      2    the output is in Constant
                                 Current mode.  Either of the
                                 CC+ or CC- bit is set.
HP663X2_OUTP_STATUS_UNREG   3    the output is unregulated.  The
                                 UNREG bit is set.
HP663X2_OUTP_STATUS_PROT    4    Protection has tripped.  Either
                                 of the OV or OCP bit is set.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    ` f � �       Output Status                     � � *  �  �    Instrument Handle                 � �s���  �    Status                             	            0    	           C    This function queries the power source's secondary output status.    �    The operating status of the power source.  The following values are returned:

Macro Name                Value  Description
----------------------------------------------------------------
HP663X2_OUTP_STATUS_OFF     0    Output is in the off state.
HP663X2_OUTP_STATUS_CV      1    Output is in Constant Voltage
                                 mode.  The CV2 bit is set.
HP663X2_OUTP_STATUS_CC      2    Output is in Constant Current
                                 mode.  The CC2 bit is set.
HP663X2_OUTP_STATUS_UNREG   3    Output is unregulated.  The
                                 UNR2 bit is set.
HP663X2_OUTP_STATUS_PROT    4    Protection has tripped.  The
                                 OC2 bit is set.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    � f � �       Output Status                     � � *  �  �    Instrument Handle                 � �s���  �    Status                             	            0    	          �    This function enables the Remote Inhibit signal to ripple through the Discrete Fault Indicator signal.  By chaining the RI/DFI signals of multiple power sources, their outputs can be shutdown by one inhibit signal.

The function configures the digital control port for Remote Inhibit/Discrete Fault Interrupt operation; sets the Remote Inhibit protection to Latching mode;  selects the Questionable summary bit for the discrete fault indicator event and enables the output of the discrete fault signal.  The RI condition in the Questionable status register is also enabled for the Questionable summary bit.  The contents of the Questionable status enable register is not overwritten.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    - ,  �  �    Instrument Handle                 zp���  �    Status                             0    	           �    This function programs the output current level, the output voltage level, and enables the output.  If option 760 is present the output relay will also be enabled.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     9    The immediate output voltage level of the power supply.     9    The immediate output current limit of the power supply.    � $  �  �    Instrument Handle                 �l���  �    Status                            � r � �       Volt Lev                          � r. �       Curr Lev                           0    	           0    0    �    This function programs the secondary output current level, the secondary output voltage level, and enables the secondary output.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     9    The immediate output voltage level of the power supply.     9    The immediate output current limit of the power supply.    !� $  �  �    Instrument Handle                 !�l���  �    Status                            #� r � �       Volt Lev                          $ r. �       Curr Lev                           0    	           0    0   V    This function sets the output voltage or the output current depending on the output parameter selection.  After the output level is set, the function measures the output voltage and current, and reads back the operating status.

Note:  This function reprograms the Status Registers when retrieving the measurement. Specifically, it programs the MAV bit to generate an SRQ (service request) signal when the measurement data is available. If you are using the service request signal in your application, you must reprogram the Service Request Enable register after the completion of this function.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Selects which output parameter to set, voltage or current:

Macro Name      Value   Description
---------------------------------
HP663X2_VOLTAGE   0     set voltage
HP663X2_CURRENT   1     set current
     9    The voltage measured at the output of the power supply.     9    The current measured at the output of the power supply.        The operating status of the power supply.  The following values are returned:

Macro Name                Value  Description
----------------------------------------------------------------
HP663X2_OUTP_STATUS_OFF     0    Output is in the off state.
HP663X2_OUTP_STATUS_CV      1    Output is in Constant Voltage
                                 mode.  The CV bit is set.
HP663X2_OUTP_STATUS_CC      2    the output is in Constant
                                 Current mode.  Either of the
                                 CC+ or CC- bit is set.
HP663X2_OUTP_STATUS_UNREG   3    the output is unregulated.  The
                                 UNREG bit is set.
HP663X2_OUTP_STATUS_PROT    4    Protection has tripped.  Either
                                 of the OV or OCP bit is set.     :    The output voltage or current level of the power supply.     X    Time in milliseconds for the output to change and settle before measurements are made.    '� ,  �  �    Instrument Handle                 '�t���  �    Status                            )� 4 � ��      Output Param                      *� � F �       Volt Q Lev                        *� � � �       Curr Q Lev                        +/ �� �       Output Status                     .P = � �       Output Lev                        .� =� �       Settling Time                      0    	                      0Voltage HP663X2_VOLTAGE Current HP663X2_CURRENT    	           	           	            0    0   ~    This function sets the secondary output voltage or the secondary output current depending on the output parameter selection.  After the secondary output level is set, the function measures the secondary output voltage and current, and reads back the operating status.

Note:  This function reprograms the Status Registers when retrieving the measurement. Specifically, it programs the MAV bit to generate an SRQ (service request) signal when the measurement data is available. If you are using the service request signal in your application, you must reprogram the Service Request Enable register after the completion of this function.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Selects which output parameter to set, voltage or current:

Macro Name      Value   Description
---------------------------------
HP663X2_VOLTAGE   0     set voltage
HP663X2_CURRENT   1     set current
     9    The voltage measured at the output of the power supply.     9    The current measured at the output of the power supply.        The operating status of the power supply.  The following values are returned:

Macro Name                Value  Description
----------------------------------------------------------------
HP663X2_OUTP_STATUS_OFF     0    Output is in the off state.
HP663X2_OUTP_STATUS_CV      1    Output is in Constant Voltage
                                 mode.  The CV bit is set.
HP663X2_OUTP_STATUS_CC      2    the output is in Constant
                                 Current mode.  Either of the
                                 CC+ or CC- bit is set.
HP663X2_OUTP_STATUS_UNREG   3    the output is unregulated.  The
                                 UNREG bit is set.
HP663X2_OUTP_STATUS_PROT    4    Protection has tripped.  Either
                                 of the OV or OCP bit is set.     :    The output voltage or current level of the power supply.     X    Time in milliseconds for the output to change and settle before measurements are made.    3� ,  �  �    Instrument Handle                 3�t���  �    Status                            5� 4 � ��      Output Param                      6� � F �       Volt Q Lev                        6� � � �       Curr Q Lev                        7" �� �       Output Status                     :C = � �       Output Lev                        :� =� �       Settling Time                      0    	                      0Voltage HP663X2_VOLTAGE Current HP663X2_CURRENT    	           	           	            0    0    C    This function provides a delay for the specified amount of time.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     N    Time delay in milliseconds.  Minimum is 0 and maximum is 30000 (30 seconds).    =V  .  �  �    Instrument Handle                 =� v���  �    Status                            ?� t � �       milliseconds                       0    	           0    �    This function controls the front panel display of the instrument.  The display can be programmed to display a customized message, blank screen, or display normal instrument measurements.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    T    The state of the front panel display.This parameter turns the front panel display on or off.  When off, the front panel display is blank.  The display annunciators are not affected by this command.

Macro Name  Value   Description
-------------------------------------
VI_TRUE       1     Display is on
VI_FALSE      0     Display is off
    '    The front panel display mode.  The following display modes
are available for the instrument:

Macro Name          Value   Description
-----------------------------------------------------
HP663X2_DISP_NORMAL   0     Displays instrument functions
HP663X2_DISP_TEXT     1     Displays user text
    ]    A case-sensitive user customized message to be displayed when the display mode is set to TEXT.  The display is capable of displaying up to 14 characters.  Message exceeding 14 characters will be truncated.

Note: to display the previous message text, send a null character string.  To clear current message text, send at least one blank character.     E    The instrument's VISA session handle returned from hp663x2_init().
    ASr���  �    Status                            C4 K ) �       Disp State                        D� K � ��      Disp Mode                         E� K1 �  �    Message Text                      G$ )  �  �    Instrument Handle                  	         ( Display On VI_TRUE Display Off VI_FALSE               2Normal HP663X2_DISP_NORMAL Text HP663X2_DISP_TEXT    "User Message"    0    �    This function queries the operating parameters of the instrument�s front panel display.  These parameters can be changed with the hpxxxx_setDisplay function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The state of the front panel display.

Macro Name  Value   Description
------------------------------------
VI_TRUE       1     Display is on
VI_FALSE      0     Display is off
    ;    The front panel display mode.  The following display modes
are available for the instrument:

Macro Name                Value   Description
----------------------------------------------------------
HP663X2_DISP_NORMAL_MODE    0     Display instrument function
HP663X2_DISP_TEXT_MODE      1     Display user text
     �    This parameter returns the user text message presently stored in the instrument.  An error occurs if no previous message was stored.  The resulting string may be truncated to 14 characters.    I� � !  �  �    Instrument Handle                 I� �i���  �    Status                            K� I 2 �       Disp State                        L� I � �       Disp Mode                         M� If �  �    Message Text                       0    	           	            	            	            D    Sets the channel being displayed on the instrument's front panel.
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     9    The channel to display on the instrument's front panel.    P, � #  �  �    Instrument Handle                 Px �k���  �    Status                            RY q � �       Display Channel                    0    	           1    G    Returns the channel being displayed on the instrument's front panel.
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     1    The display channel returned by the instrument.    S�   �  �    Instrument Handle                 S�f���  �    Status                            U� ] � �       Display Channel                    0    	           	           �    This function immediately starts acquiring new DVM data and returns the selected measurement from this data as soon as the buffer is full.  This is the easiest way to make measurements, since it requires no explicit trigger programming.

Many parameters of the measurement are programmable. These include the number of samples, the time interval between samples, the bandwidth, and the method of triggering.

Notes:
  1. There is a tradeoff between these parameters and the speed,  
     accuracy, and stability of the measurement in the presence
     of noise.
  2. This function reprograms the Status Registers when
     retrieving the measurement. Specifically, it programs the
     MAV bit to generate an SRQ (service request) signal when
     the measurement data is available. If you are using the
     service request signal in your application, you must
     reprogram the Service Request Enable register after the
     completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    (    Specifies the measurement to be calculated.

The DVM measurements available in the dc source are:

Macro Name        Value   Description
---------------------------------------------------------------
HP663X2_MEAS_DC     0     dc input voltage
HP663X2_MEAS_ACDC   1     ac+dc rms input voltage
         Measurement result.
    Z� -  �  �    Instrument Handle                 Z�u���  �    Status                            \� G � �       Measurement                       ]� G9 �       Volt Result                        0    	                      /dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC    	          �    This function immediately starts acquiring new voltage data and returns the selected measurement from this data as soon as the buffer is full.  This is the easiest way to make measurements, since it requires no explicit trigger programming.

Many parameters of the measurement are programmable. These include the number of samples, the time interval between samples, the bandwidth, and the method of triggering.

Notes:
  1. There is a tradeoff between these parameters and the speed,  
     accuracy, and stability of the measurement in the presence
     of noise.
  2. This function reprograms the Status Registers when
     retrieving the measurement. Specifically, it programs the
     MAV bit to generate an SRQ (service request) signal when
     the measurement data is available. If you are using the
     service request signal in your application, you must
     reprogram the Service Request Enable register after the
     completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        Specifies the measurement to be calculated.

The voltage measurements available in the dc source are:

Macro Name        Value   Description
---------------------------------------------------------------
HP663X2_MEAS_DC     0     dc output voltage
HP663X2_MEAS_ACDC   1     ac+dc rms output voltage
HP663X2_MEAS_HIGH   2     High level of a voltage pulse waveform
HP663X2_MEAS_LOW    3     Low level of a voltage pulse waveform
HP663X2_MEAS_MAX    4     Maximum output voltage
HP663X2_MEAS_MIN    5     Minimum output voltage
         Measurement result.
    b� -  �  �    Instrument Handle                 cFu���  �    Status                            e' G � �       Measurement                       g@ G9 �       Volt Result                        0    	                      �dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC High HP663X2_MEAS_HIGH Low HP663X2_MEAS_LOW Max HP663X2_MEAS_MAX Min HP663X2_MEAS_MIN    	          �    Initiates a voltage measurement on the second output and fetches the reading.

Note:
This function reprograms the Status Registers when retrieving the measurement. Specifically, it programs the MAV bit to generate an SRQ (service request) signal when the measurement data is available. If you are using the service request signal in your application, you must reprogram the Service Request Enable register after the completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
         Measurement result.
        Specifies the measurement to be calculated.

The secondary voltage measurements available in the dc source are:

Macro Name        Value   Description
---------------------------------------------------------------
HP663X2_MEAS_DC     0     dc output voltage
    j� -  �  �    Instrument Handle                 j�u���  �    Status                            l� G9 �       Volt Result                       l� R � �       Measurement                        0    	           	           0   �    This function immediately starts acquiring new current data and returns the selected measurement from this data as soon as the buffer is full.  This is the easiest way to make measurements, since it requires no explicit trigger programming.

Many parameters of the measurement are programmable. These include the number of samples, the time interval between samples, the bandwidth, and the method of triggering.

Notes:
  1. There is a tradeoff between these parameters and the speed,
     accuracy, and stability of the measurement in the presence
     of noise.
  2. This function reprograms the Status Registers when
     retrieving the measurement. Specifically, it programs the
     MAV bit to generate an SRQ (service request) signal when
     the measurement data is available. If you are using the
     service request signal in your application, you must
     reprogram the Service Request Enable register after the
     completion of this function.        Specifies the measurement to be calculated.

The current measurements available in the dc source are:

Macro Name        Value   Description
---------------------------------------------------------------
HP663X2_MEAS_DC     0     dc output current
HP663X2_MEAS_ACDC   1     ac+dc rms output current
HP663X2_MEAS_HIGH   2     High level of a current pulse waveform
HP663X2_MEAS_LOW    3     Low level of a current pulse waveform
HP663X2_MEAS_MAX    4     Maximum output current
HP663X2_MEAS_MIN    5     Minimum output current     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
         Measurement result.
    r� B � �       Measurement                       t� � +  �  �    Instrument Handle                 u# �s���  �    Status                            w B/ �       Curr Result                                   �dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC High HP663X2_MEAS_HIGH Low HP663X2_MEAS_LOW Max HP663X2_MEAS_MAX Min HP663X2_MEAS_MIN    0    	           	          �    Initiates a current measurement on the second output and fetches the reading.

Note:
This function reprograms the Status Registers when retrieving the measurement. Specifically, it programs the MAV bit to generate an SRQ (service request) signal when the measurement data is available. If you are using the service request signal in your application, you must reprogram the Service Request Enable register after the completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
         Measurement result.
        Specifies the measurement to be calculated.

The secondary current measurements available in the dc source are:

Macro Name        Value   Description
---------------------------------------------------------------
HP663X2_MEAS_DC     0     dc output current
    zm � +  �  �    Instrument Handle                 z� �s���  �    Status                            |� B/ �       Curr Result                       |� J � �       Measurement                        0    	           	           0   q    This function immediately starts acquiring new voltage data, and returns the digitized data as soon as the buffer is full.  The hp663x2_setSweepParams() function can be used to control the digitization parameters.  A timeout value can be set using the hp663x2_setMeasTimeout() function. The default acquisition timeout is 5 seconds.

Notes:
  1. There is a tradeoff between these parameters and the speed,
     accuracy, and stability of the measurement in the presence
     of noise.
  2. This function reprograms the Status Registers when
     retrieving the measurement. Specifically, it programs the
     MAV bit to generate an SRQ (service request) signal when
     the measurement data is available. If you are using the
     service request signal in your application, you must
     reprogram the Service Request Enable register after the
     completion of this function.     A    An array containing the instantaneous output voltage in volts.
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     c    Number of readings to be retrieved.  This value should not exceed the size of the waveformArray.
    �4 � � �       Volt Array                        �} (  �  �    Instrument Handle                 ��p���  �    Status                            �� L � �       Num Readings                       	            0    	           2048   �    This function immediately starts acquiring new current data, and returns the digitized data as soon as the buffer is full.  The hp663x2_setSweepParams() function can be used to control the digitization parameters. A timeout value can be set using the hp663x2_setMeasTimeout() function. The default acquisition timeout is 5 seconds.



Hewlett-Packard HP 663x2A Instrument Driver Help
Notes:
  1. There is a tradeoff between these parameters and the speed,
     accuracy, and stability of the measurement in the presence
     of noise.
  2. This function reprograms the Status Registers when
     retrieving the measurement. Specifically, it programs the
     MAV bit to generate an SRQ (service request) signal when
     the measurement data is available. If you are using the
     service request signal in your application, you must
     reprogram the Service Request Enable register after the
     completion of this function.     B    An array containing the instantaneous output current in amperes.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     b    Number of readings to be retrieved.  This value should not exceed the size of the waveformArray.    �� � � �       Curr Array                        � � '  �  �    Instrument Handle                 �O �q���  �    Status                            �0 A � �       Num Readings                       	            0    	           2048   �    This function performs the selected DVM measurement calculation from the previous acquired DVM data.

Note:
  This function reprograms the Status Registers when
  retrieving the measurement. Specifically, it programs the
  MAV bit to generate an SRQ (service request) signal when
  the measurement data is available. If you are using the
  service request signal in your application, you must
  reprogram the Service Request Enable register after the
  completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Specifies the DVM measurement to be calculated.

The DVM measurements available in the dc source are:

Macro Name       Value   Description
----------------------------------------------------------------
HP663X2_MEAS_DC    0     dc voltage measurement
HP663X2_MEAS_ACDC  1     ac+dc rms voltage measurement

Note: The measurement that can be selected is dependent on the sensing function of the previous measurement.  i.e. if a DVM measurement was made, then only DVM related measurement can be fetched.
         Measurement result.    �~ � #  �  �    Instrument Handle                 �� �l���  �    Status                            �� I � �       Measurement                       �� I: �       Volt Result                        0    	                      /dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC    	          �    This function performs the selected voltage measurement calculation from the previous acquired voltage data.

Note:
  This function reprograms the Status Registers when
  retrieving the measurement. Specifically, it programs the
  MAV bit to generate an SRQ (service request) signal when
  the measurement data is available. If you are using the
  service request signal in your application, you must
  reprogram the Service Request Enable register after the
  completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        Specifies the voltage measurement to be calculated.

The voltage measurements available in the dc source are:

Macro Name       Value   Description
----------------------------------------------------------------
HP663X2_MEAS_DC    0     dc voltage measurement
HP663X2_MEAS_ACDC  1     ac+dc rms voltage measurement
HP663X2_MEAS_HIGH  2     High level of a voltage pulse waveform
HP663X2_MEAS_LOW   3     Low level of a voltage pulse waveform
HP663X2_MEAS_MAX   4     Maximum value of a voltage waveform
HP663X2_MEAS_MIN   5     Minimum value of a voltage waveform

Note: The measurement that can be selected is dependent on the sensing function of the previous measurement.  i.e. if a voltage measurement was made, then only voltage related measurement can be fetched.
         Measurment result.    �� � #  �  �    Instrument Handle                 �9 �l���  �    Status                            � I � �       Measurement                       �& I: �       Volt Result                        0    	                      �dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC High HP663X2_MEAS_HIGH Low HP663X2_MEAS_LOW Max HP663X2_MEAS_MAX Min HP663X2_MEAS_MIN    	          �    This function performs the selected current measurement calculation from the previous acquired current data.

Note:
  This function reprograms the Status Registers when
  retrieving the measurement. Specifically, it programs the
  MAV bit to generate an SRQ (service request) signal when
  the measurement data is available. If you are using the
  service request signal in your application, you must
  reprogram the Service Request Enable register after the
  completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    :    Specifies the current measurement to be calculated.

The current measurements available in the dc source are:

Macro Name        Value   Description
--------------------------------------------------------------
HP663X2_MEAS_DC     0     dc voltage measurement
HP663X2_MEAS_ACDC   1     ac+dc rms voltage measurement
HP663X2_MEAS_HIGH   2     High level of a voltage pulse waveform
HP663X2_MEAS_LOW    3     Low level of a voltage pulse waveform
HP663X2_MEAS_MAX    4     Maximum value of a voltage waveform
HP663X2_MEAS_MIN    5     Minimum value of a voltage waveform

Hewlett-Packard HP 663x2A Instrument Driver Help
Note: The measurement that can be selected is dependent on the sensing function of the previous measurement.  i.e. if a current measurement was made, then only current related measurement can be fetched.
         Measurment result.    ��  3  �  �    Instrument Handle                 � ����  �    Status                            �� X � �       Measurement                       �) X  �       Curr Result                        0    	                      �dc HP663X2_MEAS_DC ac+dc rms HP663X2_MEAS_ACDC High HP663X2_MEAS_HIGH Low HP663X2_MEAS_LOW Max HP663X2_MEAS_MAX Min HP663X2_MEAS_MIN    	          �    This function retrieves the digitized data of the previous voltage measurement.

Note:
  This function reprograms the Status Registers when
  retrieving the measurement. Specifically, it programs the
  MAV bit to generate an SRQ (service request) signal when
  the measurement data is available. If you are using the
  service request signal in your application, you must
  reprogram the Service Request Enable register after the
  completion of this function.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     A    An array containing the instantaneous output voltage in volts.
    B    The maximum number of readings to retrieve.  If numReadings is greater than the sweep size, then only the number of readings in the sweep size are returned.  If the sweep size is greater than numReadings, then the only numReadings are returned.

Note: The waveformArray size must be equal to or greater than numReadings
    �� +  �  �    Instrument Handle                 ��	u���  �    Status                            �� � � �       Volt Array                        � Q � �       Num Readings                       0    	           	            2048   �    This function retrieves the digitized data of the previous current measurement.

Note:
  This function reprograms the Status Registers when
  retrieving the measurement. Specifically, it programs the
  MAV bit to generate an SRQ (service request) signal when
  the measurement data is available. If you are using the
  service request signal in your application, you must
  reprogram the Service Request Enable register after the
  completion of this function.     D    An array containing the instantaneous output current in amperes.

     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    =    The maximum number of readings to retrieve.  If numReadings is greater than the sweep size, then only the number of readings in the sweep size are returned.  If the sweep size is greater than numReadings, then the numReadings are returned.

Note: The waveformArray size must be equal to or greater than numReadings
    �/ � � �       Curr Array                        �{ � '  �  �    Instrument Handle                 �� �o���  �    Status                            �� F � �       Num Readings                       	            0    	           2048       This function sets the parameters of the DVM acquisition trigger system.  The sense function is set to DVM acquisition. The acquisition trigger system is not armed.  It must be armed with the hp663x2_arm() function before it can receive any acquisition trigger.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        The trigger source of the acquisition system.


Macro Name            Value   Description
-------------------------------------------
HP663X2_TRIG_SRC_BUS    0     Bus
HP663X2_TRIG_SRC_INT    1     Internal

Note: this parameter is applied to voltage, current, and DVM acquisitions.
    v    Specify the number of DVM acquisition triggers in succession. Programming a trigger count saves you from having to send this function  each time you send another trigger.

Macro Name             Value  Description
-----------------------------------------------------
HP663X2_ACQ_COUNT_MIN    1    Minimum trigger count
HP663X2_ACQ_COUNT_MAX   100   Maximum trigger count
    H    Specify the DVM acquisition trigger slope of an internally triggered acquisition.

Macro Name                Value   Description
---------------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     G    Specify the DVM trigger level of an internally triggered acquisition.     Q    Specify the DVM trigger hysteresis band of an internally triggered acquisition.    �� � $  �  �    Instrument Handle                 �A �l���  �    Status                            �" A p ��      Acq Trig Source                   �G A � �       Acq Trig DVM Count                �� A� ��      Acq Trig DVM Slope                � � � �       Acq Trig DVM Lev                  �d �$ �       Acq Trig DVM Hyst                  0    	                     2BUS HP663X2_TRIG_SRC_BUS INT HP663X2_TRIG_SRC_INT    1               WEITHER HP663X2_TRIG_SLOPE_EITHER POS HP663X2_TRIG_SLOPE_POS NEG HP663X2_TRIG_SLOPE_NEG    0    0    O    Query the configuration parameters of the internal DVM-triggered acquisition.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The trigger source of the acquisition subsystem.

Macro Name            Value   Description
-----------------------------------------
HP663X2_TRIG_SRC_BUS    0     bus
HP663X2_TRIG_SRC_INT    1     internal
     7    The number of DVM acquisition triggers in succession.    0    The trigger slope of an internal DVM-triggered acquisition.

Macro Name                Value   Description
-------------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     =    The trigger level of an internal DVM-triggered acquisition.     G    The trigger hysteresis band of an internal DVM-triggered acquisition.    �N �   �  �    Instrument Handle                 �� �g���  �    Status                            �{ F 9 �       Acq Trig Source                   �T H � �       Acq Trig DVM Count                �� H� �       Acq Trig DVM Slope                �� � � �       Acq Trig DVM Lev                  � �: �       Acq Trig DVM Hyst                  0    	           	            	            	            	           	              This function sets the parameters of the voltage acquisition trigger system.  The sense function is set to voltage acquisition. The acquisition trigger system is not armed.  It must be armed with the hp663x2_arm() function before it can receive any acquisition trigger.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        The trigger source of the acquisition system.


Macro Name            Value   Description
-------------------------------------------
HP663X2_TRIG_SRC_BUS    0     Bus
HP663X2_TRIG_SRC_INT    1     Internal

Note: this parameter is applied to voltage, current, and DVM acquisitions.
    z    Specify the number of voltage acquisition triggers in succession. Programming a trigger count saves you from having to send this function  each time you send another trigger.

Macro Name             Value  Description
-----------------------------------------------------
HP663X2_ACQ_COUNT_MIN    1    Minimum trigger count
HP663X2_ACQ_COUNT_MAX   100   Maximum trigger count
    L    Specify the voltage acquisition trigger slope of an internally triggered acquisition.

Macro Name                Value   Description
---------------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     K    Specify the voltage trigger level of an internally triggered acquisition.     U    Specify the voltage trigger hysteresis band of an internally triggered acquisition.    �6 � $  �  �    Instrument Handle                 Ƃ �l���  �    Status                            �c A p ��      Acq Trig Source                   Ɉ A � �       Acq Trig Volt Count               �
 A� ��      Acq Trig Volt Slope               �^ � � �       Acq Trig Volt Lev                 ̱ �$ �       Acq Trig Volt Hyst                 0    	                     2BUS HP663X2_TRIG_SRC_BUS INT HP663X2_TRIG_SRC_INT    1               WEITHER HP663X2_TRIG_SLOPE_EITHER POS HP663X2_TRIG_SLOPE_POS NEG HP663X2_TRIG_SLOPE_NEG    0    0    S    Query the configuration parameters of the internal voltage-triggered acquisition.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The trigger source of the acquisition subsystem.

Macro Name            Value   Description
-----------------------------------------
HP663X2_TRIG_SRC_BUS    0     bus
HP663X2_TRIG_SRC_INT    1     internal
     ;    The number of voltage acquisition triggers in succession.    4    The trigger slope of an internal voltage-triggered acquisition.

Macro Name                Value   Description
-------------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     A    The trigger level of an internal voltage-triggered acquisition.     K    The trigger hysteresis band of an internal voltage-triggered acquisition.    ϣ �   �  �    Instrument Handle                 �� �g���  �    Status                            �� F 9 �       Acq Trig Source                   ҩ H � �       Acq Trig Volt Count               �� H� �       Acq Trig Volt Slope               �( � � �       Acq Trig Volt Lev                 �q �: �       Acq Trig Volt Hyst                 0    	           	            	            	            	           	          
    This function sets the parameters of the acquisition trigger system.  The sense function is set to current acquisition. The acquisition trigger system is not enabled.  It must be armed with the hp663x2_arm() function before it can receive any acquisition trigger.      D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        The trigger source of the acquisition system.

Macro Name            Value   Description
---------------------------------------
HP663X2_TRIG_SRC_BUS    0     Bus
HP663X2_TRIG_SRC_INT    1     Internal 

Note: this parameter is applied to voltage, current, and DVM acquisitions.
    }    Specifies the number of current acquisition triggers in succession.  Programming a trigger count saves you from having to send this function  each time you send another trigger.

Macro Name             Value  Description
-----------------------------------------------------
HP663X2_ACQ_COUNT_MIN    1    Minimum trigger count
HP663X2_ACQ_COUNT_MAX   100   Maximum trigger count
    G    Specify the current acquisition trigger slope of an internally triggered acquisition.

Macro Name                Value   Description
----------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     K    Specify the current trigger level of an internally triggered acquisition.     U    Specify the current trigger hysteresis band of an internally triggered acquisition.    ז  #  �  �    Instrument Handle                 �� k���  �    Status                            �� L v ��      Acq Trig Source                   �� L � �       Acq Trig Curr Count               �i L� ��      Acq Trig Curr Slope               ݸ � � �       Acq Trig Curr Lev                 � �* �       Acq Trig Curr Hyst                 0    	                     2BUS HP663X2_TRIG_SRC_BUS INT HP663X2_TRIG_SRC_INT    1               WEITHER HP663X2_TRIG_SLOPE_EITHER POS HP663X2_TRIG_SLOPE_POS NEG HP663X2_TRIG_SLOPE_NEG    0    0    S    Query the configuration parameters of the internal current-triggered acquisition.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The trigger source of the acquisition subsystem.

Macro Name            Value   Description
---------------------------------------
HP663X2_TRIG_SRC_BUS    0     Bus
HP663X2_TRIG_SRC_INT    1     Internal 
     =    The number of current acquisition triggers in succession.      4    The trigger slope of an internal current-triggered acquisition.

Macro Name                Value   Description
-------------------------------------------------
HP663X2_TRIG_SLOPE_EITHER   0     Either slope
HP663X2_TRIG_SLOPE_POS      1     Positive slope
HP663X2_TRIG_SLOPE_NEG      2     Negative slope
     I    The trigger current level of an internal current-triggered acquisition.     K    The trigger hysteresis band of an internal current-triggered acquisition.    ��   �  �    Instrument Handle                 �Il���  �    Status                            �* = 7 �       Acq Trig Source                   � ? � �       Acq Trig Curr Count               �G ?� �       Acq Trig Curr Slope               � � � �       Acq Trig Curr Lev                 �� �8 �       Acq Trig Curr Hyst                 0    	           	            	            	            	           	              This function sets the sweep parameters of a waveform measurement acquisition.

Dc voltage and current are measured by acquiring a number of readings at a specified time interval. Ripple rejection is a function of the number of cycles of the ripple frequency contained in the acquisition window.  More cycles in the acquisition window results in better ripple rejection.  Increasing the sample interval will increase the number of cycles in the acquisition, but it also increases the acquisition time.

The speed of the acquisition can be increased by reducing the number of sample points.  However, the tradeoff is reduced measurement accuracy.

At power-on and after a reset function, the following parameters are set:
1. sweep interval - 15.6E-6 
2. sweep size - 2048
3. offset - 0     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        Number of points in an acquisition.

Macro Name                Value  Description
----------------------------------------------------------
HP663X2_SWEEP_SIZE_MAX    4096   Maximum number of samples
HP663X2_SWEEP_SIZE_MIN    1      Minimum number of samples
    X    Offset of the beginning of the acquisition buffer relative to the acquisition trigger.

When a measurement is initiated, the dc source continuously samples either the instantaneous output voltage or current.  The start of data that is being read into the acquisition buffer is referenced to the acquisition trigger.  This permits pre-event or post-event data sampling.

The offset to the beginning of the acquisition buffer relative to the acquisition trigger is specified by sample points.  The range for the offset is -4096 to 2E+9 (2,000,000,000) points.

When the offset is negative, the data at the beginning of the data buffer represent samples taken prior to the trigger.  When the offset is 0, all of the data are taken after the trigger.

For offsets greater than zero, the data is taken after a time delay from the receipt of the trigger.  (Time delay = Offset * Sample period)

Macro Name                  Value        Description
-------------------------------------------------------
HP663X2_SWEEP_OFFSET_MAX    2000000000   Maximum offset
HP663X2_SWEEP_OFFSET_MIN    -4095        minimum offset
    
    Time interval between samples in seconds.

Macro                   Value     Description
-------------------------------------------------------------
HP663X2_SWEEP_INT_MIN    15.6e-6  Minimum sweep interval
HP663X2_SWEEP_INT_MAX    31200    Maximum sweep interval    � � %  �  �    Instrument Handle                 �N �m���  �    Status                            �/ a � �      Sweep Size                        �< a� �      Sweep Offset                      � a C �       Sample Interval                    0    	                                w5� ���               15.6e-6    P    This function sets the sweep parameters of a waveform measurement acquisition.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
         Time period between samples.
     &    Number of points in an acquisition.
     Y    Offset of the beginning of the acquisition buffer relative to the acquisition trigger.
    �Y � +  �  �    Instrument Handle                 �� �s���  �    Status                            �� [ ( �       Sample Interval                   �� [ � �       Sweep Size                        �� [� �       Sweep Offset                       0    	           	           	            	           �    This function cancels any trigger actions presently in process. Pending trigger levels are reset to their corresponding immediate values. hp663x2_abort() also resets the WTG bit in the Operation Condition Status register (see Programming the Status Registers). If the trigger system has been programmed to continuous, it will initiates itself immediately after hp663x2_abort(), thereby setting WTG. hp663x2_abort() is also executed at power turn on.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �E (  �  �    Instrument Handle                 �� �p���  �    Status                             0    	          �    This function sets the timeout for a measurement acquisition.  The timeout is used for terminating a measurement if the measurement trigger is not received or if the sweep parameter is improperly set.  The time required for a measurement acquisition is determined by the sweep size, the sample interval and the sweep offset.  If the timeout is too short for the acquisition, the measurement function will return with a HP663X2_INSTR_ERROR_MEAS_TMO error status.

The measurement timeout applies to all measurement functions:

hp663x2_measureVolt()
hp663x2_measureCurr()
hp663x2_measureVoltArray()
hp663x2_measureCurrArray()
hp663x2_fetchVolt()
hp663x2_fetchCurr()
hp663x2_fetchVoltArray()
hp663x2_fetchCurrArray()
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    }    The acquisition timeout value in milliseconds.

Note: Although zero is a valid value, it is usually not applicable to most measurements. Use a reasonable timeout value.

Macro Name          Value       Description
-----------------------------------------------------   
HP663X2_TIMEOUT_MIN 0           Minimum timeout value
HP663X2_TIMEOUT_MAX 2147483647  Maximum timeout value
    �
 '  �  �    Instrument Handle                 o���  �    Status                           � o � �       Meas Timeout                       0    	           0    7    This function queries the measurement timeout value.
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     1    The acquisition timeout value in milliseconds.
   g +  �  �    Instrument Handle                � �s���  �    Status                           � o � �       Meas Timeout                       0    	           	           V    This function clears the latch that disables the output when an Over-Voltage, Over-Current, Over-Temperature, Remote Inhibit, or Fuse Status condition is detected. All conditions that generate the fault must be removed before the latch can be cleared. The output is then  restored to the state it was in before the fault condition occurred.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   	� � !  �  �    Instrument Handle                
3 �i���  �    Status                             0    	               This function controls the polarity of the output relay contacts.  This function is only valid for units with Option 760, otherwise an error will occur.

If output is ON when this function is invoked, the output voltage is set to 0 during the time that the relays are changing polarity.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Programming Normal causes the output relay polarity to be the same as the instrument output.  Programming Reverse causes the relay output polarity to be opposite to that of the instrument output.

Macro Name                      Value  Description
---------------------------------------------------------------
HP663X2_RELAY_POLARITY_NORMAL     0    Normal output polarity
HP663X2_RELAY_POLARITY_REVERSE    1    Reverse output polarity   � %  �  �    Instrument Handle                m���  �    Status                           � X ��      Relay Polarity                     0    	                      LNormal HP663X2_RELAY_POLARITY_NORMAL Reverse HP663X2_RELAY_POLARITY_REVERSE    �    This function returns the present polarity setting of the output relay contacts.  This function is only valid for units with Option 760, otherwise an error will occur.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        The polarity of the output relay contacts.

Macro Name                      Value  Description
---------------------------------------------------------------
HP663X2_RELAY_POLARITY_NORMAL     0    Normal output polarity
HP663X2_RELAY_POLARITY_REVERSE    1    Reverse output polarity   ^ � &  �  �    Instrument Handle                � �`���  �    Status                           � \ � �       Relay Polarity                     0    	           	           3    This function enables or disables the output of the instrument. The state of the disabled outputs is a condition of zero output voltage and current at the output terminals.

This function also controls the Option 760 output relay.  The Switch Relay parameter specifies if the output relay is also changed.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Output state of the unit.

Macro Name  Value   Description
--------------------------------------
VI_TRUE       1     Enable the output
VI_FALSE      0     Disable the output
        Option 760 output relay control.
         
Macro Name  Value   Description
---------------------------------------------------------------
VI_TRUE       1     Output relay is switched with the output
VI_FALSE      0     Output relay is NOT switched with the output
   � � (  �  �    Instrument Handle                � �o���  �    Status                           � j � �       Output State                     � j= �       Switch Relay                       0    	           ON VI_TRUE OFF VI_FALSE    YES VI_TRUE NO VI_FALSE    5    Queries the present output state of the instrument.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     |    Present state of the instrument�s output.

If VI_TRUE (1), the output is enabled.
If VI_FALSE (0), the output is disabled.   � � (  �  �    Instrument Handle                E �p���  �    Status                            & T � �       Output State                       0    	           	            o    This function selects the power-on state of the instrument. This information is saved in non-volatile memory.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
        Specifies the power-on state of the instrument. The following states can be selected:

*RST - Sets the power-on state to *RST. Refer to the *RST command in chapter 4 of the Programming Guide for information on the reset state of all programmable parameters.

RCL0 - Sets the power-on state to *RCL 0. This is the state stored in memory location 0 of the instrument.

Macro Name            Value   Description
-----------------------------------------
HP663X2_PON_STATE_RST   0     *RST
HP663X2_PON_STATE_RCL0  1     RCL0
   !� � %  �  �    Instrument Handle                ") �l���  �    Status                           $
 R ��      Pon State                          0    	                      7*RST HP663X2_PON_STATE_RST RCL0 HP663X2_PON_STATE_RCL0    E    This function queries the power-on state setting of the instrument.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Power-on state recall setting.  The following values are returned:

Macro Name            Value   Description
-----------------------------------------
HP663X2_PON_STATE_RST   0     *RST
HP663X2_PON_STATE_RCL0  1     RCL0   '` )  �  �    Instrument Handle                '�q���  �    Status                           )� K � �       Pon State                          0    	           	           2    This function opens or closes the output relay contacts.  It is only valid for units with Option 760, otherwise an error will occur. The relay is controlled independently of the output state.  If the  instrument is supplying power to a load, that power will appear at the relay contacts during switching.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Specify the desire relay contact state.

If VI_TRUE (1), closes the output relay contacts;
If VI_FALSE (0), opens the output relay contacts.   ,j $  �  �    Instrument Handle                ,�l���  �    Status                           .� `  �       Relay State                        0    	           Close VI_TRUE Open VI_FALSE    �    This function queries the state of the output relay contacts.  It is only valid for units with Option 760, otherwise an error will occur. The relay state is independently of the output state.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    State of the output relay contact.

VI_TRUE (1), indicates the output relay contacts are closed;
VI_FALSE (0), indicates the contacts are opened.   0� +  �  �    Instrument Handle                1s���  �    Status                           2� T � �       Relay State                        0    	           	           =    This function configures the digital control port for Remote Inhibit/Discrete Fault Interrupt (RI/DFI) operation and sets the RI/DFI parameters to the specified conditions. The parameters are saved in non-volatile memory.

Note:  The application must also program the Enable Register mask for each of the DFI source     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    This parameter selects the mode of operation of the Remote Inhibit protection.  This parameter is stored in non--volatile memory. The following modes can be selected:

Latching - A TTL low at the INH input latches the output in the
           protection shutdown state, which can only be cleared
           by the hp663x2_clearProt() function.
Live     - The output state follows the state of the INH input.
           A TTL low at the INH input turns the output off; a
           TTL high returns the output to its programmed state.
Off      - The instrument ignores the INH input.

Macro Name        Value   Description
------------------------------------
HP663X2_RI_OFF      0     Off
HP663X2_RI_LATCHING 1     Latching
HP663X2_RI_LIVE     2     Live
    -    Specify if the discrete fault indicator signal is enabled at the digital control port.

The discrete fault indicator (DFI) is a chassis-referenced, open-collector logic signal connected to the rear panel FLT connection, that can be used to signal external devices when a fault condition is detected.

This parameter is not stored in non-volatile memory.  It is defaulted to Disable after an instrument reset (*RST).

Macro Name  Value   Description
----------------------------------
VI_TRUE       1     DFI is enabled
VI_FALSE      0     DFI is disabled
    �    Specify the source of the DFI signal.

The internal sources that drive the discrete fault indicator signal are:
Questionable event summary bit - bit 3 of the Status Byte Register.
Operation Event summary bit - bit 7 of the Status Byte Register.
Standard Event summary bit - bit 5 of the Status Byte Register.
The Request for Service bit -  bit 6 of the Status Byte Register.
None - No discrete fault source is selected.  This has the same effect as disabling DFI. 


This parameter is not stored in non-volatile memory.  It is defaulted to Off after an instrument reset (*RST).

Macro Name            Value  Description
--------------------------------------------------------
HP663X2_DFI_SRC_PREV    0    Previous setting
HP663X2_DFI_SRC_QUES    1    Questionable summary bit
HP663X2_DFI_SRC_OPER    2    Operation summary bit
HP663X2_DFI_SRC_ESB     3    Standard event summary bit
HP663X2_DFI_SRC_RQS     4    Request for service bit
HP663X2_DFI_SRC_OFF     5    Off
   5� � &  �  �    Instrument Handle                5� �h���  �    Status                           7� S � ��      Ri Mode                          :� S � �       Dfi State                        <� S� ��      Dfi Source Bit                     0    	                      EOFF HP663X2_RI_OFF LATCHING HP663X2_RI_LATCHING LIVE HP663X2_RI_LIVE    ON VI_TRUE OFF VI_FALSE               �Previous setting HP663X2_DFI_SRC_PREV Questionable summary bit HP663X2_DFI_SRC_QUES Operation summary bit HP663X2_DFI_SRC_OPER Standard event summary bit HP663X2_DFI_SRC_ESB Request for service bit HP663X2_DFI_SRC_RQS Off HP663X2_DFI_SRC_OFF    @    This function queries the RI/DFI settings of the power source.     �    Remote inhibit operating mode.

Macro Name        Value   Description
-------------------------------------
HP663X2_RI_OFF      0     Off
HP663X2_RI_LATCHING 1     Latching
HP663X2_RI_LIVE     2     Live
     �    Discrete fault indicator operation state.

Macro Name  Value   Description
-----------------------------------
VI_TRUE       1     DFI is enabled
VI_FALSE      0     DFI is disabled
    o    Source of discrete faults.

Macro Name            Value   Description
----------------------------------------------------
HP663X2_DFI_SRC_QUES    1   Questionable summary bit
HP663X2_DFI_SRC_OPER    2   Operation summary bit
HP663X2_DFI_SRC_ESB     3   Standard event summary bit
HP663X2_DFI_SRC_RQS     4   Request for service bit
HP663X2_DFI_SRC_OFF     5   Off
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   C� < . �       Ri Mode                          Di < � �       Dfi State                        E) <� �       Dfi Source Bit                   F� � (  �  �    Instrument Handle                F� �p���  �    Status                             	            	            	            0    	           H    This function sets the trigger source setting of the output subsystem.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    This parameter specifies the trigger source of the output subsystem.  The HP 663x2 insturment supports BUS trigger only.  Therefore, the only valid value is HP663X2_TRIG_SRC_BUS.  It is also the default setting.  It is un-necessary to program the output trigger source.

Macro Name            Value   Description
--------------------------------------
HP663X2_TRIG_SRC_BUS    0     Bus
   J[ � %  �  �    Instrument Handle                J� �m���  �    Status                           L� l � �      Outp Trig Source                   0    	                      BUS HP663X2_TRIG_SRC_BUS    K    This function queries the trigger source setting of the output subsystem.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Trigger source of the output subsystem.  The HP 663X2 supports only one trigger source and it is set for BUS only.  This parameter always return HP663X2_TRIG_SRC_BUS.   O? � -  �  �    Instrument Handle                O� �u���  �    Status                           Ql ` � �       Outp Trig Source                   0    	           	            G    Query the sensor type configured for the present acquisition trigger.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The present measurement sensing type.

Macro Name      Value  Description
-------------------------------------------
HP663X2_VOLTAGE   0    Voltage sensing
HP663X2_CURRENT   1    Current sensing
HP663X2_DVM       2    DVM sessing
   S' � (  �  �    Instrument Handle                Ss �p���  �    Status                           UT a � �       Sense Function                     0    	           	            G    This function sets the current sensing range and detector parameters.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The dc current measurement range.  The value that you program must be the maximum current that you expect to measure. The instrument will select the range that gives the best resolution.  The crossover value is 20 mA.        Select the type of detector used for output current measurements.  Two choices for detecting current measurements are available:

ACDC is the preferred choice for all dynamic current measurements. When ACDC is selected, the measured output current includes the current that flows in the instrument's output capacitor. It is especially important to use ACDC detection when measuring pulse or other waveforms with frequency contents greater than several kilohertz.

DC is used only if you are making dc current measurements and you require a dc measurement offset accuracy better than 2mA on the High current measurement range. When DC is selected, the component of output current that is supplied by the instrument's output filter is not sensed.

Note that this selections gives inaccurate results on current waveforms with frequency contents greater than several kilohertz.

Macro Name            Value  Description
--------------------------------------------
HP663X2_CURR_DET_DC     1    DC
HP663X2_CURR_DET_ACDC   0    AC+DC   WP 0  �  �    Instrument Handle                W�b���  �    Status                           Y} b v �       Curr Sense Range                 Z` bt ��      Curr Sense Detector                0    	           0              3DC HP663X2_CURR_DET_DC AC+DC HP663X2_CURR_DET_ACDC    :    Query the current sensing range and detector parameters.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     #    The current sense range setting.
     %    The current sense detector setting.   _� ,  �  �    Instrument Handle                `'t���  �    Status                           b f � �       Curr Sense Range                 b3 fJ �       Curr Sense Detector                0    	           	           	            �    This function sets the window function that is used in output measurement calculations.

Note: Neither window function alters the instantaneous voltage or current data returned in the measurement array.
     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Select the following windowing function:

The Hanning window.  A signal conditioning window that reduces errors in dc and rms measurement calculation in the presence of periodic signals such as line ripple. It also reduces jitter when measuring successive pulse waveforms.  The Hanning window multiplies each point in the measurement sample by the function cos()**4.  Do not use the Hanning window when measuring single-shot pulse waveforms.

The rectangular window.  A window that returns measurement calculation without any signal conditioning.  This window may be used for pulse measurements where the exact period of the pulse waveform is known and the measurement interval can be set accordingly using the hp663x2_setSweepParam function.

Macro Name              Value   Description
------------------------------------------
HP663X2_SENSE_WIND_RECT   0     Rectangular
HP663X2_SENSE_WIND_HANN   1     Hanning   d2 � ,  �  �    Instrument Handle                d~ �t���  �    Status                           f_ Y( ��      Window Type                        0    	                     DRectangular HP663X2_SENSE_WIND_RECT Hanning HP663X2_SENSE_WIND_HANN    d    This function queries the window function setting that is used in output measurement calculations.     D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    The windowing function type.

Macro Name              Value   Description
------------------------------------------
HP663X2_SENSE_WIND_RECT   0     Rectangular
HP663X2_SENSE_WIND_HANN   1     Hanning   kj � (  �  �    Instrument Handle                k� �p���  �    Status                           m� e � �       Window Type                        0    	           	            D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   o% %  �  �    Instrument Handle                oqm���  �    Status                          ���� W � �       Sense Protection                   0    	          On VI_TRUE Off VI_FALSE    D    The instrument's VISA session handle returned from hp663x2_init().    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   r -  �  �    Instrument Handle                riu���  �    Status                          ���� Y � �       Sense Protection                   0    	           	            U    This function directly programs the selected current parameter of the power supply.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Specify the current parameter to be programmed.

Macro Name        Value  Description
-------------------------------------------------
HP663X2_CURR_IMM    0    Immediate Current Level
HP663X2_CURR_TRIG   1    Triggered Current Level     %    The current level to be programmed.   uc � !  �  �    Instrument Handle                u� �i���  �    Status                           w� S � ��      Curr Param                       x� S< �       Curr Lev                           0    	                      SImmediate Current Level HP663X2_CURR_IMM Triggered Current Level HP663X2_CURR_TRIG    0    I    This function queries the programmed current level of the power supply.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    ?    Select the current parameter to be queried.

Macro Name        Value  Description
-------------------------------------------------
HP663X2_CURR_Q      0   Current level/limit
HP663X2_CURR_Q_TRIG 1   Triggered current level
HP663X2_CURR_Q_MAX  2   Maximum output current
HP663X2_CURR_Q_MIN  3   Minimum output current         The programmed current level.   zN %  �  �    Instrument Handle                z�m���  �    Status                           || W ��      Curr Q Param                     }� WH �       Curr Lev                           0    	                      �Current Level/Limit HP663X2_CURR_Q Triggered Current Level HP663X2_CURR_Q_TRIG Maximum Output Current HP663X2_CURR_Q_MAX Minimum Output Current HP663X2_CURR_Q_MIN    	           e    This function directly programs the selected current parameter of the power supply's second output.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Specify the current parameter to be programmed.

Macro Name         Value  Description
--------------------------------------------------
HP663X2_CURR2_IMM    0    Immediate Current Level
HP663X2_CURR2_TRIG   1    Triggered Current Level     %    The current level to be programmed.   � � 2  �  �    Instrument Handle                �G �i���  �    Status                           �( S � ��      Curr Param                       � S< �       Curr Lev                           0    	                      UImmediate Current Level HP663X2_CURR2_IMM Triggered Current Level HP663X2_CURR2_TRIG    0    W    This function queries the programmed current level of the instrument's second output.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Select the current parameter to be queried.

Macro Name               Value  Description
----------------------------------------------------------------
HP663X2_CURR2_Q            0    Current level/limit
HP663X2_CURR2_Q_MAX        1    Maximum output current
HP663X2_CURR2_Q_MIN        2    Minimum output current
HP663X2_CURR2_Q_TRIG       3    Triggered current level
HP663X2_CURR2_Q_TRIG_MAX   4    Maximum triggered current level
HP663X2_CURR2_Q_TRIG_MIN   5    Minimum triggered current level

         The programmed current level.   �� %  �  �    Instrument Handle                �Fm���  �    Status                           �' W ��      Curr Q Param                     �& WH �       Curr Lev                           0    	                     Current Level/Limit HP663X2_CURR_Q Maximum Output Current HP663X2_CURR_Q_MAX Minimum Output Current HP663X2_CURR_Q_MIN Triggered Current Level HP663X2_CURR_Q_TRIG Maximum Triggered Current Level HP663X2_CURR_Q_TRIG_MAX Minimum Triggered Current Level HP663X2_CURR_Q_TRIG_MIN    	           U    This function directly programs the selected voltage parameter of the power supply.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    n    Specify the voltage parameter to be programmed.


Macro Name           Value  Description
----------------------------------------------------
HP663X2_VOLT_IMM       0    Immediate Voltage Level
HP663X2_VOLT_TRIG      1    Triggered Voltage Level
HP663X2_VOLT_OVP       2    OVP Level
HP663X2_VOLT_OVP_MAX   3    Maximum OVP
HP663X2_VOLT_OVP_MIN   4    Minimum OVP     i    The voltage level to be programmed.

This paramater is not used if maximum or minimum OVP is selected.
   �� � '  �  �    Instrument Handle                �
 �p���  �    Status                           �� R  ��      Volt Param                       �a RM �       Volt Lev                           0    	                      �Immediate Voltage Level HP663X2_VOLT_IMM Triggered Voltage Level HP663X2_VOLT_TRIG OVP Level HP663X2_VOLT_OVP Maximum OVP HP663X2_VOLT_OVP_MAX Minimum OVP HP663X2_VOLT_OVP_MIN    0    K    This function queries the selected voltage parameter of the power supply.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Specifies the voltage parameter to query.

Macro Name              Value  Description
---------------------------------------------------------------
HP663X2_VOLT_Q            0    Immediate Voltage Level
HP663X2_VOLT_Q_MAX        1    Maximum Output Voltage
HP663X2_VOLT_Q_MIN        2    Minimum Output Voltage
HP663X2_VOLT_Q_TRIG       3    Triggered Voltage Level
HP663X2_VOLT_Q_TRIG_MAX   4    Maximum Triggered Voltage
HP663X2_VOLT_Q_TRIG_MIN   5    Minimum Triggered Voltage
HP663X2_VOLT_Q_OVP        6    Overvoltage Protection Level
HP663X2_VOLT_Q_OVP_MAX    7    Maximum Overvoltage Protection
HP663X2_VOLT_Q_OVP_MIN    8    Minimum Overvoltage Protection         The specified voltage level.
   �� � *  �  �    Instrument Handle                � �r���  �    Status                           �� : ��      Volt Q Param                     �� :l �       Volt Lev                           0    	                   	  �Immediate Voltage Level HP663X2_VOLT_Q Maximum Output Voltage HP663X2_VOLT_Q_MAX Minimum Output Voltage HP663X2_VOLT_Q_MIN Triggered Voltage Level HP663X2_VOLT_Q_TRIG Maximun Triggered Voltage HP663X2_VOLT_Q_TRIG_MAX Minimum Triggered Voltage HP663X2_VOLT_Q_TRIG_MIN Over-Voltage Protection Level HP663X2_VOLT_Q_OVP Maximum Over-Voltage Protection HP663X2_VOLT_Q_OVP_MAX Minimum Over-Voltage Protection HP663X2_VOLT_Q_OVP_MIN    	           e    This function directly programs the selected voltage parameter of the power supply's second output.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Specify the voltage parameter to be programmed.


Macro Name            Value  Description
-----------------------------------------------------
HP663X2_VOLT2_IMM       0    Immediate Voltage Level
HP663X2_VOLT2_TRIG      1    Triggered Voltage Level
     i    The voltage level to be programmed.

This paramater is not used if maximum or minimum OVP is selected.
   �� � '  �  �    Instrument Handle                �* �p���  �    Status                           � R  ��      Volt Param                       � RM �       Volt Lev                           0    	                      UImmediate Voltage Level HP663X2_VOLT2_IMM Triggered Voltage Level HP663X2_VOLT2_TRIG    0    [    This function queries the selected voltage parameter of the power supply's second output.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    Specifies the voltage parameter to query.

Macro Name               Value  Description
----------------------------------------------------------
HP663X2_VOLT2_Q            0    Immediate Voltage Level
HP663X2_VOLT2_Q_MAX        1    Maximum Output Voltage
HP663X2_VOLT2_Q_MIN        2    Minimum Output Voltage
HP663X2_VOLT2_Q_TRIG       3    Triggered Voltage Level
HP663X2_VOLT2_Q_TRIG_MAX   4    Maximum Triggered Voltage
HP663X2_VOLT2_Q_TRIG_MIN   5    Minimum Triggered Voltage
         The specified voltage level.
   �2 � *  �  �    Instrument Handle                � �r���  �    Status                           �` : ��      Volt Q Param                     �N :l �       Volt Lev                           0    	                     Immediate Voltage Level HP663X2_VOLT2_Q Maximum Output Voltage HP663X2_VOLT2_Q_MAX Minimum Output Voltage HP663X2_VOLT2_Q_MIN Triggered Voltage Level HP663X2_VOLT2_Q_TRIG Maximun Triggered Voltage HP663X2_VOLT2_Q_TRIG_MAX Minimum Triggered Voltage HP663X2_VOLT2_Q_TRIG_MIN    	           <    This function sets the over-current protection parameters.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    This parameter enables or disables the power supply over-current protection (OCP) function.

Macro Name  Value  Description
----------------------------------------
VI_TRUE       1    OCP enabled
VI_FALSE      0    OCP disabled    �    The time delay before a Constant Current condition is recognized.  The OverCurrent Protection is based on the Constant Current status bit.  The delay prevents the momentary changes in the constnat current status bit that can occur during output transients from being registered as events by the status subsystem. The unit of this parameter is seconds.  The default is 0.08s.

Note: This parameter is ignored if OCP is being disabled.

The unit of this parameter is seconds.  The allowable range is:

Macro Name             Value         Description
--------------------------------------------------------------
HP663X2_OCP_DELAY_MIN    0           Minimum delay
HP663X2_OCP_DELAY_MAX    2147480000  Maximum delay   �� � '  �  �    instrumentHandle                 � �d���  �    Status                           �� T � �       Ocp Enable                       �� T/ �       Cc Delay                           0    	           On VI_TRUE Off VI_FALSE    0    ?    This function queries the over-current protection parameters.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Over-current protection enable.

Macro Name  Value  Description
-------------------------------
VI_TRUE       1    OCP enabled
VI_FALSE      0    OCP disabled     ?    Time delay before a Constant Current condition is recognized.   � � (  �  �    Instrument Handle                �Q �i���  �    Status                           �2 N � �       Ocp Enable                       �� N% �       Cc Delay                           0    	           	            	           f    This function directly programs the selected resistance parameter of the power supply's main output.     E    The instrument's VISA session handle returned from hp663x2_init().
     (    The resistance level to be programmed.     �    Specify the resistance parameter to be programmed.

Macro Name         Value  Description
--------------------------------------------------
HP663X2_RES_IMM    0    Immediate Resistance Level
HP663X2_RES_TRIG   1    Triggered Resistance Level    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   �� � L  �  �    Instrument Handle                �� SO �       Resistance Lev                   �	 J � ��      Resistance Param                 � �4���  �    Status                             0    0               WImmediate Resistance Level HP663X2_RES_IMM Triggered Resistance Level HP663X2_RES_TRIG    	          �    Specifies the resistance parameter to query.

Macro Name               Value  Description
----------------------------------------------------------
HP663X2_RES_Q            0    Immediate Voltage Level
HP663X2_RES_Q_MAX        1    Maximum Output Voltage
HP663X2_RES_Q_MIN        2    Minimum Output Voltage
HP663X2_RES_Q_TRIG       3    Triggered Voltage Level
HP663X2_RES_Q_TRIG_MAX   4    Maximum Triggered Voltage
HP663X2_RES_Q_TRIG_MIN   5    Minimum Triggered Voltage
     "    The specified resistance level.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
   �6 : �  ��      Resistance Q Param               � Q` �       Resistance Lev                   �E � b �  �    Instrument Handle                �� �E���  �    Status                                       Immediate Resistance HP663X2_RES_Q Maximum Output Resistance HP663X2_RES_Q_MAX Minimum Output Resistance HP663X2_RES_Q_MIN Triggered Resistance HP663X2_RES_Q_TRIG Maximun Triggered Resistance HP663X2_RES_Q_TRIG_MAX Minimum Triggered Resistance HP663X2_RES_Q_TRIG_MIN    	           0    	          ,    This function configures the power supply digital control port to function as a digital I/O port and outputs the digIoData parameter to the port. The port has three signal pins and a digital ground pin. Bits 0 and 1 control port pins 1 and 2.  They are output only pins.  Bit 2 controls port pin 3, which is configurable to be input or output.  A 1 must be programmed to bit 2 to configure pin 3 as an input pin.  It must be reprogrammed with a 1 when changing the values of pins 1 and 2. 

The port configuration change is stored in non-volatile memory.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    �    I/O port value. Only the least 3 significant bits are used.  Therefore the output value can range from 0 to 7.

Program  Bit            Pin
Value    Configuration  Setting
         2    1    0    4     3          2         1
------------------------------------------------------
0        0    0    0    GND   Output     Lo        Lo
1        0    0    1    GND   Output     Lo        Hi
2        0    1    0    GND   Output     Hi        Lo
3        0    1    1    GND   Output     Hi        Hi
4        1    0    0    GND   Input      Lo        Lo
5        1    0    1    GND   Input      Lo        Hi
6        1    1    0    GND   Input      Hi        Lo
7        1    1    1    GND   Input      Hi        Hi
   �� � $  �  �    Instrument Handle                �� �l���  �    Status                           �� k � �       Dig Io Data                        0    	           0   e    This function reads the digital control port when the port is configured for Digital I/O operation. Configuring of the port is done with the hp663x2_setDigio() function.  The query returns the last programmed value in bits 0 and 1 and the value read at pin 3 in bit 2.

The function returns an error status if the port is configured as a digital I/O port.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     '    Value read from the digital I/O port.   �� � &  �  �    Instrument Handle                �! �o���  �    Status                           � e � �       Dig Io Data                        0    	           	            >    This function queries the setting of the output mode switch.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     �    Output mode switch setting.  The returned value is 15,000 if the switch is set to Normal and 60,000 if the switch is set to Fast.   �3 � #  �  �    Instrument Handle                ɀ �l���  �    Status                           �a Z � �       Alc Bandwidth                      0    	           	               This function enables the selected subsystem to receive triggers.  It moves the trigger system from the Idle state to the Initiated state.  It can also optionally configure the measurement sensor to measure either voltage, current, or DVM for an acquire trigger.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    C    Select subsystem trigger to arm.

ONCE arms the output subsystem with single trigger.

CONTINUOUS arms the output subsystem with continuous trigger.

CONTINUOUS OFF turns off continuous triggering.

ACQUISITION arms the acquisition subsystem. It does not set the sense function.

VOLT ACQUISITION sets the sense function to voltage mode before arming the acquisition subsystem.

CURR ACQUISITION sets the sense function to current mode before arming the acquisition subsystem.

DVM ACQUISITION sets the sense function to DVM mode before arming the acquisition subsystem.

Macro Name                       Value  Description
----------------------------------------------------------------
HP663X2_ARM_OUTPUT_ONCE            0    Output Once
HP663X2_ARM_OUTPUT_CONTINUOUS      1    Output Continuous
HP663X2_ARM_OUTPUT_CONTINUOUS_OFF  2    Output  Continuous Off
HP663X2_ARM_ACQUISITION            3    Acquisition
HP663X2_ARM_VOLT_ACQUISITION       4    Voltage Acquisition
HP663X2_ARM_CURR_ACQUISITION       5    Current Acquisition
HP663X2_ARM_DVM_ACQUISITION        6    DVM Acquisition   ͸ �   �  �    Instrument Handle                � �n���  �    Status                           �� Q= ��      Trig System                        0    	                     ?Output Once HP663X2_ARM_OUTPUT_ONCE Output Continuous HP663X2_ARM_OUTPUT_CONTINUOUS Output  Continuous Off HP663X2_ARM_OUTPUT_CONTINUOUS_OFF Acquisition HP663X2_ARM_ACQUISITION Voltage Acquisition HP663X2_ARM_VOLT_ACQUISITION Current Acquisition HP663X2_ARM_CURR_ACQUISITION DVM Acquisition HP663X2_ARM_DVM_ACQUISITION    �    Signal a subsystem to perform its action.  The subsystem must have been previously armed with the hp663x2_arm function and the trigger source is set to BUS.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    5    Subsystem to trigger.  There are two subsystems that can be triggered in the instrument - output and measurement acquisition.

Macro Name                Value  Description
--------------------------------------------------
HP663X2_TRIG_OUTPUT         0    Output
HP663X2_TRIG_ACQUISITION    1    Acquisition   �� �   �  �    Instrument Handle                �" �q���  �    Status                           � \* ��      Sub System                         0    	                      @Output HP663X2_TRIG_OUTPUT Acquisition HP663X2_TRIG_ACQUISITION   x    The reset function places the instrument in a default state. 

The following commands are sent to the instrument:

1. Device Clear - to ensure that the instrument can execute a reset.
2. *RST - reset the instrument to factory define state.  Also, abort any acquisition pending or in progress.
3. *CLS - clear event and status registers, error queue, and the MAV status bit.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
   ܿk���  �    Status                           ޠ "  �  �    Instrument Handle                  	           0   1    The self-test function causes the instrument to perform a self-test and returns the result of that self-test.  This is used to verify that an instrument is operating properly.  A failure may indicate a potential hardware problem.

For a list of possible self-test errors, consult the Programming Guide.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     H    Numeric result from self-test operation.

0 = no error (test passed).
     @    Self-test status message.  This is limited to 256 characters.
     E    The instrument's VISA session handle returned from hp663x2_init().
   �o���  �    Status                           � 0 $ �  �    Test Result                      �� � " � �    Test Message                     � *  �  �    Instrument Handle                  	           	            	            0    l    This function returns the revision of the instrument driver and the firmware of the instrument being used.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     B    Instrument driver revision.  This is limited to 256 characters.
     D    Instrument firmware revision.  This is limited to 256 characters.
   ��q���  �    Status                           � "  �  �    Instrument Handle                � / � � ,    Instrument Driver Revision       �P � � � ,    Firmware Revision                  	           0    	            	           �    This function sends a device clear (DCL) to the instrument.

A device clear will abort the current operation and enable the instrument to accept a new command or query.

This is particularly useful in situations where it is not possible to determine the instrument state. In this case, it is customary to send a device clear before issuing a new instrument driver function.  The device clear ensures that the instrument will be able to begin processing the new commands.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
   �yt���  �    Status                           �Z !  �  �    Instrument Handle                  	           0    T    This function sends an IEEE Group Execute Trigger (GET or *TRG) to the instrument.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require atention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message function to get a textual description of errors and warnings.
   �~ � -  �  �    Instrument Handle                �� �z���  �    Status                             0    	           r    This function translates the error value returned from an instrument driver function to a user-readable string.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     =    The error return value from an instrument driver function.
     <    Error message string.  This is limited to 256 characters.
     E    The instrument's VISA session handle returned from hp663x2_init().
   �	q���  �    Status                           � 6 " �  �    Status Code                      �� � " � �    Status Message                   �	 &  �  �    Instrument Handle                  	           0    	            0   �    This function returns the error numbers and corresponding error messages in the error queue of an instrument.  See the manual for a listing of the instrument error numbers and messages.

Instrument errors may occur when a user attempts to place the instrument in an invalid state such as sending an invalid sequence of coupled commands.

Instrument errors can be detected by polling.  Automatic polling can be accomplished by using the hp663x2_errorQueryDetect() function.
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
         Instrument's error code.
     B    Instrument's error message.  This is limited to 256 characters.
   �.n���  �    Status                           � "  �  �    Instrument Handle                �\ / " �  �    Error Code                       � � " � �    Error Message                      	           0    	            	           *    This function enables or disables automatic instrument error checking.

When automatic instrument error querying is enabled, the driver will query the instrument for an error before returning from each driver function.  This adds the overhead of one instrument query to each dirver function call.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
        A boolean which enables/disables automatic instrument error querying.

Macro Name  Value  Description
------------------------------------------------------------
VI_TRUE       1    Enable automatic instrument error query.
VI_FALSE      0    Disable automatic instrument error query.
   ��	t���  �    Status                           ��	 "  �  �    Instrument Handle                �& G � �       Error Query Detect                 	           0     Enable VI_TRUE Disable VI_FALSE   <    This function indicates if automatic instrument error detection is enabled or disabled. 

When automatic instrument error querying is enalbed, the driver will query the instrument for an error before returning from each driver function.  This adds the overhead of one instrument query to each dirver function call.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     �    A boolean which indicates if automatic instrument error querying is enabled or disabled.  

If VI_TRUE (1), automatic instrument error querying is enabled.

If VI_FALSE (0), automatic instrument error querying is disabled.
    c �t���  �    Status                           D �    �  �    Instrument Handle                � 4 � �  d    Error Query Detect                 	           0    	           U    The timeout function sets the timeout value for driver I/O transactions in milliseconds. The timeout period may vary on computer platforms.

The default timeout period varies by VISA implementation.  For HP VISA, it is 2 seconds.

Some implementations of VISA will only accept a limited number of timeout values.  If you set the VISA timeout to a given value using one of those implementations, and then query the timeout, you may find that the query returns a different value than the one you set.  HP VISA sets the timeout to the precise value you specify, and returns that value when queried.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
    >    This value sets the I/O timeout for all functions in the driver. It is specified in milliseconds. 

Macro Name          Value          Description
------------------------------------------------------   
HP663X2_TIMEOUT_MIN   0            Minimum bus timoeut 
HP663X2_TIMEOUT_MAX   2147483647   Maximum bus timeout
   �t���  �    Status                           t "  �  �    Instrument Handle                � B � �      Time Out                           	           0    ���                   �    The timeout query function returns the timeout value for driver I/O transactions in milliseconds.  The timeout period may vary on computer platforms.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     e    This value sets the I/O timeout for all functions in the driver. It is specified in milliseconds. 
   mr���  �    Status                           N "  �  �    Instrument Handle                � 2 � � �    Time Out                           	           0    	            F    This function queries the operation complete bit in the Status Byte.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require atention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message function to get a textual description of errors and warnings.
     0    The status of the OPC bit in the Status Byte.
    � -  �  �    Instrument Handle                _ �z���  �    Status                           = \ � �       Instrument Ready                   0    	           	            >    This function queries the instrument�s Status Byte register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     )    The value of the Status Byte register.
   wv���  �    Status                           X "  �  �    Instrument Handle                � / � �  d    Status Byte                        	           0    	            5    This function queries the Operation Event register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     -    The value of the Operation Event register.
   � �p���  �    Status                           � � "  �  �    Instrument Handle                � H � �  d    Operation Event Register           	           0    	            9    This function queries the Operation Condition register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     1    The value of the Operation Condition register.
   /u���  �    Status                            "  �  �    Instrument Handle                ] C � �  d    Operation Condition Register       	           0    	            8    This function queries the Questionable Event register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     0    The value of the Questionable Event register.
   �r���  �    Status                           s "  �  �    Instrument Handle                � H � �  d    Questionable Event Register        	           0    	            <    This function queries the Questionable Condition register.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     5    The value of the Questionable Condition register.
    �o���  �    Status                           !� "  �  �    Instrument Handle                "& ? � �  d    Questionable Condition Register    	           0    	            ;    This function queries the Standart Event Status register.     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
     4    The value of the Standard Event Status register.
    #b #  �  �    Instrument Handle                #�	p���  �    Status                           %� ] � �  d    Standard Event Status              0    	           	            �    This function sets the enable mask of the specified status register.  The mask specifies the events to be included in the 
summary status bit.  The summary status bit is used by the status byte register and the DFI source register.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    Q    Event register select.

The following status registers are defined for the dc source:

Macro Name       Value  Description
-----------------------------------------
HP663X2_REG_ESE    0    Standard Event Status
HP663X2_REG_SRE    1    Service Request
HP663X2_REG_OPER   2    Operation Status
HP663X2_REG_QUES   3    Questionable Status     �    The mask for the selected event register.  Refer to  Programming the Status Registers for a definition of  each status register bit.  A 1 in the mask enables the specified event.   'z +  �  �    Instrument Handle                '� �x���  �    Status                           )� A � ��      Enable Register                  + R= �       Enable Mask                        0    	                     �Standard Event Status HP663X2_REG_ESE Service Request HP663X2_REG_SRE Operation Status HP663X2_REG_OPER Questionable Status HP663X2_REG_QUES    0    8    Returns the contents of the selected enable register.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.
    i    Specifies an enable register on the instrument.

The following event registers are defined for the dc source:

Macro Name       Value  Description
-----------------------------------------
HP663X2_REG_ESE    0    Standard Event Status
HP663X2_REG_SRE    1    Service Request
HP663X2_REG_OPER   2    Operation Status
HP663X2_REG_QUES   3    Questionable Status     /    The contents of the selected enable register.   -� (  �  �    Instrument Handle                -� �u���  �    Status                           /� c � ��      Enable Register                  1" j< �       Enable Mask                        0    	                      �Standard Event Status HP663X2_REG_ESE Service Request HP663X2_REG_SRE Operation Status HP663X2_REG_OPER Questionable Status HP663X2_REG_QUES    	            k    This function passes an instrument command to the instrument.  The function does not expect any response.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require atention.
 < 0  Error           The function did not complete
                      successfully.

Use the hpxxxx_error_message function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     i    The instrument command.  It must be a NULL termianted C string and may not exceed 256 bytes in length.
   3Ys���  �    Status                           56 "  �  �    Instrument Handle                5� 1 " � �    String Command                     	           0    ""   �    Passes the command string to the instrument.  The function waits for a response which must be a string (character data).

A max of size-1 bytes will be read from the instrument into the string and the string will be NULL terminated (so its length can be found with strlen).

If more than size-1 bytes are generated by the instrument then remaining bytes will be discarded and the instrument's output buffer will be cleared.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require atention.
 < 0  Error           The function did not complete
                      successfully.

Use the hpxxxx_error_message function to get a textual description of errors and warnings.
     E    The instrument's VISA session handle returned from hp663x2_init().
     i    The instrument command.  It must be a NULL termianted C string and may not exceed 256 bytes in length.
        The length of result array in bytes. 

Macro Name              Value         Description
---------------------------------------------------------------  
HP663X2_CMDSTRING_Q_MIN   1           Minimum string array size
HP663X2_CMDSTRING_Q_MAX   2147483647  Maximum string array size     $    The response from the instrument.
   8[o���  �    Status                           :8 "  �  �    Instrument Handle                :� 0 " � �    String Command                   :� r " �      String Size                      < �   � �    String Result                      	           0    " "    ���                 	            �    This function passes the string in cmd_str followed by a space and then an integer.  Note that either a ViInt16 or 32 can be passed, as the ViInt16 will be promoted.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require atention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message function to get a textual description of errors and warnings.
     i    The instrument command.  It must be a NULL termianted C string and may not exceed 256 bytes in length.
        The integer to be appended to the command string. 

Macro Name          Value        Description
-------------------------------------------------------------  
HP663X2_CMDINT_MIN  -2147483647  Minimum integer value
HP663X2_CMDINT_MAX  2147483647   Maximum integer value   >: $  �  �    Instrument Handle                >�����  �    Status                           @e 3 $ � �    Send Integer Command             @�  & �      Send Integer                       0    	           ""    ����                 t    Passes the command string to the instrument.  The function expects a 16-bit integer response from the instrument.
     i    The instrument command.  It must be a NULL termianted C string and may not exceed 256 bytes in length.
     $    The response from the instrument.
     E    The instrument's VISA session handle returned from hp663x2_init().
    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message function to get a textual description of errors and warnings.
   Cl , ' � �    Query I16 Command                C� k ' �       I16 Result                       D	 '  �  �    Instrument Handle                DVw���  �    Status                             ""    	            0    	           �    The close function terminates the software connection to the instrument and de-allocates system resources.  It is generally a good programming habit to close the instrument handle when the program is done using the instrument.    �    The meaning of the VISA status returned by the function is as follows:

 = 0  "VI_SUCCESS"    The function completed successfully.
 > 0  Warning         The function completed, but an
                      exceptional condition occurred which
                      may require attention.
 < 0  Error           The function did not complete
                      successfully.

Use the hp663x2_error_message() function to get a textual description of errors and warnings.     D    The instrument's VISA session handle returned from hp663x2_init().   H �q���  �    Status                           I� � "  �  �    Instrument Handle                  	           0 ����         �  	�     K-    init                            ����           �     K.    readOutputStatus                ����         k  �     K.    readOutputStatus2               ����         w  [     K.    rippleRiDfi                     ����         �   4     K.    outputVoltCurr                  ����         !#  $^     K.    outputVoltCurr2                 ����         %M  .�     K.    setMeasure                      ����         1  :�     K.    setMeasure2                     ����         =  ?�     K.    doDelay                         ����         @�  Gq     K.    setDisplay                      ����         I	  N�     K.    getDisplay                      ����         O�  R�     K.    setDisplayChannel               ����         SO  V     K.    getDisplayChannel               ����         V�  ^     K.    measureDvm                      ����         _0  g^     K.    measureVolt                     ����         h�  n     K.    measureVolt2                    ����         n�  w"     K.    measureCurr                     ����         x�  }�     K.    measureCurr2                    ����         ~�  �     K.    measureVoltArray                ����         �  ��     K.    measureCurrArray                ����         ��  ��     K.    fetchDvm                        ����         ��  �B     K.    fetchVolt                       ����         ��  �E     K.    fetchCurr                       ����         ��  �`     K.    fetchVoltArray                  ����         �Y  ��     K.    fetchCurrArray                  ����         ��  ��     K.    setDvmAcqTrig                   ����         ��  �_     K.    getDvmAcqTrig                   ����         �  �     K.    setVoltAcqTrig                  ����         �H  ��     K.    getVoltAcqTrig                  ����         ք  �h     K.    setCurrAcqTrig                  ����         �  �'     K.    getCurrAcqTrig                  ����         ��  �     K.    setSweepParams                  ����         �  �<     K.    getSweepParams                  ����         �z  �r     K.    abort                           ����         �� s     K.    setMeasTimeout                  ����        ( �     K.    getMeasTimeout                  ����        �      K.    clearOutpProt                   ����        � �     K.    setOutpRelayPolarity            ����        � �     K.    getOutpRelayPolarity            ����        m �     K.    setOutpState                    ����        �  �     K.    getOutpState                    ����        !f &     K.    setPonState                     ����        ' *t     K.    getPonState                     ����        +0 /-     K.    setOutpRelay                    ����        /� 3�     K.    getOutpRelay                    ����        4I @�     K.    setRiDfi                        ����        CK H�     K.    getRiDfi                        ����        J N     K.    setOutpTrigSource               ����        N� R     K.    getOutpTrigSource               ����        R� VE     K.    getAcqSenseFunc                 ����        W ^m     K.    setCurrSenseParams              ����        _� b`     K.    getCurrSenseParams              ����        c] i�     K.    setWindowType                   ����        j� ni     K.    getWindowType                   ����       ���� qR     K.    setSenseProt                    ����       ���� tJ     K.    getSenseProt                    ����        u x�     K.    setCurr                         ����        y� }�     K.    getCurr                         ����        � �L     K.    setCurr2                        ����        �� �M     K.    getCurr2                        ����        �` ��     K.    setVolt                         ����        �{ ��     K.    getVolt                         ����        �p ��     K.    setVolt2                        ����        �� �u     K.    getVolt2                        ����        �� ��     K.    setOcpParams                    ����        �� �!     K.    getOcpParams                    ����        � ��     K.    hp663x2_setRes                  ����       ���� �s     K.    hp663x2_getRes                  ����        �~ ò     K.    setDigio                        ����        �g �1     K.    getDigio                        ����        �� ��     K.    getVoltAlcBandwidth             ����        ̨ �1     K.    arm                             ����        �/ �@     K.    trigger                         ����        �? ��     K-    reset                           ����        �h �g     K-    self_test                       ����        �d �     K-    revision_query                  ����        � �     K-    dcl                             ����        �" �     K.    trg                             ����        �$ �U     K-    error_message                   ����        �K ��     K-    error_query                     ����        �� �L     K-    errorQueryDetect                ����        � z     K-    errorQueryDetect_Q              ����        6 
     K-    timeOut                         ����        
�      K-    timeOut_Q                       ����        � u     K.    opc_Q                           ����        1 �     K-    readStatusByte_Q                ����        � 2     K-    operEvent_Q                     ����        � �     K-    operCond_Q                      ����        R �     K-    quesEvent_Q                     ����        � "c     K-    quesCond_Q                      ����        # %�     K.    stdEvent_Q                      ����        &� +�     K.    setEnableReg                    ����        -C 1Y     K.    getEnableReg                    ����        2� 5�     K-    cmd                             ����        6� <F     K-    cmdString_Q                     ����        =� A�     K.    cmdInt                          ����        B� F5     K.    cmdInt16_Q                      ����        G, JD     K-    close                                 �                                     DInitialize                        ����Application Functions                DRead Output Status                   DRead Output Status 2                 DRipple RI/DFI                        DOutput Voltage and Current           DOutput Voltage and Current 2         DSet and Measure                      DSet and Measure 2                    DDelay Program                     ����Display                              DSet Display Parameters               DQuery Display Parameters             DSet Display Channel                  DQuery Display Channel             ����Measure                              DDVM Measurement                      DVoltage Measurement                  DVoltage Measurement 2                DCurrent Measurement                  DCurrent Measurement 2                DMeasure Voltage Waveform             DMeasure Current Waveform             DFetch DVM Measurement                DFetch Voltage Measurement            DFetch Current Measurement            DFetch Voltage Waveform               DFetch Current Waveform            ����Configure Acquisition Trigger        DSet DVM Acq Trigger                  DQuery DVM Acq Trigger                DSet Voltage Acq Trigger              DQuery Voltage Acq Trigger            DSet Current Acq Trigger              DQuery Current Acq Trigger            DSet Sweep Parameters                 DQuery Sweep Parameters               DTerminate Acquisition                DSet Measurement Timeout              DQuery Measurement Timeout         ����Output                               DClear Output Protection              DSet Output Relay Polarity            DQuery Output Relay Polarity          DSet Output State                     DQuery Output State                   DSet Power-On Recall State            DQuery Power-On Recall State          DSet Output Relay State               DQuery Output Relay State             DSet RI/DFI Parameters                DQuery  RI/DFI Paramters           ����Configure Trigger                    DSet Trigger Source                   DQuery Trigger Source              ����Sense                                DQuery Acquisition Sense Setting      DSet Current Sense Parameters         DQuery Current Sense Parameters       DSet Window Function Type             DQuery Window Function Type           DSet Sense Protection                 DQuery Sense Protection            ����Source                               DSet Current Parameter                DQuery Current Parameter              DSet Current Parameter 2              DQuery Current Parameter 2            DSet Voltage Parameter                DQuery Voltage Parameter              DSet Voltage Parameter 2              DQuery Voltage Parameter 2            DSet Over-Current Protection          DQuery Over-Current Protection        DSet Resistance Parameter             DQuery Resistance Parameter           DSet Digital I/O Port                 DQuery Digital I/O Port               DQuery Output Bandwidth Setting    ����Trigger                              DArm Trigger                          DGenerate Trigger                    dUtility                              DReset                                DSelf-Test                            DRevision Query                       DDevice Clear                         DGroup Execute Trigger             ����Error                                DError Message                        DError Query                          DSet Error Query Detect               DQuery Error Query Detect          ����Time Out                             DSet Timeout                          DQuery Timeout                     ����Synchronization                      DQuery Operation Complete          ����Status                               DRead Status Byte                     DOperation Status Events              DOperation Status Conditions          DQuestionable Status Events           DQuestionable Status Conditions       DStandard Event Status                DSet Enable Register                  DQuery Enable Register               CPassthrough Functions                DSend String                          DQuery String                         DSend Integer                         DQuery Int16                          DClose                           