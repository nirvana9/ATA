# Microsoft Developer Studio Project File - Name="ATA_DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ATA_DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ATA_DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATA_DLL.mak" CFG="ATA_DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATA_DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATA_DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ATA_DLL - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "DebugTrace/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../PowerDLL/Release/PowerDLL.lib eboot.lib SPMETA_DLL.lib FlashToolLib.lib SLA_Challenge.lib "DebugTrace/*.lib" /nologo /subsystem:windows /dll /machine:I386 /def:".\ATA_DLL.def" /out:"../ATATool/Release/ATA_DLL.dll" /implib:"Release/Release.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ATA_DLL - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "DebugTrace/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../PowerDLL/Debug/PowerDLL.lib eboot.lib SPMETA_DLL.lib FlashToolLib.lib SLA_Challenge.lib "DebugTrace/*.lib" /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\ATA_DLL.def" /out:"../ATATool/Debug/ATA_DLL.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ATA_DLL - Win32 Release"
# Name "ATA_DLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "TestItem_Src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_ADC.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Barcode.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Barometer.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_BT.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_BTS.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Charger.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_CMMB.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_EMMC.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_ER_State.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_ExtBuck.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_FingerPrint.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_FM.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_GPS.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_HDMI.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Humidity.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_HWVer.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Interface.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_KPad.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_LCM.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_LED.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_MATV.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Micbias.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_OTG.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_RSSI.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_RTC.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Sensor.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_SIMCard.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_SleepMode.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_SWVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_TCard.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_TP.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_USBTypeC.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_Vibrator.cpp
# End Source File
# Begin Source File

SOURCE=.\TestItem\src\ATA_DLL_TestItem_WIFI.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ATA_DLL.cpp
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL.rc
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Handle.cpp
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Mutex.cpp
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Test_Handle.cpp
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\AUTH.cpp
# End Source File
# Begin Source File

SOURCE=.\common_api.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalOBJ.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\SCERT.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\USB_Com_Func.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\xboot_debug.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "TestItem_Header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_ADC.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Audio.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Barcode.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Barometer.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_BT.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_BTS.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Camera.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Charger.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_CMMB.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_EMMC.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_ER_State.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_ExtBuck.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_FingerPrint.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_FM.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_GPS.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_HDMI.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Humidity.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_HWVer.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Interface.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_KPad.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_LCM.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_LED.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_MATV.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Micbias.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_OTG.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_RSSI.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_RTC.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Sensor.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_SIMCard.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_SleepMode.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_SWVersion.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_TCard.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_TP.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_USBTypeC.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_Vibrator.h
# End Source File
# Begin Source File

SOURCE=.\TestItem\header\ATA_DLL_TestItem_WIFI.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\android_dl_structure.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Handle.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Internal.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Mutex.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Test_Handle.h
# End Source File
# Begin Source File

SOURCE=.\ATA_DLL_Utils.h
# End Source File
# Begin Source File

SOURCE=.\AUTH.h
# End Source File
# Begin Source File

SOURCE=.\common_api.h
# End Source File
# Begin Source File

SOURCE=.\GlobalOBJ.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\interface.h
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\Mdebug.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCERT.h
# End Source File
# Begin Source File

SOURCE=.\sp_brom.h
# End Source File
# Begin Source File

SOURCE=.\spmeta.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\USB_Com_Func.h
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\xboot_debug.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ATA_DLL.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
