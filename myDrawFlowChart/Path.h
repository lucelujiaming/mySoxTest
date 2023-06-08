// Path.h: interface for the CPath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATH_H__46F037BB_D475_4E0F_830C_5FAF38EDBEA5__INCLUDED_)
#define AFX_PATH_H__46F037BB_D475_4E0F_830C_5FAF38EDBEA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "Graph.h"

class CPath : public CObject
{
public:
	CPath();
	virtual ~CPath();

public:
	void AddGraph(CGraph *graph);
	CGraph *GetHead();
	CGraph *GetNext();
	CGraph *GetEnd();
	CGraph *GetPathAt(int ID);
	int GetSum();
	void MarkPath();

	bool IsInclude(CGraph *graph, int count = 1);

private:
	std::vector<CGraph *> m_Graphs;
	int m_CurrentNum;
};

#endif // !defined(AFX_PATH_H__46F037BB_D475_4E0F_830C_5FAF38EDBEA5__INCLUDED_)
