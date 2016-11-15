README.TXT
     AGE1960 Agilent Technologies GSM Mobile Test Universal Instrument Driver (UID) for:
     	Agilent Technologies 8960 Series 10 E5515 with 
	Agilent Technologies E1960A GSM Mobile Test TA (Rev. A.09.xx or higher) 
     	Agilent Technologies E1985A GSM_AMPS/136_GPRS Mobile Test TA (Rev. A.03.xx or higher)

     UID Revision A.05.00
     Revision Date: 4/2002

UID Revision History
- ---------------------------------------------------------------------------
  A.00.00  04/99  Initial Beta Release.
  A.00.01  05/99  Beta Release added functions:
                  	hpe1960_SACCHReport_Q
                  	hpe1960_waitForClearHPIB
                  	hpe1960_useSRQMAV
                  	hpe1960_useSRQMAV_Q
                  fixed other minor bugs
                  Deleted function hpe1960_serviceRequest
  A.01.00  07/99  Original Release added the following after A.00.01.
	          	hpe1960_PowerVsTimePlot_Q
                  	hpe1960_loopback 
                  	hpe1960_readIQTuning_Q
		  Changed the hpe1960_confBER and the hpe1960_confFastBER to include
                  control of the CLSDelay and LSControl commands
  A.02.00  04/00  Updated the driver to operate with the A.03.04 and A.04.01 revisions
	          of the E1960 test set. This includes:
		  All hpe1960xx function names changed to age1960xx. If updating from 
		  A.01.00 of the driver perform a search/replace for hpe1960 with age1960.
		  New measurement Dynamic Power. Measures power on a burst by burst basis.
		  Fully functioning IQ Tuning measurement with an optional user specified
		  spur frequency. IQ Tuning now has configure and fetch functions.
		  EFS function added (Channel Mode) now supports the Enhanced Full rate
		  speech mobile phones.
		  Function added to set FACCH Signaling.
		  BER results changed to read all types. Fetch and read functions now return 
		  10 parameters.
		  Measurement Timeout on all measurements for revision A.04.01 can have
		  a minimum of 0.1 seconds and increments of 0.1 seconds. A.03.04 revision
		  settings remain at 1-second minimum and increments of 1 second.
		  New Test Application switching function added (age1960_currentTA). This is
		  only functional on revision A.04.01 or higher. However, you can change from
		  A.04.01 to A.03.04, but not the vice versa.
    		  The Status subsystem has changed to allow multiple test applications on
		  the same instrument. Some switches have been added so that interrogating 
		  and setting them, an additional header is required. See the functions
		  age1960_quesRegisterEnable and age1960_operRegisterEnable or the instrument
		  manuals for details.
  A.03.00  05/01  This revision will work with E1960A GSM Mobile Test TA Revision A.07.XX. 
                  Due to the implementation of the new RF In/Out Amplitude Offset table we do
                  not recommend the use of this driver with earlier versions of the E1960A TA. 
                  If you do use this driver with earlier versions of the TA, some test code
                  modification may be required.
                 
                  Added E1985A Rev. A.01.xx support. (Fast TA Switching)
		  Added support for E1960A GSM Mobile Test TA Revision A.07.06
                  Added support for dynamic power measurements of up to 999 points.		  
                  Added the following functions:
		  	age1960_getApproxOffset_Q - query frequency correction data. 
		  	age1960_getPresentFreq_Q - query instrument frequency settings.
		  	age1960_currentTA_Q - query current TA application name.
		  	age1960_currentTAModel_Q - query model number of current TA.
		  	age1960_currentActiveTAP - set Fast Switching TA application. (E1985A)
		  	age1960_currentActiveTAP_Q - query current Fast Switching TA. (E1985A)
		  	age1960_confAFAN - configure audio analyzer.
		  	age1960_readAFAN_Q - read audio analyzer measurements.
		  	age1960_fetchAFAN_Q - fetch audio analyzer measurements.
                  Removed the following variable and function:
		  	PathGain global variable - no longer used in age1960_cellPower or
				age1960_confAutoRange functions due to expanded system
				frequency correction table.
                  Modified the following function:
			age1960_powerVsTimePlot_Q & prepareForPvtPlot
		  	age1960_amplitudeOffset - added and modified parameter usage
			age1960_confAutoRange - added measFcnToUse parameter to allow for 
				autoranging of TX power or audio measurements.
		  The following functions are known to have backward compatibility problems
                  with TA versions < A.07.xx due to SCPI syntax changes related to the
                  amplitude offset table implementation:
			age1960_confAutoRange
			age1960_getApproxOffset_Q
			age1960_powerVsTimePlot_Q & prepareForPvtPlot
			age1960_amplitudeOffset
 	          Addressed all known bug reports.
  A.04.00  07/01  This revision will work with E1960A GSM Mobile Test TA Revision A.08.08
                  or with the GSM format of E1985A GSM/IS136_AMPS/GPRS Mobile Test A.02.06. 
                  Because of the addition of GSM850 support, it is not recommended for use
                  with earlier TA revisions.

                  Added support for GSM850 band in the following functions:
			age1960_BATable
			age1960_BCHARFCN
			age1960_cellBand
			age1960_expectedPower
			age1960_manualBand
			age1960_manualChan
			age1960_MSTXLevel
			age1960_TCHARFCN
			age1960_TCHBand
			age1960_getCorrectionFreq_Q
			age1960_getPresentFreq_Q
                  Added support for Confidence Interval for Fast Termination (CIFT) for
                  the PFER measurement.
			age1960_confPFER_CIFT function added.
                  Fixed all known defects.
 A.05.00  04/02   This revision will work with E1960A GSM Mobile Test TA Revision A.09.xx
                  or with the GSM format of E1985A GSM_AMPS/136_GPRS Mobile Test A.03.xx. 
                  Added format and suffix constants for user convenience:
			age1960_GPRS_FORMAT			"GPRS"		
			age1960_GSM_FORMAT			"GSM"			
			age1960_AMPS_IS136_FORMAT		"AMPS\IS136"	
			age1960_WCDMA_FORMAT			"WCDMA"       	

			age1960_GPRS_TA_SUFFIX			":GPRS"     	
			age1960_GSM_TA_SUFFIX			":GSM"    	
			age1960_IS136_TA_SUFFIX			":DIG136"   	
			age1960_AMPS_IS136_TA_SUFFIX		":TA136"   	
			age1960_WCDMA_TA_SUFFIX			":WCDMA"    
			age1960_C2K_TA_SUFFIX			":C2K"	
		  Added missing constant:
			age1960_CONN_STATE_UNKNOWN
		  Changed value for :
			age1960_TA_SWITCH_MAX_TIME from 120 to 240 seconds.
		  Added function for Power Vs Time user selectable mask (PCS Band):
			age1960_confPVTMask
			Added new constants:
				age1960_PVT_MASK_NARROW
				age1960_PVT_MASK_RELAXED
		  Added functions to setup ORFS mask limits:
			age1960_confORFSLimitSource
			age1960_confORFSSwitchLimits
			age1960_confORFSModLimits
   			Added new constants:
				age1960_ORFS_LIMIT_ETSI
				age1960_ORFS_LIMIT_MAN
				age1960_ORFSSW_LIM_POINTS_MIN
				age1960_ORFSSW_LIM_POINTS_MAX	
				age1960_ORFSMOD_LIM_POINTS_MIN
				age1960_ORFSMOD_LIM_POINTS_MAX
				age1960_ORFSSW_LIMIT_MIN	
				age1960_ORFSSW_LIMIT_MAX
				age1960_ORFSMOD_LIMIT_MIN	
				age1960_ORFSMOD_LIMIT_MAX
		  Added function to read ORFS mask pass/fail data:
			age1960_fetchORFSSummaryPassFail_Q
		  Modified the following routines for correct interrupt usage:
			srq_cleanup
			age1960_waitForInterrupt
			age1960_confForInterrupt
		  Modified the following routine to correct timing problem with new firmware:
			age1960_Reset
		  Modified the .h and .bas file for the following value change (firmware change):
			age1960_PVT_OFFSET_MAX = 590 us from 593 us.
		  Added the following function:
			age1960_burstTimingError_Q
		  Added use of GSM suffix to the following functions:
			age1960_cellPower
			age1960_cellBand
		  Modified the following: Increased number of integrity messages from 15 to 23.
			age1960_integrityMessage
			age1960_INTEGRITY_MAX constant changed from 15 to 23. 
		  The following global variables have been removed. Functions now query the
		  instrument to get the information that they represented:
			cell1Band, cell2band, TCHBand, manualBand, RFUUTLevel1, RFUUTLevel2
                  Fixed all known defects.
