#if !defined(AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//投影类
#include "ColorFace.h"//平面片类
#include "AntiAliasedBumpTextureZBuffer.h"
#include "LightingScene.h"

class CAntiAliasedBumpTextureBiquatricBezierPatch//有理双二次Bezier曲面片类
{
public:
	CAntiAliasedBumpTextureBiquatricBezierPatch(void);
	virtual ~CAntiAliasedBumpTextureBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//读入9个控制点
	void ReadWeight(double W[3][3]);//读入9个权因子
	void ReadPatchNumber(int nPatch);//读入面编号
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void SetTexture(CAntiAliasedBumpTexture* pTexture);//设置纹理
	void SaveFacetData(void);//绘制双二次Bezier曲面片
	void Draw(CDC* pDC, CAntiAliasedBumpTextureZBuffer* pZBuffer);//绘制曲面
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//左乘顶点矩阵
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//右乘顶点矩阵
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//左乘权因子矩阵
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//右乘权因子矩阵
	void TransposeMatrix(double M[3][3]);//转置矩阵
public:
	CColorP3 P[3][3];//三维控制点
	CColorP3 V[100];//小面顶点数组10×10
	CColorFace F[81];//小面面表数组
	double W[3][3];//权因子
	int nPatch;//面编号
	CTextureCoordinate T[100];//顶点纹理
	CProjection projection;//投影
	CLightingScene* pScene;//光照
	CAntiAliasedBumpTexture* pTexture;//纹理
};

#endif // !defined(AFX_ANTIALIASED_BUMP_TEXTURE_BIQUATRI_CBEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
