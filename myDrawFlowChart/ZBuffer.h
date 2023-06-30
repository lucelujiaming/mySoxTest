#if !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//����ɫ�Ķ�ά��������
#include "ColorVector3.h"

class CZBuffer//ZBuffer��
{
public:
	CZBuffer(void);
	virtual ~CZBuffer(void);
	void SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2);//�����㹹��������
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//��ʼ����Ȼ�����
	void FillTriangle(CDC* pDC);//���������
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1);//��ɫ���Բ�ֵ
private:
	CColorP3 P0, P1, P2;//�����εĸ�������
	CColorPoint3 point0, point1, point2;//�����ε�������������
	CColorPoint2* SpanLeft; //��ȵ���������־
	CColorPoint2* SpanRight;//��ȵ��յ������־
	int nIndex;//��¼ɨ��������
	double** zBuffer;//��Ȼ�����
	int nWidth, nHeight;//��������Ⱥ͸߶�
};

#endif // !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
