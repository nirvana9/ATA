s��        6�   S $�  �   �   ����                               ke230x      Keithley 2302 and 2306 DC Power Supplies      � � ��ViInt16  �  � ��ViInt32  � � ��ViReal64     � ��ViRsrc     	� 	��ViBoolean     	� 	��ViSession     � ��ViStatus     �  ViChar[]     � ��ViChar     � ��ViString     	�  ViInt16[]     	�  ViInt32[]     
�  	ViReal64[]     � 	 
ViBoolean[]     � ��ViConstString     � ��ViAttr  �    This instrument driver provides programming support for the Keithley 230X Series DC Power Supplies.  The driver contains all the functions that IVI and VXIplug&play require.  In addition, the driver contains high-level functions that configure the power supply and generate output in a single operation.  The driver also contains lower level functions that configure the power supply and initiate the output changes in separate operations.   

Note:  This driver requires the VISA and IVI libraries.  
     �    This class contains functions that configure the instrument.  The class includes high-level functions that configure multiple instrument settings as well as low-level functions that set, get, and check individual attribute values.
     ;    This class contains functions for configuring the output.     q    Functions to configure the instrument specific voltage protection, output characteristics and the relay output.     T    This class contains sub-classes for the set, get, and check attribute functions.       �    This class contains functions that set an attribute to a new value.  There are typesafe functions for each attribute data type.     �    This class contains functions that obtain the current value of an attribute.  There are typesafe functions for each attribute data type.     �    This class contains functions that obtain the current value of an attribute.  There are typesafe functions for each attribute data type.     L    This class contains high-level functions for measuring the output signal.      ]    This class contains functions that initiate instrument operations and report their status.
    _    This class contains functions and sub-classes that control common instrument operations.  These functions include many of functions that VXIplug&play require, such as reset, self-test, revision query, error query, and error message.  This class also contains functions that access IVI error infomation, lock the session, and perform instrument I/O.
     R    This class contains functions that retrieve and clear the IVI error information.     ?    This class contains functions that retrieve coercion records.     k    This class contains functions that lock and unlock IVI instrument driver sessions for multithread safefy.     F    This class contains functions that send and receive instrument data.    $    This function performs the following initialization actions:

- Creates a new IVI instrument driver session.

- Opens a session to the specified device using the interface and address you specify for the Resource Name parameter.

- If the ID Query parameter is set to VI_TRUE, this function queries the instrument ID and checks that it is valid for this instrument driver. Note if ID Query set to VI_FALSE then driver defaults to a Model 2302 (1 channel only). If you want ID Query to be VI_FALSE and to have the driver support a Model 2306 (2 channel) then use the function Initialize With Options.

- If the Reset parameter is set to VI_TRUE, this function resets the instrument to a known state.

- Sends initialization commands to set the instrument to the state necessary for the operation of the instrument driver.

- Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Note:  This function creates a new session each time you invoke it. Although you can open more than one IVI session for the same resource, it is best not to do so.  You can use the same session in multiple program threads.  You can use the ke230x_LockSession and ke230x_UnlockSession functions to protect sections of code that require exclusive access to the resource.

    �    Pass the resource name of the device to initialize.

You can also pass the name of a virtual instrument or logical name that you configure with the IVI Configuration utility.  The virtual instrument identifies a specific device and specifies the initial settings for the session.  A logical Name identifies a particular virtual instrument.

Refer to the following table below for the exact grammar to use for this parameter.  Optional fields are shown in square brackets ([]).

Syntax
------------------------------------------------------
GPIB[board]::<primary address>[::secondary address]::INSTR

<LogicalName>
[VInstr->]<VirtualInstrument>

If you do not specify a value for an optional field, the following values are used:

Optional Field - Value
------------------------------------------------------
board - 0
secondary address - none (31)

The following table contains example valid values for this parameter.

"Valid Value" - Description
------------------------------------------------------
"GPIB::22::INSTR" - GPIB board 0, primary address 22 no
                    secondary address

"SampleInstr" - Logical name "SampleInstr"

"VInstr->xyz432" - Virtual Instrument "xyz432"

"xyz432" - Logical Name or Virtual Instrument "xyz432"

Default Value:  "GPIB::16::INSTR"

Note: You specify the resource name with the "VInstr->" if you have the logical name that is the same as the virtual instrument name and you want to explicitly use the virtual instrument name. Otherwise, the driver uses the logical name.        Specify whether you want the instrument driver to perform an ID Query.

Valid Range:
VI_TRUE  (1) - Perform ID Query (Default Value)
VI_FALSE (0) - Skip ID Query

When you set this parameter to VI_TRUE, the driver verifies that the instrument you initialize is a type that this driver supports.  

Circumstances can arise where it is undesirable to send an ID Query command string to the instrument.  When you set this parameter to VI_FALSE, the function initializes the instrument without performing an ID Query.     �    Specify whether you want the to reset the instrument during the initialization procedure.

Valid Range:
VI_TRUE  (1) - Reset Device (Default Value)
VI_FALSE (0) - Don't Reset

    �    Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Notes:

(1) This function creates a new session each time you invoke it.  This is useful if you have multiple physical instances of the same type of instrument.  

(2) Avoid creating multiple concurrent sessions to the same physical instrument.  Although you can create more than one IVI session for the same resource, it is best not to do so.  A better approach is to use the same IVI session in multiple execution threads.  You can use functions ke230x_LockSession and ke230x_UnlockSession to protect sections of code that require exclusive access to the resource.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    * =   �  �    Resource Name                     " : �       ID Query                          - =� �       Reset Device                      � �( �  �    Instrument Handle                 �#����  �    Status                             "GPIB::16::INSTR"   Yes VI_TRUE No VI_FALSE   Yes VI_TRUE No VI_FALSE    	           	          Z    This function performs the following initialization actions:

- Creates a new IVI instrument driver and optionally sets the initial state of the following session attributes:

    KE230X_ATTR_RANGE_CHECK         
    KE230X_ATTR_QUERY_INSTR_STATUS  
    KE230X_ATTR_CACHE               
    KE230X_ATTR_SIMULATE            
    KE230X_ATTR_RECORD_COERCIONS 
    KE230X_ATTR_DRIVER_SETUP   

- Opens a session to the specified device using the interface and address you specify for the Resource Name parameter.

- If the ID Query parameter is set to VI_TRUE, this function queries the instrument ID and checks that it is valid for this instrument driver.

- If the Reset parameter is set to VI_TRUE, this function resets the instrument to a known state.

- Sends initialization commands to set the instrument to the state necessary for the operation of the instrument driver.

- Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Note:  This function creates a new session each time you invoke it. Although you can open more than one IVI session for the same resource, it is best not to do so.  You can use the same session in multiple program threads.  You can use the ke230x_LockSession and ke230x_UnlockSession functions to protect sections of code that require exclusive access to the resource.

    �    Pass the resource name of the device to initialize.

You can also pass the name of a virtual instrument or logical name that you configure with the IVI Configuration utility.  The virtual instrument identifies a specific device and specifies the initial settings for the session.  A logical Name identifies a particular virtual instrument.

Refer to the following table below for the exact grammar to use for this parameter.  Optional fields are shown in square brackets ([]).

Syntax
------------------------------------------------------
GPIB[board]::<primary address>[::secondary address]::INSTR

<LogicalName>
[VInstr->]<VirtualInstrument>

If you do not specify a value for an optional field, the following values are used:

Optional Field - Value
------------------------------------------------------
board - 0
secondary address - none (31)

The following table contains example valid values for this parameter.

"Valid Value" - Description
------------------------------------------------------
"GPIB::22::INSTR" - GPIB board 0, primary address 22 no
                    secondary address

"SampleInstr" - Logical name "SampleInstr"

"VInstr->xyz432" - Virtual Instrument "xyz432"

"xyz432" - Logical Name or Virtual Instrument "xyz432"

Default Value:  "GPIB::16::INSTR"

Note: You specify the resource name with the "VInstr->" if you have the logical name that is the same as the virtual instrument name and you want to explicitly use the virtual instrument name. Otherwise, the driver uses the logical name.        Specify whether you want the instrument driver to perform an ID Query.

Valid Range:
VI_TRUE  (1) - Perform ID Query (Default Value)
VI_FALSE (0) - Skip ID Query

When you set this parameter to VI_TRUE, the driver verifies that the instrument you initialize is a type that this driver supports.  

Circumstances can arise where it is undesirable to send an ID Query command string to the instrument.  When you set this parameter to VI_FALSE, the function initializes the instrument without performing an ID Query.     �    Specify whether you want the to reset the instrument during the initialization procedure.

Valid Range:
VI_TRUE  (1) - Reset Device (Default Value)
VI_FALSE (0) - Don't Reset

    �    Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Notes:

(1) This function creates a new session each time you invoke it.  This is useful if you have multiple physical instances of the same type of instrument.  

(2) Avoid creating multiple concurrent sessions to the same physical instrument.  Although you can create more than one IVI session for the same resource, it is best not to do so.  A better approach is to use the same IVI session in multiple execution threads.  You can use functions ke230x_LockSession and ke230x_UnlockSession to protect sections of code that require exclusive access to the resource.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    
v    You can use this control to set the initial value of certain attributes for the session.  The following table lists the attributes and the name you use in this parameter to identify the attribute.

Name              Attribute Defined Constant   
--------------------------------------------
RangeCheck        KE230X_ATTR_RANGE_CHECK
QueryInstrStatus  KE230X_ATTR_QUERY_INSTR_STATUS   
Cache             KE230X_ATTR_CACHE   
Simulate          KE230X_ATTR_SIMULATE  
RecordCoercions   KE230X_ATTR_RECORD_COERCIONS
DriverSetup       KE230X_ATTR_DRIVER_SETUP

The format of this string is, "AttributeName=Value" where AttributeName is the name of the attribute and Value is the value to which the attribute will be set.  To set multiple attributes, separate their assignments with a comma.

If you pass NULL or an empty string for this parameter and a VISA resource descriptor for the Resource Name parameter, the session uses the default values for the attributes. The default values for the attributes are shown below:

    Attribute Name     Default Value
    ----------------   -------------
    RangeCheck         VI_TRUE
    QueryInstrStatus   VI_TRUE
    Cache              VI_TRUE
    Simulate           VI_FALSE
    RecordCoercions    VI_FALSE
    DriverSetup        ""  

If you pass NULL or an empty string for this parameter and a virtual instrument or logical name for the Resource Name parameter, the session uses the values that you configure for virtual instrument or logical name with the IVI Configuration utility.

You can override the values of the attributes by assigning a value explicitly in a string you pass for this parameter.  You do not have to specify all of the attributes and may leave any of them out.  If you do not specify one of the attributes, its default value or the value that you configure with the IVI Configuration utility will be used.

