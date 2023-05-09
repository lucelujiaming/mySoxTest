// GraphManager.cpp: implementation of the CGraphManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "GraphManager.h"

#include "Rectangle.h"
#include "Ellipse.h"
#include "Diamond.h"
#include "DealDiamond.h"
#include "GenericLine.h"
#include "Arrowhead.h"
#include "ControlFlow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphManager::CGraphManager()
{
	m_FocusID = -1;
}

CGraphManager::~CGraphManager()
{
}

void CGraphManager::DrawAll( CDC *pdc )
{
	/*for(int i = GetGraphSum(); i > 0; i--)
	{
		GetGraphAt(i-1)->Draw( pdc );
		if(m_FocusID == i-1)
		{
			GetGraphAt(i-1)->DrawFocus( pdc );
		}
	}*/
	//CheckLinkGraph();

	for(int i = 0; i < GetGraphSum(); i++)
	{
		GetGraphAt(i)->Draw(pdc);
		//if(GetGraphAt(i)->IsControlFlow())
			//CheckLinkGraph(GetGraphAt(i));
	}

	CGraph *temp = GetFocusGraph();
	if(temp != NULL)
	{
		temp->Draw(pdc);
		temp->DrawFocus(pdc);
	}
}

CGraph* CGraphManager::GetGraphAt( int ID )
{
	CGraph* temp = NULL;
	if((ID >= 0) && (ID < GetGraphSum()))
	{
		temp = (CGraph*)m_Graphs.GetAt( ID );
	}
	return temp;
}

int CGraphManager::GetGraphSum()
{
	return m_Graphs.GetSize();
}

void CGraphManager::DeleteFocusGraph()
{
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		CGraph* tempF = GetGraphAt(m_FocusID);

		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph *temp = GetGraphAt(i);

			if(temp->GetPreviousGraph() == tempF || temp->GetNextgraph() == tempF)
			{
				DeleteGraphAt(i);
			}
		}

		DeleteGraphAt(m_FocusID);

		m_FocusID = -1;
	}
}

void CGraphManager::DeleteGraphAt( int ID )
{
	if((ID >= 0) && (ID < GetGraphSum()))
	{
		//CGraph* temp = (CGraph*)m_Graphs.GetAt(ID);
		//if(temp != NULL)
			//delete temp;
		m_Graphs.RemoveAt( ID );
	}
}

CGraph* CGraphManager::GetFocusGraph()
{
	CGraph* temp = NULL;
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		temp = (CGraph*)m_Graphs.GetAt( m_FocusID );

		if(temp->IsControlFlow())
			CheckLinkGraph(temp);
	}

	return temp;
}

void CGraphManager::AddGraph(CGraph* graph)
{
	m_Graphs.InsertAt(0, graph, 1);
	m_FocusID = 0;
}

/*CGraph* CGraphManager::CreateGraph( int type )
{
	CGraph* temp = NULL;
	switch( type )
	{
		case Rectangle: 
			{
				temp = new CRectangle();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case Ellipse: 
			{
				temp = new CEllipse();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case Diamond: 
			{
				temp = new CDiamond();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case DealDiamond: 
			{
				temp = new CDealDiamond();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case GenericLine: 
			{
				temp = new CGenericLine();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case Arrowhead: 
			{
				temp = new CArrowhead();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}
		case ControlFlow: 
			{
				temp = new CControlFlow();
				m_Graphs.InsertAt(0, temp, 1);
				break;
			}			
	}

	m_FocusID = 0;
	return temp;
}
*/

