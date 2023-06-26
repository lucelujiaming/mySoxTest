// MidPointDividLine.h: interface for the CMidPointDividLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDPOINT_DIVID_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_MIDPOINT_DIVID_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "AdjustPoint.h"

class CEnhanceAreaPoint //��������Ķ�ά����
{
public:
	CEnhanceAreaPoint(void) {
		x = 0.0;
		y = 0.0;
		areaCode = 0;
	}
	CEnhanceAreaPoint(double x, double y) {
		this->x = x;
		this->y = y;
	}

	friend CEnhanceAreaPoint operator +(
		const CEnhanceAreaPoint &p0, 
		const CEnhanceAreaPoint &p1)//��
	{
		CEnhanceAreaPoint pm;
		pm.x = p0.x + p1.x;
		pm.y = p0.y + p1.y;
		return pm;
	}

	friend CEnhanceAreaPoint operator /(
		const CEnhanceAreaPoint &p, double sarlar)//��ͳ�������
	{
		return CEnhanceAreaPoint(p.x / sarlar, p.y / sarlar);
	}

	virtual ~CEnhanceAreaPoint(void) {}
public:
	double x;
	double y;
	UINT areaCode;
};

class CMidPointDividLine : public CGraph  
{
//		DECLARE_SERIAL(CMidPointDividLine)
public:
	CMidPointDividLine();
	virtual ~CMidPointDividLine();
	CString GetTypeName() { return CString("CMidPointDividLine"); }

public:
	void MidPointDivider(void);//�е�ָ��㷨����
	void Encode(CEnhanceAreaPoint &pt);//�˵���뺯��
protected:
	// int Wxl, Wxr, Wyt, Wyb;
	CEnhanceAreaPoint P0, P1;//ֱ�ߵ������յ�
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

#endif // !defined(AFX_MIDPOINT_DIVID_LINE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
