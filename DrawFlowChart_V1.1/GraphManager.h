// GraphManager.h: interface for the CGraphManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_)
#define AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "PathManager.h"

class CGraphManager  
{
public:
	CGraphManager();
	virtual ~CGraphManager();

public:
	void DrawAll(CDC *pdc);
	void DeleteAll();
	void DeleteFocusGraph();
	void DeleteGraphAt(int ID);

	bool SetFocusGraphID(CPoint &pt);
	bool IsAdjustSize(CPoint &pt);
	void AdjustSize(CPoint &pt);
	void Move(int cx, int cy);

	CGraph* GetGraphAt(int ID);
	CGraph* GetFocusGraph();
	//CGraph* CreateGraph(int type);
	void MarkPath();
	void CancelMarkPath();

	void AddGraph(CGraph* graph);

	int GetGraphSum();

	int SearchPath();

	void CheckAllLinkGraph();

private:
	void CheckLinkGraph(CGraph* graph);

public:
	//static enum GRAPH_TYPE{Rectangle, Ellipse, Diamond, DealDiamond, GenericLine, Arrowhead, ControlFlow};
//private:
	CObArray m_Graphs;
	int m_FocusID;

	CPathManager m_PathManager;
};

#endif // !defined(AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_)
