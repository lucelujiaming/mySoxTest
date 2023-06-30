#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P3.h"
#include"Projection.h"

class CRationalBiquatricBezierPatch//有理双二次Bezier曲面片类
{
public:
	CRationalBiquatricBezierPatch(void);
	virtual ~CRationalBiquatricBezierPatch(void);
	void ReadControlPoint(CP3 P[3][3]);//读入9个控制点
	void ReadWeight(double W[3][3]);//读入9个权因子
	void DrawCurvedPatch(CDC* pDC);//绘制双二次Bezier曲面片
	void DrawControlGrid(CDC *pDC);//绘制控制网格
private:
	void LeftMultiplyMatrix(double M[3][3],CP3 P[3][3]);//左乘顶点矩阵
	void RightMultiplyMatrix(CP3 P[3][3],double M[3][3]);//右乘顶点矩阵
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//左乘权因子矩阵
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//右乘权因子矩阵
	void TransposeMatrix(double M[3][3]);//转置矩阵
public:
	CP3 P[3][3];//三维控制点
	double W[3][3];//权因子
	CProjection projection;//投影
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