The following are the valid values for ViBoolean attributes:

    True:     1, TRUE, or VI_TRUE
    False:    0, False, or VI_FALSE

The DriverSetup option allows the user to set which Instrument Model the driver is configured for when in simulation mode or when the IdQuery option is set to false in non-simulation mode.
By default in simulation or when IdQuery = VI_FALSE the driver thinks a 2302 is attached. 

If the DriverSetup option is used and not in simulation mode then if the IdQuery instrument model does not match the DriverSetup model then an error is generated (IVI_ERROR_FAIL_ID_QUERY).

The following are valid entries for the DriverSetup option:
"DriverSetup=Model: 2302"
"DriverSetup=Model: 2306"

Default Value:
"Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1,DriverSetup="""
    )F =   �  �    Resource Name                     /> : �       ID Query                          1I =� �       Reset Device                      2 �` �  �    Instrument Handle                 4�#����  �    Status                            ;� �  � �    Option String                      "GPIB::16::INSTR"   Yes VI_TRUE No VI_FALSE   Yes VI_TRUE No VI_FALSE    	           	           5"Simulate=0,RangeCheck=1,QueryInstrStatus=1,Cache=1"    l    Configures whether the signal that the power supply produces on a channel appears at the output connector.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     n    Pass the name of the channel you want to enable or disable.

Valid Channel Names:  1, 2

Default Value:  ""
        Pass whether you want the signal the power supply produces on a channel to appear at the output connector.  The driver uses this value to set the KE230X_ATTR_OUTPUT_ENABLED attribute.

Valid Values:
VI_TRUE  - Enable the output (Default)
VI_FALSE - Disable the output
    H_-   �  �    Instrument Handle                 I#����  �    Status                            O� 3 
 �  �    Channel Name                      Pb 3 � �  �    Enabled                                	           ""   True VI_TRUE False VI_FALSE   �    Configures the power supply's output range on a channel.  You specify whether you want to configure the voltage or current range, and the value to which to set the range.

Notes:

1) The 230x instruments have a fixed output range of 0-15 Volts. So setting to KE230X_VAL_RANGE_VOLTAGE doesn't do anything. This function is required by IVI DcPwr Class spec.

2) Setting a current range can invalidate a previously configured voltage range.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     u    Pass the name of the channel on which to specify the output range.

Valid Channel Names:  1, 2

Default Value:  ""
    9    Pass the range in which you want the power supply to operate.  The instrument driver coerces this value to the closest value the instrument supports that is greater than or equal to the value you specify.

Units: volts (for voltage range)
       amps  (for current range)

Valid Range:  

Voltage
=======================================
0.0 - 15.0 volts

Current Range           Min         Max
=======================================
0.006 A                 0.0 A       1.0 A
5.0 A                   0.006 A     5.0 A
KE230X_VAL_AUTO_RANGE   -1

Default Value:  0.0
    �    Pass the type of range you want to configure.

Valid Values:
KE230X_VAL_RANGE_VOLTAGE - Voltage Range
KE230X_VAL_RANGE_CURRENT - Current Range

Default Value: KE230X_VAL_RANGE_VOLTAGE

Notes:

1) The 230x instruments have a fixed output range of 0-15 Volts. So setting to KE230X_VAL_RANGE_VOLTAGE doesn't do anything. This function is required by IVI DcPwr Class spec.

2) Setting a current range can invalidate a previously configured current range.
    TA-   �  �    Instrument Handle                 T�#����  �    Status                            [� 3 
 �  �    Channel Name                      \K 3� �  �    Range                             ^� 3 � � �    Range Type                             	           ""    0.0               BVoltage KE230X_VAL_RANGE_VOLTAGE Current KE230X_VAL_RANGE_CURRENT    �    This function configures the current limit.  You specify the output current limit value and the behavior of the power supply when the output current is greater than or equal to that value.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     x    Pass the name of the channel on which to configure the current limit.

Valid Channel Names:  1, 2

Default Value:  ""
    {    Pass the behavior you want the power supply to exhibit when the  ouptut current is greater than or equal to the value of the limit parameter.   The driver uses this value to set the KE230X_ATTR_CURRENT_LIMIT_BEHAVIOR attribute.

Valid Values:
KE230X_VAL_CURRENT_REGULATE - Regulatory limit
KE230X_VAL_CURRENT_TRIP     - Trip limit

Default Value:  
KE230X_VAL_CURRENT_REGULATE
    E    Pass the current limit you want to use. The driver uses this value to set the KE230X_ATTR_CURRENT_LIMIT attribute.

Units: amps

Valid Range:

Current Range        Min         Max
=======================================
0.005 A              0.0 A       1.0 A
5.0 A                0.006 A     5.0 A



Default Value:  0.0 A
    b�-   �  �    Instrument Handle                 cM#����  �    Status                            j$ 3 
 �  �    Channel Name                      j� 3 � � �    Behavior                          l' 3� �  �    Limit                                  	           ""               BRegulate KE230X_VAL_CURRENT_REGULATE Trip KE230X_VAL_CURRENT_TRIP    0.0   4    This function configures the power supply's over-voltage protection.  Class defined overvoltage protection is always off on this instrument.

This function doesn't send anything to the instrument!

To configure the instrument specific overvoltage protection use function ke230x_ConfigureVoltageProtection.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     t    Pass the name of the channel on which to configure the OVP limit.

Valid Channel Names:  1, 2

Default Value:  ""
    q    Pass whether you want to enable or disable the OVP limit.   The driver uses this value to set the KE230X_ATTR_OVP_ENABLED attribute.

Valid Values:
VI_FALSE - Disable OVP limit (Default Value)

Note:

The class defined overvoltage protection is always off. To configure the instrument specific overvoltage protection use function ke230x_ConfigureVoltageProtection.


    �    Pass the over-voltage protection limit you want to use. The driver uses this value to set the KE230X_ATTR_OVP_LIMIT attribute.

Units: volts

Default Value: 0.0

Note:

This value never gets set on the instrument. 

The class defined overvoltage protection is always off. To configure the instrument specific overvoltage protection use function ke230x_ConfigureVoltageProtection.


    p'-   �  �    Instrument Handle                 p�#����  �    Status                            w� 3 
 �  �    Channel Name                      x0 3 � � �    Enabled                           y� 3� �  �    Limit                                  	           ""    True VI_TRUE False VI_FALSE    0.0    V    This function configures the DC voltage level the power supply attempts to generate.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     x    Pass the name of the channel on which to configure the voltage level.

Valid Channel Names:  1, 2

Default Value:  ""
     �    Pass the DC voltage you want the power supply to attempt to generate.  The driver uses this value to set the KE230X_ATTR_VOLTAGE_LEVEL attribute. 

Units: volts

Valid Range:  0.0 - 15.0 volts


Default Value:  0.0 volts
    |�-   �  �    Instrument Handle                 }�#����  �    Status                            �a 3 
 �  �    Channel Name                      �� 3 � �  �    Level                                  	           ""    0.0    �    This function configures the instrument specific voltage protection functionality. Voltage protection can not be turned off, but its behavior can be modified using Clamp feature and Voltage Protection Offset.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     x    Pass the name of the channel on which to configure the voltage level.

Valid Channel Names:  1, 2

Default Value:  ""
     Z    Pass the voltage protection offset.

Valid Range: 0.0 to 8.0 Volts

Default Value: 0.0 V    �    Selects the voltage protection clamp mode.

If the mode is VI_TRUE, the output will disconnect if the voltage sensed on the output is higher than the overvoltage protection value or lower than -0.6 V (diode drop).

If the mode is VI_FALSE, the output will disconnect if the voltage sensed on the output is higher than the overvoltage protection value or lower than the negative of the overvoltage protection value.

Valid Values:
VI_FALSE (0) - Disabled
VI_TRUE (1)  - Enabled (Default Value)
    ��-   �  �    Instrument Handle                 �J#����  �    Status                            �! 3 
 �  �    Channel Name                      �� 3� �  �    Voltage Protection Offset         � 3 �       Clamp                                  	           ""    0.0  " Enabled VI_TRUE Disabled VI_FALSE    X    This function configures the bandwidth and the impedance of the power supply's output.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     x    Pass the name of the channel on which to configure the voltage level.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Specifies what the output bandwidth should be when the output is on and the 5A range is selected.

While the output is disconnected, the instruments bandwidth is always low. Setting it to high will only take effect when output switched on and the 5A range is selected.

This parameter sets the KE230X_ATTR_OUTPUT_BANDWIDTH attribute.

Valid Range: KE230X_VAL_LOW
             KE230X_VAL_HIGH

Default Value: KE230X_VAL_LOW
    ��-   �  �    Instrument Handle                 �[#����  �    Status                            �2 3 
 �  �    Channel Name                      �� 3 � � �    Bandwidth                              	           ""               (Low KE230X_VAL_LOW High KE230X_VAL_HIGH    <    This function configures the channel one output impedance.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    Specifies the output impedance for the 2302 and 2306 channel 1 (battery simulator). This setting applies only to channel 1.
This parameter sets the KE230X_ATTR_OUTPUT_IMPEDANCE attribute.

Valid Range: 0.0 to 1.0 Ohm

Default Value: 0.0 Ohms
    ��-   �  �    Instrument Handle                 �~#����  �    Status                            �U 3 � �  �    Impedance                              	           0.0    j    This function connects/disconnects the output relays. There are four output relays on this power supply.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     C    Pass the relay to operate.

Valid Range: 1 to 4

Default Value: 1     s    Pass the desired relay state.

Valid Values:
VI_FALSE (0) - Disconnected (Default Value)
VI_TRUE  (1) - Connected    �y-   �  �    Instrument Handle                 �/#����  �    Status                            � 3 
 �  �    Relay                             �Q 3	 �       State                                  	           1    On VI_TRUE Off VI_FALSE   �    This function sets the value of a ViInt32 attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type.   
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box. 
  Attributes with data types other than ViInt32 are dim.  If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    ��-   �  �    Instrument Handle                 �Y#����  �    Status                            �0 � � �  �    Attribute Value                 ���� � ���                                           �K = � � �    Attribute ID                      Ŗ =  �  �    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   �    This function sets the value of a ViReal64 attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViReal64 are dim.  If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �-   �  �    Instrument Handle                 ��#����  �    Status                            ԙ � � �  �    Attribute Value                 ���� � ���                                           ִ = � � �    Attribute ID                      �� =  �  �    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   �    This function sets the value of a ViString attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    A    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �t-   �  �    Instrument Handle                 �*#����  �    Status                            � � � �  �    Attribute Value                 ���� � ���                                           � = � � �    Attribute ID                      �e =  �  �    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   �    This function sets the value of a ViBoolean attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    ��-   �  �    Instrument Handle                 ��#����  �    Status                           i � � �  �    Attribute Value                 ���� � ���                                          � = � � �    Attribute ID                     � =  �  �    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   �    This function sets the value of a ViSession attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   F-   �  �    Instrument Handle                �#����  �    Status                           � � � �  �    Attribute Value                  � =  �  �    Channel Name                     � = � � �    Attribute ID                    ���� � ���                                                	               ""                0    .Press <ENTER> for a list of 
value constants.   �    This function queries the value of a ViInt32 attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    �    Returns the current value of the attribute.  Pass the address of a ViInt32 variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type. 
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box.  
  Attributes with data types other than ViInt32 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   #�-   �  �    Instrument Handle                $N#����  �    Status                           +% � � �  �    Attribute Value                  ,� =  �  �    Channel Name                     -� = � � �    Attribute ID                           	           	            ""                0   �    This function queries the value of a ViReal64 attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    �    Returns the current value of the attribute.  Pass the address of a ViReal64 variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViReal64 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   5�-   �  �    Instrument Handle                6>#����  �    Status                           = � � �  �    Attribute Value                  >� =  �  �    Channel Name                     ?� = � � �    Attribute ID                           	           	           ""                0   4    This function queries the value of a ViString attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

You must provide a ViChar array to serve as a buffer for the value.  You pass the number of bytes in the buffer as the Buffer Size parameter.  If the current value of the attribute, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you want to call this function just to get the required buffer size, you can pass 0 for the Buffer Size and VI_NULL for the Attribute Value buffer.  

If you want the function to fill in the buffer regardless of the   number of bytes in the value, pass a negative number for the Buffer Size parameter.  


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    &    The buffer in which the function returns the current value of the attribute.  The buffer must be of type ViChar and have at least as many bytes as indicated in the Buffer Size parameter.

If the current value of the attribute, including the terminating NUL byte, contains more bytes that you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you specify 0 for the Buffer Size parameter, you can pass VI_NULL for this parameter.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the number of bytes in the ViChar array you specify for the Attribute Value parameter.  

If the current value of the attribute, including the terminating NUL byte, contains more bytes that you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Attribute Value buffer parameter.

Default Value: 512    �    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   J�-   �  �    Instrument Handle                K�#����  �    Status or Required Size          R� � L � �    Attribute Value                  V� =  �  �    Channel Name                     W� ;� �  �    Buffer Size                      Z� = � � �    Attribute ID                           	           	            ""    512                0   �    This function queries the value of a ViBoolean attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    �    Returns the current value of the attribute.  Pass the address of a ViBoolean variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   b�-   �  �    Instrument Handle                cB#����  �    Status                           j � � �  �    Attribute Value                  k� =  �  �    Channel Name                     l� = � � �    Attribute ID                           	           	            ""                0   �    This function queries the value of a ViSession attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    �    Returns the current value of the attribute.  Pass the address of a ViSession variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   t�-   �  �    Instrument Handle                u8#����  �    Status                           | � � �  �    Attribute Value                  }� =  �  �    Channel Name                     ~� = � � �    Attribute ID                           	           	            ""                0    S    This function checks the validity of a value you specify for a ViInt32 attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    @    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type. 
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box. 
  Attributes with data types other than ViInt32 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   �-   �  �    Instrument Handle                ��#����  �    Status                           �� � � �  �    Attribute Value                  �� =  �  �    Channel Name                    ���� � ���                                          �� = � � �    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    T    This function checks the validity of a value you specify for a ViReal64 attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViReal64 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   �-   �  �    Instrument Handle                ��#����  �    Status                           �� � � �  �    Attribute Value                  �� =  �  �    Channel Name                    ���� � ���                                          �� = � � �    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    T    This function checks the validity of a value you specify for a ViString attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   �-   �  �    Instrument Handle                ��#����  �    Status                           �� � � �  �    Attribute Value                  �� =  �  �    Channel Name                    ���� � ���                                          �� = � � �    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    U    This function checks the validity of a value you specify for a ViBoolean attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    D    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   �-   �  �    Instrument Handle                ��#����  �    Status                             � � �  �    Attribute Value                  �� =  �  �    Channel Name                    ���� � ���                                          �� = � � �    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    U    This function checks the validity of a value you specify for a ViSession attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     �    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1, 2

Default Value:  ""
    D    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   �-   �  �    Instrument Handle                ��#����  �    Status                           ԣ � � �  �    Attribute Value                  �� =  �  �    Channel Name                    ���� � ���                                          �� = � � �    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    X    This function takes a measurement on the output signal and returns the measured value.    �    Pass the measurement you want the power supply to take.

Valid Values:
KE230X_VAL_MEASURE_VOLTAGE - DC voltage measurement
KE230X_VAL_MEASURE_CURRENT - DC current measurement
KE230X_VAL_MEASURE_DVM     - Digital Voltmeter

Default Value: KE230X_VAL_MEASURE_VOLTAGE


Note:
If you want to perform the pulse current or the long integration measurement, use the ke230x_ConfigurePcurMeasurement, ke230x_ConfigurePcurDigitizer, ke230x_ConfigurePcurStep or ke230x_ConfigureLintMeasurement functions.
     m    Returns the measured value.

Units: volts (for voltage measurement)
       amps  (for current measurement)
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     o    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1, 2

Default Value:  ""
   � 3 � � �    Measurement Type                 � 3� �  �    Measurement                      �-   �  �    Instrument Handle                �?#����  �    Status                           � 3 
 �  �    Channel Name                                  oVoltage KE230X_VAL_MEASURE_VOLTAGE Current KE230X_VAL_MEASURE_CURRENT Digital Voltmeter KE230X_VAL_MEASURE_DVM    	               	           ""    F    This function configures the basic measurement on the output signal.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     o    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1, 2

Default Value:  ""
    k    Specifies the integration rate in line cycles for the voltage, current and digital voltmeter measurement functions of the instrument.
This parameter sets the KE230X_ATTR_MEAS_INTEGRATION_RATE attribute.

For the LINT and PCUR measurement modes this integration rate is not used by the instrument.

Valid Range: 0.01 to 10.0 power line cycles

Default Value: 1.0     �    Specifies the measurement average count for the voltage, current and digital voltmeter measurement functions.
This parameter sets the KE230X_ATTR_MEAS_AVERAGE_COUNT attribute.

Valid Range: 1 to 10

Default Value: 1   �-   �  �    Instrument Handle                �:#����  �    Status                           � 3 
 �  �    Channel Name                     � n � �  �    Integration Rate                 �� o� �  �    Average Count                   ���� m          Function                               	           ""    1.0    1               �Current KE230X_VAL_MEASURE_CURRENT Voltage KE230X_VAL_MEASURE_VOLTAGE DV Meter KE230X_VAL_MEASURE_DVM Pulse Current KE230X_VAL_MEASURE_PCURR Long Interation KE230X_VAL_MEASURE_LINT    K    This function configures the attributes for a pulse current measurement.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
    Y    Pass the integration time you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_INT_TIME_HIGH, KE230X_ATTR_MEAS_PCURR_INT_TIME_LOW or KE230X_ATTR_MEAS_PCURR_INT_TIME_AVERAGE attributes to this value according to the selected measurement mode.

Units: seconds

Valid Range:
33.33E-6 - 0.8333


Default Value:  3.333E-05
    4    Pass the pulse current measurement mode you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_TRIGGER_POLARITY attribute to this value. 

Valid Values:
KE230X_VAL_HIGH    - High
KE230X_VAL_LOW     - Low
KE230X_VAL_AVERAGE - Average triggered on high

Default Value:  KE230X_VAL_HIGH        Pass the averaging count for the pulse current measurements.

Single point measurement will return one value which is an average of given number of integrations.

Multiple point measurement will return this number of measurements.

Valid Range: 1 to 100

Default Value: 1

    Q    Pass the measurement trigger level you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL attribute to this value.

Units: amperes

Valid Range:

0.0 to 5.0 A     for 5.0 A trigger range
0.0 to 1.0 A     for 1.0 A trigger range
0.0 to 0.1 A     for 0.1 A trigger range


Default Value:  0.0 A
    ?    Pass the user measurement trigger delay you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_DELAY attribute to this value.

Units: seconds

Valid Range:
0.0 to 0.1       in pulse current measurement mode
0.0 to 5.0       in pulse current digitization mode


Default Value:  0.0
     �    Pass the PCUR timeout value you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_TIMEOUT attribute to this value.

Units: Secs

Valid Range:

0.005 - 1.00

Default Value:  1.000 Secs
    {    Pass the expected measurement trigger level to determine the measurement trigger level range of the power supply.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TLEV_RANGE attribute to this value.

Units: amperes

Valid Values:
KE230X_VAL_100_MILLI_AMP   - 100mA
KE230X_VAL_ONE_AMP         - 1 AMP
KE230X_VAL_FIVE_AMP        - 5 Amp

Default Value:  KE230X_VAL_100_MILLI_AMP
   �S-   �  �    Instrument Handle                �	#����  �    Status                           �� 3 
 �  �    Channel Name                      F 3� � �    Integration Time                 � u 
 � �    Trigger Polarity                 � 3 � � �    Average Count                    � � � �  �    Trigger Level                    W �� �  �    Trigger Delay                    � u � �  �    Timeout                          x �  � �    Trigger Level Range                    	           "1"    	3.333E-5               CHigh KE230X_VAL_HIGH Low KE230X_VAL_LOW Average KE230X_VAL_AVERAGE    1    0.0    0.0    1.0               S100 mA KE230X_VAL_100_MILLI_AMP 1 Amp KE230X_VAL_ONE_AMP 5 Amp KE230X_VAL_FIVE_AMP   �    This function configures the pulse current measurement or the long intergration measurement to autoset the integration time.

Call this function after calling one of the following to set instrument to auto integration time:
ConfigurePCURMeasurement
ConfigurePCURStep
ConfigureLINTMeasurement


By calling one of these listed functions that have a integration time parameter auto settime gets disabled.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
     !    Select PCUR or LINT Measurement   �-   �  �    Instrument Handle                O#����  �    Status                           & 3 
 �  �    Channel Name                     � |  �       Measurement                            	           "1"    Long Integration 1 PCurr 0    H    This function configures the attributes for digitizing pulse current.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
    4    Pass the pulse current measurement mode you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_TRIGGER_POLARITY attribute to this value. 

Valid Values:
KE230X_VAL_HIGH    - High
KE230X_VAL_LOW     - Low
KE230X_VAL_AVERAGE - Average triggered on high

Default Value:  KE230X_VAL_HIGH     U    Pass the number of samples to digitize.

Valid Range: 1 to 5000

Default Value: 1

    Q    Pass the measurement trigger level you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_LEVEL attribute to this value.

Units: amperes

Valid Range:

0.0 to 5.0 A     for 5.0 A trigger range
0.0 to 1.0 A     for 1.0 A trigger range
0.0 to 0.1 A     for 0.1 A trigger range


Default Value:  0.0 A
     �    Pass the user measurement trigger delay you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TRIGGER_DELAY attribute to this value.

Units: seconds

Valid Range: 0.0 - 5.0

Default Value:  0.0
    {    Pass the expected measurement trigger level to determine the measurement trigger level range of the power supply.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TLEV_RANGE attribute to this value.

Units: amperes

Valid Values:
KE230X_VAL_100_MILLI_AMP   - 100mA
KE230X_VAL_ONE_AMP         - 1 AMP
KE230X_VAL_FIVE_AMP        - 5 Amp

Default Value:  KE230X_VAL_100_MILLI_AMP
   -   �  �    Instrument Handle                �#����  �    Status                           � 3 
 �  �    Channel Name                      v 
 � �    Trigger Polarity                  = 3 � � �    Number Of Samples                 � � � �  �    Trigger Level                    !� �� �  �    Trigger Delay                    "� �  � �    Trigger Level Range                    	           "1"               CHigh KE230X_VAL_HIGH Low KE230X_VAL_LOW Average KE230X_VAL_AVERAGE    1    0.0    0.0               S100 mA KE230X_VAL_100_MILLI_AMP 1 Amp KE230X_VAL_ONE_AMP 5 Amp KE230X_VAL_FIVE_AMP    N    This function configures the attributes for step pulse current measurement.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     [    Pass the name of the channel to configure.

Valid Channel Names:  1

Default Value:  "1"
     �    Pass the number of steps for the step measurement. This parameter sets the KE230X_ATTR_MEAS_PCURR_STEP_COUNT atribute.

Specify the number of up steps using the KE230X_ATTR_MEAS_PCURR_STEP_UP attribute.

Valid Range: 1 to 20

Default Value: 1

    \    Pass the array of step trigger levels you want the power supply to use. Number of elements must be equal or greater than the value passed to Number Of Steps parameter.

Units: amperes

Valid Range:

0.0 to 5.0 A     for 5.0 A trigger range
0.0 to 1.0 A     for 1.0 A trigger range
0.0 to 0.1 A     for 0.1 A trigger range


Default Value:  0.0 A
     �    Pass the user measurement trigger delay you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_PCURR_STEP_DELAY attribute to this value.

Units: seconds

Valid Range: 0.0 - 5.0

Default Value:  0.0
     �    Pass the number of up steps for the step measurement. This parameter sets the KE230X_ATTR_MEAS_PCURR_STEP_UP attribute.

Valid Range: 0 to number of steps

Default Value: 1

     �    Pass the step integration time for up plus down steps. This parameter sets the KE230X_ATTR_MEAS_PCURR_STEP_TIME attribute.


Units: seconds

Valid Range: 33.330E-6 to 0.1 s

Default Value:  200.0E-6
     �    Specifies the timeout for the first step. This parameter sets the KE230X_ATTR_MEAS_PCURR_STEP1_TIMEOUT attribute.


Valid Range: 10.0 ms to 60.0 s

Default Value: 2.0 s     �    Specifies the timeout for the first step. This parameter sets the KE230X_ATTR_MEAS_PCURR_STEP_TIMEOUT attribute.

Unit: seconds

Valid Range: 2.0 ms to 200.0 ms

Default Value: 0.002 s (2 ms)    {    Pass the expected measurement trigger level to determine the measurement trigger level range of the power supply.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TLEV_RANGE attribute to this value.

Units: amperes

Valid Values:
KE230X_VAL_100_MILLI_AMP   - 100mA
KE230X_VAL_ONE_AMP         - 1 AMP
KE230X_VAL_FIVE_AMP        - 5 Amp

Default Value:  KE230X_VAL_100_MILLI_AMP
   ';-   �  �    Instrument Handle                '�#����  �    Status                           .� 3 
 �  �    Channel Name                     /+ 3 � � �    Number Of Steps                  0) � � �  �    Trigger Level                    1� �� 	�  �    Trigger Delay                    2n 2� � �    Number Of Up Steps               3& v 
 � �    Integration Time                 3� v � � �    First Step Timeout               4� v� � �    Step Timeout                     5r � 	 � �    Trigger Level Range                    	           "1"    1        0.0    1    	200.0E-6    2.0    0.002               S100 mA KE230X_VAL_100_MILLI_AMP 1 Amp KE230X_VAL_ONE_AMP 5 Amp KE230X_VAL_FIVE_AMP    N    This function configures the attributes for a long integration measurement.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

    E    Pass the long integration measurement trigger level you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_LINT_TRIGGER_LEVEL attribute to this value.

Units: amperes

Valid Range:
0.0 to 5.0 A        in 5 A range
0.0 to 1.0 A        in 1 A range
0.0 to 0.1 A        in 100 mA range

Default Value:  0.0 A
    F    Pass the measurement trigger edge you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_LINT_TRIGGER_EDGE attribute to this value.

Valid Values:
KE230X_VAL_RISING  - Rising trigger edge
KE230X_VAL_FALLING - Falling trigger edge
KE230X_VAL_NEITHER - Neither trigger edge

Default Value:  KE230X_VAL_RISING
     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
     �    Pass the timeout you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_LINT_TIMEOUT attribute to this value.

Units: seconds

Valid Range:  1.0 - 63.0

Default Value:  1.0        Pass the integration time you want the power supply to use.  The driver sets the KE230X_ATTR_MEAS_LINT_TIME attribute to this value.

Units: seconds

Valid Range:
0.85 - 60.0 for 60Hz line frequency
0.84 - 60.0 for 50Hz line frequency

Default Value:  1.0
    �    Pass the expected measurement trigger level to determine the measurement trigger level range of the power supply.  The driver sets the KE230X_ATTR_MEAS_PCURR_SYNC_TLEV_RANGE attribute to this value.

This is for channel 1 (Battery channel) only.

Units: amperes

Valid Values:
KE230X_VAL_100_MILLI_AMP   - 100mA
KE230X_VAL_ONE_AMP         - 1 AMP
KE230X_VAL_FIVE_AMP        - 5 Amp

Default Value:  KE230X_VAL_100_MILLI_AMP
   :<-   �  �    Instrument Handle                :�#����  �    Status                           A� w � �  �    Trigger Level                    C w� � �    Trigger Edge                     Dd 3 
 �  �    Channel Name                     D� �  � �    Timeout                          E� 3 � � �    Integration Time                 F� w  � �    Trigger Level Range                    	           0.0               ORising KE230X_VAL_RISING Falling KE230X_VAL_FALLING Neither KE230X_VAL_NEITHER    "1"    1.0    1.0               S100 mA KE230X_VAL_100_MILLI_AMP 1 Amp KE230X_VAL_ONE_AMP 5 Amp KE230X_VAL_FIVE_AMP    �    Fast, Search and Detect control how background readings are taken.

See table 4-3 in the users manual.


Default settings: FAST:OFF, SEARCH:ON, DETECT:OFF
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
     K    VI_TRUE: Enable FAST Mode
VI_FALSE: Disable FAST Mode

Default: VI_FALSE
     N    VI_TRUE: Enable SEARCH Mode
VI_FALSE: Disable SEARCH Mode

Default: VI_TRUE
     O    VI_TRUE: Enable DETECT Mode
VI_FALSE: Disable DETECT Mode

Default: VI_FALSE
     �    Choose measurement mode:

Long Integration(LINT) - 1 (KE230X_VAL_MEAS_MODE_LINT)
Pulse Current(PCUR) - 0 (KE230X_VAL_MEAS_MODE_PCUR)

Default: PCUR
   K�   �  �    Instrument Handle                L<����  �    Status                           S !  �  �    Channel Name                     Sy s 8 �       Enable Fast Mode                 S� t � �       Enable Pulse Search              T" r| �       Enable Detect Pulse Mode         Ty � 9 �       Measurement                            	           "1"    On 1 Off 0    On 1 Off 0    On 1 Off 0    Long Integration 1 PCurr 0       This function obtains the actual measurement point count.

If the KE230X_ATTR_MEAS_FUNCTION is set to KE230X_VAL_MEASURE_CURRENT
KE230X_VAL_MEASURE_VOLTAGE
KE230X_VAL_MEASURE_DVM

this attribute returns the value of the KE230X_ATTR_MEAS_AVERAGE_COUNT attribute.

If the KE230X_ATTR_MEAS_FUNCTION is set to KE230X_VAL_MEASURE_PCURR
this attribute returns the value of the KE230X_ATTR_MEAS_PCURR_AVERAGE attribute.

If the step mode is enabled this attribute returns the value of the KE230X_ATTR_MEAS_PCURR_STEP_COUNT attribute.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     ^    Pass the name of the channel to configure.

Valid Channel Names:  1, 2

Default Value:  "1"
     !    Returns the actual point count.   Y-   �  �    Instrument Handle                Y�#����  �    Status                           `� 3 
 �  �    Channel Name                     `� 3 � �  �    Point Count                            	           "1"    	            z    This function initiates a measurement, waits for the measurement operation to complete, and returns the measured value.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Returns the measured value.

Units:  The value you specify for the Function parameter determines the units of this parameter.

         DC Volts           - volts
         DC Current         - amperes
         DVM                - volts
         PCUR measurement   - amperes
         PCUR digitization  - amperes
         PCUR step          - amperes
         LINT               - amperes    �    Pass the maximum length of time in which to allow the read operation to complete.  If the operation does not complete within this time interval, the function returns the KE230X_ERROR_MAX_TIME_EXCEEDED error code.

Units: milliseconds

Defined Values:
KE230X_VAL_MAX_TIME_INFINITE (-1)  - There is no timeout.  The function waits until a measurement is available.

KE230X_VAL_MAX_TIME_IMMEDIATE (0) - If a measurement is not available, the function returns the KE230X_ERROR_MAX_TIME_EXCEEDED (0xBFFA201A) error code immediately.

Default Value: 5000 

Notes:

(1) The Maximum Time parameter affects only this function.  It has no effect on other timeout parameters or attributes.
   b�-   �  �    Instrument Handle                cL#����  �    Status                           j# 3 
 �  �    Channel Name                     j� 3� �  �    Measurement                      l 3 � �  �    Maximum Time [ms]                      	           ""    	           5000       This function initiates a multi-point measurement, waits for the measurement operation to complete, and returns the measured values.

This function automatically switchs to binary mode to do the data transfer. This is the fastest transfer mechanism and is transparent to the user.


     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Pass the maximum length of time in which to allow the read operation to complete.  If the operation does not complete within this time interval, the function returns the KE230X_ERROR_MAX_TIME_EXCEEDED (0xBFFA201A) error code.

Units: milliseconds

Defined Values:
KE230X_VAL_MAX_TIME_INFINITE (-1)  - There is no timeout.  The function waits until a measurement is available.

KE230X_VAL_MAX_TIME_IMMEDIATE (0) - If a measurement is not available, the function returns the KE230X_ERROR_MAX_TIME_EXCEEDED (0xBFFA201A) error code immediately.

Default Value: 5000 

Notes:

(1) The Maximum Time parameter affects only this function.  It has no effect on other timeout parameters or attributes.
     W    Pass the number of elements in the Measurement Array parameter.

Default Value: None
        Returns an array of measured values.

Units:  The value you specify for the Function parameter determines the units of this parameter.

         DC Volts           - volts
         DC Current         - amperes
         DVM                - volts
         PCUR measurement   - amperes
         PCUR digitization  - amperes
         PCUR step          - amperes
         LINT               - amperes

Notes:

(1) The size of the Measurement Array must be at least the size you specify in the Measurement Array Size parameter.
     ]    Returns the number of measurements the function places in the Measurement Array parameter.
   q$-   �  �    Instrument Handle                q�#����  �    Status                           x� 3 
 �  �    Channel Name                     y 3 � �  �    Maximum Time [ms]                {� � 
 �  �    Measurement Array Size           |8 � � �  �    Measurement Array                ~N �� �  �    Actual Number of Points                	           ""    5000        	            	            �    This function returns a reading from a previously initiated measurement.  You initiate a measurement by calling the ke230x_SendMeasSWTrigger function.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
    �    Returns the measured value.

Units:  The value you specify for the Function parameter determines the units of this parameter.

         DC Volts           - volts
         DC Current         - amperes
         DVM                - volts
         PCUR measurement   - amperes
         PCUR digitization  - amperes
         PCUR step          - amperes
         LINT               - amperes
   �-   �  �    Instrument Handle                ��#����  �    Status                           �� 3 
 �  �    Channel Name                     �� 3 � �  �    Measurement                            	           ""    	          L    This function returns a reading array from a previously initiated multi-point measurement.  You initiate a multi-point measurement by calling the ke230x_SendMeasSWTrigger function.

This function automatically switchs to binary mode to do the data transfer. This is the fastest transfer mechanism and is transparent to the user.

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
     W    Pass the number of elements in the Measurement Array parameter.

Default Value: None
        Returns an array of measured values.

Units:  The value you specify for the Function parameter determines the units of this parameter.

         DC Volts           - volts
         DC Current         - amperes
         DVM                - volts
         PCUR measurement   - amperes
         PCUR digitization  - amperes
         PCUR step          - amperes
         LINT               - amperes

Notes:

(1) The size of the Measurement Array must be at least the size you specify in the Measurement Array Size parameter.
     ]    Returns the number of measurements the function places in the Measurement Array parameter.
   ��-   �  �    Instrument Handle                ��#����  �    Status                           �^ 3 
 �  �    Channel Name                     �� } 
 �  �    Measurement Array Size           �' } � �  �    Measurement Array                �= }� �  �    Actual Number of Points                	           ""        	            	            Q    This function sends the measurement software trigger for the specified channel.     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
   �k-   �  �    Instrument Handle                �!#����  �    Status                           �� 3 
 �  �    Channel Name                           	           ""    �    This function returns whether an acquisition has been completed or not on the given channel.

Notes:

(1) This function does not check the instrument status.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    Returns the acquisition status of the oscilloscope.  Possible values that this parameter returns are:

VI_FALSE (0) - Operation not complete
VI_TRUE  (1) - Oeration complete
     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
   ��-   �  �    Instrument Handle                �u#����  �    Status                           �L 3 � �  �    Complete                         � 3 
 �  �    Channel Name                           	           	            ""   +    This function returns whether the power supply is in a particular output state.

A constant voltage condition occurs when the output voltage is equal to the value of the KE230X_ATTR_VOLTAGE_LEVEL attribute and the current is less than or equal to the value of the KE230X_ATTR_CURRENT_LIMIT attribute.

A constant current condition occurs when the output current is equal to the value of the KE230X_ATTR_CURRENT_LIMIT attribute and the KE230X_ATTR_CURRENT_LIMIT_BEHAVIOR attribute is set to KE230X_VAL_CURRENT_REGULATE.

An unregulated condition occurs when the output voltage is less than the value of the KE230X_ATTR_VOLTAGE_LEVEL attribute and the current is less than the value of the KE230X_ATTR_CURRENT_LIMIT attribute.

An over-voltage condition occurs when the output voltage is equal to or greater than the value of the KE230X_ATTR_OVP_LIMIT attribute and the KE230X_ATTR_OVP_ENABLED attribute is set to VI_TRUE.

An over-current condition occurs when the output current is equal to or greater than the value of the KE230X_ATTR_CURRENT_LIMIT attribute and the KE230X_ATTR_CURRENT_LIMIT_BEHAVIOR attribute is set to KE230X_VAL_CURRENT_TRIP.  

When either an over-voltage condition or an over-current condition occurs, the power supply's output protection disables the output.  If the power supply is in an over-voltage or over-current state, it does not produce power until the output protection is reset.  The ke230x_ResetOutputProtection function resets the output protection.  Once the output protection is reset, the power supply resumes generating a power signal.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     v    Pass the name of the channel on which to query for an output state.

Valid Channel Names:  1, 2

Default Value:  ""
        Pass the output state for which you want to query.

Valid Values:
KE230X_VAL_OUTPUT_CONSTANT_CURRENT - Constant Current State

KE230X_VAL_OUTPUT_OVER_VOLTAGE - Over-voltage State

KE230X_VAL_OUTPUT_OVER_CURRENT - Over-current State


Default Value:
KE230X_VAL_OUTPUT_CONSTANT_CURRENT
     �    This parameter returns VI_TRUE if the power supply is currently in the state you specify with the outputState parameter, and VI_FALSE if it is not.
   ��-   �  �    Instrument Handle                �M#����  �    Status                           �$ 3 
 �  �    Channel Name                     �� 3 � � �    Output State                     �� 3� �  �    In State                               	           ""               �Constant Current KE230X_VAL_OUTPUT_CONSTANT_CURRENT Over Voltage KE230X_VAL_OUTPUT_OVER_VOLTAGE Over Current KE230X_VAL_OUTPUT_OVER_CURRENT    	           i    This function returns the maximum programmable current limit that the power supply accepts for a particular voltage level on a channel for the output range to which the power supply is currently configured.

This is a supported IVI DC Pwr Class function. The 230x power supplies just return the max current limit so the 'voltage level' parameter is ignored.

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    Pass the name of the channel on which to query for the maximum programmable current limit.

Valid Channel Names:  1, 2

Default Value:  ""
     �    For 230x this parameter is ignored.

Pass the voltage level for which to determine the maximum programmable current limit.  

Units: volts (V).

Valid Range:  0.0 - 15.0 volts

Default Value: 0.0
     s    This parameter returns the maximum programmable current limit for the voltage level you specify.

Units: amps (A)   ��-   �  �    Instrument Handle                �S#����  �    Status                           �* 3 
 �  �    Channel Name                     ǿ 3 � � �    Voltage Level                    ȍ 3� �  �    Max Current Limit                      	           ""    0.0    	          �    This function returns the maximum programmable voltage level that the power supply accepts for a particular current limit on a channel for the output range to which the power supply is currently configured.

This is a supported IVI DC Pwr class function.

The 230x power supplies don't support querying for the max voltage level for the particular current limit. The current limit value is ignored and the max voltage level is returned from the instrument.

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    Pass the name of the channel on which to query for the maximum programmable voltage level.

Valid Channel Names:  1, 2

Default Value:  ""
    F    This parameter is ignored.

Pass the current limit for which to determine the maximum programmable voltage level.  

Units: amps (A).

Valid Range:

Current Range        Min         Max
=======================================
0.006 A              0.0 A       1.0 A
5.0 A                0.006 A     5.0 A

Default Value: 0.0
     t    This parameter returns the maximum programmable voltage level for the current limit you specify.

Units: volts (V)   �-   �  �    Instrument Handle                ��#����  �    Status                           ӛ 3 
 �  �    Channel Name                     �0 3 � � �    Current Limit                    �~ 3� �  �    Max Voltage Level                      	           ""    0.0    	               This function is part of IVI DCPwr Class spec.

For the Ke230x there is no command that the driver can send to reset the output protection. User needs to remove the overload condition and switch the output back on using Ke230x_ConfigureOutputEnabled.



     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     b    Pass the name of the channel you want to reset.

Valid Channel Names:  1, 2

Default Value:  ""
   �4-   �  �    Instrument Handle                ��#����  �    Status                           �� 3 
 �  �    Channel Name                           	           ""    Z    This function runs the instrument's self test routine and returns the test result(s). 

    &    This control contains the value returned from the instrument self test.  Zero means success.  For any other code, see the device's operator's manual.

Self-Test Code    Description
---------------------------------------
   0              Passed self test
   1              Self test failed

     �    Returns the self-test response string from the instrument. See the device's operation manual for an explanation of the string's contents.

You must pass a ViChar array with at least 256 bytes.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   �B =  �  �    Self Test Result                 �p = � � ,    Self-Test Message                �:#����  �    Status                           �-   �  �    Instrument Handle                  	           	            	              &    This function resets the instrument to a known state and sends initialization commands to the instrument.  The initialization commands set instrument settings such as Headers Off, Short Command form, and Data Transfer Binary to the state necessary for the operation of the instrument driver.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ��#����  �    Status                           ��-   �  �    Instrument Handle                  	               �    This function resets the instrument and applies initial user specified settings from the Logical Name which was used to initialize the session.  If the session was created without a Logical Name, this function is equivalent to the ke230x_reset function.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ��#����  �    Status                           ��-   �  �    Instrument Handle                  	               �    This function places the instrument in a quiescent state where it has minimal or no impact on the system to which it is connected.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ��#����  �    Status                           �-   �  �    Instrument Handle                  	               `    This function returns the revision numbers of the instrument driver and instrument firmware.

     �    Returns the instrument driver software revision numbers in the form of a string.

