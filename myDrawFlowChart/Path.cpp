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
	m_Graphs.push_back(graph);
	m_CurrentNum = 0;
}

CGraph* CPath::GetHead()
{
	CGraph *headPath = NULL;
	if(GetSum() != 0)
	{
		headPath = (CGraph*)m_Graphs[0];
	}
	return headPath;
}

CGraph* CPath::GetEnd()
{
	return (CGraph*)m_Graphs[m_Graphs.size()];
}

CGraph* CPath::GetNext()
{
	CGraph *nextPath = NULL;
	m_CurrentNum++;

	if(GetSum() > m_CurrentNum)
	{
		nextPath = (CGraph*)m_Graphs[m_CurrentNum];
	}

	return nextPath;
}

int CPath::GetSum()
{
	return m_Graphs.size();
}

CGraph* CPath::GetPathAt(int ID)
{
	return (CGraph*)m_Graphs[ID];
}

void CPath::MarkPath()
{
	for(int i = 0; i < GetSum(); i++)
	{
		CGraph *currentPath = (CGraph*)m_Graphs[i];
		currentPath->SetIsMark(true);

		CGraph *previousPath = currentPath->GetPreviousGraph();
		if(previousPath != NULL)
		{
			previousPath->SetIsMark(true);
		}

		CGraph *nextPath = currentPath->GetNextgraph();
		if(nextPath != NULL)
		{
			nextPath->SetIsMark(true);
		}
	}
}

bool CPath::IsInclude(CGraph *graph, int count)
{
	int iCounter = 0;
	for(int i = 0; i < GetSum(); i++)
	{
		CGraph *currentGraph = (CGraph*)m_Graphs[i];
		if(currentGraph == graph) 
        {
			iCounter++;
        }
	}
	if(iCounter == count) 
    {
        return true;
	}

	return false;
}