bool CGraphManager::SetFocusGraphID( CPoint &pt )
{
	CGraph* temp1 = GetFocusGraph();
	CGraph* temp = NULL;
	for(int i = 0; i < GetGraphSum(); i++)
	{
		temp = GetGraphAt(i);
		if(temp->IsSelected(pt))
		{
			if(temp1 != temp)
			{
				DeleteGraphAt( i );
				m_Graphs.InsertAt(0, temp, 1);
				m_FocusID = 0;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	m_FocusID = -1;
	return true;
}

bool CGraphManager::IsAdjustSize( CPoint &pt )
{
	CGraph* temp = GetFocusGraph();
	bool flag = false;
	
	if(temp != NULL)
	{
		if(temp->IsOn( pt ))
		{
			flag = true;
		}
	}

	return flag;
}

void CGraphManager::AdjustSize( CPoint &pt )
{
	CGraph* focusGraph = GetFocusGraph();
	if(focusGraph != NULL)
	{
		focusGraph->AdjustSize( pt );
	}
}

void CGraphManager::Move(int cx, int cy)
{
	CGraph* tempF = GetFocusGraph();
	if(tempF != NULL)
	{
		tempF->Move(cx, cy);
		/*if(!temp->IsControlFlow())
		{
		}*/
		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph* temp = GetGraphAt(i);
			if(temp->GetPreviousGraph() == tempF)
			{
				CPoint tempPoint;
				temp->GetEndPoint(tempPoint);
				temp->SetEndPoint(tempPoint + CPoint(cx, cy));
			}

			if(temp->GetNextgraph() == tempF)
			{
				CPoint tempPoint;
				temp->GetStartPoint(tempPoint);
				temp->SetStartPoint(tempPoint + CPoint(cx, cy));
			}
		}
	}
}

void CGraphManager::CheckLinkGraph(CGraph* graph)
{
	//if(graph == NULL && !graph->IsControlFlow()) return;

	for(int j = 0; j < GetGraphSum(); j++)
	{
		CGraph *temp = GetGraphAt(j);
		if(!temp->IsControlFlow())
		{
			graph->SetNextgraph(temp);
			graph->SetPreviousGraph(temp);
		}
	}
}

void CGraphManager::CheckAllLinkGraph()
{
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph* graph = GetGraphAt(i);
	
		CheckLinkGraph(graph);
	}
}

int CGraphManager::SearchPath()
{
	m_PathManager.DeleteAll();

	CObArray tempBorders, tempBs, copyAllBorders, copyAllBordersTemp;
	// 找到开始和结束图元
	CGraph *startGraph = NULL;
	CGraph *endGraph = NULL;
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph *temp = GetGraphAt(i);

		if(temp->IsControlFlow())
		{
			copyAllBorders.Add(temp);
			copyAllBordersTemp.Add(temp);
		}

		CString str;
		temp->GetText(str);
		if(str == "开始")
		{
			startGraph = temp;
		}
		if(str == "结束")
		{
			endGraph = temp;
		}
	}

	// 找到第一条边，并初始化路径管理器
	for(i = 0; i < copyAllBorders.GetSize(); i++)
	{
		CGraph *temp = (CGraph *)copyAllBorders.GetAt(i);
		if(temp->GetPreviousGraph() == startGraph)
		{
			m_PathManager.InitPaths(temp);
			//copyAllBordersTemp.RemoveAt(i);
			if(temp->GetNextgraph() != NULL && temp->GetNextgraph() != endGraph)
				tempBorders.Add(temp);
		}
	}
	if(tempBorders.GetSize() == 0 && m_PathManager.GetSum() == 0) return 0;
	//copyAllBorders.RemoveAll();
	//copyAllBorders.Append(copyAllBordersTemp);

	while(true)
	{
		for(int j = 0; j < tempBorders.GetSize(); j++)
		{
			CGraph *temp = (CGraph *)tempBorders.GetAt(j);
			bool flag = false;
			for(int k = 0; k < copyAllBorders.GetSize(); k++)
			{
				CGraph *tempK = (CGraph *)copyAllBorders.GetAt(k);
				if(tempK->GetPreviousGraph() == temp->GetNextgraph())
				{
					if(!m_PathManager.UpdatePaths(tempK)) 
					{
						flag = true;
						break;
					}
					if(tempK->GetNextgraph() != NULL && tempK->GetNextgraph() != endGraph)
						tempBs.Add(tempK);
				}
			}
			//if(flag) continue;
		}

		if(tempBs.GetSize() == 0) break;

		tempBorders.RemoveAll();	
		tempBorders.Append(tempBs);
		tempBs.RemoveAll();
	}

	return m_PathManager.GetSum();
}

void CGraphManager::MarkPath()
{
	CancelMarkPath();

	CPath *temp;
	temp = m_PathManager.GetPath();
	if(temp != NULL)
	{
		temp->MarkPath();
	}
}

void CGraphManager::CancelMarkPath()
{
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph *temp = GetGraphAt(i);
		temp->SetIsMark(false);
	}
}