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

// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
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
	int nDeltz = (point2.x - point0.x) * (point1.y - point0.y)
		       - (point2.y - point0.y) * (point1.x - point0.x); // 点矢量叉积的z分量
	if (nDeltz > 0) // 左三角形。说明：
	{
		// 从0开始计算画线的起始点。
		nIndex = 0;
		// P[0]到P[1]为画线的起始点。
		EdgeFlag(point0, point1, LEFT);
		// P[1]到P[2]为画线的起始点。
		EdgeFlag(point1, point2, LEFT);
		// 从0开始计算画线的结束点。
		nIndex = 0;
		// P[0]到P[2]为画线的结束点。
		EdgeFlag(point0, point2, RIGHT);
	}
	else // 右三角形。说明：
	{
		// 从0开始计算画线的起始点。
		nIndex = 0;
		// P[0]到P[2]为画线的起始点。
		EdgeFlag(point0, point2, LEFT);
		// 从0开始计算画线的结束点。
		nIndex = 0;
		// P[0]到P[1]为画线的结束点。
		EdgeFlag(point0, point1, RIGHT);
		// P[1]到P[2]为画线的结束点。
		EdgeFlag(point1, point2, RIGHT);
	}
	// 1. 根据多边形表面顶点坐标可以计算出两个边向量。
	CVector3 Vector01(P0, P1), Vector02(P0, P2);
	// 2. 调用ColorVector3.cpp中的CrossProduct函数来
	//    计算两个边向量的叉乘。得到法向量。参见公式(7-2)。
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	// 平面方程Ax+By+Cz＋D=0(7-1)的系数ABC就是法向量的系数。
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;
	// 而系数D使用下面的公式计算。参见公式(7-3)。
	double D = -A * P0.x - B * P0.y - C * P0.z;
	// 如果C等于零，说明多边形表面的法向量和Z轴垂直。
	// 投影变成一条直线。在算法中可以不考虑。
	if (fabs(C) < 1e-4)
		C = 1.0;
	// 扫描线深度步长。也就是x每移动一个像素增加的深度值。参见公式(7-5)。
	double DepthStep = -A / C;
	// 下面开始填充三角形。
	// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
	// 下闭上开。开始扫描y坐标范围内的每一行。
	for (int y = point0.y; y < point2.y; y++)
	{
		// 得到y坐标在Span中对应的索引。
		int n = y - point0.y;
		// 左闭右开。从跨度的左标志数组到右标志数组扫描每一行中的每一个点。
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++) 
		{
			// 计算当前像素点(x, y)处的深度值。
			// z = -(Ax + By + D) / C。参见公式(7-4)。
			double CurrentDepth = -(A * x + B * y + D) / C;
			// 把计算出来的当前像素点(x, y)处的深度值，
			// 与已经存储在深度缓冲器中的原可见像素的深度值相对比。
			// 如果当前采样点的深度小于帧缓冲器中原采样点的深度，
			// 表明当前像素更靠近观察者 并遮挡了原像素。
			// 因此上，ZBuffer算法的实质就是对于一个给定视线上的(x, y)，
			// 查找距离视点最近的z(x, y)值。
			if (CurrentDepth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// 使用当前采样点的深度更新深度缓冲器。
				zBuffer[x + nWidth / 2][y + nHeight / 2] = CurrentDepth;
				// 这里是整个绘制过程的核心逻辑。也就是Phong明暗处理。
				// 我们不再简单地采用双线性差值算法计算多边形内一个像素点的颜色线性插值。
		        // 2. 而是使用双线性插值计算多边形内部各点的法向量。
				CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
				// 3. 最后才使用多边形网格上各点的法向量调用简单光照模型计算所获得的光强。
				//    而Gouraud明暗处理则是直接使用多边形顶点调用简单光照模型计算所获得的光强。
				//    Phong明暗处理则是通过双线性插值使用的得到多边形网格上各点的法向量，之后用这个
				//    法向量调用简单光照模型计算所获得的光强。
				//    明显后者的处理更加细致。
				CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, CurrentDepth), ptNormal, pScene->pMaterial);
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(I.red * 255, I.green * 255, I.blue * 255));
			}
			// CurrentDepth += DepthStep;
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

// DDA算法离散边来线性插值填充跨度Span的起点或终点数组。
void CVector3ZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA算法离散边
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;	// 计算斜率的倒数。
	double x = PStart.x;		// 找到起点。
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// 这里是整个绘制过程的核心逻辑。也就是Phong明暗处理。
		// 我们不再简单地采用双线性差值算法计算多边形内一个像素点的颜色线性插值。
		// 2. 而是使用双线性插值计算多边形边上各点的法向量。
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

// 法矢量线性插值
CVector3 CVector3ZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}