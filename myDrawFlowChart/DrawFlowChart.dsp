# Microsoft Developer Studio Project File - Name="DrawFlowChart" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DrawFlowChart - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DrawFlowChart.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DrawFlowChart.mak" CFG="DrawFlowChart - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DrawFlowChart - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DrawFlowChart - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DrawFlowChart - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "DrawFlowChart - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DrawFlowChart - Win32 Release"
# Name "DrawFlowChart - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AdjustPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\ArcLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrowLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BezierLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BresenhamLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BrokenLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CubicBox.cpp
# End Source File
# Begin Source File

SOURCE=.\CubicSplineLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomBezierLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomRoundRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\CylinderGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DDALine.cpp
# End Source File
# Begin Source File

SOURCE=.\Diamond.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChart.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChart.rc
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChartDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChartView.cpp
# End Source File
# Begin Source File

SOURCE=.\Ellipse.cpp
# End Source File
# Begin Source File

SOURCE=.\End.cpp
# End Source File
# Begin Source File

SOURCE=.\Facet.cpp
# End Source File
# Begin Source File

SOURCE=.\flowchart_cJSON.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GenericLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Hexagon.cpp
# End Source File
# Begin Source File

SOURCE=.\Ladder.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\MiddleCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\OrthogonalWire.cpp
# End Source File
# Begin Source File

SOURCE=.\P2.cpp
# End Source File
# Begin Source File

SOURCE=.\P3.cpp
# End Source File
# Begin Source File

SOURCE=.\Parallelogram.cpp
# End Source File
# Begin Source File

SOURCE=.\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\PathManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PolygonalLine.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadraticSplineLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Rectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundHeadRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Transform3.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdjustPoint.h
# End Source File
# Begin Source File

SOURCE=.\ArcLine.h
# End Source File
# Begin Source File

SOURCE=.\ArrowLine.h
# End Source File
# Begin Source File

SOURCE=.\BezierLine.h
# End Source File
# Begin Source File

SOURCE=.\BresenhamLine.h
# End Source File
# Begin Source File

SOURCE=.\BrokenLine.h
# End Source File
# Begin Source File

SOURCE=.\CubicBox.h
# End Source File
# Begin Source File

SOURCE=.\CubicSplineLine.h
# End Source File
# Begin Source File

SOURCE=.\CustomBezierLine.h
# End Source File
# Begin Source File

SOURCE=.\CustomRoundRectangle.h
# End Source File
# Begin Source File

SOURCE=.\CylinderGraph.h
# End Source File
# Begin Source File

SOURCE=.\DDALine.h
# End Source File
# Begin Source File

SOURCE=.\Diamond.h
# End Source File
# Begin Source File

SOURCE=.\DocumentGraph.h
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChart.h
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChartDoc.h
# End Source File
# Begin Source File

SOURCE=.\DrawFlowChartView.h
# End Source File
# Begin Source File

SOURCE=.\Ellipse.h
# End Source File
# Begin Source File

SOURCE=.\End.h
# End Source File
# Begin Source File

SOURCE=.\Facet.h
# End Source File
# Begin Source File

SOURCE=.\flowchart_cJSON.h
# End Source File
# Begin Source File

SOURCE=.\GenericLine.h
# End Source File
# Begin Source File

SOURCE=.\Graph.h
# End Source File
# Begin Source File

SOURCE=.\GraphFactory.h
# End Source File
# Begin Source File

SOURCE=.\GraphManager.h
# End Source File
# Begin Source File

SOURCE=.\Hexagon.h
# End Source File
# Begin Source File

SOURCE=.\Ladder.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MeshSphere.h
# End Source File
# Begin Source File

SOURCE=.\MiddleCircle.h
# End Source File
# Begin Source File

SOURCE=.\OrthogonalWire.h
# End Source File
# Begin Source File

SOURCE=.\P2.h
# End Source File
# Begin Source File

SOURCE=.\P3.h
# End Source File
# Begin Source File

SOURCE=.\Parallelogram.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\PathManager.h
# End Source File
# Begin Source File

SOURCE=.\PolygonalLine.h
# End Source File
# Begin Source File

SOURCE=.\QuadraticSplineLine.h
# End Source File
# Begin Source File

SOURCE=.\Rectangle.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoundHeadRectangle.h
# End Source File
# Begin Source File

SOURCE=.\RoundRectangle.h
# End Source File
# Begin Source File

SOURCE=.\Start.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Transform3.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DrawFlowChart.ico
# End Source File
# Begin Source File

SOURCE=.\res\DrawFlowChart.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DrawFlowChartDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\graphics.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
