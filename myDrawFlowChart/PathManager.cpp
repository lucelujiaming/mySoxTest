// PathManager.cpp: implementation of the CPathManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "PathManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPathManager::CPathManager()
{
	m_CurrentID = -1;
}

CPathManager::~CPathManager()
{

}

void CPathManager::InitPaths(CGraph *graph)
{
	CPath *temp = new CPath();
	temp->AddGraph(graph);

	m_Paths.Add(temp);

	m_CurrentID = 0;
}

bool CPathManager::UpdatePaths(CGraph *graph)
{
	CPath *temp;
	CGraph *tempGraph;

	bool flag = true;

	for(int i = 0; i < GetSum(); i++)
	{
		temp = (CPath*)m_Paths.GetAt(i);
		tempGraph = temp->GetEnd();

		if(graph != tempGraph && graph->GetPreviousGraph() == tempGraph->GetPreviousGraph())
		{
			if(temp->IsInclude(graph))
			{
				flag = false;
				break;
			}
			CPath *tempPath;
			tempPath = ClonePath(temp);
			tempPath->AddGraph(graph);

			m_Paths.Add(tempPath);
			break;
		}
		if(graph->GetPreviousGraph() == tempGraph->GetNextgraph())
		{
			if(temp->IsInclude(graph, 2))
			{
				flag = false;
				break;
			}
			temp->AddGraph(graph);
			//return;
		}
	}

	return flag;
}

int CPathManager::GetSum()
{
	return m_Paths.GetSize();
}

CPath* CPathManager::ClonePath(CPath *path)
{
	CPath *temp = new CPath();
	for(int i = 0; i < path->GetSum()-1; i++)
	{
		temp->AddGraph(path->GetPathAt(i));
	}

	return temp;
}

void CPathManager::DeleteAll()
{
	m_Paths.RemoveAll();
}

CPath* CPathManager::GetPath()
{
	CPath *temp = NULL;
	if(m_Paths.GetSize() == 0) return NULL;
	m_CurrentID++;
	if(m_CurrentID >=0 && m_CurrentID < m_Paths.GetSize())
	{
		
		temp = (CPath *)m_Paths.GetAt(m_CurrentID);
	}
	else
	{
		m_CurrentID = -1;
	}

	return temp;
}