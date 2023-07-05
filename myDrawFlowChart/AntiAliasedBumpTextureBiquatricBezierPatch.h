#if !defined(AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//ͶӰ��
#include "ColorFace.h"//ƽ��Ƭ��
#include "AntiAliasedBumpTextureZBuffer.h"
#include "LightingScene.h"

class CAntiAliasedBumpTextureBiquatricBezierPatch//����˫����Bezier����Ƭ��
{
public:
	CAntiAliasedBumpTextureBiquatricBezierPatch(void);
	virtual ~CAntiAliasedBumpTextureBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//����9�����Ƶ�
	void ReadWeight(double W[3][3]);//����9��Ȩ����
	void ReadPatchNumber(int nPatch);//��������
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void SetTexture(CAntiAliasedBumpTexture* pTexture);//��������
	void SaveFacetData(void);//����˫����Bezier����Ƭ
	void Draw(CDC* pDC, CAntiAliasedBumpTextureZBuffer* pZBuffer);//��������
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//��˶������
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//�ҳ˶������
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//���Ȩ���Ӿ���
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//�ҳ�Ȩ���Ӿ���
	void TransposeMatrix(double M[3][3]);//ת�þ���
public:
	CColorP3 P[3][3];//��ά���Ƶ�
	CColorP3 V[100];//С�涥������10��10
	CColorFace F[81];//С���������
	double W[3][3];//Ȩ����
	int nPatch;//����
	CTextureCoordinate T[100];//��������
	CProjection projection;//ͶӰ
	CLightingScene* pScene;//����
	CAntiAliasedBumpTexture* pTexture;//����
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
