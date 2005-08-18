# Microsoft Developer Studio Project File - Name="streamtogrid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=streamtogrid - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "streamtogrid.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "streamtogrid.mak" CFG="streamtogrid - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "streamtogrid - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "streamtogrid - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "streamtogrid - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../Shapetogrid" /I "../../tardemcpp" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USEESRILIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /FORCE:MULTIPLE
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "streamtogrid - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Shapetogrid" /I "../../tardemcpp" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USEESRILIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /FORCE:MULTIPLE
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "streamtogrid - Win32 Release"
# Name "streamtogrid - Win32 Debug"
# Begin Group "Shapetogrid"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Shapetogrid\boundbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\dbfadd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\dbfcreate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\dbfdump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\dbfopen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\FileHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\linklist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\myshp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\polygon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\polyline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\PolyLineToRaster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\shpopen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\shputils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Shapetogrid\StreamJoin.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\gridio.cpp
# End Source File
# Begin Source File

SOURCE=..\streamtogrid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Esri\Av_gis30\Arcview\Gridio\avgridio.lib
# End Source File
# End Target
# End Project
