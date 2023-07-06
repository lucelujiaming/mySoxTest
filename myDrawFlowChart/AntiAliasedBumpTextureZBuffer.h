#if !defined(AFX_ANTIALIASED_BUMP_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ANTIALIASED_BUMP_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint3.h"                // ����ɫ�Ķ�ά��������
#include "LightingScene.h"              // Blinn-Phongģ��
#include "AntiAliasedBumpTexture.h"     // ͼ������

class CAntiAliasedBumpTextureZBuffer//�����������
{
public:
    CAntiAliasedBumpTextureZBuffer(void);
    virtual ~CAntiAliasedBumpTextureZBuffer(void);
    void SetPoint(CColorP3* P, CVector3* N, 
                  CTextureCoordinate* T);            // �����㹹��������
    void ReadGradient(double** Bu, double** Bv);     // ����ͼ����ݶ�
    void InitialDepthBuffer(
		int nWidth, int nHeight, double zDepth);     // ��ʼ����Ȼ�����
    void FillTriangle(CDC* pDC, CColorP3 Eye, 
		CLightingScene* pScene, CRGB** DestImage);   // ���������
private:
    void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//�߱���㷨
    void SortPoint(void);//��������
    CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//��ʸ�����Բ�ֵ
    CTextureCoordinate Interp(double m, double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1);//�����ַ���Բ�ֵ
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
    CColorP3 P[3];					//�����εĸ�������
    CColorPoint3 point[3];			//�����ε������������ꡣ��P[3]�������������
    CColorPoint2* SpanLeft;			//��ȵ���������־
    CColorPoint2* SpanRight;		//��ȵ��յ������־
    int nIndex;						//��¼ɨ��������
    double** zBuffer;				//��Ȼ�����
    int nWidth, nHeight;			//��������Ⱥ͸߶�
    double** Bu, **Bv;				//ͼ���ݶ�
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