You must pass a ViChar array with at least 256 bytes.     �    Returns the instrument firmware revision numbers in the form of a string.

You must pass a ViChar array with at least 256 bytes.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   < = 3 �  �    Instrument Driver Revision       � =6 �  �    Firmware Revision                X#����  �    Status                           /-   �  �    Instrument Handle                  	            	            	               V    This function reads an error code and a message from the instrument's error queue.

     B    Returns the error code read from the instrument's error queue.

     �    Returns the error message string read from the instrument's error message queue.

You must pass a ViChar array with at least 256 bytes.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ? =  �  �    Error Code                       � = � � ,    Error Message                    #����  �    Status                           �-   �  �    Instrument Handle                  	            	            	               n    This function converts a status code returned by an instrument driver function into a user-readable string.     %b    Pass the Status parameter that is returned from any of the instrument driver functions.
          
Default Value:  0  (VI_SUCCESS)
          
ke230x Status Codes:
Status    Description
-------------------------------------------------

ERRORS:

BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED

IviPower Status Codes:
Status    Description
-------------------------------------------------
ERRORS:
BFFA1001  The trigger source is not software trigger.

IVI Engine Status Codes:
Status    Description
-------------------------------------------------
ERRORS:
BFFA0001  Instrument error. Call ke230x_error_query.
BFFA0002  Cannot open file.
BFFA0003  Error reading from file.
BFFA0004  Error writing to file.
BFFA0005  Driver module file not found.
BFFA0006  Cannot open driver module file for reading.
BFFA0007  Driver module has invalid file format or invalid data.
BFFA0008  Driver module contains undefined references.
BFFA0009  Cannot find function in driver module.
BFFA000A  Failure loading driver module.
BFFA000B  Invalid path name.
BFFA000C  Invalid attribute.
BFFA000D  IVI attribute is not writable.
BFFA000E  IVI attribute is not readable.
BFFA000F  Invalid parameter.
BFFA0010  Invalid value.
BFFA0011  Function not supported.
BFFA0012  Attribute not supported.
BFFA0013  Value not supported.
BFFA0014  Invalid type.
BFFA0015  Types do not match.
BFFA0016  Attribute already has a value waiting to be updated.
BFFA0017  Specified item already exists.
BFFA0018  Not a valid configuration.
BFFA0019  Requested item does not exist or value not available.
BFFA001A  Requested attribute value not known.
BFFA001B  No range table.
BFFA001C  Range table is invalid.
BFFA001D  Object or item is not initialized.
BFFA001E  Non-interchangeable behavior.
BFFA001F  No channel table has been built for the session.
BFFA0020  Channel name specified is not valid.
BFFA0021  Unable to allocate system resource.
BFFA0022  Permission to access file was denied.
BFFA0023  Too many files are already open.
BFFA0024  Unable to create temporary file in target directory.
BFFA0025  All temporary filenames already used.
BFFA0026  Disk is full.
BFFA0027  Cannot find configuration file on disk.
BFFA0028  Cannot open configuration file.
BFFA0029  Error reading configuration file.
BFFA002A  Invalid ViInt32 value in configuration file.
BFFA002B  Invalid ViReal64 value in configuration file.
BFFA002C  Invalid ViBoolean value in configuration file.
BFFA002D  Entry missing from configuration file.
BFFA002E  Initialization failed in driver DLL.
BFFA002F  Driver module has unresolved external reference.
BFFA0030  Cannot find CVI Run-Time Engine.
BFFA0031  Cannot open CVI Run-Time Engine.
BFFA0032  CVI Run-Time Engine has invalid format.
BFFA0033  CVI Run-Time Engine is missing required function(s).
BFFA0034  CVI Run-Time Engine initialization failed.
BFFA0035  CVI Run-Time Engine has unresolved external reference.
BFFA0036  Failure loading CVI Run-Time Engine.
BFFA0037  Cannot open DLL for read exports.
BFFA0038  DLL file is corrupt.
BFFA0039  No DLL export table in DLL.
BFFA003A  Unknown attribute name in default configuration file.
BFFA003B  Unknown attribute value in default configuration file.
BFFA003C  Memory pointer specified is not known.
BFFA003D  Unable to find any channel strings.
BFFA003E  Duplicate channel string.
BFFA003F  Duplicate virtual channel name.
BFFA0040  Missing virtual channel name.
BFFA0041  Bad virtual channel name.
BFFA0042  Unassigned virtual channel name.
BFFA0043  Bad virtual channel assignment.
BFFA0044  Channel name required.
BFFA0045  Channel name not allowed.
BFFA0046  Attribute not valid for channel.
BFFA0047  Attribute must be channel based.
BFFA0048  Channel already excluded.
BFFA0049  Missing option name (nothing before the '=').
BFFA004A  Missing option value (nothing after the '=').
BFFA004B  Bad option name.
BFFA004C  Bad option value.
BFFA004D  Operation only valid on a class driver session.
BFFA004E  "ivi.ini" filename is reserved.
BFFA004F  Duplicate run-time configuration entry.
BFFA0050  Index parameter is one-based.
BFFA0051  Index parameter is too high.
BFFA0052  Attribute is not cacheable.
BFFA0053  You cannot export a ViAddr attribute to the end-user.
BFFA0054  Bad channel string in channel string list.
BFFA0055  Bad prefix name in default configuration file.
         
