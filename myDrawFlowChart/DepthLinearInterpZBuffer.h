#if !defined(AFX_DEPTHLINEAR_INTERP_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_DEPTHLINEAR_INTERP_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//����ɫ�Ķ�ά��������
#include "LightingScene.h"//Blinn-Phongģ��

class CDepthLinearInterpZBuffer//�����������
{
public:
	CDepthLinearInterpZBuffer(void);
	virtual ~CDepthLinearInterpZBuffer(void);
	void SetPoint(CColorP3* P, CVector3* N);//�����㹹��������
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//��ʼ����Ȼ�����
	void FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene);//���������
private:
	void EdgeFlag(CColorPoint3 PStart, CColorPoint3 PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
	CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//��ʸ�����Բ�ֵ
	double Interp(double m, double m0, double m1, double z0, double z1);//������Բ�ֵ
private:
	CColorP3 P[3];//�����εĸ�������
	CColorPoint3 point[3];//�����ε�������������
	CColorPoint3* SpanLeft; //��ȵ���������־
	CColorPoint3* SpanRight;//��ȵ��յ������־
	int nIndex;//��¼ɨ��������
	double** zBuffer;//��Ȼ�����
	int nWidth, nHeight;//��������Ⱥ͸߶�
};

#endif // !defined(AFX_DEPTHLINEAR_INTERP_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
