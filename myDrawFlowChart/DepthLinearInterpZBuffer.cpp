#include "stdafx.h"

#include "DepthLinearInterpZBuffer.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CDepthLinearInterpZBuffer::CDepthLinearInterpZBuffer(void)
{

}

CDepthLinearInterpZBuffer::~CDepthLinearInterpZBuffer(void)
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

/************************************************************************/
/* 初始化深度缓冲空间。一共有宽度 * 深度个元素。                        */
/*     nWidth  - 显示宽度。                                             */
/*     nHeight - 显示高度。                                             */
/*     nDepth  - 最大深度。                                             */
/************************************************************************/
void CDepthLinearInterpZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
{
	// 记录宽度和深度。
	this->nWidth = nWidth, this->nHeight = nHeight;
	// 使用宽度和深度分配深度缓冲空间。一共有宽度 * 深度个元素。
	// 每一个元素都是一个double。
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
	{
		zBuffer[i] = new double[nHeight];
	}
	// 初始化深度缓冲空间。把每一个元素都预置为最大深度。
	for (i = 0; i < nWidth; i++) 
	{
		for (int j = 0; j < nHeight; j++)
		{
			zBuffer[i][j] = nDepth;   // 将深度缓冲区的每一个元素设置为nDepth。
		}
	}
}

void CDepthLinearInterpZBuffer::SetPoint(CColorP3* P, CVector3* N)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
        // 读入顶点。
		this->P[nPoint] = P[nPoint];
        // 并且将顶点整数化。
		point[nPoint].x = ROUND(P[nPoint].x);
		point[nPoint].y = ROUND(P[nPoint].y);
		point[nPoint].z = P[nPoint].z;
		point[nPoint].c = P[nPoint].c;
		point[nPoint].n = N[nPoint];
	}	
}

// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
void CDepthLinearInterpZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene)
{
	// 顶点按照y坐标由小到大排序
	SortPoint();
	//定义三角形覆盖的扫描线条数
	int nTotalScanLine = point[2].y - point[0].y + 1;
	//定义span的起点与终点数组
	SpanLeft = new CColorPoint3[nTotalScanLine];
	SpanRight = new CColorPoint3[nTotalScanLine];
	//判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y) 
	           - (point[2].y - point[0].y) * (point[1].x - point[0].x);//点矢量叉积的z分量
	if (nDeltz > 0) // 左三角形。说明：
	{
		// 从0开始计算画线的起始点。
		nIndex = 0;
		// P[0]到P[1]为画线的起始点。
		EdgeFlag(point[0], point[1], LEFT);
		// P[1]到P[2]为画线的起始点。
		EdgeFlag(point[1], point[2], LEFT);
		// 从0开始计算画线的结束点。
		nIndex = 0;
		// P[0]到P[2]为画线的结束点。
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
	// 下面是ZBuffer算法的核心内容。
	for (int y = point[0].y; y < point[2].y; y++)//下闭上开
	{
		// 得到y坐标在Span中对应的索引。
		int n = y - point[0].y;
		// 左闭右开。从跨度的左标志数组到右标志数组扫描每一行中的每一个点。
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++)
		{
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			// 对于深度，沿着跨度两侧进行线性插值。
			double depth = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].z, SpanRight[n].z); 
			// 计算当前像素点的光照。
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, depth), ptNormal, pScene->pMaterial);
			// 如果当前采样点的深度小于帧缓冲器中原采样点的深度
			if (depth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// 使用当前采样点的深度更新深度缓冲器
				zBuffer[x + nWidth / 2][y + nHeight / 2] = depth;
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(I.red * 255, I.green * 255, I.blue * 255));
			}
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

void CDepthLinearInterpZBuffer::EdgeFlag(CColorPoint3 PStart, CColorPoint3 PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// 采用双线性差值算法计算多边形内一个像素点的颜色线性插值。
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		// 采用双线性差值算法计算多边形内一个像素点的深度。
		double zDepth = Interp(y, PStart.y, PEnd.y, PStart.z, PEnd.z);
		if (bFeature)
        {
			SpanLeft[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
        }
		else
        {
			SpanRight[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
        }
		x += m;
	}
}

void CDepthLinearInterpZBuffer::SortPoint(void)//排序
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

CRGB CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//颜色线性插值
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}

CVector3 CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)//法矢量线性插值
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

double CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, double z0, double z1)//深度线性插值
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}
