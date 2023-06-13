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
#include "ArrowLine.h"
#include "BrokenLine.h"

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
void CGraphManager::DrawAll( CDC *pdc, BOOL bShowSelectBorder )
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
		GetGraphAt(i)->Draw(pdc, bShowSelectBorder);
		//if(GetGraphAt(i)->IsControlFlow())
			//CheckLinkGraph(GetGraphAt(i));
	}

	// 获得当前选中的图形对象
	CGraph *focusGraph = GetFocusGraph();
	// 如果有选中的图形对象，
	if(focusGraph != NULL)
	{
		// 还要按照选中的情况进行绘制。包括绘制一个虚线框和绿色连接点。
		focusGraph->Draw(pdc, bShowSelectBorder);
		focusGraph->DrawFocus(pdc);
	}
}

/************************************************************************/
/* 功能：根据ID获得一个图形对象                                         */
/************************************************************************/
CGraph* CGraphManager::GetGraphAt( int ID )
{
	CGraph* resultGraph = NULL;
	if((ID >= 0) && (ID < GetGraphSum()))
	{
		resultGraph = (CGraph*)m_Graphs[ID];
	}
	return resultGraph;
}

/************************************************************************/
/* 功能：获得图形对象个数                                               */
/************************************************************************/
int CGraphManager::GetGraphSum()
{
	return m_Graphs.size();
}

/************************************************************************/
/* 功能：按下删除按键以后触发，删除当前选中图形对象。                   */
/************************************************************************/
void CGraphManager::DeleteFocusGraph()
{
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		CGraph* focusGraph = GetGraphAt(m_FocusID);

		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph *objGraph = GetGraphAt(i);

			if(objGraph->GetPreviousGraph() == focusGraph || objGraph->GetNextgraph() == focusGraph)
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
		//CGraph* objGraph = (CGraph*)m_Graphs.GetAt(ID);
		//if(objGraph != NULL)
		//	delete objGraph;
		m_Graphs.erase( m_Graphs.begin() + ID );
	}
}

/************************************************************************/
/* 功能：获得当前选中的图形对象。                                       */
/************************************************************************/
CGraph* CGraphManager::GetFocusGraph()
{
	CGraph* objGraph = NULL;
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		objGraph = (CGraph*)m_Graphs[m_FocusID];

		if(objGraph->IsControlFlow())
			CheckLinkGraph(objGraph);
	}
	return objGraph;
}

