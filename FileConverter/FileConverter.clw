; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFileConverterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "FileConverter.h"

ClassCount=3
Class1=CFileConverterApp
Class2=CFileConverterDlg

ResourceCount=3
Resource2=IDD_FILECONVERTER_DIALOG
Resource1=IDR_MAINFRAME
Class3=COffsetDialog
Resource3=IDD_GetOffset

[CLS:CFileConverterApp]
Type=0
HeaderFile=FileConverter.h
ImplementationFile=FileConverter.cpp
Filter=N

[CLS:CFileConverterDlg]
Type=0
HeaderFile=FileConverterDlg.h
ImplementationFile=FileConverterDlg.cpp
Filter=D
LastObject=IDC_ConvertButton
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_FILECONVERTER_DIALOG]
Type=1
Class=CFileConverterDlg
ControlCount=21
Control1=IDC_ConvertButton,button,1342242817
Control2=IDC_FromFileOne,button,1342242816
Control3=IDC_ToFileOne,button,1342242816
Control4=IDC_FromFileTwo,button,1342242816
Control5=IDC_ToFileTwo,button,1342242816
Control6=IDC_FromFileThree,button,1342242816
Control7=IDC_ToFileThree,button,1342242816
Control8=IDC_ClearButton,button,1342242816
Control9=IDC_ExitButton,button,1342242816
Control10=IDC_EDIT1,edit,1350568064
Control11=IDC_EDIT2,edit,1350568064
Control12=IDC_EDIT3,edit,1350568064
Control13=IDC_EDIT4,edit,1350568064
Control14=IDC_EDIT5,edit,1350568064
Control15=IDC_EDIT6,edit,1350568064
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352

[DLG:IDD_GetOffset]
Type=1
Class=COffsetDialog
ControlCount=4
Control1=IDC_EDIT1,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352

[CLS:COffsetDialog]
Type=0
HeaderFile=OffsetDialog.h
ImplementationFile=OffsetDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDOK

