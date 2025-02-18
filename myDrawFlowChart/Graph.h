// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
#define AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "MainFrm.h"
#include "AdjustPoint.h"

#define CCONNECTPOINT_INVALID_OPTION      -1
#define CCONNECTPOINT_RECT_LEFT_TOP       0    // 左上角
#define CCONNECTPOINT_RECT_LEFT_BOTTOM    1    // 左下角
#define CCONNECTPOINT_RECT_RIGHT_TOP      2    // 右上角
#define CCONNECTPOINT_RECT_RIGHT_BOTTOM   3    // 右下角
#define CCONNECTPOINT_RECT_CNT            4    
#define CCONNECTPOINT_RECT_MIDDLE_TOP     4    // 左上角和右上角的中间位置
#define CCONNECTPOINT_RECT_MIDDLE_RIGHT   5    // 右上角和右下角的中间位置
#define CCONNECTPOINT_RECT_MIDDLE_BOTTOM  6    // 左下角和右下角的中间位置
#define CCONNECTPOINT_RECT_MIDDLE_LEFT    7    // 左上角和左下角的中间位置
#define CCONNECTPOINT_RECT_MAX            8  

#define CCONNECTPOINT_LINE_START          0    // 起点
#define CCONNECTPOINT_LINE_END            1    // 终点
#define CCONNECTPOINT_LINE_MAX            2

#define CCONNECTPOINT_POLYGONALLINE_START          0    // 起点
#define CCONNECTPOINT_POLYGONALLINE_MIDDLE         1    // 中间点
#define CCONNECTPOINT_POLYGONALLINE_END            2    // 终点
#define CCONNECTPOINT_POLYGONALLINE_MAX			   3

class CGraph  // : public CObject
{
public:
	// 得到和设置该图元的文字
	virtual void GetText(CString &str);
	virtual void SetText(CString &str);
	// 得到和设置该图元矩形区域的开始点(左上角)和结束点(右下角)
	virtual void GetStartPoint(CPoint &pt);
	virtual void GetEndPoint(CPoint &pt);
	virtual void SetStartPoint(CPoint &pt);
	virtual void SetEndPoint(CPoint &pt);
	virtual void SetLastPoint(CPoint &pt);
	// 是否在该图元区域内
	virtual bool IsIn(CPoint &pt);

	// 是否在该图元边界上
	virtual bool IsOn(CPoint &pt);
	virtual int  IsConnectOn(CAdjustPoint *pt);

	// 是否被选中
	virtual bool IsSelected(CPoint &pt);

	// 是否可编辑
	virtual bool IsEditable();
	virtual bool IsControlFlow();

	// 设置该图元的前赴图元和后继图元
	virtual void SetPreviousGraph(CGraph *previousGraph);
	virtual void SetNextgraph(CGraph *nextGraph);
	virtual CGraph* GetPreviousGraph();
	virtual CGraph* GetNextgraph();

	// 设置该图元是否被标记
	virtual void SetIsMark(bool isMark);

	virtual void Draw(CDC *pdc, BOOL bShowSelectBorder = TRUE) = 0;
	virtual void DrawFocus(CDC *pdc) = 0;
	virtual void Move(int cx, int cy) = 0;
	// 调整图元大小
	virtual void AdjustSize(CPoint &pt);

	// 保存图元参数。
	virtual void SaveParamsToJSON(cJSON * objJSON) = 0;
	// 加载图元参数。
	virtual void LoadParamsFromJSON(cJSON * objJSON) = 0;

	// 加载该图元的前赴图元和后继图元。只在文件加载的时候调用。
	void loadPreviousGraph(CGraph *previousGraph) { m_Previous = previousGraph; }
	void loadNextgraph(CGraph *nextGraph) { m_Next = nextGraph; }

public:
	virtual CString GetTypeName() = 0;
	virtual void AdjustFocusPoint();

public:
	CGraph();
	virtual ~CGraph();

	void printAllPoints(CString strCaption);
	
	int  getGraphSeq() { return m_iGraphSeq ; }
	void setGraphSeq(int iGraphSeq) { m_iGraphSeq = iGraphSeq; }
	
	void setSCreenSize(int iCXX, int iCYY) { m_ScreenX = iCXX, m_ScreenY = iCYY; }

public:
	bool m_IsMark; // 是否标记
	CString m_text; // 描述文字
	std::vector<CAdjustPoint *>  m_Points; // 该图元包含的所有连接点(CConnentPoint)
	
	int     m_iGraphSeq ;
	// Connect Point information
	int     m_iPreviousGraphSeq ;
	int     m_iPreviousConnPointIdx ;
	int     m_iNextGraphSeq ;
	int     m_iNextConnPointIdx ;

	int   m_ScreenX;
	int   m_ScreenY;

protected:
	CPoint m_Start; // 开始点
	CPoint m_End; // 结束点
	
	CGraph* m_Previous;
	CGraph* m_Next;
};

#endif // !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
