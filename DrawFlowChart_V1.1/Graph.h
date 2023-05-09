// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
#define AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConnectPoint.h"

class CGraph : public CObject
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
	// 是否在该图元区域内
	virtual bool IsIn(CPoint &pt);

	// 是否在该图元边界上
	virtual bool IsOn(CPoint &pt);
	virtual bool IsOn(CConnectPoint *pt);

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

	virtual void Draw(CDC *pdc) = 0;
	virtual void DrawFocus(CDC *pdc) = 0;
	virtual void Move(int cx, int cy) = 0;
	// 调整图元大小
	virtual void AdjustSize(CPoint &pt);
	virtual void Serialize(CArchive& ar) = 0;

public:
	CGraph();
	virtual ~CGraph();

public:
	bool m_IsMark; // 是否标记
	CPoint m_Start; // 开始点
	CPoint m_End; // 结束点
	CString m_text; // 描述文字
	CObArray m_Points; // 该图元包含的所有连接点(CConnentPoint)
};

#endif // !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
