#include "stdafx.h"
#include "Vector3ZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CVector3ZBuffer::CVector3ZBuffer(void)
{

}

CVector3ZBuffer::~CVector3ZBuffer(void)
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

void CVector3ZBuffer::SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2, CVector3 N0, CVector3 N1, CVector3 N2)
{
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	point0.x = ROUND(P0.x);
	point0.y = ROUND(P0.y);
	point0.z = P0.z;
	point0.c = P0.c;
	point0.n = N0;
	point1.x = ROUND(P1.x);
	point1.y = ROUND(P1.y);
	point1.z = P1.z;
	point1.c = P1.c;
	point1.n = N1;
	point2.x = ROUND(P2.x);
	point2.y = ROUND(P2.y);
	point2.z = P2.z;
	point2.c = P2.c;
	point2.n = N2;
}

void CVector3ZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)//初始化深度缓冲
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
		zBuffer[i] = new double[nHeight];
	for (i = 0; i < nWidth; i++)//初始化深度缓冲
		for (int j = 0; j < nHeight; j++)
			zBuffer[i][j] = nDepth;
}

void CVector3ZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene)
{
	//顶点按照y坐标由小到大排序
	SortPoint();
	//定义三角形覆盖的扫描线条数
	int nTotalScanLine = point2.y - point0.y + 1;
	//定义span的起点与终点数组
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	//判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (point2.x - point0.x) * (point1.y - point0.y) - (point2.y - point0.y) * (point1.x - point0.x);//点矢量叉积的z分量
	if (nDeltz > 0)//左三角形
	{
		nIndex = 0;
		EdgeFlag(point0, point1, LEFT);
		EdgeFlag(point1, point2, LEFT);
		nIndex = 0;
		EdgeFlag(point0, point2, RIGHT);
	}
	else//右三角形
	{
		nIndex = 0;
		EdgeFlag(point0, point2, LEFT);
		nIndex = 0;
		EdgeFlag(point0, point1, RIGHT);
		EdgeFlag(point1, point2, RIGHT);
	}
	CVector3 Vector01(P0, P1), Vector02(P0, P2);
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;//平面方程Ax+By+Cz＋D=0的系数
	double D = -A * P0.x - B * P0.y - C * P0.z;//系数D
	if (fabs(C) < 1e-4)
		C = 1.0;
	double DepthStep = -A / C;//扫描线深度步长
	for (int y = point0.y; y < point2.y; y++)//下闭上开
	{
		int n = y - point0.y;
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++)//左闭右开
		{
			double CurrentDepth = -(A * x + B * y + D) / C;//z=-(Ax+By+D)/C
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
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

void CVector3ZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal);
		else
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y,ptNormal);
		x += m;
	}
}

void CVector3ZBuffer::SortPoint(void)//排序
{
	CColorPoint3 pt;//要求排序后，P[0].y<P[1].y<P[2].y
	if (point0.y > point1.y)
	{
		pt = point0;
		point0 = point1;
		point1 = pt;
	}
	if (point0.y > point2.y)
	{
		pt = point0;
		point0 = point2;
		point2 = pt;
	}
	if (point1.y > point2.y)
	{
		pt = point1;
		point1 = point2;
		point2 = pt;
	}
}

CVector3 CVector3ZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)//法矢量线性插值
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}