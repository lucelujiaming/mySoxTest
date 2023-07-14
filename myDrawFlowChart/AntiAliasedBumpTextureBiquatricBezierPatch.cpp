#include "stdafx.h"
#include "AntiAliasedBumpTextureBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CAntiAliasedBumpTextureBiquatricBezierPatch::CAntiAliasedBumpTextureBiquatricBezierPatch(void)
{

}


CAntiAliasedBumpTextureBiquatricBezierPatch::~CAntiAliasedBumpTextureBiquatricBezierPatch(void)
{

}

void CAntiAliasedBumpTextureBiquatricBezierPatch::ReadControlPoint(CColorP3 P[3][3])
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0;j < 3; j++)
	    {
   			this->P[i][j] = P[i][j];
	    }
    }
}

void CAntiAliasedBumpTextureBiquatricBezierPatch::ReadWeight(double W[3][3])
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0;j < 3; j++)
	    {
   			this->W[i][j] = W[i][j];
	    }
    }
}

void CAntiAliasedBumpTextureBiquatricBezierPatch::ReadPatchNumber(int nPatch)
{
	this->nPatch = nPatch;
}

void CAntiAliasedBumpTextureBiquatricBezierPatch::SetTexture(CAntiAliasedBumpTexture* pTexture)
{
	this->pTexture = pTexture;
}

void CAntiAliasedBumpTextureBiquatricBezierPatch::SaveFacetData(void)
 {
	double M[3][3];//系数矩阵Mbe
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CColorP3 Pw[3][3];//曲线计算用控制点数组
	for(int i = 0; i < 3; i++)
    {
		for(int j = 0;j < 3;j++)
        {
			Pw[i][j] = P[i][j] * W[i][j];
        }
    }
	RightMultiplyMatrix(Pw, M);//系数矩阵右乘三维点矩阵
	RightMultiplyMatrix(W, M);//系数矩阵右乘权因子矩阵
	TransposeMatrix(M);//计算转置矩阵
	LeftMultiplyMatrix(M, Pw);//系数矩阵左乘三维点矩阵
	LeftMultiplyMatrix(M, W);//系数矩阵左乘权因子矩阵
	int nStep = 9;//曲面细分步长（10条线，9格）
	double u0, u1, u2, v0, v1, v2;//u，v参数的幂
	for (int v = 0; v < nStep + 1; v++)//先v后u
	{
		for (int u = 0; u < nStep + 1; u++)
		{
			v2 = (v / double(nStep)) * (v / double(nStep)), v1 = v / double(nStep), v0 = 1;
			u2 = (u / double(nStep)) * (u / double(nStep)), u1 = u / double(nStep), u0 = 1;
			CColorP3 numerator = (v2 * Pw[0][0] + v1 * Pw[1][0] + v0 * Pw[2][0]) * u2
				+ (v2 * Pw[0][1] + v1 * Pw[1][1] + v0 * Pw[2][1]) * u1
				+ (v2 * Pw[0][2] + v1 * Pw[1][2] + v0 * Pw[2][2]) * u0;
			double denominator = (v2 * W[0][0] + v1 * W[1][0] + v0 * W[2][0]) * u2
				+ (v2 * W[0][1] + v1 * W[1][1] + v0 * W[2][1]) * u1
				+ (v2 * W[0][2] + v1 * W[1][2] + v0 * W[2][2]) * u0;
			V[v * (nStep + 1) + u] = numerator / denominator;
			T[v * (nStep + 1) + u].u = (pTexture->nDestWidth - 1) * (u / double(nStep) + nPatch % 4) * 0.25;//此处修改为目标图像
			T[v * (nStep + 1) + u].v = (pTexture->nDestHeight - 1) * (v / double(nStep) + !(nPatch / 4)) * 0.5;
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

void CAntiAliasedBumpTextureBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CAntiAliasedBumpTextureBiquatricBezierPatch::Draw(CDC* pDC, CAntiAliasedBumpTextureZBuffer* pZBuffer)
{
	SaveFacetData();//读取平面片的点表与面表
	CColorP3 colorEye = projection.GetColorEye();
	CColorP3 Point4[4], ScreenPoint4[4];//当前点与投影点
	CVector3 N4[4];//点法矢量
	CTextureCoordinate T4[4];//纹理地址
	pZBuffer->ReadGradient(pTexture->Bu, pTexture->Bv);
	for (int nFace = 0; nFace < 81; nFace++)//曲面片划分为81个平面片
	{
		// 每个平面片为四边形
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{
			Point4[nPoint] = V[F[nFace].ptIndex[nPoint]];
			ScreenPoint4[nPoint] = projection.ThreeDimColorPerspectiveProjection(Point4[nPoint]);//三维透视投影
			N4[nPoint] = CVector3(Point4[nPoint]);
			T4[nPoint] = T[F[nFace].ptIndex[nPoint]];
		}
		// 每个平面片为一个四边形，我们把他分成两个三角形进行显示。
		//绘制左上三角形
		CColorP3 TLP[3] = { ScreenPoint4[0] ,ScreenPoint4[2] ,ScreenPoint4[3] };
		CVector3 TLN[3] = { N4[0], N4[2], N4[3] };
		CTextureCoordinate TLT[3] = { T4[0], T4[2], T4[3] };
		pZBuffer->SetPoint(TLP, TLN, TLT);//左上三角形的顶点P，法矢量N和纹理地址T
		pZBuffer->FillTriangle(pDC, colorEye, pScene, pTexture->DestImage);
		//绘制右下三角形
		CColorP3 DRP[3] = { ScreenPoint4[0] ,ScreenPoint4[1] ,ScreenPoint4[2] };
		CVector3 DRN[3] = { N4[0], N4[1], N4[2] };
		CTextureCoordinate DRT[3] = { T4[0], T4[1], T4[2] };//右下三角形的顶点P，法矢量N和纹理地址T
		pZBuffer->SetPoint(DRP, DRN, DRT);
		pZBuffer->FillTriangle(pDC, colorEye, pScene, pTexture->DestImage);
	}
}

// 左乘矩阵M*P
void CAntiAliasedBumpTextureBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])
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

// 左乘矩阵M*W
void CAntiAliasedBumpTextureBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])
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

void CAntiAliasedBumpTextureBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])//右乘矩阵P*M
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

void CAntiAliasedBumpTextureBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//右乘矩阵W*M
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

void CAntiAliasedBumpTextureBiquatricBezierPatch::TransposeMatrix(double M[3][3])//转置矩阵
{
	double T[3][3];//临时矩阵
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
