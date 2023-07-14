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
	CPath *newPath = new CPath();
	newPath->AddGraph(graph);

	m_Paths.Add(newPath);

	m_CurrentID = 0;
}

bool CPathManager::UpdatePaths(CGraph *graph)
{
	CPath *currentPath;
	CGraph *endPathGraph;

	bool flag = true;

	for(int i = 0; i < GetSum(); i++)
	{
		currentPath = (CPath*)m_Paths.GetAt(i);
		endPathGraph = currentPath->GetEnd();

		if(graph != endPathGraph && graph->GetPreviousGraph() == endPathGraph->GetPreviousGraph())
		{
			if(currentPath->IsInclude(graph))
			{
				flag = false;
				break;
			}
			CPath *clonePath;
			clonePath = ClonePath(currentPath);
			clonePath->AddGraph(graph);

			m_Paths.Add(clonePath);
			break;
		}
		if(graph->GetPreviousGraph() == endPathGraph->GetNextgraph())
		{
			if(currentPath->IsInclude(graph, 2))
			{
				flag = false;
				break;
			}
			currentPath->AddGraph(graph);
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
	CPath *newPath = new CPath();
	for(int i = 0; i < path->GetSum()-1; i++)
	{
		newPath->AddGraph(path->GetPathAt(i));
	}

	return newPath;
}

void CPathManager::DeleteAll()
{
	m_Paths.RemoveAll();
}

CPath* CPathManager::GetPath()
{
	CPath *currentPath = NULL;
	if(m_Paths.GetSize() == 0) 
    {
        return NULL;
    }
	m_CurrentID++;
	if(m_CurrentID >=0 && m_CurrentID < m_Paths.GetSize())
	{
		
		currentPath = (CPath *)m_Paths.GetAt(m_CurrentID);
	}
	else
	{
		m_CurrentID = -1;
	}

	return currentPath;
}
