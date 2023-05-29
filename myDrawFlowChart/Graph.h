// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
#define AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConnectPoint.h"

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

class CGraph : public CObject
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
	// �Ƿ��ڸ�ͼԪ������
	virtual bool IsIn(CPoint &pt);

	// �Ƿ��ڸ�ͼԪ�߽���
	virtual bool IsOn(CPoint &pt);
	virtual int  IsConnectOn(CConnectPoint *pt);

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

	virtual void Draw(CDC *pdc) = 0;
	virtual void DrawFocus(CDC *pdc) = 0;
	virtual void Move(int cx, int cy) = 0;
	// ����ͼԪ��С
	virtual void AdjustSize(CPoint &pt);
	virtual void Serialize(CArchive& ar) = 0;

public:
	virtual CString GetTypeName() = 0;

public:
	CGraph();
	virtual ~CGraph();

public:
	bool m_IsMark; // �Ƿ���
	CString m_text; // ��������
	CObArray m_Points; // ��ͼԪ�������������ӵ�(CConnentPoint)
	
	int     m_iPreviousConnPointIdx ;
	int     m_iNextConnPointIdx ;

protected:
	CPoint m_Start; // ��ʼ��
	CPoint m_End; // ������
};

#endif // !defined(AFX_GRAPH_H__AC673236_A970_4407_939B_BDA6723EFFF1__INCLUDED_)
