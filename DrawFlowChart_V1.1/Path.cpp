// Path.cpp: implementation of the CPath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "Path.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPath::CPath()
{
	m_CurrentNum = -1;
}

CPath::~CPath()
{

}

void CPath::AddGraph(CGraph *graph)
{
	m_Graphs.Add(graph);
	m_CurrentNum = 0;
}

CGraph* CPath::GetHead()
{
	CGraph *temp = NULL;
	if(GetSum() != 0)
	{
		temp = (CGraph*)m_Graphs.GetAt(0);
	}
	return temp;
}

CGraph* CPath::GetEnd()
{
	return (CGraph*)m_Graphs.GetAt(m_Graphs.GetSize()-1);
}

CGraph* CPath::GetNext()
{
	CGraph *temp = NULL;
	m_CurrentNum++;

	if(GetSum() > m_CurrentNum)
	{
		temp = (CGraph*)m_Graphs.GetAt(m_CurrentNum);
	}

	return temp;
}

int CPath::GetSum()
{
	return m_Graphs.GetSize();
}

CGraph* CPath::GetPathAt(int ID)
{
	return (CGraph*)m_Graphs.GetAt(ID);
}

void CPath::MarkPath()
{
	for(int i = 0; i < GetSum(); i++)
	{
		CGraph *temp = (CGraph*)m_Graphs.GetAt(i);
		temp->SetIsMark(true);

		CGraph *tempPre = temp->GetPreviousGraph();
		if(tempPre != NULL)
		{
			tempPre->SetIsMark(true);
		}

		CGraph *tempNext = temp->GetNextgraph();
		if(tempNext != NULL)
		{
			tempNext->SetIsMark(true);
		}
	}
}

bool CPath::IsInclude(CGraph *graph, int count)
{
	int temp = 0;
	for(int i = 0; i < GetSum(); i++)
	{
		CGraph *temp = (CGraph*)m_Graphs.GetAt(i);
		if(temp == graph) temp++;
	}
	if(temp == count) return true;

	return false;
}