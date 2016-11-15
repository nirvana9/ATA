README.TXT
     AG 663X2 Series Dynamic Measurement DC Power Supply
     Universal Instrument Driver (UID)
     Driver Revision B.00.00
     May 1, 2000

Revision History
- ---------------------------------------------------------------------------
  A.00.00  10/27/97  Initial release.
  A.00.01  12/15/97  Update release.
  A.00.02  05/12/98  Added support for the 66311A.
  A.00.03  09/16/98  HP 66311A ID query bug fixed.
                     Fast machine measurement bug fixed.
  A.01.00  01/29/99  Added support for the 66309 and 66111 series.
  B.00.00  05/01/00  Added support for the 66319 and 66321 series.
                     Rebranded for Agilent Technologies.

- ---------------------------------------------------------------------------

Contents:

1    Introduction

2    VTL/VISA Specific Information
2.1    Instrument Addresses
2.2    Language Specific Features

3    Using the AG 663X2 UID in Application Development Environments
3.1    Microsoft Visual C/C++
3.2    Microsoft Visual Basic
3.3    LabWindows/CVI

4.0  Known Defects
4.1    Timeouts with NI GPIB ver 1.2 on Windows NT

5.0  Online Information

- ---------------------------------------------------------------------------

1.   Introduction

The help file included with the AG 663X2 Universal Instrument Driver contains
instrument and programming information. This readme file contains additional
information concerning the driver.

The AG 663X2 Windows 95 and Windows NT drivers are actually a single 32-bit
driver.  All of directory paths which follow assume Windows 95.  For Windows
NT, change references from win95 to winnt. 

This AG 663X2 driver conforms to all portions of the VXIplug&play driver
standard which are applicable to conventional GPIB and RS-232.  Two
elements of the standard do not apply to the AG 663X2 since it is not a
VXI instrument.  First, there is no soft front panel, as the AG 663X2
can be controlled from it's hardware front panel.  Second, there is no
knowledge base file, which is primarily a description of a VXI board.

This driver features:

1.  Conformance with the VXIplug&play standard.  The only exceptions are that
    it does not have a soft front panel or a knowledge base file.

2.  It is built on top of, and uses the services provided by VISA.  VISA
    supports GPIB and VXI protocols.  The driver can be used with any
    GPIB card for which the manufacturer has provided a VISA DLL.

3.  It includes a "Function Panel" (.fp) file which allows it to be used
    with visual programming environments such as Agilent VEE, LabWindows, and
    LabVIEW.

4.  It includes a comprehensive on-line help file which complements the
    instrument  manual.  The help file presents application programming
    examples, a cross-reference between instrument commands and driver
    functions, and detailed documentation of each function, with examples.

5.  Source is included so that the driver can be modified if desired.  The
    source conforms to VXI Plug & Play standards.  Modifications should only
    be done by people who are familiar with the VXIplug&play standard.

6.  It includes a Visual Basic include file (.bas) which contains the function
    calls in Visual Basic syntax, so that driver functions can be called from
    Visual Basic.  If you use Visual Basic with this driver, you should be
    familiar with C/C++ function declarations.  In particular, care must be
    taken when working with C/C++ pointers.
 
- ---------------------------------------------------------------------------

2.   VISA Specific Information

The following topics refer to instances when the driver is used with
different versions of VISA.

2.1    Instrument Addresses

When using Agilent Technologies VXIplug&play instrument drivers, instrument
addresses must be all uppercase letters. Implementation of the addressing
scheme is vendor specific and some vendors support mixed cases. However,
for maximum portability, the instrument address should use uppercase
characters only.

For example:  use "GPIB0::20" instead of "gpib0::20"

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

     The AG 663X2 driver is supplied as both a source code file and as a
     dynamic link library file (dll).  There are several advantages to
     using the dll form of the driver.  These include: 

     1. Transportability across different computer platforms
     2. A higher level of support from Agilent Technologies for the
        compiled driver
     3. Faster load time for your project

Labwindows/CVI (R) by default will attempt to load the source version
of the instrument driver.  To load the dll you must include the file
hp663x2.fp in your project.  This file can be found in vxipnp\win95\hp663x2
Do not include hp663x2.c in your project. You must also provide an
include path for hp663x2.h.  This is done by adding vxipnp\win95\include
to the include paths (CVI Project Option menu).

- ---------------------------------------------------------------------------

4.0  Known Defects

4.1  When using National Instruments GPIB Driver ver 1.2 on Windows NT,
     and a bus timeout occurs, an infinite series of GPIB Timeout error message
     boxes appear. The soultion is to upgrade to version 1.3 of the National
     Instruments GPIB Driver. 

- ---------------------------------------------------------------------------

5.0  Online Information

The latest version of this driver and other AG drivers are available
on the Internet at

http://www.agilent.com/find/inst_drivers.

The AG 663X2 driver is located in a self-extracting archive file called
663x2.exe.

If you do not have internet access, drivers can be obtained by
contacting your country support organization.  In the U.S., the phone
number is 800-452-4844.  A list of contact information for other
countries is available on the Agilent Technologies website at

http://www.agilent.com/find/contacts00.

- ---------------------------------------------------------------------------

LabWindows/CVI is a registered trademark of National Instruments.

