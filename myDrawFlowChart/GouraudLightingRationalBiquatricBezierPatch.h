#if !defined(AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_)
#define AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Projection.h"//ͶӰ��
#include "Face.h"//ƽ��Ƭ��
#include"ZBuffer.h"
#include "LightingScene.h"

class CGouraudLightingRationalBiquatricBezierPatch//����˫����Bezier����Ƭ��
{
public:
	CGouraudLightingRationalBiquatricBezierPatch(void);
	virtual ~CGouraudLightingRationalBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//����9�����Ƶ�
	void ReadWeight(double W[3][3]);//����9��Ȩ����
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void SaveFacetData(void);//����˫����Bezier����Ƭ
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//��������
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//��˶������
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//�ҳ˶������
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//���Ȩ���Ӿ���
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//�ҳ�Ȩ���Ӿ���
	void TransposeMatrix(double M[3][3]);//ת�þ���
public:
	CColorP3 P[3][3];//��ά���Ƶ�
	CColorP3 V[100];//С�涥������10��10
	CFace F[81];//С���������
	double W[3][3];//Ȩ����
	CProjection projection;//ͶӰ
	CLightingScene* pScene;//����
};

#endif // !defined(AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_)
