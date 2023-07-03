#if !defined(AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Projection.h"//投影类
#include "ColorFace.h"//平面片类
#include"ZBuffer.h"
#include "LightingScene.h"

class CLightingBicubicBezierPatch
{
public:
	CLightingBicubicBezierPatch(void);
	virtual ~CLightingBicubicBezierPatch(void);
	void ReadControlPoint(CColorP3 P[4][4]);//读入16个控制点
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void SaveFacetData(void);//绘制网格点数据
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//绘制曲面
private:
	void LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4]);//左乘顶点矩阵
	void RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4]);//右乘顶点矩阵
	void TransposeMatrix(double M[4][4]);//转置矩阵
public:
	CColorP3 P[4][4];//三维控制点
	CColorP3 V[100];//小面顶点数组10×10
	CColorFace F[81];//小面面表数组
	CProjection projection;//投影对象
	CLightingScene* pScene;//光照
};

#endif // !defined(AFX_LIGHTING_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
