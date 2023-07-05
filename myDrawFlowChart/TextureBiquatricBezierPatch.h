#if !defined(AFX_TEXTURE_BIQUATRICBEZIER_PATCH_H_)
#define AFX_TEXTURE_BIQUATRICBEZIER_PATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//ͶӰ��
#include "TextureFace.h"//ƽ��Ƭ��
#include "TextureZBuffer.h"
#include "LightingScene.h"
#include "Texture.h"

class CTextureBiquatricBezierPatch//����˫����Bezier����Ƭ��
{
public:
	CTextureBiquatricBezierPatch(void);
	virtual ~CTextureBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//����9�����Ƶ�
	void ReadWeight(double W[3][3]);//����9��Ȩ����
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void SetTexture(CTexture* pTexture);//��������
	void SaveFacetData(void);//����˫����Bezier����Ƭ
	void Draw(CDC* pDC, CTextureZBuffer* pZBuffer);//��������
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//��˶������
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//�ҳ˶������
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//���Ȩ���Ӿ���
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//�ҳ�Ȩ���Ӿ���
	void TransposeMatrix(double M[3][3]);//ת�þ���
public:
	CColorP3 P[3][3];//��ά���Ƶ�
	CColorP3 V[100];//С�涥������10��10
	CTextureFace F[81];//С���������
	double W[3][3];//Ȩ����
	CTextureCoordinate T[100];//��������
	CProjection projection;//ͶӰ
	CLightingScene* pScene;//����
	CTexture* pTexture;//����
};

#endif // !defined(AFX_TEXTURE_BIQUATRICBEZIER_PATCH_H_)
