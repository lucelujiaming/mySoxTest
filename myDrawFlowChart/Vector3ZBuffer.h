#if !defined(AFX_Vector3_ZBuffer_H_)
#define AFX_Vector3_ZBuffer_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//����ɫ�Ķ�ά��������
#include "LightingScene.h"//Blinn-Phongģ��

class CVector3ZBuffer//�����������
{
public:
	CVector3ZBuffer(void);
	virtual ~CVector3ZBuffer(void);
	void SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2, CVector3 N0, CVector3 N1, CVector3 N2);//�����㹹��������
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//��ʼ����Ȼ�����
	void FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene);//���������
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
	CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//��ʸ�����Բ�ֵ	
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
	CColorP3 P0, P1, P2;//�����εĸ�������
	CColorPoint3 point0, point1, point2;//�����ε�������������
	CColorPoint2* SpanLeft; //��ȵ���������־
	CColorPoint2* SpanRight;//��ȵ��յ������־
	int nIndex;//��¼ɨ��������
	double** zBuffer;//��Ȼ�����
	int nWidth, nHeight;//��������Ⱥ͸߶�
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_Vector3_ZBuffer_H_)
