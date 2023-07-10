#include "stdafx.h"
#include "GouraudBicubicBezierPatch.h"
#define ROUND(d) int(d + 0.5)

CGouraudBicubicBezierPatch::CGouraudBicubicBezierPatch(void)
{

}

CGouraudBicubicBezierPatch::~CGouraudBicubicBezierPatch(void)
{

}

// 双三次Bezier曲面由两组三次Bezier曲线交织而成，
// 控制网格由16个控制点组成。
void CGouraudBicubicBezierPatch::ReadControlPoint(CColorP3 P[4][4])
{
   for(int i = 0;i < 4;i++)
   {
	   for(int j = 0;j < 4;j++)
	   {
   			this->P[i][j]  = P[i][j];
	   }
   }
}

void CGouraudBicubicBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

/************************************************************************/
/* 根据双三次曲面片的定义，展开式如下：                                 */
/*   p(u, v) = [B0,3(u) B1,3(u) B2,3(u) B3,3(u)]                        */
/*       | P0,0 P0,1 P0,2 P0,3 |   | B0,3(v) |                          */
/*     * | P1,0 P1,1 P1,2 P1,3 | * | B1,3(v) |    (u, v) = [0, 1]       */
/*       | P2,0 P2,1 P2,2 P2,3 |   | B2,3(v) |           * [0, 1]       */
/*       | P3,0 P3,1 P3,2 P3,3 |   | B3,3(v) |                          */
/* 其中Px,x是控制点。而B0,3(u)，B1,3(u)，B2,3(u)，B3,3(u)               */
/* 和B0,3(v)，B1,3(v)，B2,3(v)，B3,3(v)是三次Bernstein基函数。          */
/************************************************************************/
/* 根据三次Bernstein基函数，也就是说：                                  */
/*     B0,3(u) = (1 - u)^3                                              */
/*             = -u^3 + 3u^2 - 3u + 1                                   */
/*     B1,3(u) = 3u(1 - u)^2                                            */
/*             = 3u^3 - 6u^2 + 3u                                       */
/*     B2,3(u) = 3u^2 * (1 - u)                                         */
/*             = -3u^3 + 3u^2                                           */
/*     B3,3(u) = u^3                                                    */
/* 同理有：                                                             */
/*     B0,3(v) = (1 - v)^3                                              */
/*             = -v^3 + 3v^2 - 3v + 1                                   */
/*     B1,3(v) = 3v(1 - v)^2                                            */
/*             = 3v^3 - 6v^2 + 3v                                       */
/*     B2,3(v) = 3v^2 * (1 - v)                                         */
/*             = -3v^3 + 3v^2                                           */
/*     B3,3(v) = v^3                                                    */
/************************************************************************/
/* 因为我们绘制的时候，是使用u和v从0到1进行绘制。因此上，               */
/* 我们需要把u和v分离出来。于是我们把上面的展开式写成如下的形式。       */
/*   p(u, v) = [u^3 u^2 u^1 u^0] *                                      */
/*        | -1  3 -3  1 |   | P0,0 P0,1 P0,2 P0,3 |                     */
/*        |  3 -6  3  0 | * | P1,0 P1,1 P1,2 P1,3 | *                   */
/*        | -3  3  0  0 |   | P2,0 P2,1 P2,2 P2,3 |                     */
/*        |  1  0  0  0 |   | P3,0 P3,1 P3,2 P3,3 |                     */
/*        | -1  3 -3  1 |   | v^3 |                                     */
/*        |  3 -6  3  0 | * | v^2 |   (u, v) = [0, 1]                   */
/*        | -3  3  0  0 |   | v^1 |          * [0, 1]                   */
/*        |  1  0  0  0 |   | v^0 |                                     */
/* 注意这里的两个系数矩阵只是看起来一样。其实互为转置关系。             */
/* 下面的代码就是上面这个公式的计算过程。                               */
/* 注意的是，代码中，这个乘法运算是从右往左乘的。也就是先算右侧的乘法。 */
/************************************************************************/
void CGouraudBicubicBezierPatch::SaveFacetData(void)
{
	double M[4][4]; // 系数矩阵M
	// 这里给出的是这里给出的三次Bernstein基函数的系数。
	// 以B0,3(v)，B1,3(v)，B2,3(v)，B3,3(v)为例。其中：
	// B0,3(v) = -v^3 + 3v^2 - 3v + 1 
	M[0][0] = -1, M[0][1] = 3, M[0][2] = -3, M[0][3] = 1;
	// B1,3(v) = 3v^3 - 6v^2 + 3v 
	M[1][0] = 3,  M[1][1] = -6, M[1][2] = 3,  M[1][3] = 0;
	// B2,3(v) = -3v^3 + 3v^2 
	M[2][0] = -3, M[2][1] = 3,  M[2][2] = 0,  M[2][3] = 0;
	// B3,3(v) = v^3 
	M[3][0] = 1,  M[3][1] = 0,  M[3][2] = 0,  M[3][3] = 0;
	CColorP3 P3[4][4];//曲线计算用控制点数组
	// 复制一份控制点数组
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			P3[i][j] = P[i][j];
		}
	}
	//  系数矩阵右乘三维点矩阵
	//  计算 | P0,0 P0,1 P0,2 P0,3 |   | -1  3 -3  1 |
	//       | P1,0 P1,1 P1,2 P1,3 | * |  3 -6  3  0 |
	//       | P2,0 P2,1 P2,2 P2,3 |   | -3  3  0  0 |
	//       | P3,0 P3,1 P3,2 P3,3 |   |  1  0  0  0 |
	//  结果保存在P3中。
	RightMultiplyMatrix(P3, M);
	//  计算转置矩阵。得到：
	//     | -1  3 -3  1 |
	//     |  3 -6  3  0 |
	//     | -3  3  0  0 |
	//     |  1  0  0  0 |
	TransposeMatrix(M);
	//  系数矩阵左乘三维点矩阵
	//  计算：
	//    | -1  3 -3  1 |   | P0,0 P0,1 P0,2 P0,3 |
	//    |  3 -6  3  0 | * | P1,0 P1,1 P1,2 P1,3 |
	//    | -3  3  0  0 |   | P2,0 P2,1 P2,2 P2,3 |
	//    |  1  0  0  0 |   | P3,0 P3,1 P3,2 P3,3 |
	//  结果保存在P3中。
	LeftMultiplyMatrix(M, P3); 
	int nStep = 9; // 曲面细分的步长
	// 这六个临时变量用于保存u，v参数的幂
	double u0,u1,u2,u3,v0,v1,v2,v3; 
	// 代码执行到这里，结果已经保存到P3中了。
	// 下面使用P3用两次循环，绘制由两组双三次贝塞尔曲线交叉组成的双三次贝塞尔曲面片。
	// 下面这个两次循环，首先绘制上下方向的曲线。
	for (int u = 0; u <= nStep; u ++)
		for (int v = 0; v <= nStep; v ++)
		{
			// 计算u的三次方，二次方，一次方和零次方。
			u3 = (u / double(nStep)) * (u / double(nStep)) * (u / double(nStep));
			u2 = (u / double(nStep)) * (u / double(nStep));
			u1 = (u / double(nStep));
			u0 = 1;
			// 计算v的三次方，二次方，一次方和零次方。
			v3 = (v / double(nStep)) * (v / double(nStep)) * (v / double(nStep));
			v2 = (v / double(nStep)) * (v / double(nStep));
			v1 = (v / double(nStep));
			v0 = 1;
			// 使用前面计算好的P3，算出p(u, v)。保存在V中。
			V[u * (nStep + 1) + v] = (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
				+ (u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
				+ (u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
				+ (u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;
		}
		// 9×9个平面片面表信息，定义每个平面片的索引号
		for (int nFacet = 0; nFacet < 81; nFacet++)
		{
			F[nFacet].SetPtNumber(4);
			F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;
			F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;
			F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;
			F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;
		}
}

void CGouraudBicubicBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
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
			Point[nPoint] = V[F[nFace].ptIndex[nPoint]];
			// 把这个顶点使用三维透视投影投影到二维。用于显示。
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// 调用光照函数计算这个顶点的颜色。
			ScreenPoint[nPoint].c = pScene->Illuminate(colorEye, Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);
		}
		// 每个平面片为一个四边形，我们把他分成两个三角形进行显示。
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); // 上三角形
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]); // 下三角形
		pZBuffer->FillTriangle(pDC);
	}
}


void CGouraudBicubicBezierPatch::LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4])//左乘矩阵M*P
{
	CColorP3 PTemp [4][4];//临时矩阵
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j] + M[i][3] * P[3][j];
	for(i = 0;i < 4;i++)
		for(int j =0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}
void CGouraudBicubicBezierPatch::RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4])//右乘矩阵P*M
{
	CColorP3 PTemp [4][4];//临时矩阵
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for(i=0;i < 4;i++)
		for(int j=0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}

void CGouraudBicubicBezierPatch::TransposeMatrix(double M[4][4])//转置矩阵
{
	double PTemp[4][4];//临时矩阵
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp[j][i] = M[i][j];
	for(i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			M[i][j] = PTemp[i][j];
}