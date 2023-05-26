// PathManager.h: interface for the CPathManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATHMANAGER_H__4334B3A9_14A9_4D09_A2A2_ACAB108BCD21__INCLUDED_)
#define AFX_PATHMANAGER_H__4334B3A9_14A9_4D09_A2A2_ACAB108BCD21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "Path.h"

class CPathManager  
{
public:
	CPathManager();
	virtual ~CPathManager();

public:
	void InitPaths(CGraph *graph);
	bool UpdatePaths(CGraph *graph);
	int GetSum();
	void DeleteAll();
	CPath* GetPath();

private:
	CPath* ClonePath(CPath *path);

private:
	CObArray m_Paths;
	int m_CurrentID;
};

#endif // !defined(AFX_PATHMANAGER_H__4334B3A9_14A9_4D09_A2A2_ACAB108BCD21__INCLUDED_)
