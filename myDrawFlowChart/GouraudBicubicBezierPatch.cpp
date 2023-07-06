#include "stdafx.h"
#include "GouraudBicubicBezierPatch.h"
#define ROUND(d) int(d + 0.5)

CGouraudBicubicBezierPatch::CGouraudBicubicBezierPatch(void)
{

}

CGouraudBicubicBezierPatch::~CGouraudBicubicBezierPatch(void)
{

}

void CGouraudBicubicBezierPatch::ReadControlPoint(CColorP3 P[4][4])
{
   for(int i = 0;i < 4;i++)
	   for(int j = 0;j < 4;j++)
   			this->P[i][j]  = P[i][j];
}

void CGouraudBicubicBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CGouraudBicubicBezierPatch::SaveFacetData(void)
{
	double M[4][4];//系数矩阵M
	M[0][0] = -1, M[0][1] = 3, M[0][2] = -3, M[0][3] = 1;
	M[1][0] = 3, M[1][1] = -6, M[1][2] = 3, M[1][3] = 0;
	M[2][0] = -3, M[2][1] = 3, M[2][2] = 0, M[2][3] = 0;
	M[3][0] = 1, M[3][1] = 0, M[3][2] = 0, M[3][3] = 0;
	CColorP3 P3[4][4];//曲线计算用控制点数组
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			P3[i][j] = P[i][j];
	RightMultiplyMatrix(P3, M);//系数矩阵右乘三维点矩阵
	TransposeMatrix(M);//计算转置矩阵
	LeftMultiplyMatrix(M, P3);//系数矩阵左乘三维点矩阵
	int nStep = 9;//曲面细分步长
	double u0, u1, u2, u3, v0, v1, v2, v3;//u，v参数的幂
	for (int u = 0; u <= nStep; u ++)
		for (int v = 0; v <= nStep; v ++)
		{
			u3 = (u / double(nStep)) * (u / double(nStep)) * (u / double(nStep));
			u2 = (u / double(nStep)) * (u / double(nStep));
			u1 = (u / double(nStep));
			u0 = 1;
			v3 = (v / double(nStep)) * (v / double(nStep)) * (v / double(nStep));
			v2 = (v / double(nStep)) * (v / double(nStep));
			v1 = (v / double(nStep));
			v0 = 1;
			V[u * (nStep + 1) + v] = (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
				+ (u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
				+ (u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
				+ (u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;
		}
		for (int nFacet = 0; nFacet < 81; nFacet++)//9×9个平面片面表信息，定义每个平面片的索引号
		{
			F[nFacet].SetPtNumber(4);
			F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;
			F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;
			F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;
			F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;
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

void CGouraudBicubicBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
{
	SaveFacetData();//读取平面片的点表与面表
	CColorP3 Eye = projection.GetColorEye();
	CColorP3 Point[4], ScreenPoint[4];//当前点与投影点
	for (int nFace = 0; nFace < 81; nFace++)//曲面片划分为81个平面片
	{
		// 每个平面片为四边形
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{
			Point[nPoint] = V[F[nFace].ptIndex[nPoint]];
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);//三维透视投影
			ScreenPoint[nPoint].c = pScene->Illuminate(Eye, Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);//调用光照函数
		}
		// 每个平面片为一个四边形，我们把他分成两个三角形进行显示。
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]);//上三角形
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);//下三角形
		pZBuffer->FillTriangle(pDC);
	}
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