# Microsoft Developer Studio Project File - Name="TaskTracker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TaskTracker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TaskTracker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaskTracker.mak" CFG="TaskTracker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaskTracker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TaskTracker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win32/TaskTracker", RAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaskTracker - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W4 /WX /GX /O2 /I "..\Wcl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX"AppHeaders.hpp" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /i "..\Wcl" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Release/TaskTrak.exe"

!ELSEIF  "$(CFG)" == "TaskTracker - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W4 /WX /Gm /GX /ZI /Od /I "..\Wcl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /YX"AppHeaders.hpp" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /i "..\Wcl" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/TaskTrak.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TaskTracker - Win32 Release"
# Name "TaskTracker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddSessionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppCmds.cpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ClockInDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ClockOutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ClpWndReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSessionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileReport.cpp
# End Source File
# Begin Source File

SOURCE=.\FileReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifySessionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintReport.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintReportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PruneSessionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PruneTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Session.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionList.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionListEnum.cpp
# End Source File
# Begin Source File

SOURCE=.\SwitchTasksDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskList.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\TaskTracker.rc
# End Source File
# Begin Source File

SOURCE=.\TextReport.cpp
# End Source File
# Begin Source File

SOURCE=.\ViewReportDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AddSessionDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppCmds.hpp
# End Source File
# Begin Source File

SOURCE=.\AppDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\AppHeaders.hpp
# End Source File
# Begin Source File

SOURCE=.\AppToolBar.hpp
# End Source File
# Begin Source File

SOURCE=.\AppWnd.hpp
# End Source File
# Begin Source File

SOURCE=.\ClockInDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ClockOutDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ClpWndReportDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\EditSessionDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\FileReport.hpp
# End Source File
# Begin Source File

SOURCE=.\FileReportDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\ModifySessionDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\PrintReport.hpp
# End Source File
# Begin Source File

SOURCE=.\PrintReportDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\PruneSessionsDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\PruneTasksDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\Report.hpp
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Session.hpp
# End Source File
# Begin Source File

SOURCE=.\SessionList.hpp
# End Source File
# Begin Source File

SOURCE=.\SessionListBox.hpp
# End Source File
# Begin Source File

SOURCE=.\SessionListEnum.hpp
# End Source File
# Begin Source File

SOURCE=.\SwitchTasksDlg.hpp
# End Source File
# Begin Source File

SOURCE=.\TaskComboBox.hpp
# End Source File
# Begin Source File

SOURCE=.\TaskList.hpp
# End Source File
# Begin Source File

SOURCE=.\TaskListEnum.hpp
# End Source File
# Begin Source File

SOURCE=.\TaskTracker.hpp
# End Source File
# Begin Source File

SOURCE=.\TextReport.hpp
# End Source File
# Begin Source File

SOURCE=.\ViewReportDlg.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\TaskTracker.ico
# End Source File
# Begin Source File

SOURCE=.\TaskTracker.rcv
# End Source File
# Begin Source File

SOURCE=.\ToolBar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\TODO.Txt
# End Source File
# End Target
# End Project
