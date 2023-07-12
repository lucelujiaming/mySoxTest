#include "stdafx.h"
#include "GouraudBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CGouraudBiquatricBezierPatch::CGouraudBiquatricBezierPatch(void)
{

}


CGouraudBiquatricBezierPatch::~CGouraudBiquatricBezierPatch(void)
{

}

// 有理双二次Bezier曲面由两组二次Bezier曲线交织而成，
// 控制网格由9个控制点和9个权重因子组成。
void CGouraudBiquatricBezierPatch::ReadControlPoint(CColorP3 objBezierControlPoint[3][3])
{
   for(int i = 0; i < 3; i++)
   {
	   for(int j = 0;j < 3; j++)
	   {
   			this->m_objBezierControlPoint[i][j] = objBezierControlPoint[i][j];
	   }
   }
}

// 双二次Bezier曲面由两组二次Bezier曲线交织而成，
// 控制网格由9个控制点和9个权重因子组成。
void CGouraudBiquatricBezierPatch::ReadWeight(double objWeightFactors[3][3])
{
   for(int i = 0; i < 3; i++)
   {
	   for(int j = 0;j < 3; j++)
	   {
   			this->m_objWeightFactors[i][j] = objWeightFactors[i][j];
	   }
   }
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
/* 根据二次Bernstein基函数，也就是说：                                  */
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
/*                             |  1 -2  1 |                             */
/*   p(u, v) = [u^2 u^1 u^0] * | -2  2  0 | *                           */
/*                             |  1  0  0 |                             */ 
/*   | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   |  1 -2  1 |   | v^2 |   */
/*   | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | -2  2  0 | * | v   | / */
/*   | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   |  1  0  0 |   | 1   |   */
/*                   |  1 -2  1 |                                       */
/*   [u^2 u^1 u^0] * | -2  2  0 | *                                     */
/*                   |  1  0  0 |                                       */
/*   | w0,0 w0,1 w0,2 |     |  1 -2  1 |   | v^2 |                      */
/*   | w1,0 w1,1 w1,2 |  *  | -2  2  0 | * | v   |                      */
/*   | w2,0 w2,1 w2,2 |     |  1  0  0 |   | 1   |                      */
/* 注意这里的两个系数矩阵只是看起来一样。其实互为转置关系。             */
/* 下面的代码就是上面这个公式的计算过程。                               */
/************************************************************************/
void CGouraudBiquatricBezierPatch::SaveFacetData(void)
 {
	double M[3][3];//系数矩阵Mbe
	// 这里给出的是前面提到的二次Bernstein基函数的系数。
	// 这里给出的是B0,2(v)，B1,2(v)，B2,2(v)。其中：
	// B0,2(v) = (1 - v)^2 = v^2 - 2v + 1
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	// B1,2(v) = 2v(1 - v) = -2v^2 + 2v
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	// B2,2(v) = v^2       = v^2
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CColorP3 Pw[3][3];//曲线计算用控制点数组
	// 计算 | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |
	//      | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 |
	//      | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |
	//  结果保存在Pw中。
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			Pw[i][j] = m_objBezierControlPoint[i][j] * m_objWeightFactors[i][j];
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
	RightMultiplyMatrix(m_objWeightFactors, M);
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
	LeftMultiplyMatrix(M, m_objWeightFactors);
	int nStep = 9; // 曲面细分步长（10条线，9格）
	// 这六个临时变量用于保存u，v参数的幂
	double u0, u1, u2, v0, v1, v2;
	// 代码执行到这里，结果已经保存到Pw和W中了。
	// 下面使用Pw和W用两次循环，生成交叉曲面片的点。
	for (int u = 0; u < nStep + 1; u++)
	{
		for (int v = 0; v < nStep + 1; v++)
		{
			// 计算u的二次方，一次方和零次方。
			u2 = (u / double(nStep)) * (u / double(nStep)), u1 = u / double(nStep), u0 = 1;

			// 计算v的二次方，一次方和零次方。
			v2 = (v / double(nStep)) * (v / double(nStep)), v1 = v / double(nStep), v0 = 1;

			// 使用前面计算好的Pw，算出p(u, v)的分子。保存在numerator中。
			CColorP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
				               + (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
				               + (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			// 使用前面计算好的W，算出p(u, v)的的分母。保存在denominator中。
			double denominator = (u2 * m_objWeightFactors[0][0] + u1 * m_objWeightFactors[1][0] + u0 * m_objWeightFactors[2][0]) * v2
			                   + (u2 * m_objWeightFactors[0][1] + u1 * m_objWeightFactors[1][1] + u0 * m_objWeightFactors[2][1]) * v1
				               + (u2 * m_objWeightFactors[0][2] + u1 * m_objWeightFactors[1][2] + u0 * m_objWeightFactors[2][2]) * v0;
			// 使用前面计算好的分子和分母。计算p(u, v)的值。
			m_objBezierPatchPoint[u * (nStep + 1) + v] = numerator / denominator;
		}
	}
	/************************************************************************/
	/* 9×9个平面片面表信息，定义每个平面片的索引号。                        */
	/* 下面的代码巧妙的给出了每一个小面对应的四个顶点坐标索引。             */
	/* 只要手工列出这些点的索引就可以看明白代码了。100个顶点的索引如下：    */
	/*      0  1  2  3  4  5  6  7  8  9                                    */
	/*     10 11 12 13 14 15 16 17 18 19                                    */
	/*     20 21 22 23 24 25 26 27 28 29                                    */
	/*     30 31 32 33 34 35 36 37 38 39                                    */
	/*     40 41 42 43 44 45 46 47 48 49                                    */
	/*     50 51 52 53 54 55 56 57 58 59                                    */
	/*     60 61 62 63 64 65 66 67 68 69                                    */
	/*     70 71 72 73 74 75 76 77 78 79                                    */
	/*     80 81 82 83 84 85 86 87 88 89                                    */
	/*     90 91 92 93 94 95 96 97 98 99                                    */
	/* 以m_objBezierPatchFace[0]为例，对应的四个坐标恰好                    */
	/* 是(0, 1, 11, 10)，也就是上面的左上角的四个点。                       */
	/* 以m_objBezierPatchFace[80]为例，对应的四个坐标恰好                   */
	/* 是(88, 89, 99, 98)，也就是上面的左上角的四个点。                     */
	/************************************************************************/
	for (int nFacet = 0; nFacet < 81; nFacet++)
	{
		m_objBezierPatchFace[nFacet].SetPtNumber(4);		
		m_objBezierPatchFace[nFacet].ptIndex[0] = nFacet / nStep + nFacet;//0
		m_objBezierPatchFace[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;//1
		m_objBezierPatchFace[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;//11
		m_objBezierPatchFace[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;//10		
	}
}

void CGouraudBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CGouraudBiquatricBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
{
	SaveFacetData();//读取平面片的点表与面表
	CColorP3 colorEye = projection.GetColorEye();
	CColorP3 Point[4], ScreenPoint[4];//当前点与投影点
	// 曲面片划分为81个平面片
	for (int nFace = 0; nFace < 81; nFace++)
	{
		// 每个平面片为四边形
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{
			// 取出平面片四边形的一个顶点的坐标。
			Point[nPoint] = m_objBezierPatchPoint[m_objBezierPatchFace[nFace].ptIndex[nPoint]];
			// 把这个顶点使用三维透视投影投影到二维。用于显示。
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(
									m_objBezierPatchPoint[m_objBezierPatchFace[nFace].ptIndex[nPoint]]);
			// 调用光照函数计算这个顶点的颜色。
			ScreenPoint[nPoint].c = pScene->Illuminate(
					colorEye, Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);
		}
		// 每个平面片为一个四边形，我们把他分成两个三角形进行显示。
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); // 上三角形
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]); // 下三角形
		pZBuffer->FillTriangle(pDC);
	}
}

void CGouraudBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])//左乘矩阵M*P
{
	CColorP3 T[3][3];//临时矩阵
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

void CGouraudBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//左乘矩阵M*W
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

void CGouraudBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])//右乘矩阵P*M
{
	CColorP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
		}
	}
	for(i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			P[i][j] = T[i][j];
		}
	}
}

void CGouraudBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//右乘矩阵W*M
{
	double T[3][3];//临时矩阵
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

void CGouraudBiquatricBezierPatch::TransposeMatrix(double M[3][3])//转置矩阵
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