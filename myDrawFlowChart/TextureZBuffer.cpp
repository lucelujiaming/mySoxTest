#include "stdafx.h"
#include "TextureZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CTextureZBuffer::CTextureZBuffer(void)
{

}

CTextureZBuffer::~CTextureZBuffer(void)
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

// 使用顶点P，法矢量N和纹理地址T构造三角形。
void CTextureZBuffer::SetPoint(CColorP3* objPoint, 
		CVector3* objNormal, CTextureCoordinate* objTexture)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
		this->P[nPoint] = objPoint[nPoint];
		point[nPoint].x = ROUND(objPoint[nPoint].x);
		point[nPoint].y = ROUND(objPoint[nPoint].y);
		point[nPoint].z = objPoint[nPoint].z;
		point[nPoint].c = objPoint[nPoint].c;
		point[nPoint].n = objNormal[nPoint];
		point[nPoint].t = objTexture[nPoint];
	}
}

// 初始化深度缓冲
void CTextureZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
	{
		zBuffer[i] = new double[nHeight];
	}
	for (i = 0; i < nWidth; i++) // 初始化深度缓冲
	{
		for (int j = 0; j < nHeight; j++)
		{
			zBuffer[i][j] = nDepth;  // 将深度缓冲区的每一个元素设置为nDepth。
		}
	}
}

// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
void CTextureZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, 
								   CLightingScene* pScene, CTexture* pTexture)
{
	//顶点按照y坐标由小到大排序
	SortPoint();
	//定义三角形覆盖的扫描线条数
	int nTotalScanLine = point[2].y - point[0].y + 1;
	//定义span的起点与终点数组
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	//判断P1点位于P0P2边的左侧还是右侧
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y) 
	           - (point[2].y - point[0].y) * (point[1].x - point[0].x); // 点矢量叉积的z分量
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
	else // 右三角形。说明：
	{
		// 从0开始计算画线的起始点。
		nIndex = 0;
		// P[0]到P[2]为画线的起始点。
		EdgeFlag(point[0], point[2], LEFT);
		// 从0开始计算画线的结束点。
		nIndex = 0;
		// P[0]到P[1]为画线的结束点。
		EdgeFlag(point[0], point[1], RIGHT);
		// P[1]到P[2]为画线的结束点。
		EdgeFlag(point[1], point[2], RIGHT);
	}
	// 1. 根据多边形表面顶点坐标可以计算出两个边向量。
	CVector3 Vector01(P[0], P[1]), Vector02(P[0], P[2]);
	// 2. 调用ColorVector3.cpp中的CrossProduct函数来
	//    计算两个边向量的叉乘。得到法向量。参见公式(7-2)。
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	// 平面方程Ax+By+Cz＋D=0(7-1)的系数ABC就是法向量的系数。
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;
	// 而系数D使用下面的公式计算。参见公式(7-3)。
	double D = -A * P[0].x - B * P[0].y - C * P[0].z;
	// 如果C等于零，说明多边形表面的法向量和Z轴垂直。
	// 投影变成一条直线。在算法中可以不考虑。
	if (fabs(C) < 1e-4)
    {
		C = 1.0;
    }
	// 扫描线深度步长。也就是x每移动一个像素增加的深度值。参见公式(7-5)。
	double DepthStep = -A / C;//扫描线深度步长
	// 下面开始填充三角形。
	// 本算法的实质就是对于一个给定视线上的(x, y)，查找距离视点最近的z(x, y)值。
	// 下闭上开。开始扫描y坐标范围内的每一行。
	for (int y = point[0].y; y < point[2].y; y++)
	{
		// 得到y坐标在Span中对应的索引。
		int n = y - point[0].y;
		// 左闭右开。从跨度的左标志数组到右标志数组扫描每一行中的每一个点。
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++) 
		{
			// 计算当前像素点(x, y)处的深度值。
			// z = -(Ax + By + D) / C。参见公式(7-4)。
			double CurrentDepth = -(A * x + B * y + D) / C;
			// 法矢量线性插值。
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			// 纹理地址线性插值。
			CTextureCoordinate T = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].t, SpanRight[n].t);
			// 使用前面得到的纹理地址来获得纹理中对应位置的颜色。
			CRGB Texel = GetTexture(ROUND(T.u), ROUND(T.v), pTexture);
			// 将纹理颜色设置为漫反射率。
			pScene->pMaterial->SetDiffuse(Texel);
			// 将纹理颜色设置为环境反射率。
			pScene->pMaterial->SetAmbient(Texel);
			// 调用线性插值获得深度。
			// double CurrentDepth = Interp(x, SpanLeft[n].x, SpanRight[n].x, 
			//  	SpanLeft[n].z, SpanRight[n].z);
			// 计算当前像素点的光照。
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, CurrentDepth), 
				ptNormal, pScene->pMaterial);
			// 下面开始消隐
			// 如果当前采样点的深度小于帧缓冲器中原采样点的深度
			if (CurrentDepth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// 使用当前采样点的深度更新深度缓冲器。
				zBuffer[x + nWidth / 2][y + nHeight / 2] = CurrentDepth;
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

void CTextureZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA算法离散边
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

void CTextureZBuffer::SortPoint(void)//排序
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

// 法矢量线性插值
CVector3 CTextureZBuffer::Interp(double m, 
						double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

// 纹理地址线性插值
CTextureCoordinate CTextureZBuffer::Interp(double m, 
		double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1)
{
	CTextureCoordinate texture;
	texture = (m1 - m) / (m1 - m0) * T0 + (m - m0) / (m1 - m0) * T1;
	return texture;
}

// 深度线性插值
double CTextureZBuffer::Interp(double m, double m0, double m1, double z0, double z1)
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}

// 使用uv读取纹理中对应位置的颜色。
CRGB CTextureZBuffer::GetTexture(int u, int v, CTexture* pTexture) // 读取纹素
{
	// 定义v访问的顺序。默认的访问顺序从左下角开始。这句话调整为从左上角开始。
	v = pTexture->bmp.bmHeight - 1 - v;
	/*检测图片的边界，防止越界*/
	if (u < 0) 
    {
		u = 0; 
    }
	if (v < 0) 
    {
		v = 0;
    }
	if (u > pTexture->bmp.bmWidth - 1) 	
    {
		u = pTexture->bmp.bmWidth - 1;
    }
	if (v > pTexture->bmp.bmHeight - 1)	
    {
		v = pTexture->bmp.bmHeight - 1;
    }
	/*查找对应纹理空间的颜色值*/
	// 循环每一列，每行读四个字节。
	// 包含RGB和透明度
	int position = v * pTexture->bmp.bmWidthBytes + 4 * u;
	return  CRGB(pTexture->image[position + 2] / 255.0, 
		         pTexture->image[position + 1] / 255.0, 
				 pTexture->image[position] / 255.0);
}
