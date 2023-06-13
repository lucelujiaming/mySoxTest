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
/* ���ܣ����Ʋ���                                                       */
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
		// ����ÿһ��ͼ�ζ����Draw����
		GetGraphAt(i)->Draw(pdc, bShowSelectBorder);
		//if(GetGraphAt(i)->IsControlFlow())
			//CheckLinkGraph(GetGraphAt(i));
	}

	// ��õ�ǰѡ�е�ͼ�ζ���
	CGraph *focusGraph = GetFocusGraph();
	// �����ѡ�е�ͼ�ζ���
	if(focusGraph != NULL)
	{
		// ��Ҫ����ѡ�е�������л��ơ���������һ�����߿����ɫ���ӵ㡣
		focusGraph->Draw(pdc, bShowSelectBorder);
		focusGraph->DrawFocus(pdc);
	}
}

/************************************************************************/
/* ���ܣ�����ID���һ��ͼ�ζ���                                         */
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
/* ���ܣ����ͼ�ζ������                                               */
/************************************************************************/
int CGraphManager::GetGraphSum()
{
	return m_Graphs.size();
}

/************************************************************************/
/* ���ܣ�����ɾ�������Ժ󴥷���ɾ����ǰѡ��ͼ�ζ���                   */
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
/* ���ܣ���һ�������m_Graphs��ɾ����                                   */
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
/* ���ܣ���õ�ǰѡ�е�ͼ�ζ���                                       */
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
/* ���ܣ����һ��ͼ�ζ���                                             */
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
/* ���ܣ����һ��ͼ�ζ����Ժ󣬷�����ѡ�в�����                         */
/* ����ֵ��ѡ��ͼԪ�����仯�����棬���򷵻ؼ١�                         */
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
/* ���ܣ��Ƿ��ڵ���һ��ͼ�ζ����С��                                   */
/************************************************************************/
bool CGraphManager::IsAdjustSize( CPoint &pt )
{
	CGraph* focusGraph = GetFocusGraph();
	bool flag = false;
	
	if(focusGraph != NULL)
	{
		// ��ǰ�����Ƿ���ͼԪ�߽硣
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
/* ���ܣ��ƶ�һ��ͼԪ����                                             */
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
			// �����ǰͼԪ�����ǰͼԪ�����ǵ�ǰѡ��ͼԪ����
			if(objGraph->GetPreviousGraph() == focusGraph)
			{
				if(objGraph->m_iPreviousConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// ��������������ߡ�
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
			// �����ǰͼԪ����ĺ�ͼԪ�����ǵ�ǰѡ��ͼԪ����
			if(objGraph->GetNextgraph() == focusGraph)
			{
				if(objGraph->m_iNextConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// ��������������ߡ�
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
/* ���ܣ�����һ��ͼԪ��ǰ��ͼԪ����                                   */
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
/* ���ܣ��������ͼԪ��ǰ��ͼԪ����                                   */
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
/* ���ܣ�·������                                                     */
/************************************************************************/
int CGraphManager::SearchPath()
{
	int i = 0;
	m_PathManager.DeleteAll();

	std::vector<CGraph *> obArrayBorders, obArrayBs, copyAllBorders; // , copyAllBordersTemp;
	// �ҵ���ʼ�ͽ���ͼԪ
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
			if(str == "��ʼ")
			{
				startGraph = objGraph;
			}
			else if(str == "����")
			{
				endGraph = objGraph;
			}
		}
	}

	// �ҵ���һ���ߣ�����ʼ��·��������
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
/* ���ܣ�·����ǡ�                                                     */
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
/* ���ܣ�·�����ȡ����                                                 */
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