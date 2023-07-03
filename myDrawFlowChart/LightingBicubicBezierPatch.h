#if !defined(AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Projection.h"//ͶӰ��
#include "ColorFace.h"//ƽ��Ƭ��
#include"ZBuffer.h"
#include "LightingScene.h"

class CLightingBicubicBezierPatch
{
public:
	CLightingBicubicBezierPatch(void);
	virtual ~CLightingBicubicBezierPatch(void);
	void ReadControlPoint(CColorP3 P[4][4]);//����16�����Ƶ�
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void SaveFacetData(void);//�������������
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//��������
private:
	void LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4]);//��˶������
	void RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4]);//�ҳ˶������
	void TransposeMatrix(double M[4][4]);//ת�þ���
public:
	CColorP3 P[4][4];//��ά���Ƶ�
	CColorP3 V[100];//С�涥������10��10
	CColorFace F[81];//С���������
	CProjection projection;//ͶӰ����
	CLightingScene* pScene;//����
};

#endif // !defined(AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
