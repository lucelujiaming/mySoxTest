; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDrawFlowChartView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DrawFlowChart.h"
LastPage=0

ClassCount=5
Class1=CDrawFlowChartApp
Class2=CDrawFlowChartDoc
Class3=CDrawFlowChartView
Class4=CMainFrame

ResourceCount=2
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX

[CLS:CDrawFlowChartApp]
Type=0
HeaderFile=DrawFlowChart.h
ImplementationFile=DrawFlowChart.cpp
Filter=N

[CLS:CDrawFlowChartDoc]
Type=0
HeaderFile=DrawFlowChartDoc.h
ImplementationFile=DrawFlowChartDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CDrawFlowChartDoc

[CLS:CDrawFlowChartView]
Type=0
HeaderFile=DrawFlowChartView.h
ImplementationFile=DrawFlowChartView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CDrawFlowChartView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_TOOLBAR_STOP




[CLS:CAboutDlg]
Type=0
HeaderFile=DrawFlowChart.cpp
ImplementationFile=DrawFlowChart.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_SAVE_BMP
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_APP_ABOUT
Command18=ID_START
Command19=ID_END
Command20=ID_RECTANGLE
Command21=ID_ELLIPSE
Command22=ID_DIAMOND
Command23=ID_PARALLELOGRAM
Command24=ID_CONTROLFLOW
Command25=ID_GENERICLINE
Command26=ID_ARROWHEAD
Command27=ID_SEARCH_PATH
Command28=ID_MARK_PATH
Command29=ID_STOP_MARK
CommandCount=29

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_TOOLBAR_START
Command9=ID_TOOLBAR_END
Command10=ID_TOOLBAR_RECT
Command11=ID_TOOLBAR_ELLIPSE
Command12=ID_TOOLBAR_JUDGE
Command13=ID_TOOLBAR_PROCESS
Command14=ID_TOOLBAR_FLOWCONTROL
Command15=ID_TOOLBAR_POLYGONAL_LINE
Command16=ID_TOOLBAR_GENERICLINE
Command17=ID_TOOLBAR_ARROWHEAD
Command18=ID_TOOLBAR_SEARCH
Command19=ID_TOOLBAR_NEXT
Command20=ID_TOOLBAR_STOP
Command21=ID_APP_ABOUT
CommandCount=21
