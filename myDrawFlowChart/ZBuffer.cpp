#include "stdafx.h"
#include "ZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CZBuffer::CZBuffer(void)
{
}

CZBuffer::~CZBuffer(void)
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
void CZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
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
			zBuffer[i][j] = nDepth;
		}
	}
}

void CZBuffer::SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2)
{
	// 读入三个顶点。
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	// 并且将这三个顶点整数化。
	point0.x = ROUND(P0.x);
	point0.y = ROUND(P0.y);
	point0.z = P0.z;
	point0.c = P0.c;
	point1.x = ROUND(P1.x);
	point1.y = ROUND(P1.y);
	point1.z = P1.z;
	point1.c = P1.c;
	point2.x = ROUND(P2.x);
	point2.y = ROUND(P2.y);
	point2.z = P2.z;
	point2.c = P2.c;
}

/************************************************************************/
/* 这个函数除了ZBuffer算法以外，还实现了三角形绘制颜色的双线性差值。    */
/* 三角形绘制颜色的双线性差值的逻辑如下：                               */
/*   1. 首先在调用这个函数之前，我们使用简单光照模型或者是              */
/*      Gouraud明暗处理算法，也就是双线性光强强度插值模型。             */
/*      计算出来三角形每一个顶点的颜色。                                */
/*   2. 根据三角形顶点的光强，按照扫描线顺序使用双线性插值计算三角形    */
/*      边上每一点的光强。然后将光强分解为RGB三原色的颜色值。           */
/*      即在CZBuffer::EdgeFlag中调用Interp函数。                        */
/*   3. 在扫描线与三角形相交跨度内，使用双线性插值计算每一点的光强。    */
/*      然后将光强分解为RGB三原色的颜色值。                             */
/*      即在CZBuffer::FillTriangle中调用Interp函数。                    */
/*   这里所有的颜色差值都采用了Gouraud明暗处理算法中提到的双线性插值。  */
/************************************************************************/
void CZBuffer::FillTriangle(CDC* pDC)
{
	// 顶点按照y坐标由小到大排序
	SortPoint();
	// 定义三角形覆盖的扫描线条数
	int nTotalScanLine = point2.y - point0.y + 1;
	// 定义Span的起点与终点数组
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	// 判断P1点位于P0P2边的左侧还是右侧
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
	CColorVector3 Vector01(P0, P1), Vector02(P0, P2);
	// 2. 调用ColorVector3.cpp中的CrossProduct函数来
	//    计算两个边向量的叉乘。得到法向量。参见公式(7-2)。
	CColorVector3 fNormal = CrossProduct(Vector01, Vector02);
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
	// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
	// 下闭上开。开始扫描y坐标范围内的每一行。
	for (int y = point0.y; y < point2.y; y++)
	{
		// 得到y坐标在Span中对应的索引。
		int n = y - point0.y;
		// 左闭右开。开始扫描每一行中的每一个点。
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
				// 采用双线性差值算法计算多边形内一个像素点的颜色线性插值。
				CRGB clr = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].c, SpanRight[n].c);
				// 则调用SetPixelV将当前像素的颜色值写入帧缓冲器。
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(clr.red * 255, clr.green * 255, clr.blue * 255));
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
void CZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;	// 计算斜率的倒数。
	double x = PStart.x;		// 找到起点。
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// 采用双线性差值算法计算多边形内一个像素点的颜色线性插值。
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		// // 采用双线性差值算法计算多边形内一个像素点的深度。
		// double zDepth = Interp(y, PStart.y, PEnd.y, PStart.z, PEnd.z);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
		else
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
		x += m;
	}
}

void CZBuffer::SortPoint(void)//排序
{
	// 要求排序后，P[0].y<P[1].y<P[2].y
	CColorPoint3 pt;
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

CRGB CZBuffer::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//颜色线性插值
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}
// 法矢量线性插值
CVector3 CZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

double CZBuffer::Interp(double m, double m0, double m1, double z0, double z1)//深度线性插值
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}