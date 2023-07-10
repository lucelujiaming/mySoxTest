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

/************************************************************************/
/* 有理双二次Bezier曲面片的定义如下：                                   */
/*   p(u, v) = [B0,2(u) B1,2(u) B2,2(u)]                                */
/*       | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |          */
/*     * | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |  /       */
/*       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |          */
/*                                | w0,0 w0,1 w0,2 |   | B0,2(v) |      */
/*    [B0,2(u) B1,2(u) B2,2(u)] * | w1,0 w1,1 w1,2 | * | B1,2(v) |      */
/*                                | w2,0 w2,1 w2,2 |   | B2,2(v) |      */
/* 其中Px,x是控制点。wij是控制顶点的权因子。而B0,2(u)，B1,2(u)，B2,2(u) */
/* 和B0,2(v)，B1,2(v)，B2,2(v)是二次Bernstein基函数。                   */
/************************************************************************/
/* 根据二次Bernstein基函数，也就是说：                                                           */
/*     B0,2(u) = (1 - u)^2 = u^2 - 2u + 1                               */
/*     B1,2(u) = 2u(1 - u) = -2u^2 + 2u                                 */
/*     B2,2(u) = u^2       = u^2                                        */
/* 同理有：                                                             */
/*     B0,2(v) = (1 - v)^2 = v^2 - 2v + 1                               */
/*     B1,2(v) = 2v(1 - v) = -2v^2 + 2v                                 */
/*     B2,2(v) = v^2       = v^2                                        */
/************************************************************************/
/* 因为我们绘制的时候，是使用u和v从0到1进行绘制。因此上，               */
/* 我们需要把u和v分离出来。于是我们把上面的展开式写成如下的形式。       */
/*   p(u, v) = [u^2 u^1 u^0] *                                          */
/*        |  1 -2  1 |   | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |        */
/*        | -2  2  0 | * | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | *      */
/*        |  1  0  0 |   | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |        */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/*                          */
/* 下面的代码就是使用B0,2(u)，B1,2(u)，B2,2(u)                          */
/* 和B0,2(v)，B1,2(v)，B2,2(v)计算p(u, v)的过程。                       */
/************************************************************************/
void CRationalBiquatricBezierPatch::DrawCurvedPatch(CDC* pDC)
 {
	double M[3][3]; // 系数矩阵Mbe
	// 这里给出的是B0,2(v)，B1,2(v)，B2,2(v)。其中：
	// B0,2(v) = (1 - v)^2 = v^2 - 2v + 1
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	// B1,2(v) = 2v(1 - v) = -2v^2 + 2v
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	// B2,2(v) = v^2       = v^2
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CP3 Pw[3][3]; // 曲线计算用控制点数组
	// 计算 | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |
	//      | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 |
	//      | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |
	//  结果保存在Pw中。
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			Pw[i][j] = P[i][j] * W[i][j];
		}
	}
	//  系数矩阵右乘三维点矩阵
	//  计算 | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |
	//       | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |
	//       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |
	//  结果保存在Pw中。
	RightMultiplyMatrix(Pw, M);
	// 系数矩阵右乘权因子矩阵
	//  计算  | w0,0 w0,1 w0,2 |   | B0,2(v) |
	//        | w1,0 w1,1 w1,2 | * | B1,2(v) |
	//        | w2,0 w2,1 w2,2 |   | B2,2(v) |
	//  结果保存在W中。
	RightMultiplyMatrix(W, M);
	//  计算转置矩阵。得到：
	//     [B0,2(u)，B1,2(u)，B2,2(u)]
	TransposeMatrix(M);//计算转置矩阵
	// 系数矩阵左乘三维点矩阵
	//  计算 [B0,2(u) B1,2(u) B2,2(u)] *
	//       | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |
	//       | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |
	//       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |
	//  结果保存在Pw中。
	LeftMultiplyMatrix(M, Pw);
	// 系数矩阵左乘权因子矩阵
	//  计算 [B0,2(u) B1,2(u) B2,2(u)] *
	//        | w0,0 w0,1 w0,2 |   | B0,2(v) |
	//        | w1,0 w1,1 w1,2 | * | B1,2(v) |
	//        | w2,0 w2,1 w2,2 |   | B2,2(v) |
	//  结果保存在W中。
	LeftMultiplyMatrix(M, W);
	double tStep = 0.1;//t的步长
	double u0, u1, u2, v0, v1, v2;//u，v参数的幂
	// 代码执行到这里，结果已经保存到Pw和W中了。
	// 下面使用Pw和W用两次循环，绘制交叉曲面片。
	for(double u = 0; u <= 1; u += tStep)
	{
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
			// 使用P3算出三维坐标。
			CP3 pt = numerator / denominator;
			// 投影到二维。用于显示。
			CP2 Point2 = projection.PerspectiveProjection(pt);//投影
			// 绘制网格。
			if(v == 0)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}
	}
	for(double v = 0; v <= 1;v += tStep)
	{
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
			// 使用P3算出三维坐标。
			CP3 pt = numerator / denominator;
			// 投影到二维。用于显示。
			CP2 Point2 = projection.PerspectiveProjection(pt);//投影
			// 绘制网格。
			if(0 == u)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}
	}
}

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CP3 P[3][3])//左乘矩阵M*P
{
	CP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			P[i][j] = T[i][j];
		}
	}
}

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//左乘矩阵M*W
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];
		}
	}
	for(i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			W[i][j] = T[i][j];
		}
	}
}

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(CP3 P[3][3], double M[3][3])//右乘矩阵P*M
{
	CP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
		}
	}
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//右乘矩阵W*M
{
	double T[3][3]; // 临时矩阵
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			W[i][j] = T[i][j];
		}
	}
}

void CRationalBiquatricBezierPatch::TransposeMatrix(double M[3][3])//转置矩阵
{
	double T[3][3]; // 临时矩阵
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[j][i] = M[i][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			M[i][j] = T[i][j];
		}
	}
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
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	for(int j = 0;j < 3;j++)
	{
		pDC->MoveTo(ROUND(P2[0][j].x), ROUND(P2[0][j].y));
		for(int i = 1;i < 3;i++)
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}