#include "stdafx.h"
#include "GouraudLightingRationalBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CGouraudLightingRationalBiquatricBezierPatch::CGouraudLightingRationalBiquatricBezierPatch(void)
{

}


CGouraudLightingRationalBiquatricBezierPatch::~CGouraudLightingRationalBiquatricBezierPatch(void)
{

}

void CGouraudLightingRationalBiquatricBezierPatch::ReadControlPoint(CColorP3 P[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->P[i][j] = P[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::ReadWeight(double W[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->W[i][j] = W[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::SaveFacetData(void)
 {
	double M[3][3];//系数矩阵Mbe
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CColorP3 Pw[3][3];//曲线计算用控制点数组
	for(int i = 0; i < 3; i++)
		for(int j = 0;j < 3;j++)
			Pw[i][j] = P[i][j] * W[i][j];
	RightMultiplyMatrix(Pw, M);//系数矩阵右乘三维点矩阵
	RightMultiplyMatrix(W, M);//系数矩阵右乘权因子矩阵
	TransposeMatrix(M);//计算转置矩阵
	LeftMultiplyMatrix(M, Pw);//系数矩阵左乘三维点矩阵
	LeftMultiplyMatrix(M, W);//系数矩阵左乘权因子矩阵
	int nStep = 9;//曲面细分步长（10条线，9格）
	double u0, u1, u2, v0, v1, v2;//u，v参数的幂
	for (int u = 0; u < nStep + 1; u++)
	{
		for (int v = 0; v < nStep + 1; v++)
		{
			u2 = (u / double(nStep)) * (u / double(nStep)), u1 = u / double(nStep), u0 = 1;
			v2 = (v / double(nStep)) * (v / double(nStep)), v1 = v / double(nStep), v0 = 1;
			CColorP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
				+ (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
				+ (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			double denominator = (u2 * W[0][0] + u1 * W[1][0] + u0 * W[2][0]) * v2
				+ (u2 * W[0][1] + u1 * W[1][1] + u0 * W[2][1]) * v1
				+ (u2 * W[0][2] + u1 * W[1][2] + u0 * W[2][2]) * v0;
			V[u * (nStep + 1) + v] = numerator / denominator;
		}
	}
	for (int nFacet = 0; nFacet < 81; nFacet++)//9×9个平面片面表信息，定义每个平面片的索引号
	{
		F[nFacet].SetPtNumber(4);		
		F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;//0
		F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;//1
		F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;//11
		F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;//10		
	}
}

void CGouraudLightingRationalBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CGouraudLightingRationalBiquatricBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
{
	SaveFacetData();//读取平面片的点表与面表
	CColorP3 Eye = projection.GetColorEye();
	CColorP3 Point[4], ScreenPoint[4];//当前点与投影点
	for (int nFace = 0; nFace < 81; nFace++)//曲面片划分为81个平面片
	{
		for (int nPoint = 0; nPoint < 4; nPoint++)//每个平面片为四边形
		{				
			Point[nPoint] = V[F[nFace].ptIndex[nPoint]];
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);//三维透视投影
			ScreenPoint[nPoint].c = pScene->Illuminate(Eye, Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);//调用光照函数
		}
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]);//上三角形
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);//下三角形
		pZBuffer->FillTriangle(pDC);
	}
}

void CGouraudLightingRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])//左乘矩阵M*P
{
	CColorP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];			
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//左乘矩阵M*W
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];		
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])//右乘矩阵P*M
{
	CColorP3 T[3][3];//临时矩阵
	for(int i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)	
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//右乘矩阵W*M
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CGouraudLightingRationalBiquatricBezierPatch::TransposeMatrix(double M[3][3])//转置矩阵
{
	double T[3][3];//临时矩阵
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[j][i] = M[i][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			M[i][j] = T[i][j];
}