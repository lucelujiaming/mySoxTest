// DrawFlowChartView.cpp : implementation of the CDrawFlowChartView class
//

#include "stdafx.h"
#include "DrawFlowChart.h"

#include "DrawFlowChartDoc.h"
#include "DrawFlowChartView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView

IMPLEMENT_DYNCREATE(CDrawFlowChartView, CView)

BEGIN_MESSAGE_MAP(CDrawFlowChartView, CView)
	//{{AFX_MSG_MAP(CDrawFlowChartView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RECTANGLE, OnCreateRectangle)
	ON_COMMAND(ID_ELLIPSE, OnCreateEllipse)
	ON_COMMAND(ID_DIAMOND, OnCreateDiamond)
	ON_COMMAND(ID_PARALLELOGRAM, OnCreateDealParallelogram)
	ON_COMMAND(ID_GENERICLINE, OnCreateGenericLine)
	ON_COMMAND(ID_ARROWHEAD, OnCreateArrowhead)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_CONTROLFLOW, OnCreateControlFlow)
	ON_COMMAND(ID_START, OnCreateStart)
	ON_COMMAND(ID_END, OnCreateEnd)
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_COMMAND(ID_SEARCH_PATH, OnSearchPath)
	ON_COMMAND(ID_MARK_PATH, OnMarkPath)
	ON_COMMAND(ID_STOP_MARK, OnStopMark)
	ON_COMMAND(ID_TOOLBAR_END, OnToolbarEnd)
	ON_COMMAND(ID_TOOLBAR_FLOWCONTROL, OnToolbarFlowcontrol)
	ON_COMMAND(ID_TOOLBAR_ARROWHEAD,   OnToolbarArrowHead)
	ON_COMMAND(ID_TOOLBAR_GENERICLINE, OnToolbarGenericLine)
	ON_COMMAND(ID_TOOLBAR_JUDGE, OnToolbarJudge)
	ON_COMMAND(ID_TOOLBAR_ELLIPSE, OnToolbarEllipse)
	ON_COMMAND(ID_TOOLBAR_NEXT, OnToolbarNext)
	ON_COMMAND(ID_TOOLBAR_PROCESS, OnToolbarProcess)
	ON_COMMAND(ID_TOOLBAR_RECT, OnToolbarRect)
	ON_COMMAND(ID_TOOLBAR_SEARCH, OnToolbarSearch)
	ON_COMMAND(ID_TOOLBAR_START, OnToolbarStart)
	ON_COMMAND(ID_TOOLBAR_STOP, OnToolbarStop)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView construction/destruction

CDrawFlowChartView::CDrawFlowChartView()
{
	// TODO: add construction code here

}

CDrawFlowChartView::~CDrawFlowChartView()
{
}

BOOL CDrawFlowChartView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	m_OperateType = SELECT;
	m_LBtnSignal = false;
	m_LDoubleBtnSignal = false;
	m_IsControlFlow = false;

	m_FocusGraph = NULL;

	pedit = new CEdit(); 

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView drawing

void CDrawFlowChartView::OnDraw(CDC* pDC)
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	pDoc->m_GraphManager.DrawAll( pDC );
}

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView printing

BOOL CDrawFlowChartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawFlowChartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawFlowChartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView diagnostics

