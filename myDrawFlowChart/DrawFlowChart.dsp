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

SOURCE=.\ActiveEdgeTable.cpp
# End Source File
# Begin Source File

SOURCE=.\AdjustPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ArbitraryRotateCube.cpp
# End Source File
# Begin Source File

SOURCE=.\ArcLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ArrowLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BackfaceCullCube.cpp
# End Source File
# Begin Source File

SOURCE=.\BackfaceCullRotateCube.cpp
# End Source File
# Begin Source File

SOURCE=.\BezierLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierPatchGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\BresenhamLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BrokenLine.cpp
# End Source File
# Begin Source File

SOURCE=.\BucketTable.cpp
# End Source File
# Begin Source File

SOURCE=.\BucketTableFill.cpp
# End Source File
# Begin Source File

SOURCE=.\BumpTextureBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\BumpTextureSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\BumpTextureSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\BumpTextureZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CabinetProjectionCube.cpp
# End Source File
# Begin Source File

SOURCE=.\CabinetRotateCube.cpp
# End Source File
# Begin Source File

SOURCE=.\CohenSutherlandClipLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorCube.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorFace.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorP2.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorP3.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPoint.cpp

!IF  "$(CFG)" == "DrawFlowChart - Win32 Release"

!ELSEIF  "$(CFG)" == "DrawFlowChart - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ColorPoint2.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorPoint3.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStack.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorTransform3.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorVector3.cpp
# End Source File
# Begin Source File

SOURCE=.\CubicBezierCurve.cpp
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

SOURCE=.\DepthFace.cpp
# End Source File
# Begin Source File

SOURCE=.\DepthLinearInterpZBuffer.cpp
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

SOURCE=.\EdgeFillPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\EdgeTableFillPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\Ellipse.cpp
# End Source File
# Begin Source File

SOURCE=.\End.cpp
# End Source File
# Begin Source File

SOURCE=.\Face.cpp
# End Source File
# Begin Source File

SOURCE=.\Facet.cpp
# End Source File
# Begin Source File

SOURCE=.\FenceFillPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatColorTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatFillTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\flowchart_cJSON.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\GenericLine.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryCube.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudBicubicBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudBicubicBezierSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudBiquatricBezierSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudZBufferBicubicBezierSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\GouraudZBufferBiquatricBezierSphere.cpp
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

SOURCE=.\LiangBarskyClipLine.cpp
# End Source File
# Begin Source File

SOURCE=.\LightingScene.cpp
# End Source File
# Begin Source File

SOURCE=.\LightSource.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\MiddleCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\MidPointDividLine.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalRotation.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjFileCube.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjFileCubeGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjFileTeapot.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjFileTeapotGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\OrthogonalProjectionCube.cpp
# End Source File
# Begin Source File

SOURCE=.\OrthogonalRotateCube.cpp
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

SOURCE=.\PerspectiveProjectionCube.cpp
# End Source File
# Begin Source File

SOURCE=.\PerspectiveRotateCube.cpp
# End Source File
# Begin Source File

SOURCE=.\PhongBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\PhongBiquatricBezierSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\PhongZBufferBiquatricBezierSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\PolygonalLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Projection.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadraticSplineLine.cpp
# End Source File
# Begin Source File

SOURCE=.\RationalBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierCircle.cpp
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\Rectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\RGB.cpp
# End Source File
# Begin Source File

SOURCE=.\RotatePentagram.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundHeadRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\RoundRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanLineFillPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\SmoothColorTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\SmoothFillTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\SpatialArrangedCube.cpp
# End Source File
# Begin Source File

SOURCE=.\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureBiquatricBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureCoordinate.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureCube.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureCubeGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureFace.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureSphereGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreeCrossRotateCube.cpp
# End Source File
# Begin Source File

SOURCE=.\Transform2.cpp
# End Source File
# Begin Source File

