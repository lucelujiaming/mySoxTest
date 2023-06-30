#include "stdafx.h"
#include "ZBufferCube.h"
#define ROUND(d) int(d + 0.5)//四舍五入的宏定义

CZBufferCube::CZBufferCube(void)
{

}

CZBufferCube::~CZBufferCube(void)
{

}

void CZBufferCube::ReadVertex(void)//点表
{
	V[0].x = 0, V[0].y = 0, V[0].z = 0;
	V[1].x = 1, V[1].y = 0, V[1].z = 0;
	V[2].x = 1, V[2].y = 1, V[2].z = 0;
	V[3].x = 0, V[3].y = 1, V[3].z = 0;
	V[4].x = 0, V[4].y = 0, V[4].z = 1;
	V[5].x = 1, V[5].y = 0, V[5].z = 1;
	V[6].x = 1, V[6].y = 1, V[6].z = 1;
	V[7].x = 0, V[7].y = 1, V[7].z = 1;
}

void CZBufferCube::ReadFace(void)//面表
{
	F[0].ptNumber = 4, F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5, F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7, F[0].c = CRGB(1.0, 0.0, 0.0);//前面
	F[1].ptNumber = 4, F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3, F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1, F[1].c = CRGB(0.0, 1.0, 0.0);//后面
	F[2].ptNumber = 4, F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4, F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3, F[2].c = CRGB(0.0, 0.0, 1.0);//左面
	F[3].ptNumber = 4, F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2, F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5, F[3].c = CRGB(1.0, 1.0, 0.0);//右面
	F[4].ptNumber = 4, F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3, F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6, F[4].c = CRGB(1.0, 0.0, 1.0);//顶面
	F[5].ptNumber = 4, F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1, F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4, F[5].c = CRGB(0.0, 1.0, 1.0);//底面
}

void CZBufferCube::Draw(CDC* pDC, CZBuffer* pZBuffer, int nSign)
{
	CColorP3 ScreenPoint[4];//三维投影点
	for (int nFace = 0; nFace < 6; nFace++)//面循环
	{
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++)//顶点循环
		{
			ScreenPoint[nPoint] = projection.PerspectiveProjection3(V[F[nFace].ptIndex[nPoint]]);//三维透视投影
			if (nSign == 0)
				ScreenPoint[nPoint].c = CRGB(0.5, 0.0, 0.0);
			else if (nSign == 1)
				ScreenPoint[nPoint].c = CRGB(0.0, 0.5, 0.0);
			else if (nSign == 2)
				ScreenPoint[nPoint].c = CRGB(0.0, 0.0, 0.5);
			else
				ScreenPoint[nPoint].c = F[nFace].c;
		}
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]);//上三角形
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);//下三角形
		pZBuffer->FillTriangle(pDC);
	}
}