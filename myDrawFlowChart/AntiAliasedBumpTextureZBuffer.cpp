#include "stdafx.h"
#include "AntiAliasedBumpTextureZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CAntiAliasedBumpTextureZBuffer::CAntiAliasedBumpTextureZBuffer(void)
{

}

CAntiAliasedBumpTextureZBuffer::~CAntiAliasedBumpTextureZBuffer(void)
{
	for (int i = 0; i < nWidth; i++)
	{
		delete[] zBuffer[i];
		zBuffer[i] = NULL;
	}
	if (zBuffer != NULL)
	{
		delete zBuffer;
		zBuffer = NULL;
	}
}

void CAntiAliasedBumpTextureZBuffer::SetPoint(CColorP3* P, CVector3* N, CTextureCoordinate* T)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
		this->P[nPoint] = P[nPoint];
		point[nPoint].x = ROUND(P[nPoint].x);
		point[nPoint].y = ROUND(P[nPoint].y);
		point[nPoint].z = P[nPoint].z;
		point[nPoint].c = P[nPoint].c;
		point[nPoint].n = N[nPoint];
		point[nPoint].t = T[nPoint];
	}
}

void CAntiAliasedBumpTextureZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)//初始化深度缓冲
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
    {
		zBuffer[i] = new double[nHeight];
    }
	for (i = 0; i < nWidth; i++)//初始化深度缓冲
    {
		for (int j = 0; j < nHeight; j++)
        {
			zBuffer[i][j] = nDepth;  // 将深度缓冲区的每一个元素设置为nDepth。
        }
    }
}

void CAntiAliasedBumpTextureZBuffer::ReadGradient(double** Bu, double** Bv)
{
	this->Bu = Bu;
	this->Bv = Bv;
}

// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
void CAntiAliasedBumpTextureZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene, CRGB** DestImage)
{
	// 顶点按照y坐标由小到大排序
	SortPoint();
	// 定义三角形覆盖的扫描线条数
	int nTotalScanLine = point[2].y - point[0].y + 1;
	// 定义Span的起点与终点数组
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	// 判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y)
		       - (point[2].y - point[0].y) * (point[1].x - point[0].x); // 点矢量叉积的z分量
	if (nDeltz > 0)//左三角形
	{
		nIndex = 0;
		EdgeFlag(point[0], point[1], LEFT);
		EdgeFlag(point[1], point[2], LEFT);
		nIndex = 0;
		EdgeFlag(point[0], point[2], RIGHT);
	}
	else//右三角形
	{
		nIndex = 0;
		EdgeFlag(point[0], point[2], LEFT);
		nIndex = 0;
		EdgeFlag(point[0], point[1], RIGHT);
		EdgeFlag(point[1], point[2], RIGHT);
	}
	CVector3 Vector01(P[0], P[1]), Vector02(P[0], P[2]);
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;//平面方程Ax+By+Cz＋D=0的系数
	double D = -A * P[0].x - B * P[0].y - C * P[0].z;//系数D
	if (fabs(C) < 1e-4)
    {
		C = 1.0;
    }
	double DepthStep = -A / C;//扫描线深度步长
	for (int y = point[0].y; y < point[2].y; y++)//下闭上开
	{
		int n = y - point[0].y;
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++)//左闭右开
		{
			double CurrentDepth = -(A * x + B * y + D) / C;//z=-(Ax+By+D)/C
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			CTextureCoordinate T = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].t, SpanRight[n].t);
			CRGB Texel = DestImage[ROUND(T.v)][ROUND(T.u)];
			pScene->pMaterial->SetDiffuse(Texel);
			int BumpScale = 1000;//凹凸比例
			CVector3 n1 = BumpScale * CVector3(Bu[ROUND(T.v)][ROUND(T.u)], 0, 0);
			CVector3 n2 = BumpScale * CVector3(0, Bv[ROUND(T.v)][ROUND(T.u)], 0);
			ptNormal = ptNormal + n1 + n2;
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, CurrentDepth), ptNormal, pScene->pMaterial);
			if (CurrentDepth <= zBuffer[x + nWidth / 2][y + nHeight / 2])//如果当前采样点的深度小于帧缓冲器中原采样点的深度
			{
				zBuffer[x + nWidth / 2][y + nHeight / 2] = CurrentDepth;//使用当前采样点的深度更新深度缓冲器
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(I.red * 255, I.green * 255, I.blue * 255));
			}
			CurrentDepth += DepthStep;
		}
	}
	if(SpanLeft)
	{
		delete []SpanLeft;
		SpanLeft = NULL;
	}
	if(SpanRight)
	{
		delete []SpanRight;
		SpanRight = NULL;
	}
}

void CAntiAliasedBumpTextureZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		CTextureCoordinate T = Interp(y, PStart.y, PEnd.y, PStart.t, PEnd.t);
		if (bFeature)
        {
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal, T);
        }
		else
        {
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal, T);
        }
		x += m;
	}
}

void CAntiAliasedBumpTextureZBuffer::SortPoint(void)//排序
{
	CColorPoint3 pt;//要求排序后，P[0].y<P[1].y<P[2].y
	if (point[0].y > point[1].y)
	{
		pt = point[0];
		point[0] = point[1];
		point[1] = pt;
	}
	if (point[0].y > point[2].y)
	{
		pt = point[0];
		point[0] = point[2];
		point[2] = pt;
	}
	if (point[1].y > point[2].y)
	{
		pt = point[1];
		point[1] = point[2];
		point[2] = pt;
	}
}

CVector3 CAntiAliasedBumpTextureZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)//法矢量线性插值
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

CTextureCoordinate CAntiAliasedBumpTextureZBuffer::Interp(double m, double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1)//纹理地址线性插值
{
	CTextureCoordinate texture;
	texture = (m1 - m) / (m1 - m0) * T0 + (m - m0) / (m1 - m0) * T1;
	return texture;
}
