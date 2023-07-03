// BicubicBezierPatchGraph.h: interface for the CBicubicBezierPatchGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"
#include "BicubicBezierPatch.h"

class CBicubicBezierPatchGraph : public CGraph  
{
//		DECLARE_SERIAL(CBicubicBezierPatchGraph)
public:
	CBicubicBezierPatchGraph();
	virtual ~CBicubicBezierPatchGraph();
	CString GetTypeName() { return CString("CBicubicBezierPatchGraph"); }

public:
	void ReadPoint(void);               // ������ƶ���ζ����16�����Ƶ�
	void DrawObject(CDC* pDC);//����ͼ��
	
protected:
	// ˫����Bezier��������������Bezier���߽�֯���ɣ�
	// ����������16�����Ƶ���ɡ�
	CP3 P[4][4];//���Ƶ�
	CBicubicBezierPatch bezier;
public:
	void Draw( CDC *pdc, BOOL bShowSelectBorder = TRUE );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void SaveParamsToJSON(cJSON * objJSON);
	void LoadParamsFromJSON(cJSON * objJSON);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	int  IsConnectOn(CAdjustPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;
	
	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_BICUBIC_BEZIER_PATCH_GRAPH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
