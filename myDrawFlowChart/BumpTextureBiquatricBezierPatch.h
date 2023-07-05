#if !defined(AFX_BUMP_TEXTURE_BIQUATRIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUMP_TEXTURE_BIQUATRIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//投影类
#include "Face.h"//平面片类
#include "BumpTextureZBuffer.h"
#include "LightingScene.h"
#include "Texture.h"

class CBumpTextureBiquatricBezierPatch//有理双二次Bezier曲面片类
{
public:
	CBumpTextureBiquatricBezierPatch(void);
	virtual ~CBumpTextureBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//读入9个控制点
	void ReadWeight(double W[3][3]);//读入9个权因子
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void SetTexture(CTexture* pTexture);
	void SaveFacetData(void);//绘制双二次Bezier曲面片
	void Draw(CDC* pDC, CBumpTextureZBuffer* pZBuffer);//绘制曲面
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//左乘顶点矩阵
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//右乘顶点矩阵
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//左乘权因子矩阵
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//右乘权因子矩阵
	void TransposeMatrix(double M[3][3]);//转置矩阵
public:
	CColorP3 P[3][3];//三维控制点
	CColorP3 V[100];//小面顶点数组10×10
	CFace F[81];//小面面表数组
	double W[3][3];//权因子
	CTextureCoordinate T[100];//顶点纹理
	CProjection projection;//投影
	CLightingScene* pScene;//光照
	CTexture* pTexture;//纹理
};

#endif // !defined(AFX_BUMP_TEXTURE_BIQUATRIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