#ifdef _DEBUG
void CDrawFlowChartView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawFlowChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawFlowChartDoc* CDrawFlowChartView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawFlowChartDoc)));
	return (CDrawFlowChartDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawFlowChartView message handlers

void CDrawFlowChartView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDrawFlowChartDoc* pDoc = GetDocument();

	m_LBtnSignal = true; // 设置鼠标左键为down

	switch(m_OperateType)
	{
		case SELECT:
		case MOVE:
		case ADJUST_SIZE:
			{
				bool flag = false;
				flag = pDoc->m_GraphManager.SetFocusGraphID(point);

				if(pDoc->m_GraphManager.IsAdjustSize(point))
				{
					m_OperateType = ADJUST_SIZE;
				}
				else
				{
					m_OperateType = MOVE;
					m_Start1 = point;
				}

				if(flag)
				{
					if(m_LDoubleBtnSignal)
					{
						CString str;
						pedit->GetWindowText(str);
						if(m_FocusGraph != NULL)
						{
							m_FocusGraph->SetText(str);
						}
						pedit->DestroyWindow();
						m_LDoubleBtnSignal = false;
					}
				}

				Invalidate();
				break;
			}
		case CREATE:
			{
				if(m_LDoubleBtnSignal)
				{
					CString str;
					pedit->GetWindowText(str);
					if(m_FocusGraph != NULL)
					{
						m_FocusGraph->SetText(str);
					}
					pedit->DestroyWindow();
					m_LDoubleBtnSignal = false;
				}
				CGraph* temp = pDoc->m_GraphManager.GetFocusGraph();
				if(temp != NULL)
				{
					if(!m_IsControlFlow)
					{
						temp->SetStartPoint(point);
					}
					else
					{
						temp->SetEndPoint(point);
					}
				}
				break;
			}
	}
	m_FocusGraph = pDoc->m_GraphManager.GetFocusGraph();

	CView::OnLButtonDown(nFlags, point);
}

void CDrawFlowChartView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your message handler code here and/or call default

	switch(m_OperateType)
	{
		case SELECT:
		case MOVE:
		case ADJUST_SIZE:
			{
				break;
			}
		case CREATE:
			{
				if(!m_IsControlFlow)
				{
					CGraph* temp = pDoc->m_GraphManager.GetFocusGraph();
					temp->SetEndPoint( point );
				}
				break;
			}
	}

	Invalidate();
	if(!m_IsControlFlow)
		m_OperateType = SELECT; // 修改操作类型为默认值
	m_LBtnSignal = false; // 设置鼠标左键为up

	CView::OnLButtonUp(nFlags, point);
}

void CDrawFlowChartView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your message handler code here and/or call default
	CString strStatusBar;
	CString strName;
	CPoint ptStart, ptEnd;
    strStatusBar.Format(_T("[%d,%d]  "), point.x, point.y);
	GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(strStatusBar);

	if(m_IsControlFlow)
	{
		CGraph* tempFocus = pDoc->m_GraphManager.GetFocusGraph();
		if(tempFocus != NULL)
		{
			tempFocus->SetStartPoint(point);
			tempFocus->GetStartPoint(ptStart);
			tempFocus->GetEndPoint(ptEnd);
			strName = tempFocus->GetTypeName();
			strStatusBar.Format(_T("[%d, %d] - We selected [(%d, %d), (%d, %d)] with "), 
				point.x, point.y, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
			strStatusBar += strName;
			GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(strStatusBar);
			strStatusBar += "\r\n";
			TRACE(strStatusBar);
			Invalidate();
		}
	}
	else 
	{
		CGraph* tempFocus = pDoc->m_GraphManager.GetFocusGraph();
		if(tempFocus != NULL)
		{
			tempFocus->GetStartPoint(ptStart);
			tempFocus->GetEndPoint(ptEnd);
			strName = tempFocus->GetTypeName();
			strStatusBar.Format(_T("[%d, %d] - We selected [(%d, %d), (%d, %d)] with "), 
				point.x, point.y, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
			strStatusBar += strName;
			GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(strStatusBar);
			strStatusBar += "\r\n";
			TRACE(strStatusBar);
			// It would cause the flash of graph
			// Invalidate();
		}
	}

	// 移动操作的前提是鼠标左键按下
	if(!m_LBtnSignal)
	{ 
		return;
	}

	switch(m_OperateType)
	{
		case MOVE:
			{
				pDoc->m_GraphManager.Move(point.x - m_Start1.x, point.y - m_Start1.y);
				m_Start1 = point;
				break;
			}
		case CREATE:
			{
				CGraph* temp = pDoc->m_GraphManager.GetFocusGraph();
				if(temp != NULL)
				{
					if(!m_IsControlFlow)
						temp->SetEndPoint( point );
				}
				break;
			}
		case ADJUST_SIZE:
			{
				CGraph* temp = pDoc->m_GraphManager.GetFocusGraph();
				if(temp != NULL)
				{
					temp->AdjustSize( point );
				}
				break;
			}
	}

	Invalidate();
	CView::OnMouseMove(nFlags, point);
}


/**
*   鼠标有以下几种操作
*   1. 空操作
*   2. 画图形
*   3. 移动图形
*   4. 改变图形大小
*
**/

void CDrawFlowChartView::OnCreateRectangle() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}
	
	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.Rectangle );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateRectangle());
}

