// Graph.cpp: implementation of the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Graph.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraph::CGraph()
{
	m_Start = CPoint(0, 0);
	m_End = CPoint(0, 0);
	m_IsMark = false;
	m_iPreviousConnPointIdx = CCONNECTPOINT_INVALID_OPTION;
	m_iNextConnPointIdx = CCONNECTPOINT_INVALID_OPTION;
}

CGraph::~CGraph()
{

}

void CGraph::GetText(CString &str)
{
	str = m_text;
}

void CGraph::SetText(CString &str)
{
	m_text = str;
}

// �õ������ø�ͼԪ��������Ŀ�ʼ��(���Ͻ�)�ͽ�����(���½�)
void CGraph::GetStartPoint(CPoint &pt)
{
	pt = m_Start;
}

void CGraph::GetEndPoint(CPoint &pt)
{
	pt = m_End;
}

void CGraph::SetStartPoint(CPoint &pt)
{
	m_Start = pt;
}

void CGraph::SetEndPoint(CPoint &pt)
{
	m_End = pt;
}
// �Ƿ��ڸ�ͼԪ������
bool CGraph::IsIn(CPoint &pt)
{
	return false;
}
// �Ƿ��ڸ�ͼԪ�߽���
bool CGraph::IsOn(CPoint &pt)
{
	return false;
}

int CGraph::IsConnectOn(CConnectPoint *pt)
{
	return CCONNECTPOINT_INVALID_OPTION;
}

bool CGraph::IsEditable()
{ 
	return true;
}

bool CGraph::IsControlFlow()
{ 
	return false;
}

void CGraph::SetPreviousGraph(CGraph *previousGraph)
{
}
void CGraph::SetNextgraph(CGraph *nextGraph)
{
}

CGraph* CGraph::GetPreviousGraph()
{ 
	return NULL;
}

CGraph* CGraph::GetNextgraph()
{ 
	return NULL;
}

void CGraph::SetIsMark(bool isMark)
{
	m_IsMark = isMark;
}

bool CGraph::IsSelected( CPoint &pt )
{
	// return (IsIn( pt ) || IsOn( pt ));
	bool bRet = IsIn( pt );
	if(bRet)
	{
		return true;
	}
	bRet = IsOn( pt );
	return bRet;
}

void CGraph::AdjustSize(CPoint &pt)
{
}