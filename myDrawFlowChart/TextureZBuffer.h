#if !defined(AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint3.h"//����ɫ����ά��������
#include "LightingScene.h"//Blinn-Phongģ��
#include "Texture.h"//ͼ������
#include "TextureCoordinate.h"//ͼ������

class CTextureZBuffer//�����������
{
public:
	CTextureZBuffer(void);
	virtual ~CTextureZBuffer(void);
	void SetPoint(CColorP3* P, CVector3* N, CTextureCoordinate* T);//�����㹹��������
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//��ʼ����Ȼ�����
	void FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene, CTexture* pTexture);//���������
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
	CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//��ʸ�����Բ�ֵ
	CTextureCoordinate Interp(double m, double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1);//�����ַ���Բ�ֵ
	CRGB GetTexture(int u, int v, CTexture* pTexture);//��ȡ����
private:
	CColorP3 P[3];//�����εĸ�������
	CColorPoint3 point[3];//�����ε�������������
	CColorPoint2* SpanLeft; //��ȵ���������־
	CColorPoint2* SpanRight;//��ȵ��յ������־
	int nIndex;//��¼ɨ��������
	double** zBuffer;//��Ȼ�����
	int nWidth, nHeight;//��������Ⱥ͸߶�
};

#endif // !defined(AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