void CDrawFlowChartView::OnCreateEllipse() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.Ellipse );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateEllipse());
}

void CDrawFlowChartView::OnCreateDiamond() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.Diamond );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateJudgeDiamond());
}

void CDrawFlowChartView::OnCreateDealParallelogram() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.DealParallelogram );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateProcessDiamond());
}

void CDrawFlowChartView::OnCreateGenericLine() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.GenericLine );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateLine());
}

void CDrawFlowChartView::OnCreateArrowhead() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = false;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.Arrowhead );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateArrowLine());
}

void CDrawFlowChartView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your message handler code here and/or call default

	pDoc->m_GraphManager.SetFocusGraphID(point);

	CGraph* temp = pDoc->m_GraphManager.GetFocusGraph();
	m_FocusGraph = temp;
	if(temp != NULL && temp->IsEditable())
	{
		m_LDoubleBtnSignal = true;
		CPoint temp1, temp2;
		temp->GetStartPoint(temp1);
		temp->GetEndPoint(temp2);
		CRect rect = CRect(temp1 + CPoint(12, 12), temp2 + CPoint(-12, -12));
		pedit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rect, this, 1);
		CString str;
		temp->GetText(str);
		SetDlgItemText(1, str);
	}

	if(m_OperateType == CREATE)
	{
		m_IsControlFlow = false;
		m_OperateType = SELECT;
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CDrawFlowChartView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	    case VK_DELETE:
			{
				pDoc->m_GraphManager.DeleteFocusGraph();
				Invalidate();
				break;
			}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDrawFlowChartView::OnCreateControlFlow() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	if(m_OperateType != CREATE)
	{
		m_OperateType = CREATE;
	}
	else
	{
		pDoc->m_GraphManager.DeleteFocusGraph();
	}

	m_IsControlFlow = true;
	//CGraph* temp = pDoc->m_GraphManager.CreateGraph( CGraphManager.ControlFlow );
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateControlFlow());
}

void CDrawFlowChartView::OnCreateStart() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateStart());
	Invalidate();
}

void CDrawFlowChartView::OnCreateEnd() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	pDoc->m_GraphManager.AddGraph(pDoc->m_GraphFactory.CreateEnd());
	Invalidate();
}

