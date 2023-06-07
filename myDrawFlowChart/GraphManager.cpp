// GraphManager.cpp: implementation of the CGraphManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawFlowChart.h"
#include "GraphManager.h"

#include "Rectangle.h"
#include "Ellipse.h"
#include "Diamond.h"
#include "Parallelogram.h"
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

/************************************************************************/
/* 功能：绘制操作                                                       */
/************************************************************************/
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
		// 调用每一个图形对象的Draw函数
		GetGraphAt(i)->Draw(pdc);
		//if(GetGraphAt(i)->IsControlFlow())
			//CheckLinkGraph(GetGraphAt(i));
	}

	// 获得当前选中的图形对象
	CGraph *temp = GetFocusGraph();
	// 如果有选中的图形对象，
	if(temp != NULL)
	{
		// 还要按照选中的情况进行绘制。包括绘制一个虚线框和绿色连接点。
		temp->Draw(pdc);
		temp->DrawFocus(pdc);
	}
}

/************************************************************************/
/* 功能：根据ID获得一个图形对象                                         */
/************************************************************************/
CGraph* CGraphManager::GetGraphAt( int ID )
{
	CGraph* temp = NULL;
	if((ID >= 0) && (ID < GetGraphSum()))
	{
		temp = (CGraph*)m_Graphs.GetAt( ID );
	}
	return temp;
}

/************************************************************************/
/* 功能：获得图形对象个数                                               */
/************************************************************************/
int CGraphManager::GetGraphSum()
{
	return m_Graphs.GetSize();
}

/************************************************************************/
/* 功能：按下删除按键以后触发，删除当前选中图形对象。                   */
/************************************************************************/
void CGraphManager::DeleteFocusGraph()
{
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		CGraph* tempFocus = GetGraphAt(m_FocusID);

		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph *temp = GetGraphAt(i);

			if(temp->GetPreviousGraph() == tempFocus || temp->GetNextgraph() == tempFocus)
			{
				DeleteGraphAt(i);
			}
		}

		DeleteGraphAt(m_FocusID);

		m_FocusID = -1;
	}
}

/************************************************************************/
/* 功能：把一个对象从m_Graphs中删除。                                   */
/************************************************************************/
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

/************************************************************************/
/* 功能：获得当前选中的图形对象。                                       */
/************************************************************************/
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

/************************************************************************/
/* 功能：添加一个图形对象。                                             */
/************************************************************************/
void CGraphManager::AddGraph(CGraph* graph)
{
	m_Graphs.InsertAt(0, graph, 1);
	m_FocusID = 0;
}

