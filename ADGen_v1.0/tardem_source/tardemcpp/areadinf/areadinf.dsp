# Microsoft Developer Studio Project File - Name="areadinf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=areadinf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "areadinf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "areadinf.mak" CFG="areadinf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "areadinf - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "areadinf - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "areadinf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD F90 /compile_only /include:"Release/" /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../shape code" /I "../../TARDEMOCX Source" /I "../../tardemcpp" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USEESRILIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "areadinf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /include:"Debug/" /nologo /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../shape code" /I "../../TARDEMOCX Source" /I "../../tardemcpp" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "USEESRILIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "areadinf - Win32 Release"
# Name "areadinf - Win32 Debug"
# Begin Group "tardem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\avcalls_null.cpp
# End Source File
# Begin Source File

SOURCE=..\feedback_print.cpp
# End Source File
# Begin Source File

SOURCE=..\gridio.cpp
# End Source File
# Begin Source File

SOURCE=..\nrutil.cpp
# End Source File
# Begin Source File

SOURCE=..\tardemlib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Esri\Av_gis30\Arcview\Gridio\avgridio.lib
# End Source File
# End Group
# Begin Group "Shape"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\Shape Code\cell.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\dbf.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\exception.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\field.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\point.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shape.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shapefile.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shapemain.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shp_point.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shp_polygon.cpp"
# End Source File
# Begin Source File

SOURCE="..\..\Shape Code\shp_polyline.cpp"
# End Source File
# End Group
# Begin Source File

SOURCE=..\areadinf.cpp
# End Source File
# End Target
# End Project