VISA Status Codes:
Status    Description
-------------------------------------------------
WARNINGS:
3FFF0002 Event enabled for one or more specified mechanisms.
3FFF0003 Event disabled for one or more specified mechanisms.
3FFF0004 Successful, but queue already empty.
3FFF0005 Specified termination character was read.
3FFF0006 Number of bytes transferred equals input count.
3FFF0077 Configuration non-existant or could not be loaded.
3FFF007D Open successful, but the device not responding.
3FFF0080 Wait successful, but more event objects available.
3FFF0082 Specified object reference is uninitialized.
3FFF0084 Attribute value not supported.
3FFF0085 Status code could not be interpreted.
3FFF0088 Specified I/O buffer type not supported.
3FFF0098 Successful,  but invoke no handlers for this event.
3FFF0099 Successful but session has nested shared locks.
3FFF009A Successful but session has nested exclusive locks.
3FFF009B Successful but operation not asynchronous.
         
ERRORS:
BFFF0000 Unknown system error (miscellaneous error).
BFFF000E Session or object reference is invalid.
BFFF000F Resource is locked.
BFFF0010 Invalid expression specified for search.
BFFF0011 Resource is not present in the system.
BFFF0012 Invalid resource reference specified.  Parsing error.
BFFF0013 Invalid access mode.
BFFF0015 Timeout expired before operation completed.
BFFF0016 Unable to deallocate session data structures.
BFFF001B Specified degree is invalid.
BFFF001C Specified job identifier is invalid.
BFFF001D Attribute is not supported by the referenced object.
BFFF001E Attribute state not supported by the referenced object.
BFFF001F Specified attribute is read-only.
BFFF0020 Lock type lock not supported by this resource.
BFFF0021 Invalid access key.
BFFF0026 Specified event type not supported by the resource.
BFFF0027 Invalid mechanism specified.
BFFF0028 A handler was not installed.
BFFF0029 Handler reference either invalid or was not installed.
BFFF002A Specified event context invalid.
BFFF002D Event queue for specified type has overflowed.
BFFF002F Event type must be enabled in order to receive.
BFFF0030 User abort during transfer.
BFFF0034 Violation of raw write protocol during transfer.
BFFF0035 Violation of raw read protocol during transfer.
BFFF0036 Device reported output protocol error during transfer.
BFFF0037 Device reported input protocol error during transfer.
BFFF0038 Bus error during transfer.
BFFF0039 Unable to queue asynchronous operation.
BFFF003A Unable to start operation because setup is invalid.
BFFF003B Unable to queue the asynchronous operation.
BFFF003C Insufficient resources to perform memory allocation.
BFFF003D Invalid buffer mask specified.
BFFF003E I/O error.
BFFF003F Format specifier invalid.
BFFF0041 Format specifier not supported.
BFFF0042 Trigger line is currently in use.
BFFF004A Service request not received for the session.
BFFF004E Invalid address space specified.
BFFF0051 Invalid offset specified.
BFFF0052 Invalid access width specified.
BFFF0054 Offset not accessible from this hardware.
BFFF0055 Source and destination widths are different.
BFFF0057 Session not currently mapped.
BFFF0059 Previous response still pending.
BFFF005F No listeners condition detected.
BFFF0060 Interface not currently the controller in charge.
BFFF0061 Interface not the system controller.
BFFF0067 Session does not support this operation.
BFFF006A A parity error occurred during transfer.
BFFF006B A framing error occurred during transfer.
BFFF006C An overrun error occurred during transfer.
BFFF0070 Offset not properly aligned for operation access width.
BFFF0071 Specified user buffer not valid.
BFFF0072 Resource valid, but VISA cannot access it.
BFFF0076 Width not supported by this hardware.
BFFF0078 Invalid parameter value, parameter unknown.
BFFF0079 Invalid protocol.
BFFF007B Invalid window size.
BFFF0080 Session currently contains a mapped window.
BFFF0081 Operation not implemented.
BFFF0083 Invalid length.
BFFF0091 Invalid mode.
BFFF009C Session did not have a lock on the resource.
BFFF009D The device does not export any memory.
BFFF009E VISA-required code library not located or not loaded.
         
