#include "stdafx.h"
#include "TextureCube.h"

CTextureCube::CTextureCube(void)
{

}

CTextureCube::~CTextureCube(void)
{

}

void CTextureCube::ReadVertex(void)//点表
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

void CTextureCube::ReadFace(void)//面表
{
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5, F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7;//前面
	F[0].t[0] = CTextureCoordinate(0.25, 0.33);
	F[0].t[1] = CTextureCoordinate(0.5, 0.33);
	F[0].t[2] = CTextureCoordinate(0.5, 0.66);
	F[0].t[3] = CTextureCoordinate(0.25, 0.66);//前面纹理地址
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1;//后面
	F[1].t[0] = CTextureCoordinate(1, 0.33);
	F[1].t[1] = CTextureCoordinate(1, 0.66);
	F[1].t[2] = CTextureCoordinate(0.75, 0.66);
	F[1].t[3] = CTextureCoordinate(0.75, 0.33);//后面纹理地址	
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3;//左面
	F[2].t[0] = CTextureCoordinate(0, 0.33);
	F[2].t[1] = CTextureCoordinate(0.25, 0.33);
	F[2].t[2] = CTextureCoordinate(0.25, 0.66);
	F[2].t[3] = CTextureCoordinate(0, 0.66);   //左面纹理地址
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5;//右面
	F[3].t[0] = CTextureCoordinate(0.75, 0.33);
	F[3].t[1] = CTextureCoordinate(0.75, 0.66);
	F[3].t[2] = CTextureCoordinate(0.5, 0.66);
	F[3].t[3] = CTextureCoordinate(0.5, 0.33);//右面纹理地址
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6;//顶面
	F[4].t[0] = CTextureCoordinate(0.5, 1);
	F[4].t[1] = CTextureCoordinate(0.25, 1);
	F[4].t[2] = CTextureCoordinate(0.25, 0.66);
	F[4].t[3] = CTextureCoordinate(0.5, 0.66);//顶面纹理地址
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4;//底面
	F[5].t[0] = CTextureCoordinate(0.25, 0);
	F[5].t[1] = CTextureCoordinate(0.5, 0);
	F[5].t[2] = CTextureCoordinate(0.5, 0.33);
	F[5].t[3] = CTextureCoordinate(0.25, 0.33);//底面纹理地址
}

void CTextureCube::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CTextureCube::SetTexture(CTexture* pTexture)
{
	this->pTexture = pTexture;
}

void CTextureCube::Draw(CDC* pDC, CTextureZBuffer* pZBuffer)
{
	CColorP3 ScreenPoint4[4];//三维投影点
	CColorP3 Eye = projection.GetColorEye();//视点
	CVector3 N4[4];//顶点的法矢量
	CTextureCoordinate T4[4];//顶点的纹理坐标
	for (int nFace = 0; nFace < 6; nFace++)//面循环
	{
		CVector3 Vector01(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[1]]);//面的一个边矢量
		CVector3 Vector02(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[2]]);//面的另一个边矢量
		CVector3 FaceNormal = CrossProduct(Vector01, Vector02);//面的法矢量
		FaceNormal = FaceNormal.Normalize();//归一化法矢量
		for (int nPoint = 0; nPoint < 4; nPoint++)//顶点循环
		{
			ScreenPoint4[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);//透视投影
			N4[nPoint] = FaceNormal;
			T4[nPoint].u = F[nFace].t[nPoint].u * (pTexture->bmp.bmWidth - 1);
			T4[nPoint].v = F[nFace].t[nPoint].v * (pTexture->bmp.bmHeight - 1);
		}
		// 立方体的每一个面都是一个矩形，我们把他分成两个三角形进行显示。
		//绘制左上三角形
		CColorP3 TLP[3] = { ScreenPoint4[0] ,ScreenPoint4[2] ,ScreenPoint4[3] };
		CVector3 TLN[3] = { N4[0], N4[2], N4[3] };
		CTextureCoordinate TLT[3] = { T4[0], T4[2], T4[3] };
		pZBuffer->SetPoint(TLP, TLN, TLT);//左上三角形的顶点P，法矢量N和纹理地址T
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
		//绘制右下三角形
		CColorP3 DRP[3] = { ScreenPoint4[0] ,ScreenPoint4[1] ,ScreenPoint4[2] };
		CVector3 DRN[3] = { N4[0], N4[1], N4[2] };
		CTextureCoordinate DRT[3] = { T4[0], T4[1], T4[2] };
		pZBuffer->SetPoint(DRP, DRN, DRT);//右下三角形的顶点P，法矢量N和纹理地址T
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
	}
}