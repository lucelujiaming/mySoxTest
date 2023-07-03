#include "stdafx.h"
#include "RationalBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CRationalBiquatricBezierPatch::CRationalBiquatricBezierPatch(void)
{

}


CRationalBiquatricBezierPatch::~CRationalBiquatricBezierPatch(void)
{

}

void CRationalBiquatricBezierPatch::ReadControlPoint(CP3 P[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->P[i][j] = P[i][j];
}

void CRationalBiquatricBezierPatch::ReadWeight(double W[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->W[i][j] = W[i][j];
}
void CRationalBiquatricBezierPatch::DrawCurvedPatch(CDC* pDC)
 {
	double M[3][3];//系数矩阵Mbe
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CP3 Pw[3][3];//曲线计算用控制点数组
	for(int i = 0; i < 3; i++)
		for(int j = 0;j < 3;j++)
			Pw[i][j] = P[i][j] * W[i][j];
	RightMultiplyMatrix(Pw, M);//系数矩阵右乘三维点矩阵
	RightMultiplyMatrix(W, M);//系数矩阵右乘权因子矩阵
	TransposeMatrix(M);//计算转置矩阵
	LeftMultiplyMatrix(M, Pw);//系数矩阵左乘三维点矩阵
	LeftMultiplyMatrix(M, W);//系数矩阵左乘权因子矩阵
	double tStep = 0.1;//t的步长
	double u0, u1, u2, v0, v1, v2;//u，v参数的幂
	for(double u = 0; u <= 1; u += tStep)
		for(double v = 0; v <= 1; v += tStep)
		{
			u2 = u * u, u1 = u, u0 = 1;
			v2 = v * v, v1 = v, v0 = 1;
			CP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
			               +(u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
			               +(u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			double denominator = (u2 * W[0][0] + u1 * W[1][0] + u0 * W[2][0]) * v2
			                   + (u2 * W[0][1] + u1 * W[1][1] + u0 * W[2][1]) * v1
			                   + (u2 * W[0][2] + u1 * W[1][2] + u0 * W[2][2]) * v0;
			CP3 pt = numerator / denominator;
			CP2 Point2 = projection.PerspectiveProjection(pt);//投影
			if(v == 0)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}		  
	for(double v = 0; v <= 1;v += tStep)
		for(double u = 0;u <= 1;u += tStep)
		{
			u2 = u * u, u1 = u, u0 = 1; 
			v2 = v * v, v1 = v, v0 = 1;
			CP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
			              + (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
			              + (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			double denominator = (u2 * W[0][0] + u1 * W[1][0] + u0 * W[2][0]) * v2
			                    +(u2 * W[0][1] + u1 * W[1][1] + u0 * W[2][1]) * v1
			                    +(u2 * W[0][2] + u1 * W[1][2] + u0 * W[2][2]) * v0;
			CP3 pt = numerator / denominator;						
			CP2 Point2 = projection.PerspectiveProjection(pt);//投影    
			if(0 == u)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}
}

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CP3 P[3][3])//左乘矩阵M*P
{
	CP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];			
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//左乘矩阵M*W
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];		
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(CP3 P[3][3], double M[3][3])//右乘矩阵P*M
{
	CP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)	
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//右乘矩阵W*M
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::TransposeMatrix(double M[3][3])//转置矩阵
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[j][i] = M[i][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			M[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::DrawControlGrid(CDC* pDC)//绘制控制网格
{
	CP2 P2[3][3];//二维控制点
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			P2[i][j] = projection.PerspectiveProjection(P[i][j]);
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0, 0, 255));
	pOldPen=pDC->SelectObject(&NewPen);
	for(i = 0;i < 3;i++)
	{
		pDC->MoveTo(ROUND(P2[i][0].x), ROUND(P2[i][0].y));
		for(int j = 1;j < 3;j++)
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
	}
	for(int j = 0;j < 3;j++)
	{
		pDC->MoveTo(ROUND(P2[0][j].x), ROUND(P2[0][j].y));
		for(int i = 1;i < 3;i++)
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}