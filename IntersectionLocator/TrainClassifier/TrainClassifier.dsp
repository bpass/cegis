# Microsoft Developer Studio Project File - Name="TrainClassifier" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=TrainClassifier - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TrainClassifier.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrainClassifier.mak" CFG="TrainClassifier - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrainClassifier - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "TrainClassifier - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrainClassifier - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "TrainClassifier - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../intersectionlocator" /I "D:\rstelzleni\DataIntegration\FeatureLibrary\GDAL\gdal120\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TrainClassifier - Win32 Release"
# Name "TrainClassifier - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Classifier.cpp
# End Source File
# Begin Source File

SOURCE=..\InMemRaster.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE="..\..\..\DataIntegration\FeatureLibrary\GDAL\gdal-1.2.0b\gdal_i.lib"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Classifier.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_config.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_conv.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_csv.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_error.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_list.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_minixml.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_multiproc.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_odbc.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_port.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_string.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\cpl_vsi.h
# End Source File
# Begin Source File

SOURCE=..\IntersectionLocator\Error.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\gdal.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\gdal_frmts.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\gdal_priv.h
# End Source File
# Begin Source File

SOURCE=..\InMemRaster.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_api.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_attrind.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_core.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_feature.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_featurestyle.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_geometry.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_p.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_spatialref.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogr_srs_api.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\ogrsf_frmts.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\oledb_sup.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\oledbgis.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfcdatasource.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfcenumerator.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfclsid.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfcschemarowsets.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfctable.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\sfiiddef.h
# End Source File
# Begin Source File

SOURCE=..\..\DataIntegration\FeatureLibrary\GDAL\gdal120\include\swq.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