- ---------------------------------------------------------------------------

Contents:

1    Introduction

2    VTL/VISA Specific Information
2.1    Instrument Addresses
2.2    Language Specific Features

3    Using the Agilent E1960A Driver in Application Development Environments
3.1    Microsoft Visual C/C++
3.2    Microsoft Visual Basic
3.3    LabWindows/CVI

4.0  Known Defects

5.0  Online Information

- ---------------------------------------------------------------------------

1.   Introduction

The help file included with the Agilent E1960A driver contains instrument and
programming information. This readme file contains additional information
concerning the driver.

The Agilent E1960A Windows 95 and Windows NT drivers are actually a single 32-bit
driver. All of directory paths that follow assume Windows 95. For Windows
NT, change references from win95 to winnt. 

This Agilent E1960A driver conforms to all portions of the VXIplug&play driver
Standard that are applicable to conventional GP-IB and RS-232. Two
elements of the standard do not apply to the Agilent E1960A, since it is not a
VXI instrument. First, there is no soft front panel, as the Agilent E1960A
can be controlled from its hardware front panel. Second, there is no
knowledge base file, which is primarily a description of a VXI board.

This driver features:

1.  Conformance with the VXIplug&play standard. The only exceptions are that
    it does not have a soft front panel or a knowledge base file.