SOURCE=.\Transform3.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3ZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\WuAntiLine.cpp
# End Source File
# Begin Source File

SOURCE=.\ZBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ZBufferCube.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActiveEdgeTable.h
# End Source File
# Begin Source File

SOURCE=.\AdjustPoint.h
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTexture.h
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureSphere.h
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\AntiAliasedBumpTextureZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ArbitraryRotateCube.h
# End Source File
# Begin Source File

SOURCE=.\ArcLine.h
# End Source File
# Begin Source File

SOURCE=.\ArrowLine.h
# End Source File
# Begin Source File

SOURCE=.\BackfaceCullCube.h
# End Source File
# Begin Source File

SOURCE=.\BackfaceCullRotateCube.h
# End Source File
# Begin Source File

SOURCE=.\BezierLine.h
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierPatchGraph.h
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierSphere.h
# End Source File
# Begin Source File

SOURCE=.\BicubicBezierSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\BresenhamLine.h
# End Source File
# Begin Source File

SOURCE=.\BrokenLine.h
# End Source File
# Begin Source File

SOURCE=.\BucketTable.h
# End Source File
# Begin Source File

SOURCE=.\BucketTableFill.h
# End Source File
# Begin Source File

SOURCE=.\BumpTextureBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\BumpTextureSphere.h
# End Source File
# Begin Source File

SOURCE=.\BumpTextureSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\BumpTextureZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CabinetProjectionCube.h
# End Source File
# Begin Source File

SOURCE=.\CabinetRotateCube.h
# End Source File
# Begin Source File

SOURCE=.\CohenSutherlandClipLine.h
# End Source File
# Begin Source File

SOURCE=.\ColorCube.h
# End Source File
# Begin Source File

SOURCE=.\ColorFace.h
# End Source File
# Begin Source File

SOURCE=.\ColorP2.h
# End Source File
# Begin Source File

SOURCE=.\ColorP3.h
# End Source File
# Begin Source File

SOURCE=.\ColorPoint.h
# End Source File
# Begin Source File

SOURCE=.\ColorPoint2.h
# End Source File
# Begin Source File

SOURCE=.\ColorPoint3.h
# End Source File
# Begin Source File

SOURCE=.\ColorStack.h
# End Source File
# Begin Source File

SOURCE=.\ColorTransform3.h
# End Source File
# Begin Source File

SOURCE=.\ColorVector3.h
# End Source File
# Begin Source File

SOURCE=.\CubicBezierCurve.h
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

SOURCE=.\DepthFace.h
# End Source File
# Begin Source File

SOURCE=.\DepthLinearInterpZBuffer.h
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

SOURCE=.\EdgeFillPolygon.h
# End Source File
# Begin Source File

SOURCE=.\EdgeTableFillPolygon.h
# End Source File
# Begin Source File

SOURCE=.\Ellipse.h
# End Source File
# Begin Source File

SOURCE=.\End.h
# End Source File
# Begin Source File

SOURCE=.\Face.h
# End Source File
# Begin Source File

SOURCE=.\Facet.h
# End Source File
# Begin Source File

SOURCE=.\FenceFillPolygon.h
# End Source File
# Begin Source File

SOURCE=.\FlatColorTriangle.h
# End Source File
# Begin Source File

SOURCE=.\FlatFillTriangle.h
# End Source File
# Begin Source File

SOURCE=.\flowchart_cJSON.h
# End Source File
# Begin Source File

SOURCE=.\GenericLine.h
# End Source File
# Begin Source File

SOURCE=.\GeometryCube.h
# End Source File
# Begin Source File

SOURCE=.\GouraudBicubicBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\GouraudBicubicBezierSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\GouraudBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\GouraudBiquatricBezierSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\GouraudZBufferBicubicBezierSphere.h
# End Source File
# Begin Source File

SOURCE=.\GouraudZBufferBiquatricBezierSphere.h
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

SOURCE=.\LiangBarskyClipLine.h
# End Source File
# Begin Source File