VXIPnP Driver Status Codes:
Status    Description
-------------------------------------------------
WARNINGS:
3FFC0101  Instrument does not have ID Query capability.
3FFC0102  Instrument does not have Reset capability.
3FFC0103  Instrument does not have Self-Test capability.
3FFC0104  Instrument does not have Error Query capability.
3FFC0105  Instrument does not have Revision Query capability.
          
ERRORS:
BFFC0001  Parameter 1 out of range, or error trying to set it.
BFFC0002  Parameter 2 out of range, or error trying to set it.
BFFC0003  Parameter 3 out of range, or error trying to set it.
BFFC0004  Parameter 4 out of range, or error trying to set it.
BFFC0005  Parameter 5 out of range, or error trying to set it.
BFFC0006  Parameter 6 out of range, or error trying to set it.
BFFC0007  Parameter 7 out of range, or error trying to set it.
BFFC0008  Parameter 8 out of range, or error trying to set it.
BFFC0011  Instrument failed the ID Query.
BFFC0012  Invalid response from instrument.     �    Returns the user-readable message string that corresponds to the status code you specify.

You must pass a ViChar array with at least 256 bytes.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

        The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

You can pass VI_NULL for this parameter.  This is useful when one of the initialize functions fail.

Default Value:  VI_NULL
    =  �  h    Error Code                       @� = � � �    Error Message                    A#����  �    Status                           G�-   �  �    Instrument Handle                  0    	            	           VI_NULL    P    This function invalidates the cached values of all attributes for the session.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   Jh#����  �    Status                           Q\-   �  �    Instrument Handle                  	              b    This function returns the highest-level channel name that corresponds to the specific driver channel string that is in the channel table at an index you specify.  By passing 0 for the buffer size, the caller can ascertain the buffer size required to get the entire channel name string and then call the function again with a sufficiently large buffer.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     <    A 1-based index into the channel table.

