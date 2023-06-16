// MeshSphere.cpp: implementation of the CMeshSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "DrawFlowChart.h"
#include "MeshSphere.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PI 3.1415926
#define ROUND(d) int(d+0.5)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// IMPLEMENT_SERIAL(CMeshSphere, CObject, 1)

/************************************************************************/
/* ���ܣ�����������д������ʼ��ͽ����㡣���趨�����ӵ㡣               */
/************************************************************************/
CMeshSphere::CMeshSphere()
{
	m_text = "Cubic";
	m_Start = CPoint(100, 100);
	m_End = CPoint(200, 145);

	CAdjustPoint *connPoint = NULL; 
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
		connPoint = new CAdjustPoint();
		m_Points.push_back(connPoint);
	}

		// TODO: add construction code here
//	Alpha=0;
//	Beta=0;
	ReadPoint();
	ReadFacet();
	tran.SetMatrix(P,614);

}

CMeshSphere::~CMeshSphere()
{

}

void CMeshSphere::ReadPoint(void)
{
	int sAlpha=10,sBeta=10;//ά�ȷ����뾭�ȷ���ĵȷ���
	double sAlpha1,sBeta1,r=200;//rΪ��뾶
	N1=180/sAlpha,N2=360/sBeta;
	P[0].x=0;//���㱱��������
	P[0].y=r;
	P[0].z=0;
	for(int i=0;i<N1-1;i++)
	{ 
		sAlpha1=(i+1)*sAlpha*PI/180;
		for(int j=0;j<N2;j++)
		{
			sBeta1=j*sBeta*PI/180;
			P[i*N2+j+1].x=r*sin(sAlpha1)*sin(sBeta1);
			P[i*N2+j+1].y=r*cos(sAlpha1);
			P[i*N2+j+1].z=r*sin(sAlpha1)*cos(sBeta1);
		}
	}
	P[(N1-1)*N2+1].x=0;//�����ϼ�������
	P[(N1-1)*N2+1].y=-r;
	P[(N1-1)*N2+1].z=0;
}

void CMeshSphere::DoubleBuffer(CDC* pDC)//˫����
{
	CRect rect;//����ͻ�������
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMain->GetClientRect(&rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//����������Χ,x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//�ͻ�������Ϊԭ��
	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDC);//����һ������ʾpDC���ݵ��ڴ�memDC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г��ص���ʱλͼ 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=memDC.SelectObject(&NewBitmap);//������λͼѡ��memDC 
	memDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ
	memDC.SetMapMode(MM_ANISOTROPIC);//memDC�Զ�������ϵ
	memDC.SetWindowExt(rect.Width(),rect.Height());
	memDC.SetViewportExt(rect.Width(),-rect.Height());
	memDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	rect.OffsetRect(-rect.Width()/2,-rect.Height()/2);
	DrawGraph(&memDC, CPoint(0, 0));//��memDC����ͼ��
	pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�memDC�е�λͼ��������ʾpDC��
	memDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	memDC.DeleteDC();//ɾ��memDC
}

void CMeshSphere::DrawGraph(CDC* pDC, CPoint ptStart)//�����������߿�
{
	CPoint Point3[3];//�ϱ���������
	CPoint Point4[4];//���嶥����
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			if(3==F[i][j].pNumber)//��������������
			{
				for(int m=0;m<F[i][j].pNumber;m++)
				{
					Point3[m].x=ROUND(P[F[i][j].pIndex[m]].x);//����ͶӰ
					Point3[m].y=ROUND(P[F[i][j].pIndex[m]].y);
					Point3[m] += ptStart;
				}
				pDC->MoveTo(Point3[0].x,Point3[0].y);
				pDC->LineTo(Point3[1].x,Point3[1].y);
				pDC->LineTo(Point3[2].x,Point3[2].y);
				pDC->LineTo(Point3[0].x,Point3[0].y);
			}
			else//�����ı�������
			{
				for(int m=0;m<F[i][j].pNumber;m++)
				{
					Point4[m].x=ROUND(P[F[i][j].pIndex[m]].x);
					Point4[m].y=ROUND(P[F[i][j].pIndex[m]].y);
					Point4[m] += ptStart;
				}
				pDC->MoveTo(Point4[0].x,Point4[0].y);
				pDC->LineTo(Point4[1].x,Point4[1].y);
				pDC->LineTo(Point4[2].x,Point4[2].y);
				pDC->LineTo(Point4[3].x,Point4[3].y);
				pDC->LineTo(Point4[0].x,Point4[0].y);
			}
		}
	}
}

