#if !defined(AFX_Vector3_ZBuffer_H_)
#define AFX_Vector3_ZBuffer_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//带颜色的二维整数点类
#include "LightingScene.h"//Blinn-Phong模型

class CVector3ZBuffer//三角形填充类
{
public:
	CVector3ZBuffer(void);
	virtual ~CVector3ZBuffer(void);
	void SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2, CVector3 N0, CVector3 N1, CVector3 N2);//三顶点构造三角形
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//初始化深度缓冲器
	void FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene);//填充三角形
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
	CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//法矢量线性插值	
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
	CColorP3 P0, P1, P2;//三角形的浮点坐标
	CColorPoint3 point0, point1, point2;//三角形的整数顶点坐标
	CColorPoint2* SpanLeft; //跨度的起点数组标志
	CColorPoint2* SpanRight;//跨度的终点数组标志
	int nIndex;//记录扫描线条数
	double** zBuffer;//深度缓冲区
	int nWidth, nHeight;//缓冲区宽度和高度
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_Vector3_ZBuffer_H_)