Default Value: 1
    �    Returns the highest-level channel name that corresponds to the specific driver channel string that is in the channel table at an index you specify..

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the channel string, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.    �    Pass the number of bytes in the ViChar array you specify for the channel name parameter.

If the channel name, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies BufferSize - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Description buffer parameter.

Default Value:  None   S�-   �  �    Instrument Handle                T�#����  �    Status                           ]^ = Q �  �    Index                            ]� � Q � �    Channel String                   `� =@ �  �    BufferSize                             	           1    	               �    This function retrieves and then clears the IVI error information for the session or the current execution thread. One exception exists: If the BufferSize parameter is 0, the function does not clear the error information. By passing 0 for the buffer size, the caller can ascertain the buffer size required to get the entire error description string and then call the function again with a sufficiently large buffer.

If the user specifies a valid IVI session for the InstrumentHandle parameter, Get Error retrieves and then clears the error information for the session.  If the user passes VI_NULL for the InstrumentHandle parameter, this function retrieves and then clears the error information for the current execution thread.  If the InstrumentHandle parameter is an invalid session, the function does nothing and returns an error. Normally, the error information describes the first error that occurred since the user last called ke230x_GetError or ke230x_ClearError.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Pass the number of bytes in the ViChar array you specify for the Description parameter.