SOURCE=.\LightingScene.h
# End Source File
# Begin Source File

SOURCE=.\LightSource.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Material.h
# End Source File
# Begin Source File

SOURCE=.\MeshSphere.h
# End Source File
# Begin Source File

SOURCE=.\MiddleCircle.h
# End Source File
# Begin Source File

SOURCE=.\MidPointDividLine.h
# End Source File
# Begin Source File

SOURCE=.\NormalRotation.h
# End Source File
# Begin Source File

SOURCE=.\ObjFileCube.h
# End Source File
# Begin Source File

SOURCE=.\ObjFileCubeGraph.h
# End Source File
# Begin Source File

SOURCE=.\ObjFileTeapot.h
# End Source File
# Begin Source File

SOURCE=.\ObjFileTeapotGraph.h
# End Source File
# Begin Source File

SOURCE=.\OrthogonalProjectionCube.h
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

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=.\Path.h
# End Source File
# Begin Source File

SOURCE=.\PathManager.h
# End Source File
# Begin Source File

SOURCE=.\PerspectiveProjectionCube.h
# End Source File
# Begin Source File

SOURCE=.\PerspectiveRotateCube.h
# End Source File
# Begin Source File

SOURCE=.\PhongBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\PhongBiquatricBezierSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\PhongZBufferBiquatricBezierSphere.h
# End Source File
# Begin Source File

SOURCE=.\PolygonalLine.h
# End Source File
# Begin Source File

SOURCE=.\Projection.h
# End Source File
# Begin Source File

SOURCE=.\QuadraticSplineLine.h
# End Source File
# Begin Source File

SOURCE=.\RationalBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierCircle.h
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierCurve.h
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierSphere.h
# End Source File
# Begin Source File

SOURCE=.\RationalQuadraticBezierSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\Rectangle.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RGB.h
# End Source File
# Begin Source File

SOURCE=.\RotatePentagram.h
# End Source File
# Begin Source File

SOURCE=.\RoundHeadRectangle.h
# End Source File
# Begin Source File

SOURCE=.\RoundRectangle.h
# End Source File
# Begin Source File

SOURCE=.\ScanLineFillPolygon.h
# End Source File
# Begin Source File

SOURCE=.\SmoothColorTriangle.h
# End Source File
# Begin Source File

SOURCE=.\SmoothFillTriangle.h
# End Source File
# Begin Source File

SOURCE=.\SpatialArrangedCube.h
# End Source File
# Begin Source File

SOURCE=.\Start.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Texture.h
# End Source File
# Begin Source File

SOURCE=.\TextureBiquatricBezierPatch.h
# End Source File
# Begin Source File

SOURCE=.\TextureCoordinate.h
# End Source File
# Begin Source File

SOURCE=.\TextureCube.h
# End Source File
# Begin Source File

SOURCE=.\TextureCubeGraph.h
# End Source File
# Begin Source File

SOURCE=.\TextureFace.h
# End Source File
# Begin Source File

SOURCE=.\TextureSphere.h
# End Source File
# Begin Source File

SOURCE=.\TextureSphereGraph.h
# End Source File
# Begin Source File

SOURCE=.\TextureZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ThreeCrossRotateCube.h
# End Source File
# Begin Source File

SOURCE=.\Transform2.h
# End Source File
# Begin Source File

SOURCE=.\Transform3.h
# End Source File
# Begin Source File

SOURCE=.\Triangle.h
# End Source File
# Begin Source File

SOURCE=.\Vector3.h
# End Source File
# Begin Source File

SOURCE=.\Vector3ZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\WuAntiLine.h
# End Source File
# Begin Source File

SOURCE=.\ZBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ZBufferCube.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bear.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bumpTexture.bmp
# End Source File
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

SOURCE=.\res\landscape.bmp
# End Source File
# Begin Source File

SOURCE=.\res\texture.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\world.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
