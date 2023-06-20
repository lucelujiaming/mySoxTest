#if !defined(AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint.h"//����ɫ�Ķ�ά��������

class CColorTriangle//�����������
{
public:
	CColorTriangle(void);
	CColorTriangle(CColorPoint P0, CColorPoint P1, CColorPoint P2);//�����㹹��������
	virtual ~CColorTriangle(void);
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//��ɫ���Բ�ֵ
	void Fill(CDC* pDC);//���������
private:
	void EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
private:
	CColorPoint P[3];//����
	CColorPoint* SpanLeft; //��ȵ�����־����
	CColorPoint* SpanRight;//��ȵ��յ��־����
	int nIndex;//ɨ��������
};

#endif // !defined(AFX_COLORTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