/************************************************************************/
/* 功能：点击一个图形对象以后，发生的选中操作。                         */
/* 返回值：选中图元发生变化返回真，否则返回假。                         */
/************************************************************************/
bool CGraphManager::SetFocusGraphID( CPoint &pt )
{
	CGraph* oldFocusGraph = GetFocusGraph();
	CGraph* objGraph = NULL;
	for(int i = 0; i < GetGraphSum(); i++)
	{
		objGraph = GetGraphAt(i);
		if(objGraph->IsSelected(pt))
		{
			if(oldFocusGraph != objGraph)
			{
				DeleteGraphAt( i );
				m_Graphs.InsertAt(0, objGraph, 1);
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

/************************************************************************/
/* 功能：是否在调整一个图形对象大小。                                   */
/************************************************************************/
bool CGraphManager::IsAdjustSize( CPoint &pt )
{
	CGraph* tempFocus = GetFocusGraph();
	bool flag = false;
	
	if(tempFocus != NULL)
	{
		// 当前坐标是否在图元边界。
		if(tempFocus->IsOn( pt ))
		{
			flag = true;
		}
	}

	return flag;
}

//	void CGraphManager::AdjustSize( CPoint &pt )
//	{
//		CGraph* focusGraph = GetFocusGraph();
//		if(focusGraph != NULL)
//		{
//			focusGraph->AdjustSize( pt );
//		}
//	}

/************************************************************************/
/* 功能：移动一个图元对象。                                             */
/************************************************************************/
void CGraphManager::Move(int cx, int cy)
{
	CGraph* tempFocus = GetFocusGraph();
	if(tempFocus != NULL)
	{
		tempFocus->Move(cx, cy);
		/*if(!temp->IsControlFlow())
		{
		}*/
		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph* temp = GetGraphAt(i);
			// 如果当前图元对象的前图元对象是当前选中图元对象。
			if(temp->GetPreviousGraph() == tempFocus)
			{
				if(temp->m_iPreviousConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// 跟随调整这条连线。
					CPoint tempPoint;
					temp->GetStartPoint(tempPoint);
					temp->SetStartPoint(tempPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewStart = (CAdjustPoint*)tempFocus->m_Points.GetAt(temp->m_iPreviousConnPointIdx);
					if(pNewStart)
					{
						temp->SetStartPoint(pNewStart->GetPoint());
					}
				}
			}
			// 如果当前图元对象的后图元对象是当前选中图元对象。
			if(temp->GetNextgraph() == tempFocus)
			{
				if(temp->m_iNextConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// 跟随调整这条连线。
					CPoint tempPoint;
					temp->GetEndPoint(tempPoint);
					temp->SetEndPoint(tempPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewEnd = (CAdjustPoint*)tempFocus->m_Points.GetAt(temp->m_iNextConnPointIdx);
					if(pNewEnd)
					{
						temp->SetEndPoint(pNewEnd->GetPoint());
					}
				}
			}
		}
	}
}

/************************************************************************/
/* 功能：设置一个图元的前后图元对象。                                   */
/************************************************************************/
void CGraphManager::CheckLinkGraph(CGraph* graph)
{
	//if(graph == NULL && !graph->IsControlFlow()) return;

	for(int j = 0; j < GetGraphSum(); j++)
	{
		CGraph *temp = GetGraphAt(j);
		if(!temp->IsControlFlow())
		{
		    // TRACE("---------------------111111111111111---------------------------\r\n");
			graph->SetNextgraph(temp);
		    // TRACE("---------------------222222222222222---------------------------\r\n");
			graph->SetPreviousGraph(temp);
		    // TRACE("---------------------333333333333333---------------------------\r\n");
		}
	}
}

/************************************************************************/
/* 功能：检查所有图元的前后图元对象。                                   */
/************************************************************************/
void CGraphManager::CheckAllLinkGraph()
{
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph* graph = GetGraphAt(i);
	
		CheckLinkGraph(graph);
	}
}

/************************************************************************/
/* 功能：路径处理。                                                     */
/************************************************************************/
int CGraphManager::SearchPath()
{
	int i = 0;
	m_PathManager.DeleteAll();

	CObArray tempBorders, tempBs, copyAllBorders; // , copyAllBordersTemp;
	// 找到开始和结束图元
	CGraph *startGraph = NULL;
	CGraph *endGraph = NULL;
	for(i = 0; i < GetGraphSum(); i++)
	{
		CGraph *temp = GetGraphAt(i);

		if(temp->IsControlFlow())
		{
			copyAllBorders.Add(temp);
			// copyAllBordersTemp.Add(temp);
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

/************************************************************************/
/* 功能：路径标记。                                                     */
/************************************************************************/
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

/************************************************************************/
/* 功能：路径标记取消。                                                 */
/************************************************************************/
void CGraphManager::CancelMarkPath()
{
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph *temp = GetGraphAt(i);
		temp->SetIsMark(false);
	}
}


/*CGraph* CGraphManager::CreateGraph( int type )
{
	CGraph* objGraph = NULL;
	switch( type )
	{
		case Rectangle: 
			{
				objGraph = new CRectangle();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case Ellipse: 
			{
				objGraph = new CEllipse();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case Diamond: 
			{
				objGraph = new CDiamond();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case DealParallelogram: 
			{
				objGraph = new CDealParallelogram();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case GenericLine: 
			{
				objGraph = new CGenericLine();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case Arrowhead: 
			{
				objGraph = new CArrowhead();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}
		case ControlFlow: 
			{
				objGraph = new CControlFlow();
				m_Graphs.InsertAt(0, objGraph, 1);
				break;
			}			
	}

	m_FocusID = 0;
	return objGraph;
}
*/