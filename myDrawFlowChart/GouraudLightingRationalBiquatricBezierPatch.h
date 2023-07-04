#if !defined(AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_)
#define AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Projection.h"//投影类
#include "Face.h"//平面片类
#include"ZBuffer.h"
#include "LightingScene.h"

class CGouraudLightingRationalBiquatricBezierPatch//有理双二次Bezier曲面片类
{
public:
	CGouraudLightingRationalBiquatricBezierPatch(void);
	virtual ~CGouraudLightingRationalBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//读入9个控制点
	void ReadWeight(double W[3][3]);//读入9个权因子
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void SaveFacetData(void);//绘制双二次Bezier曲面片
	void Draw(CDC* pDC, CZBuffer* pZBuffer);//绘制曲面
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
	CProjection projection;//投影
	CLightingScene* pScene;//光照
};

#endif // !defined(AFX_GOURAUD_LIGHTING_RATIONAL_BIQUATRICBEZIER_PATCH_H_)