void CDrawFlowChartView::OnGetMap()  
{  
    CDC* pDC = GetWindowDC();  
    CBitmap bitmap;  
    CDC memDC ;  
    CRect rect;  
    GetWindowRect(rect);  
    memDC.CreateCompatibleDC(pDC);  
  
    bitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());  
    memDC.SelectObject(&bitmap);  
    memDC.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);  
  
    CFileDialog fDlg(FALSE,_T("bmp"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("*.bmp"),this);  
    if (fDlg.DoModal()==IDOK)  
    {  
  
        CString bmpfile = fDlg.GetPathName();  
  
        CFile file(bmpfile,CFile::modeCreate|CFile::modeWrite);  
          
        BITMAP bInfo;  
        bitmap.GetBitmap(&bInfo);  
  
        //计算调色板大小  
        int panelsize = 0;  
        if (bInfo.bmBitsPixel<24) //非真彩色  
        {  
            panelsize = (int)pow((double)2, bInfo.bmBitsPixel) * sizeof(RGBQUAD);  
        }  
  
        //定义位图信息  
        BITMAPINFO*  bMapInfo = (BITMAPINFO*)LocalAlloc(LPTR,sizeof(BITMAPINFO)+panelsize);  
        bMapInfo->bmiHeader.biBitCount = bInfo.bmBitsPixel;  
        bMapInfo->bmiHeader.biClrImportant = 0;  
        bMapInfo->bmiHeader.biCompression = 0;  
        bMapInfo->bmiHeader.biHeight = bInfo.bmHeight;  
        bMapInfo->bmiHeader.biPlanes = bInfo.bmPlanes;  
        bMapInfo->bmiHeader.biSize = sizeof(BITMAPINFO);  
        bMapInfo->bmiHeader.biSizeImage = bInfo.bmHeight*bInfo.bmWidthBytes;  
        bMapInfo->bmiHeader.biWidth = bInfo.bmWidth;  
        bMapInfo->bmiHeader.biXPelsPerMeter = 0;  
        bMapInfo->bmiHeader.biYPelsPerMeter = 0;  
  
        //获取位图的实际数据  
        char* pData = new char[bMapInfo->bmiHeader.biSizeImage];  
        int len = GetDIBits(pDC->m_hDC,bitmap,0,bInfo.bmHeight,pData,bMapInfo,DIB_RGB_COLORS);  
  
        BITMAPFILEHEADER bFileHeader;  
        bFileHeader.bfType = 0x4D42;  
        bFileHeader.bfReserved1 = 0;  
        bFileHeader.bfReserved2 = 0;  
        bFileHeader.bfSize = sizeof(BITMAPFILEHEADER);   
        bFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+panelsize;  
          
        //向文件中写入位图数据  
        file.Write(&bFileHeader,sizeof(BITMAPFILEHEADER));  
        file.Write(&bMapInfo->bmiHeader,sizeof(BITMAPINFOHEADER));  
        file.Write(pData,bMapInfo->bmiHeader.biSizeImage+panelsize);  
        file.Close();  
        delete pData;  
        LocalFree(bMapInfo);  
    }  
    bitmap.DeleteObject();  
    memDC.DeleteDC();   
}  

void CDrawFlowChartView::OnSaveBmp() 
{
	// TODO: Add your command handler code here
	OnGetMap();
}

void CDrawFlowChartView::OnSearchPath() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	int temp = pDoc->m_GraphManager.SearchPath();
	CString str;
	str.Format("目前发现 %d 条路径", temp);  
	MessageBox(str);
}

void CDrawFlowChartView::OnMarkPath() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	pDoc->m_GraphManager.MarkPath();
	Invalidate();
}

void CDrawFlowChartView::OnStopMark() 
{
	CDrawFlowChartDoc* pDoc = GetDocument();
	// TODO: Add your command handler code here
	pDoc->m_GraphManager.CancelMarkPath();
	Invalidate();
}

void CDrawFlowChartView::OnToolbarEnd() 
{
	// TODO: Add your command handler code here
	OnCreateEnd();
}

void CDrawFlowChartView::OnToolbarFlowcontrol() 
{
	// TODO: Add your command handler code here
	OnCreateControlFlow();
}

void CDrawFlowChartView::OnToolbarArrowHead() 
{
	// TODO: Add your command handler code here
	OnCreateArrowhead();
}

void CDrawFlowChartView::OnToolbarGenericLine() 
{
	// TODO: Add your command handler code here
	OnCreateGenericLine();
}

void CDrawFlowChartView::OnToolbarJudge() 
{
	// TODO: Add your command handler code here
	OnCreateDiamond();
}

void CDrawFlowChartView::OnToolbarEllipse() 
{
	// TODO: Add your command handler code here
	OnCreateEllipse();
}

void CDrawFlowChartView::OnToolbarNext() 
{
	// TODO: Add your command handler code here
	OnMarkPath();
}

void CDrawFlowChartView::OnToolbarProcess() 
{
	// TODO: Add your command handler code here
	OnCreateDealParallelogram();
}

void CDrawFlowChartView::OnToolbarRect() 
{
	// TODO: Add your command handler code here
	OnCreateRectangle();
}

void CDrawFlowChartView::OnToolbarSearch() 
{
	// TODO: Add your command handler code here
	OnSearchPath();
}

void CDrawFlowChartView::OnToolbarStart() 
{
	// TODO: Add your command handler code here
	OnCreateStart();
}

void CDrawFlowChartView::OnToolbarStop() 
{
	// TODO: Add your command handler code here
	OnStopMark();
}