If the error description, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies BufferSize - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Description buffer parameter.

Default Value:  None     �    Returns the error code for the session or execution thread.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.
    �    Returns the error description for the IVI session or execution thread.  If there is no description, the function returns an empty string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the error description, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.   h�#����  �    Status                           q9-   �  �    Instrument Handle                q� => �  �    BufferSize                       t� = Q �  �    Code                             ub � Q � �    Description                        	                   	           	           �    This function clears the error information for the current execution thread and the IVI session you specify.  If you pass VI_NULL for the Instrument Handle parameter, this function clears the error information only for the current execution thread. 

The error information includes a primary error code, secondary error code, and an error elaboration string.  For a particular session, this information is the same as the values held in the following attributes:

KE230X_ATTR_PRIMARY_ERROR
KE230X_ATTR_SECONDARY_ERROR
KE230X_ATTR_ERROR_ELABORATION

This function sets the primary and secondary error codes to VI_SUCCESS (0), and sets the error elaboration string to "".

The IVI Library also maintains this error information separately for each thread.  This is useful if you do not have a session handle to pass to ke230x_ClearErrorInfo or ke230x_GetErrorInfo, which occurs when a call to ke230x_init or ke230x_InitWithOptions fails.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   }E#����  �    Status                           �-   �  �    Instrument Handle                  	              9    This function returns the error information associated with an IVI session or with the current execution thread.  If you specify a valid IVI session for the Instrument Handle parameter, this function retrieves and then clears the error information for the session.  If you pass VI_NULL for the Instrument Handle parameter, this function retrieves and then clears the error information for the current execution thread.  

The error information includes a primary error, secondary error, and an error elaboration string.  For a particular session, this information is the same as the values held in the following attributes:

KE230X_ATTR_PRIMARY_ERROR
KE230X_ATTR_SECONDARY_ERROR
KE230X_ATTR_ERROR_ELABORATION

The IVI Library also maintains this error information separately for each thread.  This is useful if you do not have a session handle to pass to ke230x_GetErrorInfo or ke230x_ClearErrorInfo, which
occurs when a call to ke230x_init or ke230x_InitWithOptions fails.

You can call ke230x_error_message to obtain a text description of the primary or secondary error value.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  VI_NULL
    �    The primary error code for the session or execution thread.   The value is the same as the one held by the attribute KE230X_ATTR_PRIMARY_ERROR.
  
A value of VI_SUCCESS (0) indicates that no error occurred,  A positive value indicates a warning.  A negative value indicates an error.  

You can call ke230x_error_message to get a text description of the value.

If you are not interested in this value, you can pass VI_NULL.    �    The secondary error code for the session or execution thread.  If the primary error code is non-zero, the secondary error code can further describe the error or warning condition.  The value is the same as the one held by the attribute KE230X_ATTR_SECONDARY_ERROR.

A value of VI_SUCCESS (0) indicates no further description.  

You can call ke230x_error_message to get a text description of the value.

If you are not interested in this value, you can pass VI_NULL.
    �    The error elaboration string for the session or execution thread.  If the primary error code is non-zero, the elaboration string can further describe the error or warning condition.   The value is the same as the one held by the attribute KE230X_ATTR_ERROR_ELABORATION.  

If you are not interested in this value, you can pass VI_NULL.  Otherwise, you must pass a ViChar array with at least 256 bytes.
   ��#����  �    Status                           �d-   �  �    Instrument Handle                � 5 R �  �    Primary Error                    �� 5A �  �    Secondary Error                  �� � Q � �    Error Elaboration                  	           VI_NULL    	           	           	           �    This function clears the error information for the current execution thread and the IVI session you specify.  If you pass VI_NULL for the Instrument Handle parameter, this function clears the error information only for the current execution thread. 

The error information includes a primary error code, secondary error code, and an error elaboration string.  For a particular session, this information is the same as the values held in the following attributes:

KE230X_ATTR_PRIMARY_ERROR
KE230X_ATTR_SECONDARY_ERROR
KE230X_ATTR_ERROR_ELABORATION

This function sets the primary and secondary error codes to VI_SUCCESS (0), and sets the error elaboration string to "".

The IVI Library also maintains this error information separately for each thread.  This is useful if you do not have a session handle to pass to ke230x_ClearErrorInfo or ke230x_GetErrorInfo, which occurs when a call to ke230x_init or ke230x_InitWithOptions fails.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   �>#����  �    Status                           �-   �  �    Instrument Handle                  	              �    This function returns the coercion information associated with the IVI session.  This function retrieves and clears the oldest instance in which the instrument driver coerced a value you specified to another value.

If you set the KE230X_ATTR_RECORD_COERCIONS attribute to VI_TRUE, the instrument driver keeps a list of all coercions it makes on ViInt32 or ViReal64 values you pass to instrument driver functions.  You use this function to retrieve information from that list.

If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Coercion Record buffer parameter.

The function returns an empty string in the Coercion Record parameter if no coercion records remain for the session.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init function.  The handle identifies a particular instrument session.

Default Value:  None    �    Returns the next coercion record for the IVI session.  If there are no coercion records, the function returns an empty string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

This parameter returns an empty string if no coercion records remain for the session.
    �    Pass the number of bytes in the ViChar array you specify for the Coercion Record parameter.

If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Coercion Record buffer parameter.

Default Value:  None

   �#����  �    Status or Required Size          ��-   �  �    Instrument Handle                �K � Q � �    Coercion Record                  �Q = � �  �    Buffer Size                        	               	               -    This function returns the interchangeability warnings associated with the IVI session. It retrieves and clears the oldest instance in which the class driver recorded an interchangeability warning.  Interchangeability warnings indicate that using your application with a different instrument might cause different behavior. You use this function to retrieve interchangeability warnings.

The driver performs interchangeability checking when the KE230X_ATTR_INTERCHANGE_CHECK attribute is set to VI_TRUE.

The function returns an empty string in the Interchange Warning parameter if no interchangeability warnings remain for the session.

In general, the instrument driver generates interchangeability warnings when an attribute that affects the behavior of the instrument is in a state that you did not specify.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Pass the number of bytes in the ViChar array you specify for the Interchange Warning parameter.

If the next interchangeability warning string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value. For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Interchange Warning buffer parameter.

Default Value:  None
        Returns the next interchange warning for the IVI session. If there are no interchange warnings, the function returns an empty  string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter. If the next interchangeability warning string, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

This parameter returns an empty string if no interchangeability
warnings remain for the session.

   �o#����  �    Status or Required Size          �c-   �  �    Instrument Handle                � = � �  �    Buffer Size                      � � Q � �    Interchange Warning                	                   	            A    This function clears the list of current interchange warnings.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors   ʀ-   �  �    Instrument Handle                �6#����  �    Status                                 	          �    When developing a complex test system that consists of multiple test modules, it is generally a good idea to design the test modules so that they can run in any order.  To do so requires ensuring that each test module completely configures the state of each instrument it uses.  If a particular test module does not completely configure the state of an instrument, the state of the instrument depends on the configuration from a previously executed test module.  If you execute the test modules in a different order, the behavior of the instrument and therefore the entire test module is likely to change.  This change in behavior is generally instrument specific and represents an interchangeability problem.

You can use this function to test for such cases.  After you call this function, the interchangeability checking algorithms in the specific driver ignore all previous configuration operations.  By calling this function at the beginning of a test module, you can determine whether the test module has dependencies on the operation of previously executed test modules.

This function does not clear the interchangeability warnings from the list of previously recorded interchangeability warnings.  If you want to guarantee that the ke230x_GetNextInterchangeWarning function only returns those interchangeability warnings that are generated after calling this function, you must clear the list of interchangeability warnings.  You can clear the interchangeability warnings list by repeatedly calling the ke230x_GetNextInterchangeWarning function until no more interchangeability warnings are returned.  If you are not interested in the content of those warnings, you can call the ke230x_ClearInterchangeWarnings function.
     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetError function.  To clear the error information from the driver, call the ke230x_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
3FFA4001  Current decreased to satisfy voltage requirements.
3FFA4002  Voltage decreased to satisfy current requirements.
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviFgen  Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviFgen  Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors   �p-   �  �    Instrument Handle                �&#����  �    Status                                 	          "    This function obtains a multithread lock on the instrument session.  Before it does so, it waits until all other execution threads have released their locks on the instrument session.

Other threads might have obtained a lock on this session in the following ways:

- The user's application called ke230x_LockSession.

- A call to the instrument driver locked the session.

- A call to the IVI engine locked the session.

After your call to ke230x_LockSession returns successfully, no other threads can access the instrument session until you call ke230x_UnlockSession.

Use ke230x_LockSession and ke230x_UnlockSession around a sequence of calls to instrument driver functions if you require that the instrument retain its settings through the end of the sequence.

You can safely make nested calls to ke230x_LockSession within the same thread.  To completely unlock the session, you must balance each call to ke230x_LockSession with a call to ke230x_UnlockSession.  If, however, you use the Caller Has Lock parameter in all calls to ke230x_LockSession and ke230x_UnlockSession within a function, the IVI Library locks the session only once within the function regardless of the number of calls you make to ke230x_LockSession.  This allows you to call ke230x_UnlockSession just once at the end of the function. 
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    This parameter serves as a convenience.  If you do not want to use this parameter, pass VI_NULL. 

Use this parameter in complex functions to keep track of whether you obtain a lock and therefore need to unlock the session.  Pass the address of a local ViBoolean variable.  In the declaration of the local variable, initialize it to VI_FALSE.  Pass the address of the same local variable to any other calls you make to ke230x_LockSession or ke230x_UnlockSession in the same function.

The parameter is an input/output parameter.  ke230x_LockSession and ke230x_UnlockSession each inspect the current value and take the following actions:

- If the value is VI_TRUE, ke230x_LockSession does not lock the session again.  If the value is VI_FALSE, ke230x_LockSession obtains the lock and sets the value of the parameter to VI_TRUE.

- If the value is VI_FALSE, ke230x_UnlockSession does not attempt to unlock the session.  If the value is VI_TRUE, ke230x_UnlockSession releases the lock and sets the value of the parameter to VI_FALSE.
 
