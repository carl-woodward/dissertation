# Microsoft Developer Studio Project File - Name="RegKit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RegKit - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RegKit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RegKit.mak" CFG="RegKit - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RegKit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RegKit - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RegKit - Win32 Release"

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
# ADD CPP /nologo /MD /w /W0 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "RegKit - Win32 Debug"

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
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RegKit - Win32 Release"
# Name "RegKit - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DllList.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveList.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveTree.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSnap.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlLogo.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlPane.cpp
# End Source File
# Begin Source File

SOURCE=.\ListPane.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegDocList.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKit.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKit.rc
# End Source File
# Begin Source File

SOURCE=.\RegKitDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RegKitView.cpp
# End Source File
# Begin Source File

SOURCE=.\RegList.cpp
# End Source File
# Begin Source File

SOURCE=.\RegSnap.cpp
# End Source File
# Begin Source File

SOURCE=.\RegTree.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Summary.cpp
# End Source File
# Begin Source File

SOURCE=.\SummaryDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DllList.h
# End Source File
# Begin Source File

SOURCE=.\DriveList.h
# End Source File
# Begin Source File

SOURCE=.\DriveTree.h
# End Source File
# Begin Source File

SOURCE=.\FileSnap.h
# End Source File
# Begin Source File

SOURCE=.\HtmlLogo.h
# End Source File
# Begin Source File

SOURCE=.\HtmlPane.h
# End Source File
# Begin Source File

SOURCE=.\ListPane.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ProgDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegDocList.h
# End Source File
# Begin Source File

SOURCE=.\RegKit.h
# End Source File
# Begin Source File

SOURCE=.\RegKitDoc.h
# End Source File
# Begin Source File

SOURCE=.\RegKitView.h
# End Source File
# Begin Source File

SOURCE=.\RegList.h
# End Source File
# Begin Source File

SOURCE=.\RegSnap.h
# End Source File
# Begin Source File

SOURCE=.\RegTree.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Summary.h
# End Source File
# Begin Source File

SOURCE=.\SummaryDlg.h
# End Source File
# Begin Source File

SOURCE=.\WarnDis.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\added.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deleted.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dll_doc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dll_file.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drivtree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filesnap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fileview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\home.bmp
# End Source File
# Begin Source File

SOURCE=.\res\l_file.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regdoc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regdword.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regerror.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RegKit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RegKit.ico
# End Source File
# Begin Source File

SOURCE=.\res\RegKit.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RegKitDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\reglist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regsnap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regtree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regview.bmp
# End Source File
# Begin Source File

SOURCE=.\res\s_file.bmp
# End Source File
# Begin Source File

SOURCE=.\res\summary.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RegKit.reg
# End Source File
# End Target
# End Project
