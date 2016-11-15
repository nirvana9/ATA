@Rem This batch file puts all Primary Interop Assembly in this directory
@Rem into the Global Assembly Cache using gacutil. It assumes every dll
@Rem is a PIA. It also runs regasm on them if Visual Studio .NET is installed.

@Rem Run this batch file if the .NET Framework or Visual Studio .NET is
@Rem installed after the IVI Shared Components are installed.

@Rem The first batch parameter can specify a directory different from
@Rem the current directory.
set IVIPIA_DIR=%~1
if "%~1"=="" set IVIPIA_DIR="."
set IVIPIA_SPEC="%IVIPIA_DIR%\*.Interop.dll"

for %%f in (%IVIPIA_SPEC%) do gacutil -nologo -i "%%f"

if not defined VSCOMNTOOLS goto end
@Rem Running regasm allows the development environment to properly add a 
@Rem reference to the assembly and to make IntelliSense work.
@Rem 
@Rem The environment variable, VSCOMNTOOLS, is defined by the Visual 
@Rem Studio .NET installation. If it's defined, that directory contains 
@Rem a batch file, vsvars32.bat,which sets the PATH so reagsm can execute.
 call %VSCOMNTOOLS%vsvars32.bat
 for %%f in (%IVIPIA_SPEC%) do regasm -nologo -codebase "%%f"
:end
