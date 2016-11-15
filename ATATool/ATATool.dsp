# Microsoft Developer Studio Project File - Name="ATATool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ATATool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ATATool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATATool.mak" CFG="ATATool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATATool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ATATool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ATATool - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\ResizableLib\\" /I "..\ResizableLib" /I "..\XListCtrl_demo\XListCtrl" /I "DebugTrace\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 "DebugTrace\*.lib" /nologo /subsystem:windows /machine:I386 /libpath:"..\XListCtrl_demo\bin" /libpath:"..\ATA_DLL\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ATATool - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\ResizableLib" /I "..\XListCtrl_demo\XListCtrl" /I "DebugTrace\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 "DebugTrace\*.lib" /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\XListCtrl_demo\bin" /libpath:"..\ATA_DLL\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ATATool - Win32 Release"
# Name "ATATool - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATATool.cpp
# End Source File
# Begin Source File

SOURCE=.\ATATool.rc
# End Source File
# Begin Source File

SOURCE=.\ATAToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePwd.cpp
# End Source File
# Begin Source File

SOURCE=.\PasswdInput.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageBT.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageBTS.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageEarthphone.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageFM.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageGPS.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageHWVer.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageKPad.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageLED.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMATV.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMicbias.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPagePMCal.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSensor.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSuperClass.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageVibrator.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPageWIFI.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TestItemConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\TestObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\xboot_debug.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ATATool.h
# End Source File
# Begin Source File

SOURCE=.\ATAToolDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChangePwd.h
# End Source File
# Begin Source File

SOURCE=.\GlobleSetting.h
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\Mdebug.h
# End Source File
# Begin Source File

SOURCE=.\PasswdInput.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageAudio.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageBT.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageBTS.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageCamera.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageEarthphone.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageFM.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageGPS.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageHWVer.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageKPad.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageLED.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMATV.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageMicbias.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPagePMCal.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSensor.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageSuperClass.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageVibrator.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageWIFI.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TestItemConfig.h
# End Source File
# Begin Source File

SOURCE=.\TestObject.h
# End Source File
# Begin Source File

SOURCE=.\DebugTrace\xboot_debug.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ATATool.ico
# End Source File
# Begin Source File

SOURCE=.\res\ATATool.rc2
# End Source File
# Begin Source File

SOURCE=.\res\icon_lis.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
