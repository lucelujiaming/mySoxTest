#if !defined(AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint2.h"//����ɫ�Ķ�ά��������
#include "ColorP2.h"//����ɫ�Ķ�ά������

class CTriangle
{
public:
	CTriangle(void);	
	virtual ~CTriangle(void);
	void SetPoint(CColorP2 P0, CColorP2 P1, CColorP2 P2);//�����㹹��������
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//��ɫ���Բ�ֵ
	void Fill(CDC* pDC);//���������
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
	CColorP2 P0, P1, P2;//�����εĸ�������
	CColorPoint2 P[3];//�����ε�������������
	CColorPoint2* SpanLeft; //��ȵ�����־����
	CColorPoint2* SpanRight;//��ȵ��յ��־����
	int nIndex;//ɨ��������
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_TRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