/************************************************************************/
/* 功能：添加一个图形对象。                                             */
/************************************************************************/
void CGraphManager::AddGraph(CGraph* graph, cJSON * jsonParam)
{
	if(jsonParam)
	{
		graph->LoadParamsFromJSON(jsonParam);
	}
	m_Graphs.insert(m_Graphs.begin(), graph);
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
				m_Graphs.insert(m_Graphs.begin(), objGraph);
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
	CGraph* focusGraph = GetFocusGraph();
	bool flag = false;
	
	if(focusGraph != NULL)
	{
		// 当前坐标是否在图元边界。
		if(focusGraph->IsOn( pt ))
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
	CGraph* focusGraph = GetFocusGraph();
	if(focusGraph != NULL)
	{
		focusGraph->Move(cx, cy);
		/*
		if(!focusGraph->IsControlFlow())
		{
		}
		*/
		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph* objGraph = GetGraphAt(i);
			// 如果当前图元对象的前图元对象是当前选中图元对象。
			if(objGraph->GetPreviousGraph() == focusGraph)
			{
				if(objGraph->m_iPreviousConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// 跟随调整这条连线。
					CPoint newStartPoint;
					objGraph->GetStartPoint(newStartPoint);
					objGraph->SetStartPoint(newStartPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewStart = (CAdjustPoint*)focusGraph->m_Points[objGraph->m_iPreviousConnPointIdx];
					if(pNewStart)
					{
						objGraph->SetStartPoint(pNewStart->GetPoint());
					}
				}
			}
			// 如果当前图元对象的后图元对象是当前选中图元对象。
			if(objGraph->GetNextgraph() == focusGraph)
			{
				if(objGraph->m_iNextConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// 跟随调整这条连线。
					CPoint newEndPoint;
					objGraph->GetEndPoint(newEndPoint);
					objGraph->SetEndPoint(newEndPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewEnd = (CAdjustPoint*)focusGraph->m_Points[objGraph->m_iNextConnPointIdx];
					if(pNewEnd)
					{
						objGraph->SetEndPoint(pNewEnd->GetPoint());
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
		CGraph *objGraph = GetGraphAt(j);
		if(!objGraph->IsControlFlow())
		{
		    // TRACE("---------------------111111111111111---------------------------\r\n");
			graph->SetNextgraph(objGraph);
		    // TRACE("---------------------222222222222222---------------------------\r\n");
			graph->SetPreviousGraph(objGraph);
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
		CGraph* objGraph = GetGraphAt(i);
		if(objGraph && objGraph->IsControlFlow())
		{	
			// SetPreviousGraph 
			if(objGraph->m_iPreviousGraphSeq > 0)
			{
				for(int i = 0; i < GetGraphSum(); i++)
				{
					CGraph* graph = GetGraphAt(i);
					if(graph && graph->getGraphSeq() == objGraph->m_iPreviousGraphSeq)
					{	
						objGraph->loadPreviousGraph(graph);
						break;
					}
				}
			}
			// SetNextgraph 
			if(objGraph->m_iNextGraphSeq > 0)
			{
				for(int i = 0; i < GetGraphSum(); i++)
				{
					CGraph* graph = GetGraphAt(i);
					if(graph && graph->getGraphSeq() == objGraph->m_iNextGraphSeq)
					{	
						objGraph->loadNextgraph(graph);
						break;
					}
				}
			}
		}
	}
}

void CGraphManager::SaveToJSON(cJSON * objJSON)
{
	int iGraphSeq = 0;
	cJSON_AddNumberToObject(objJSON, "FocusID", m_FocusID);

	std::vector<CGraph *>::iterator iter;
	// Set the GraphSeq to all of Graphs
	for (iter = m_Graphs.begin(); iter != m_Graphs.end(); iter++)
	{
		CGraph * objGraph  = *iter;
		iGraphSeq++;
		objGraph->setGraphSeq(iGraphSeq);
	}
	// Serialize all of Graphs
	for (iter = m_Graphs.begin(); iter != m_Graphs.end(); iter++)
	{
		CGraph * objGraph  = *iter;
		objGraph->SaveParamsToJSON(objJSON);
	}
}

void CGraphManager::LoadFromJSON(cJSON * objJSON)
{
	return ;
}

/************************************************************************/
/* 功能：路径处理。                                                     */
/************************************************************************/
int CGraphManager::SearchPath()
{
	int i = 0;
	m_PathManager.DeleteAll();

	std::vector<CGraph *> obArrayBorders, obArrayBs, copyAllBorders; // , copyAllBordersTemp;
	// 找到开始和结束图元
	CGraph *startGraph = NULL;
	CGraph *endGraph = NULL;
	for(i = 0; i < GetGraphSum(); i++)
	{
		CGraph *objGraph = GetGraphAt(i);

		if(objGraph->IsControlFlow())
		{
			copyAllBorders.push_back(objGraph);
			// copyAllBorders.Add(objGraph);
		}
		else
		{
			CString str;
			objGraph->GetText(str);
			if(str == "开始")
			{
				startGraph = objGraph;
			}
			else if(str == "结束")
			{
				endGraph = objGraph;
			}
		}
	}

	// 找到第一条边，并初始化路径管理器
	for(i = 0; i < copyAllBorders.size(); i++)
	{
		CGraph *objGraph = (CGraph *)copyAllBorders[i];
		if(objGraph->GetPreviousGraph() == startGraph)
		{
			m_PathManager.InitPaths(objGraph);
			//copyAllBordersTemp.RemoveAt(i);
			if(objGraph->GetNextgraph() != NULL && objGraph->GetNextgraph() != endGraph)
				obArrayBorders.push_back(objGraph);
		}
	}
	if(obArrayBorders.size() == 0 && m_PathManager.GetSum() == 0) 
		return 0;
	//copyAllBorders.RemoveAll();
	//copyAllBorders.Append(copyAllBordersTemp);

	while(true)
	{
		for(int j = 0; j < obArrayBorders.size(); j++)
		{
			CGraph *objGraph = (CGraph *)obArrayBorders[j];
			bool flag = false;
			for(int k = 0; k < copyAllBorders.size(); k++)
			{
				CGraph *currentGraph = (CGraph *)copyAllBorders[k];
				if(currentGraph->GetPreviousGraph() == objGraph->GetNextgraph())
				{
					if(!m_PathManager.UpdatePaths(currentGraph)) 
					{
						flag = true;
						break;
					}
					if(currentGraph->GetNextgraph() != NULL && currentGraph->GetNextgraph() != endGraph)
						obArrayBs.push_back(currentGraph);
				}
			}
			//if(flag) continue;
		}

		if(obArrayBs.size() == 0) break;

		obArrayBorders.clear();	
		obArrayBorders.insert(obArrayBorders.end(), obArrayBs.begin(), obArrayBs.end());
		obArrayBs.clear();
	}

	return m_PathManager.GetSum();
}

/************************************************************************/
/* 功能：路径标记。                                                     */
/************************************************************************/
void CGraphManager::MarkPath()
{
	CancelMarkPath();

	CPath *currentPath;
	currentPath = m_PathManager.GetPath();
	if(currentPath != NULL)
	{
		currentPath->MarkPath();
	}
}

/************************************************************************/
/* 功能：路径标记取消。                                                 */
/************************************************************************/
void CGraphManager::CancelMarkPath()
{
	for(int i = 0; i < GetGraphSum(); i++)
	{
		CGraph *currentPath = GetGraphAt(i);
		currentPath->SetIsMark(false);
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
		case ArrowLine: 
			{
				objGraph = new CArrowLine();
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