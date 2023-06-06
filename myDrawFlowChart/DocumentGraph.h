// DocumentGraph.h: interface for the CDocumentGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"
/************************************************************************/
/* ͨ���۲�Visio�д��·������ߵ��ĵ�ͼԪ���֣�                          */
/* ��ͼԪ�䳤��ʱ���·������ߵ������̶Ȳ���Ӱ�졣                     */
/* ���ǵ�ͼԪ��ñ�ƽ��ʱ���·������������Ȼά��ԭ�е������̶ȣ�     */
/* �ͻ���ɲ����ߺ�ͼԪ�����ཻ������״��������ϣ�Visio���˴���      */
/*                                                                      */
/* ʹ��Visio�ĵ������ܣ���������ͼԪ������svgʸ��ͼ��                   */
/* ͨ���Ķ�svg�����ű���֪�����ͼԪ���·���������һ��Բ����            */
/*                                                                      */
/* ����ͨ���Ķ�����svg�����ű����ܽᷢ�֣�                              */
/* 1. ��ͼԪ�����ڱ�ƽ״̬��ʱ��ͼԪ��ȵ���Բ���뾶��2.4�����ҡ�     */
/*    ����ϣ��߶�ΪH�����ΪW��ͼԪ��Բ���߶ȵļ��㹫ʽΪ��            */
/*          W/2.4 - sqrt((W/2.4) * (W/2.4) - (W/4) * (W/4))             */
/* 2. ��ͼԪ���ڱ�ƽ״̬��ʱ�����������������㷨��                  */
/*    Բ���߶Ⱦͻ����ͼԪ�߶ȡ�����ϣ���Ҫʹ��ͼԪ�߶ȷ���Բ���뾶��  */
/*    ���ڸ߶�ΪH�����ΪW��ͼԪ��Ϊ�˼���Բ���뾶R��                   */
/*    ������Բ���߶�ΪͼԪ�߶ȵ�һ�롣��������ļ��㣬�У�              */
/*          H/2 = ArcHeight = R - sqrt(R * R - (W/4) * (W/4))           */
/*     �����У�                                                         */
/*          R = H/4 + W * W/(16 * H)                                    */
/* 3. Ϊ���ж�ͼԪ�Ƿ�ƫƽ�����ȼ���Ĭ��Բ���߶ȡ�                      */
/*    ֮���ж����Բ���߶��Ƿ����ͼԪ�߶ȵ�һ�롣                      */
/*    ������������Ĭ��Բ���߶ȴ��ڸ߶ȵ�һ�룬��Ϊ��ƽͼԪ��          */
/*  �����Draw�Ļ����߼����������2.4��������                           */
/************************************************************************/
#define  MAGIC_DOCUMENT_RADIUS_RATIO                 2.4
// ����
#define CUSTOM_DOCUMENTGRAPH_TEXT_XBORDER            8    // X�������ֱ߿���
#define CUSTOM_DOCUMENTGRAPH_TEXT_YBORDER            8    // Y�������ֱ߿���


class CDocumentGraph : public CGraph  
{
	DECLARE_SERIAL(CDocumentGraph)

public:
	CDocumentGraph();
	virtual ~CDocumentGraph();
	CString GetTypeName() { return CString("CDocumentGraph"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	int  IsConnectOn(CAdjustPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;		// ���޸ĵĵ������š�

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
