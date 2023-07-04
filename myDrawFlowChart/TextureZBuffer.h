#if !defined(AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint3.h"//带颜色的三维整数点类
#include "LightingScene.h"//Blinn-Phong模型
#include "Texture.h"//图像纹理
#include "TextureCoordinate.h"//图像纹理

class CTextureZBuffer//三角形填充类
{
public:
	CTextureZBuffer(void);
	virtual ~CTextureZBuffer(void);
	void SetPoint(CColorP3* P, CVector3* N, CTextureCoordinate* T);//三顶点构造三角形
	void InitialDepthBuffer(int nWidth, int nHeight, double zDepth);//初始化深度缓冲器
	void FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene, CTexture* pTexture);//填充三角形
private:
	void EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature);//边标记算法
	void SortPoint(void);//顶点排序
	CVector3 Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1);//法矢量线性插值
	CTextureCoordinate Interp(double m, double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1);//纹理地址线性插值
	CRGB GetTexture(int u, int v, CTexture* pTexture);//读取纹理
private:
	CColorP3 P[3];//三角形的浮点坐标
	CColorPoint3 point[3];//三角形的整数顶点坐标
	CColorPoint2* SpanLeft; //跨度的起点数组标志
	CColorPoint2* SpanRight;//跨度的终点数组标志
	int nIndex;//记录扫描线条数
	double** zBuffer;//深度缓冲区
	int nWidth, nHeight;//缓冲区宽度和高度
};

#endif // !defined(AFX_TEXTURE_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
