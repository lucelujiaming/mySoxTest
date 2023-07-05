#include "stdafx.h"
#include "AntiAliasedBumpTexture.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define PI 3.1415926
CAntiAliasedBumpTexture::CAntiAliasedBumpTexture(void)
{
	Bu = NULL;
	Bv = NULL;
}

CAntiAliasedBumpTexture::~CAntiAliasedBumpTexture(void)
{
	for (int i = 0; i < bmp.bmHeight; i++)
	{
		delete[]SrcImage[i];
		SrcImage[i] = NULL;
	}
	delete[]SrcImage;
	SrcImage = NULL;
	for (i = 0; i < nDestHeight; i++)
	{
		delete[]DestImage[i];
		DestImage[i] = NULL;
		delete Bu[i];
		Bu[i] = NULL;
		delete Bv[i];
		Bv[i] = NULL;
	}
	delete[]DestImage;
	DestImage = NULL;
	delete[]Bu;
	Bu = NULL;
	delete[]Bv;
	Bv = NULL;
}

void CAntiAliasedBumpTexture::SetDestImage(double r)//设置目标图像，球的表面积为4*PIr*r
{
	nDestWidth = ROUND(2 * PI * r);//目标图像的宽度
	nDestHeight = ROUND(2 * r);//目标图像的高度
}

void CAntiAliasedBumpTexture::ReadBitmap(UINT nIDResource)//读入位图
{
	CBitmap NewBitmap;//DDB位图
	NewBitmap.LoadBitmap(nIDResource);
	NewBitmap.GetBitmap(&bmp);//将CBitmap的信息保存到Bitmap结构体中
	int nbytesize = bmp.bmWidthBytes * bmp.bmHeight;
	BYTE* image = new BYTE[nbytesize];//纹理的一维数组
	NewBitmap.GetBitmapBits(nbytesize, (LPVOID)image);
	SrcImage = new CRGB * [bmp.bmHeight];//动态定义纹理的二维数组
	for (int i = 0; i < bmp.bmHeight; i++)
		SrcImage[i] = new CRGB[bmp.bmWidth];
	for (int v = 0; v < bmp.bmHeight; v++)
		for (int u = 0; u < bmp.bmWidth; u++)
		{
			int pos = v * bmp.bmWidthBytes + 4 * u;//颜色分量位置
			v = bmp.bmHeight - 1 - v;//位图从左下角向右上角绘制
			SrcImage[v][u] = CRGB(image[pos + 2] / 255.0, image[pos + 1] / 255.0, image[pos] / 255.0);
		}
	DestImage = new CRGB *[nDestHeight];
	for (i = 0; i < nDestHeight; i++)
		DestImage[i] = new CRGB[nDestWidth];
	AntiAliasedBumpTextureAntialiasing(DestImage, nDestWidth, nDestHeight, SrcImage, bmp.bmWidth, bmp.bmHeight);
	Bu = new double*[nDestHeight];
	Bv = new double*[nDestHeight];
	for (i = 0; i < nDestHeight; i++)
	{
		Bu[i] = new double[nDestWidth];
		Bv[i] = new double[nDestWidth];
	}
	for (v = 0; v < nDestHeight; v++)
		for (int u = 0; u < nDestWidth; u++)
		{
			int forwardU, forwardV, backwardU, backwardV;//一阶中心差分
			forwardU = u + 1, backwardU = u - 1;
			forwardV = v + 1, backwardV = v - 1;
			//检测图片的边界，防止越界
			if (backwardU < 0) 	backwardU = 0;
			if (forwardU > nDestWidth - 1) 	forwardU = nDestWidth - 1;
			if (backwardV < 0)	backwardV = 0;
			if (forwardV > nDestHeight - 1) forwardV = nDestHeight - 1;
			Bu[v][u] = DestImage[v][backwardU].red - DestImage[v][forwardU].red;
			Bv[v][u] = DestImage[backwardV][u].red - DestImage[forwardV][u].red;
		}
	delete[]image;
}

void CAntiAliasedBumpTexture::AntiAliasedBumpTextureAntialiasing(CRGB** DestImage, int nDestWidth, int nDestHeight, CRGB** SrcImage, int nSrcWidth, int nSrcHeight)//纹理反走样
{
	double WScale = double(nDestWidth) / nSrcWidth;//放大倍数=目标/源
	double HScale = double(nDestHeight) / nSrcHeight;
	for (int i = 0; i < nDestHeight; i++)
	{
		double Sy = i / HScale;
		int ty = int(Sy);
		double v = fabs(Sy - ty);
		for (int j = 0; j < nDestWidth; j++)
		{
			double Sx = j / WScale;
			int tx = int(Sx);
			double u = fabs(Sx - tx);
			if (tx >= nSrcWidth - 2)
				tx = nSrcWidth - 2;
			if (ty >= nSrcHeight - 2)
				ty = nSrcHeight - 2;
			DestImage[i][j] = (1 - u) * (1 - v) * SrcImage[ty][tx] + v * (1 - u) * SrcImage[ty + 1][tx]
				+ u * (1 - v) * SrcImage[ty][tx + 1] + u * v * SrcImage[ty + 1][tx + 1];//双线性插值
		}
	}
}