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
/* ���ܣ����Ʋ���                                                       */
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
		// ����ÿһ��ͼ�ζ����Draw����
		GetGraphAt(i)->Draw(pdc);
		//if(GetGraphAt(i)->IsControlFlow())
			//CheckLinkGraph(GetGraphAt(i));
	}

	// ��õ�ǰѡ�е�ͼ�ζ���
	CGraph *focusGraph = GetFocusGraph();
	// �����ѡ�е�ͼ�ζ���
	if(focusGraph != NULL)
	{
		// ��Ҫ����ѡ�е�������л��ơ���������һ�����߿����ɫ���ӵ㡣
		focusGraph->Draw(pdc);
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
		resultGraph = (CGraph*)m_Graphs.GetAt( ID );
	}
	return resultGraph;
}

/************************************************************************/
/* ���ܣ����ͼ�ζ������                                               */
/************************************************************************/
int CGraphManager::GetGraphSum()
{
	return m_Graphs.GetSize();
}

/************************************************************************/
/* ���ܣ�����ɾ�������Ժ󴥷���ɾ����ǰѡ��ͼ�ζ���                   */
/************************************************************************/
void CGraphManager::DeleteFocusGraph()
{
	if((m_FocusID >= 0) && (m_FocusID < GetGraphSum()))
	{
		CGraph* tempFocus = GetGraphAt(m_FocusID);

		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph *objGraph = GetGraphAt(i);

			if(objGraph->GetPreviousGraph() == tempFocus || objGraph->GetNextgraph() == tempFocus)
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
		m_Graphs.RemoveAt( ID );
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
		objGraph = (CGraph*)m_Graphs.GetAt( m_FocusID );

		if(objGraph->IsControlFlow())
			CheckLinkGraph(objGraph);
	}
	return objGraph;
}

/************************************************************************/
/* ���ܣ����һ��ͼ�ζ���                                             */
/************************************************************************/
void CGraphManager::AddGraph(CGraph* graph)
{
	m_Graphs.InsertAt(0, graph, 1);
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
/* ���ܣ��Ƿ��ڵ���һ��ͼ�ζ����С��                                   */
/************************************************************************/
bool CGraphManager::IsAdjustSize( CPoint &pt )
{
	CGraph* tempFocus = GetFocusGraph();
	bool flag = false;
	
	if(tempFocus != NULL)
	{
		// ��ǰ�����Ƿ���ͼԪ�߽硣
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
/* ���ܣ��ƶ�һ��ͼԪ����                                             */
/************************************************************************/
void CGraphManager::Move(int cx, int cy)
{
	CGraph* tempFocus = GetFocusGraph();
	if(tempFocus != NULL)
	{
		tempFocus->Move(cx, cy);
		/*
		if(!tempFocus->IsControlFlow())
		{
		}
		*/
		for(int i = 0; i < GetGraphSum(); i++)
		{
			CGraph* objGraph = GetGraphAt(i);
			// �����ǰͼԪ�����ǰͼԪ�����ǵ�ǰѡ��ͼԪ����
			if(objGraph->GetPreviousGraph() == tempFocus)
			{
				if(objGraph->m_iPreviousConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// ��������������ߡ�
					CPoint tempPoint;
					objGraph->GetStartPoint(tempPoint);
					objGraph->SetStartPoint(tempPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewStart = (CAdjustPoint*)tempFocus->m_Points.GetAt(objGraph->m_iPreviousConnPointIdx);
					if(pNewStart)
					{
						objGraph->SetStartPoint(pNewStart->GetPoint());
					}
				}
			}
			// �����ǰͼԪ����ĺ�ͼԪ�����ǵ�ǰѡ��ͼԪ����
			if(objGraph->GetNextgraph() == tempFocus)
			{
				if(objGraph->m_iNextConnPointIdx == CCONNECTPOINT_INVALID_OPTION)
				{
					// ��������������ߡ�
					CPoint tempPoint;
					objGraph->GetEndPoint(tempPoint);
					objGraph->SetEndPoint(tempPoint + CPoint(cx, cy));
				}
				else
				{
					CAdjustPoint *pNewEnd = (CAdjustPoint*)tempFocus->m_Points.GetAt(objGraph->m_iNextConnPointIdx);
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
		CGraph* graph = GetGraphAt(i);
	
		CheckLinkGraph(graph);
	}
}

/************************************************************************/
/* ���ܣ�·������                                                     */
/************************************************************************/
int CGraphManager::SearchPath()
{
	int i = 0;
	m_PathManager.DeleteAll();

	CObArray tempBorders, tempBs, copyAllBorders; // , copyAllBordersTemp;
	// �ҵ���ʼ�ͽ���ͼԪ
	CGraph *startGraph = NULL;
	CGraph *endGraph = NULL;
	for(i = 0; i < GetGraphSum(); i++)
	{
		CGraph *objGraph = GetGraphAt(i);

		if(objGraph->IsControlFlow())
		{
			copyAllBorders.Add(objGraph);
			// copyAllBorders.Add(objGraph);
		}

		CString str;
		objGraph->GetText(str);
		if(str == "��ʼ")
		{
			startGraph = objGraph;
		}
		if(str == "����")
		{
			endGraph = objGraph;
		}
	}

	// �ҵ���һ���ߣ�����ʼ��·��������
	for(i = 0; i < copyAllBorders.GetSize(); i++)
	{
		CGraph *objGraph = (CGraph *)copyAllBorders.GetAt(i);
		if(objGraph->GetPreviousGraph() == startGraph)
		{
			m_PathManager.InitPaths(objGraph);
			//copyAllBordersTemp.RemoveAt(i);
			if(objGraph->GetNextgraph() != NULL && objGraph->GetNextgraph() != endGraph)
				tempBorders.Add(objGraph);
		}
	}
	if(tempBorders.GetSize() == 0 && m_PathManager.GetSum() == 0) return 0;
	//copyAllBorders.RemoveAll();
	//copyAllBorders.Append(copyAllBordersTemp);

	while(true)
	{
		for(int j = 0; j < tempBorders.GetSize(); j++)
		{
			CGraph *objGraph = (CGraph *)tempBorders.GetAt(j);
			bool flag = false;
			for(int k = 0; k < copyAllBorders.GetSize(); k++)
			{
				CGraph *tempK = (CGraph *)copyAllBorders.GetAt(k);
				if(tempK->GetPreviousGraph() == objGraph->GetNextgraph())
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
/* ���ܣ�·����ǡ�                                                     */
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
/* ���ܣ�·�����ȡ����                                                 */
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