2.  It is built on top of, and uses the services provided by, VISA. VISA
    supports GP-IB and VXI protocols. The driver can be used with any
    GP-IB card that the manufacturer has provided a VISA DLL for.

3.  It includes a "Function Panel" (.fp) file, which allows it to be used
    with visual programming environments such as Agilent VEE, LabWindows, and
    LabVIEW.

4.  It includes a comprehensive on-line help file which complements the
    instrument manual. The help file presents application programming
    examples, a cross-reference between instrument commands and driver
    functions, and detailed documentation of each function, with examples.

5.  Source is included so that the driver can be modified if desired. The
    source conforms to VXI Plug&Play standards. Modifications should only
    be done by people who are familiar with the VXIplug&play standard.

6.  It includes a Visual Basic include file (.bas) that contains the function
    calls in Visual Basic syntax, so that driver functions can be called from
    Visual Basic. If you use Visual Basic with this driver, you should be
    familiar with C/C++ function declarations. In particular, care must be
    taken when working with C/C++ pointers.
 
- ---------------------------------------------------------------------------

2.   VISA Specific Information

The following topics refer to instances when the driver is used with
different versions of VISA.

2.1    Instrument Addresses

When using Agilent VXIplug&play instrument drivers, instrument addresses must
be all uppercase letters. Implementation of the addressing scheme is
vendor specific and some vendors support mixed cases. However, for
maximum portability, the instrument address should use uppercase
characters only.

For example:  use "GPIB0::14" instead of "gpib0::14"

2.2   Language Specific Features

Language Specific features are not supported with this driver.


- --------------------------------------------------------------------------

3.   Application Development Environments

     For general instructions on using the driver with Agilent VEE,
     Microsoft Visual C++, Borland C++, Microsoft Visual Basic,
     LabVIEW, and LabWindows/CVI, see the online help file.

     Additional notes:

3.1  Microsoft Visual C++ 4.0 (& higher) and Borland C++ 4.5 (& higher)

     A.) The driver uses Pascal calling conventions.
     B.) Do not build the driver in the directory in which it was installed.

3.2  Microsoft Visual Basic 4.0 (& higher)

     Refer to the Microsoft Visual BASIC manual for additional
     information on calling DLLs.

3.3  LabWindows CVI/(R) 4.0 (& higher)

     The Agilent E1960A driver is supplied as both a source code file and as a
     dynamic link library file (dll). There are several advantages to
     using the dll form of the driver. These include: 

     1. Transportability across different computer platforms
     2. A higher level of support from Agilent Technologies for the compiled
        driver
     3. Faster load time for your project

LabWindows/CVI (R) by default will attempt to load the source version
of the instrument driver. To load the dll you must include the file
age1960.fp in your project. This file can be found in vxipnp\win95\age1960
Do not include age1960.c in your project. You must also provide an
include path for age1960.h. This is done by adding vxipnp\win95\include
to the include paths (CVI Project Option menu).

- ---------------------------------------------------------------------------

4.0  Known Defects

There are no known defects in the Agilent E1960A driver.

- ---------------------------------------------------------------------------

5.0  Online Information

The latest version of this driver and other Agilent Technology drivers
are available on the Internet at:

http://www.agilent.com/find/inst_drivers

If you do not have Internet access, the drivers can be obtained by contacting
your country support organization. In the U.S., the phone number is 800-452-4844.  
A list of contact information for other countries is available on the 
Agilent Technologies website at
 
http://www.agilent.com/find/assist

----------------------------------------------------------------------------


LabWindows/CVI is a registered trademark of National Instruments.