void CMeshSphere::ReadFacet(void)
{
	for(int i=0;i<N2;i++)//���챱����������
	{
		int tempi=i+1;
		if(N2==tempi)
			tempi=0;
		int NorthIndex[3];
		NorthIndex[0]=0;
		NorthIndex[1]=i+1;
		NorthIndex[2]=tempi+1;
		for(int k=0;k<3;k++)
		{
			F[0][i].pIndex[k]=NorthIndex[k];
			F[0][i].pNumber=3;
		}
	}
	for( i=1;i<N1-1;i++)//���������ı�������
	{
		for(int j=0;j<N2;j++)
		{
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2)
				tempj=0;
			int BodyIndex[4];
			BodyIndex[0]=(i-1)*N2+j+1;	
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			for(int k=0;k<4;k++)
			{
				F[i][j].pIndex[k]=BodyIndex[k];
				F[i][j].pNumber=4;
			}
		}
	}

	for(int j=0;j<N2;j++)//�����ϼ�����������
	{
		int tempj=j+1;
		if(N2==tempj)
			tempj=0;
		int SouthIndex[3];
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		for(int k=0;k<3;k++)
		{
			F[N1-1][j].pIndex[k]=SouthIndex[k];
			F[N1-1][j].pNumber=3;
		}
	}
}

/************************************************************************/
/* ���ܣ����ƺ�����������һ��Բ�Ǿ��κ���������֡�                     */
/************************************************************************/
void CMeshSphere::Draw(CDC *pdc, BOOL bShowSelectBorder)
{
	AdjustFocusPoint();

	CPen *oldPen, newPen;
	if(m_IsMark)
	{
		newPen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
		oldPen = pdc->SelectObject(&newPen);
	}

	// pdc->RoundRect(CRect(m_Start, m_End), CPoint(35, 35));
	CPoint ptMiddle = m_End - m_Start;
	ptMiddle.x /= 2;
	ptMiddle.y /= 2;
	ptMiddle += m_Start;
	DrawGraph(pdc, ptMiddle);

	if(m_IsMark)
		pdc->SelectObject(oldPen);

	pdc->DrawText(m_text, CRect(m_Start+CPoint(10, 15), m_End+CPoint(-8, -8)), DT_CENTER);
}

/************************************************************************/
/* ���ܣ�ѡ�л��ƺ��������������ӵ㡣                                   */
/************************************************************************/
void CMeshSphere::DrawFocus(CDC *pdc)
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < m_Points.size(); i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->Draw(pdc);
	}
}

/************************************************************************/
/* ���ܣ� �ƶ���������                                                */
/************************************************************************/
void CMeshSphere::Move(int cx, int cy)
{
	m_Start +=  CPoint(cx, cy);
	m_End +=  CPoint(cx, cy);
}

/************************************************************************/
/* ���ܣ��ж��Ƿ�ɱ༭��                                               */
/************************************************************************/
bool CMeshSphere::IsEditable()
{
	return false;
}

/************************************************************************/
/* ���ܣ��ж��Ƿ���ͼԪ�����ڡ�                                         */
/************************************************************************/
bool CMeshSphere::IsIn(CPoint &pt)
{
	bool flag = false;
	CRect rectStart = CRect( m_Start, m_End );
	if(rectStart.PtInRect( pt ))
	{
		flag = true;
	}
	return flag;
}

/************************************************************************/
/* ���ܣ� �ж��Ƿ���ͼԪ�߽��ϡ�                                        */
/************************************************************************/
int CMeshSphere::IsConnectOn(CAdjustPoint *pt)
{
	CAdjustPoint *connPoint = NULL;
	for(int i = 0; i < CCONNECTPOINT_RECT_MAX; i++)
	{
	    connPoint = (CAdjustPoint *)m_Points[i];
		if(connPoint->IsOn(pt->GetPoint()))
		{
			pt->SetPoint(connPoint->GetPoint());
		    return i;
		}
	}
	return CCONNECTPOINT_INVALID_OPTION;
}

