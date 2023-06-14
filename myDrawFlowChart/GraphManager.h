// GraphManager.h: interface for the CGraphManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_)
#define AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Graph.h"
#include "PathManager.h"

class CGraphManager  
{
public:
	CGraphManager();
	virtual ~CGraphManager();

public:
	void DrawAll(CDC *pdc, BOOL bShowSelectBorder = TRUE);
	void DeleteAll() { m_Graphs.clear(); }
	void DeleteFocusGraph();
	void DeleteGraphAt(int ID);

	bool SetFocusGraphID(CPoint &pt);
	bool IsAdjustSize(CPoint &pt);
//	void AdjustSize(CPoint &pt);
	void Move(int cx, int cy);

	CGraph* GetGraphAt(int ID);
	CGraph* GetFocusGraph();
	//CGraph* CreateGraph(int type);
	void MarkPath();
	void CancelMarkPath();

	void AddGraph(CGraph* graph, cJSON * jsonParam = NULL);

	int GetGraphSum();

	int SearchPath();

	void CheckAllLinkGraph();

	void SaveToJSON(cJSON * objJSON);
	void LoadFromJSON(cJSON * objJSON);

	void setSCreenSize(int iCXX, int iCYY);
private:
	void CheckLinkGraph(CGraph* graph);

public:
	//static enum GRAPH_TYPE{Rectangle, Ellipse, Diamond, DealParallelogram, GenericLine, ArrowLine, ControlFlow};
//private:
	std::vector<CGraph *> m_Graphs;
	int m_FocusID;

	CPathManager m_PathManager;
};

#endif // !defined(AFX_GRAPHMANAGER_H__241A3F2E_3AF2_4A6C_BF75_5D8D735EF733__INCLUDED_)