Thus, you can, call ke230x_UnlockSession at the end of your function without worrying about whether you actually have the lock.  

Example:

ViStatus TestFunc (ViSession vi, ViInt32 flags)
{
    ViStatus error = VI_SUCCESS;
    ViBoolean haveLock = VI_FALSE;

    if (flags & BIT_1)
        {
        viCheckErr( ke230x_LockSession(vi, &haveLock));
        viCheckErr( TakeAction1(vi));
        if (flags & BIT_2)
            {
            viCheckErr( ke230x_UnlockSession(vi, &haveLock));
            viCheckErr( TakeAction2(vi));
            viCheckErr( ke230x_LockSession(vi, &haveLock);
            } 
        if (flags & BIT_3)
            viCheckErr( TakeAction3(vi));
        }

Error:
    /* 
       At this point, you cannot really be sure that 
       you have the lock.  Fortunately, the haveLock 
       variable takes care of that for you.          
    */
    ke230x_UnlockSession(vi, &haveLock);
    return error;
}   �#����  �    Status                           �-   �  �    Instrument Handle                �K H � �  �    Caller Has Lock                    	               	            �    This function releases a lock that you acquired on an instrument session using ke230x_LockSession.  Refer to ke230x_LockSession for additional information on session locks.
    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    �    This parameter serves as a convenience.  If you do not want to use this parameter, pass VI_NULL. 

Use this parameter in complex functions to keep track of whether you obtain a lock and therefore need to unlock the session. 
Pass the address of a local ViBoolean variable.  In the declaration of the local variable, initialize it to VI_FALSE.  Pass the address of the same local variable to any other calls you make to ke230x_LockSession or ke230x_UnlockSession in the same function.

The parameter is an input/output parameter.  ke230x_LockSession and ke230x_UnlockSession each inspect the current value and take the following actions:

- If the value is VI_TRUE, ke230x_LockSession does not lock the session again.  If the value is VI_FALSE, ke230x_LockSession obtains the lock and sets the value of the parameter to VI_TRUE.

- If the value is VI_FALSE, ke230x_UnlockSession does not attempt to unlock the session.  If the value is VI_TRUE, ke230x_UnlockSession releases the lock and sets the value of the parameter to VI_FALSE.
 
Thus, you can, call ke230x_UnlockSession at the end of your function without worrying about whether you actually have the lock.  

Example:

ViStatus TestFunc (ViSession vi, ViInt32 flags)
{
    ViStatus error = VI_SUCCESS;
    ViBoolean haveLock = VI_FALSE;

    if (flags & BIT_1)
        {
        viCheckErr( ke230x_LockSession(vi, &haveLock));
        viCheckErr( TakeAction1(vi));
        if (flags & BIT_2)
            {
            viCheckErr( ke230x_UnlockSession(vi, &haveLock));
            viCheckErr( TakeAction2(vi));
            viCheckErr( ke230x_LockSession(vi, &haveLock);
            } 
        if (flags & BIT_3)
            viCheckErr( TakeAction3(vi));
        }

Error:
    /* 
       At this point, you cannot really be sure that 
       you have the lock.  Fortunately, the haveLock 
       variable takes care of that for you.          
    */
    ke230x_UnlockSession(vi, &haveLock);
    return error;
}   �s#����  �    Status                           �J-   �  �    Instrument Handle                �  H � �  �    Caller Has Lock                    	               	            �    This function writes a user-specified string to the instrument.

Note:  This function bypasses IVI attribute state caching.  Therefore, when you call this function, the cached values for all attributes will be invalidated.     2    Pass the string to be written to the instrument.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   Y A � �  �    Write Buffer                     �#����  �    Status                           j-   �  �    Instrument Handle                      	               /    This function reads data from the instrument.     c    After this function executes, this parameter contains the data that was read from the instrument.    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     �    Pass the maximum number of bytes to read from the instruments.  

Valid Range:  0 to the number of elements in the Read Buffer.

Default Value:  0

     ^    Returns the number of bytes actually read from the instrument and stored in the Read Buffer.   
 @ �  �    Read Buffer                      u#����  �    Status                           L-   �  �    Instrument Handle                 @ C �  �    Number of Bytes To Read          � �' �  �    Num Bytes Read                     	            	               0    	           �    This function performs the following operations:

- Closes the instrument I/O session.

- Destroys the instrument driver session and all of its attributes.

- Deallocates any memory resources the driver uses.

Notes:

(1) You must unlock the session before calling ke230x_close.

(2) After calling ke230x_close, you cannot use the instrument driver again until you call ke230x_init or ke230x_InitWithOptions.

    �    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the ke230x_error_message function.  To obtain additional information about the error condition, call the ke230x_GetErrorInfo function.  To clear the error information from the driver, call the ke230x_ClearErrorInfo function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
BFFA4001  KE230X_ERROR_RESET_PROTECTION_NOT_SUPPORTED
BFFA4002  KE230X_ERROR_MAX_TIME_EXCEEDED
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviPower Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviPower Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP Driver Errors

     �    The ViSession handle that you obtain from the ke230x_init or ke230x_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   �#����  �    Status                           #�-   �  �    Instrument Handle                  	            ����         
�  "x     K.    init                            ����         #�  F     K.    InitWithOptions                 ����         G�  Qx     K.    ConfigureOutputEnabled          ����         R�  `Y     K.    ConfigureOutputRange            ����         a�  mt     K.    ConfigureCurrentLimit           ����         n�  {1     K.    ConfigureOVP                    ����         |v  ��     K.    ConfigureVoltageLevel           ����         ��  ��     K.    ConfigureVoltageProtection      ����         �E  �c     K.    ConfigureOutputBandwidth        ����         ��  �Q     K.    ConfigureChannelOneImpedance    ����         �  ��     K.    ConfigureOutputRelays           ����         ��  Ɲ     K.    SetAttributeViInt32             ����         �8  �     K.    SetAttributeViReal64            ����         ޠ  �l     K.    SetAttributeViString            ����         � 	�     K.    SetAttributeViBoolean           ����        q  @     K.    SetAttributeViSession           ����        !� 2�     K.    GetAttributeViInt32             ����        3� D     K.    GetAttributeViReal64            ����        E� _T     K.    GetAttributeViString            ����        `� q�     K.    GetAttributeViBoolean           ����        r� �|     K.    GetAttributeViSession           ����        �� �     K.    CheckAttributeViInt32           ����        �� �     K.    CheckAttributeViReal64          ����        �� �     K.    CheckAttributeViString          ����        �� �     K.    CheckAttributeViBoolean         ����        ̹ �!     K.    CheckAttributeViSession         ����        ޼ �     K.    Measure                         ����        �6 ��     K.    ConfigureBasicMeasurement       ����        �  � 
    K.    ConfigurePcurMeasurement        ����        � �     K.    ConfigureAutoTime               ����        � $_     K.    ConfigurePcurDigitizer          ����        &� 6�     K.    ConfigurePcurStep               ����        9� HM     K.    ConfigureLintMeasurement        ����        J� U     K.    ConfigureFastSearchDetect       ����        V� a     K.    GetActualPointCount             ����        b n�     K.    ReadMeasurement                 ����        o� ~�     K.    ReadMultiPointMeasurement       ����        �` ��     K.    FetchMeasurement                ����        �} ��     K.    FetchMultiPointMeasurement      ����        � �b     K.    SendMeasSWTrigger               ����        � �n     K.    MeasurementComplete             ����        �d �f     K.    QueryOutputState                ����        �, �     K.    QueryMaxCurrentLimit            ����        �: ��     K.    QueryMaxVoltageLevel            ����        �, �+     K.    ResetOutputProtection           ����        �� ��     K.    self_test                       ����        �� �~     K.    reset                           ����        �� ��     K.    ResetWithDefaults               ����        �# Z     K.    Disable                         ����        � �     K.    revision_query                  ����        � �     K.    error_query                     ����        � I     K.    error_message                   ����        J R     K.    InvalidateAllAttributes         ����        R� c�     K.    GetChannelName                  ����        d� x^     K.    GetError                        ����        y� ��     K.    ClearError                      ����        �L �H     K.    GetErrorInfo                    ����        �� ��     K.    ClearErrorInfo                  ����        �E �F     K.    GetNextCoercionRecord           ����        �: �C     K.    GetNextInterchangeWarning       ����        �7 �*     K.    ClearInterchangeWarnings        ����        Ҥ �     K.    ResetInterchangeCheck           ����        � �     K.    LockSession                     ����        �� �     K.    UnlockSession                   ����        q       K.    WriteInstrData                  ����        �      K.    ReadInstrData                   ����        < $m     K.    close                                 �                                     DInitialize                           DInitialize With Options             �Configuration Functions             �Output                               DConfigure Output Enabled             DConfigure Output Range               DConfigure Current Limit              DConfigure OVP                        DConfigure Voltage Level             �Additional Output Settings           DConfigure Voltage Protection         DConfigure Output Bandwidth           DConfigure Channel One Impedance      DConfigure Output Relays             sSet/Get/Check Attribute             �Set Attribute                        DSet Attribute ViInt32                DSet Attribute ViReal64               DSet Attribute ViString               DSet Attribute ViBoolean              DSet Attribute ViSession             XGet Attribute                        DGet Attribute ViInt32                DGet Attribute ViReal64               DGet Attribute ViString               DGet Attribute ViBoolean              DGet Attribute ViSession             �Check Attribute                      DCheck Attribute ViInt32              DCheck Attribute ViReal64             DCheck Attribute ViString             DCheck Attribute ViBoolean            DCheck Attribute ViSession           |Measure Output                       DMeasure                              DConfigure Basic Measurement          DConfigure PCUR Measurement           DConfigure Auto Time                  DConfigure PCUR Digitizer             DConfigure PCUR Step Measurement      DConfigure LINT Measurement           DConfigure Fast Search Detect         DGet Actual Point Count               DRead Measurement (single)            DRead Measurement (multiple)          DFetch Measurement (single)           DFetch Measurement (multiple)         DSend Measurement SW Trigger          DMeasurement Complete                �Action/Status Functions              DQuery Output State                   DQuery Max Current Limit              DQuery Max Voltage Level              DReset Output Protection             5Utility Functions                    DSelf-Test                            DReset                                DReset With Defaults                  DDisable                              DRevision Query                       DError-Query                          DError Message                        DInvalidate All Attributes            DGet Channel Name                    	�Error                                DGet Error                            DClear Error                          DGet Error Info                       DClear Error Info                    	�Coercion Info                        DGet Next Coercion Record          ����Interchangeability Info              DGet Next Interchange Warning         DClear Interchange Warnings           DReset Interchange Check             
=Locking                              DLock Session                         DUnlock Session                      
�Instrument I/O                       DWrite Instrument Data                DRead Instrument Data                 DClose                           