/************************************************************************/
/* ���ܣ�������ʼ��ͽ���������������ӵ����ꡣ                         */
/************************************************************************/
void CMeshSphere::AdjustFocusPoint()
{
	CAdjustPoint *connPoint = NULL;
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_LEFT_TOP];
	connPoint->SetPoint(m_Start);
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_LEFT_BOTTOM];
	connPoint->SetPoint(CPoint(m_Start.x, m_End.y));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_RIGHT_TOP];
	connPoint->SetPoint(CPoint(m_End.x, m_Start.y));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_RIGHT_BOTTOM];
	connPoint->SetPoint(m_End);
	for(int i = 0; i < CCONNECTPOINT_RECT_CNT; i++)
	{
		connPoint = (CAdjustPoint *)m_Points[i];
		connPoint->SetType(false);
	}

	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_TOP];
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_Start.y ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_RIGHT];
	connPoint->SetPoint(CPoint( m_End.x, (m_Start.y+m_End.y)/2 ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_BOTTOM];
	connPoint->SetPoint(CPoint( (m_Start.x+m_End.x)/2, m_End.y ));
	connPoint = (CAdjustPoint *)m_Points[CCONNECTPOINT_RECT_MIDDLE_LEFT];
	connPoint->SetPoint(CPoint( m_Start.x, (m_Start.y+m_End.y)/2 ));
}

/************************************************************************/
/* ���ܣ����л�������                                                   */
/************************************************************************/
void CMeshSphere::SaveParamsToJSON(cJSON * objJSON)
{
//	if(ar.IsStoring())
//	{
//		ar<<m_Start<<m_End<<m_text;
//	}
//	else
//	{
//		ar>>m_Start>>m_End>>m_text;
//	}

	cJSON * jsonGraph = cJSON_CreateObject();
	cJSON_AddStringToObject(jsonGraph, "Type", GetTypeName());
	cJSON_AddNumberToObject(jsonGraph, "GraphSeq", getGraphSeq());
	
	cJSON * jsonStart = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonStart, "x", m_Start.x);
	cJSON_AddNumberToObject(jsonStart, "y", m_Start.y);
	cJSON_AddItemToObject(jsonGraph, "Start", jsonStart);

	cJSON * jsonEnd = cJSON_CreateObject();
	cJSON_AddNumberToObject(jsonEnd, "x", m_End.x);
	cJSON_AddNumberToObject(jsonEnd, "y", m_End.y);
	cJSON_AddItemToObject(jsonGraph, "End", jsonEnd);

	cJSON_AddStringToObject(jsonGraph, "Text", m_text);

	cJSON_AddItemToObject(objJSON, GetTypeName(), jsonGraph);
}

void CMeshSphere::LoadParamsFromJSON(cJSON * objJSON)
{
	cJSON *child = objJSON->child;
    while(child)
    {   
        switch ((child->type)&255)
        {   
        case cJSON_True:    
            TRACE("cJSON_True"); 
			break;
        case cJSON_Number:    
            {   
                if(strcmp(child->string, "GraphSeq") == 0)
                {   
					setGraphSeq((int)child->valueint);
                }
            }   
            break;
        case cJSON_String: 
            TRACE("cJSON_String\n"); 
            {   
                if(strcmp(child->string, "Text") == 0)
                {   
                    m_text = CString(child->valuestring);
                }
            }    
            break;
        case cJSON_Array:
            TRACE("cJSON_Array\n"); 
            break;
        case cJSON_Object:  
            TRACE("cJSON_Object\n"); 
            {   
                if(strcmp(child->string, "Start") == 0)
                {   
					cJSON *grandChild = child->child;
					while(grandChild)
					{
						switch ((grandChild->type)&255)
						{   
						case cJSON_Number:    
							if(strcmp(grandChild->string, "x") == 0)
							{   
								m_Start.x = (int)grandChild->valueint;
							}
							else if(strcmp(grandChild->string, "y") == 0)
							{   
								m_Start.y = (int)grandChild->valueint;
							}
							break;
						}
						grandChild = grandChild->next ;
					}
                }
				else if(strcmp(child->string, "End") == 0)
                {   
					cJSON *grandChild = child->child;
					while(grandChild)
					{
						switch ((grandChild->type)&255)
						{   
						case cJSON_Number:    
							if(strcmp(grandChild->string, "x") == 0)
							{   
								m_End.x = (int)grandChild->valueint;
							}
							else if(strcmp(grandChild->string, "y") == 0)
							{   
								m_End.y = (int)grandChild->valueint;
							}
							break;
						}
						grandChild = grandChild->next ;
					}
                }
            }   
            break;
        }   
        child = child->next ;
    }
}
