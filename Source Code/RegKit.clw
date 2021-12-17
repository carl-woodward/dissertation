; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDllList
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "regkit.h"
LastPage=0

ClassCount=20
Class1=CDllList
Class2=CDriveList
Class3=CDriveTree
Class4=CFileSnap
Class5=CHtmlLogo
Class6=CHtmlPane
Class7=CListPane
Class8=CMainFrame
Class9=CProgDlg
Class10=CRegDocList
Class11=CRegDocTree
Class12=CRegKitApp
Class13=CAboutDlg
Class14=CRegKitDoc
Class15=CRegKitView
Class16=CRegList
Class17=CRegSnap
Class18=CRegTree

ResourceCount=4
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDD_PROG_DLG
Class19=CSummary
Resource3=IDR_MAINFRAME (English (U.S.))
Class20=CSummaryDlg
Resource4=IDD_SUMMARY

[CLS:CDllList]
Type=0
BaseClass=CListView
HeaderFile=DllList.h
ImplementationFile=DllList.cpp
LastObject=CDllList

[CLS:CDriveList]
Type=0
BaseClass=CListView
HeaderFile=DriveList.h
ImplementationFile=DriveList.cpp

[CLS:CDriveTree]
Type=0
BaseClass=CTreeView
HeaderFile=DriveTree.h
ImplementationFile=DriveTree.cpp

[CLS:CFileSnap]
Type=0
BaseClass=CListView
HeaderFile=FileSnap.h
ImplementationFile=FileSnap.cpp

[CLS:CHtmlLogo]
Type=0
BaseClass=CHtmlView
HeaderFile=HtmlLogo.h
ImplementationFile=HtmlLogo.cpp

[CLS:CHtmlPane]
Type=0
BaseClass=CHtmlView
HeaderFile=HtmlPane.h
ImplementationFile=HtmlPane.cpp

[CLS:CListPane]
Type=0
BaseClass=CListView
HeaderFile=ListPane.h
ImplementationFile=ListPane.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame

[CLS:CProgDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProgDlg.h
ImplementationFile=ProgDlg.cpp

[CLS:CRegDocList]
Type=0
BaseClass=CListView
HeaderFile=RegDocList.h
ImplementationFile=RegDocList.cpp

[CLS:CRegDocTree]
Type=0
BaseClass=CTreeView
HeaderFile=RegDocTree.h
ImplementationFile=RegDocTree.cpp

[CLS:CRegKitApp]
Type=0
BaseClass=CWinApp
HeaderFile=RegKit.h
ImplementationFile=RegKit.cpp
LastObject=CRegKitApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=RegKit.cpp
ImplementationFile=RegKit.cpp
LastObject=CAboutDlg

[CLS:CRegKitDoc]
Type=0
BaseClass=CDocument
HeaderFile=RegKitDoc.h
ImplementationFile=RegKitDoc.cpp
LastObject=CRegKitDoc

[CLS:CRegKitView]
Type=0
BaseClass=CListView
HeaderFile=RegKitView.h
ImplementationFile=RegKitView.cpp
LastObject=CRegKitView

[CLS:CRegList]
Type=0
BaseClass=CListView
HeaderFile=RegList.h
ImplementationFile=RegList.cpp

[CLS:CRegSnap]
Type=0
BaseClass=CListView
HeaderFile=RegSnap.h
ImplementationFile=RegSnap.cpp

[CLS:CRegTree]
Type=0
BaseClass=CTreeView
HeaderFile=RegTree.h
ImplementationFile=RegTree.cpp

[DLG:IDD_PROG_DLG]
Type=1
Class=CProgDlg
ControlCount=0

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_PRINT
Command2=ID_FILE_PRINT_PREVIEW
Command3=ID_FILE_PRINT_SETUP
Command4=ID_APP_EXIT
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_PRINT
Command2=ID_NEXT_PANE
Command3=ID_PREV_PANE
CommandCount=3

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[CLS:CSummary]
Type=0
HeaderFile=Summary.h
ImplementationFile=Summary.cpp
BaseClass=CHtmlView
Filter=C
LastObject=CSummary

[DLG:IDD_SUMMARY]
Type=1
Class=CSummaryDlg
ControlCount=0

[CLS:CSummaryDlg]
Type=0
HeaderFile=SummaryDlg.h
ImplementationFile=SummaryDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSummaryDlg

