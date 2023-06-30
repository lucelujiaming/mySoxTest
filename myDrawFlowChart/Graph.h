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
#define CCONNECTPOINT_RECT_LEFT_TOP       0    // ���Ͻ�
#define CCONNECTPOINT_RECT_LEFT_BOTTOM    1    // ���½�
#define CCONNECTPOINT_RECT_RIGHT_TOP      2    // ���Ͻ�
#define CCONNECTPOINT_RECT_RIGHT_BOTTOM   3    // ���½�
#define CCONNECTPOINT_RECT_CNT            4    
#define CCONNECTPOINT_RECT_MIDDLE_TOP     4    // ���ϽǺ����Ͻǵ��м�λ��
#define CCONNECTPOINT_RECT_MIDDLE_RIGHT   5    // ���ϽǺ����½ǵ��м�λ��
#define CCONNECTPOINT_RECT_MIDDLE_BOTTOM  6    // ���½Ǻ����½ǵ��м�λ��
#define CCONNECTPOINT_RECT_MIDDLE_LEFT    7    // ���ϽǺ����½ǵ��м�λ��
#define CCONNECTPOINT_RECT_MAX            8  

#define CCONNECTPOINT_LINE_START          0    // ���
#define CCONNECTPOINT_LINE_END            1    // �յ�
#define CCONNECTPOINT_LINE_MAX            2

#define CCONNECTPOINT_POLYGONALLINE_START          0    // ���
#define CCONNECTPOINT_POLYGONALLINE_MIDDLE         1    // �м��
#define CCONNECTPOINT_POLYGONALLINE_END            2    // �յ�
#define CCONNECTPOINT_POLYGONALLINE_MAX			   3

class CGraph  // : public CObject
{
public:
	// �õ������ø�ͼԪ������
	virtual void GetText(CString &str);
	virtual void SetText(CString &str);
	// �õ������ø�ͼԪ��������Ŀ�ʼ��(���Ͻ�)�ͽ�����(���½�)
	virtual void GetStartPoint(CPoint &pt);
	virtual void GetEndPoint(CPoint &pt);
	virtual void SetStartPoint(CPoint &pt);
	virtual void SetEndPoint(CPoint &pt);
	virtual void SetLastPoint(CPoint &pt);
	// �Ƿ��ڸ�ͼԪ������
	virtual bool IsIn(CPoint &pt);

	// �Ƿ��ڸ�ͼԪ�߽���
	virtual bool IsOn(CPoint &pt);
	virtual int  IsConnectOn(CAdjustPoint *pt);

	// �Ƿ�ѡ��
	virtual bool IsSelected(CPoint &pt);

	// �Ƿ�ɱ༭
	virtual bool IsEditable();
	virtual bool IsControlFlow();

	// ���ø�ͼԪ��ǰ��ͼԪ�ͺ��ͼԪ
	virtual void SetPreviousGraph(CGraph *previousGraph);
	virtual void SetNextgraph(CGraph *nextGraph);
	virtual CGraph* GetPreviousGraph();
	virtual CGraph* GetNextgraph();

	// ���ø�ͼԪ�Ƿ񱻱��
	virtual void SetIsMark(bool isMark);

	virtual void Draw(CDC *pdc, BOOL bShowSelectBorder = TRUE) = 0;
	virtual void DrawFocus(CDC *pdc) = 0;
	virtual void Move(int cx, int cy) = 0;
	// ����ͼԪ��С
	virtual void AdjustSize(CPoint &pt);

	// ����ͼԪ������
	virtual void SaveParamsToJSON(cJSON * objJSON) = 0;
	// ����ͼԪ������
	virtual void LoadParamsFromJSON(cJSON * objJSON) = 0;

	// ���ظ�ͼԪ��ǰ��ͼԪ�ͺ��ͼԪ��ֻ���ļ����ص�ʱ����á�
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
	bool m_IsMark; // �Ƿ���
	CString m_text; // ��������
	std::vector<CAdjustPoint *>  m_Points; // ��ͼԪ�������������ӵ�(CConnentPoint)
	
	int     m_iGraphSeq ;
	// Connect Point information
	int     m_iPreviousGraphSeq ;
	int     m_iPreviousConnPointIdx ;
	int     m_iNextGraphSeq ;
	int     m_iNextConnPointIdx ;

	int   m_ScreenX;
	int   m_ScreenY;

protected:
	CPoint m_Start; // ��ʼ��
	CPoint m_End; // ������
	
	CGraph* m_Previous;
	CGraph* m_Next;
};

#endif // !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
