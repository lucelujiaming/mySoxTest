#include "stdafx.h"
#include "BumpTextureBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CBumpTextureBiquatricBezierPatch::CBumpTextureBiquatricBezierPatch(void)
{

}


CBumpTextureBiquatricBezierPatch::~CBumpTextureBiquatricBezierPatch(void)
{

}

void CBumpTextureBiquatricBezierPatch::ReadControlPoint(CColorP3 P[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->P[i][j] = P[i][j];
}

void CBumpTextureBiquatricBezierPatch::ReadWeight(double W[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->W[i][j] = W[i][j];
}

void CBumpTextureBiquatricBezierPatch::SaveFacetData(void)
 {
	double M[3][3];//ϵ������Mbe
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CColorP3 Pw[3][3];//���߼����ÿ��Ƶ�����
	for(int i = 0; i < 3; i++)
		for(int j = 0;j < 3;j++)
			Pw[i][j] = P[i][j] * W[i][j];
	RightMultiplyMatrix(Pw, M);//ϵ�������ҳ���ά�����
	RightMultiplyMatrix(W, M);//ϵ�������ҳ�Ȩ���Ӿ���
	TransposeMatrix(M);//����ת�þ���
	LeftMultiplyMatrix(M, Pw);//ϵ�����������ά�����
	LeftMultiplyMatrix(M, W);//ϵ���������Ȩ���Ӿ���
	int nStep = 9;//����ϸ�ֲ�����10���ߣ�9��
	double u0, u1, u2, v0, v1, v2;//u��v��������
	for (int u = 0; u < nStep + 1; u++)
	{
		for (int v = 0; v < nStep + 1; v++)
		{
			u2 = (u / double(nStep)) * (u / double(nStep)), u1 = u / double(nStep), u0 = 1;
			v2 = (v / double(nStep)) * (v / double(nStep)), v1 = v / double(nStep), v0 = 1;
			CColorP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
				+ (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
				+ (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			double denominator = (u2 * W[0][0] + u1 * W[1][0] + u0 * W[2][0]) * v2
				+ (u2 * W[0][1] + u1 * W[1][1] + u0 * W[2][1]) * v1
				+ (u2 * W[0][2] + u1 * W[1][2] + u0 * W[2][2]) * v0;
			V[u * (nStep + 1) + v] = numerator / denominator;
			T[u * (nStep + 1) + v].u = (pTexture->bmp.bmWidth - 1) * u / double(nStep);
			T[u * (nStep + 1) + v].v = (pTexture->bmp.bmHeight - 1) * v / double(nStep);
		}		
	}
	for (int nFacet = 0; nFacet < 81; nFacet++)//9��9��ƽ��Ƭ�����Ϣ������ÿ��ƽ��Ƭ��������
	{
		F[nFacet].SetPtNumber(4);		
		F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;//0
		F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;//1
		F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;//11
		F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;//10		
	}
}

void CBumpTextureBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CBumpTextureBiquatricBezierPatch::SetTexture(CTexture* pTexture)
{
	this->pTexture = pTexture;
}

void CBumpTextureBiquatricBezierPatch::Draw(CDC* pDC, CBumpTextureZBuffer* pZBuffer)
{
	SaveFacetData();//��ȡƽ��Ƭ�ĵ�������
	CColorP3 Eye = projection.GetColorEye();
	CColorP3 ScreenPoint4[4];//��ǰ����ͶӰ��
	CVector3 N4[4];//�㷨ʸ��
	CTextureCoordinate T4[4];//����
	for (int nFace = 0; nFace < 81; nFace++)//����Ƭ����Ϊ81��ƽ��Ƭ
	{
		// ÿ��ƽ��ƬΪ�ı���
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{				
			int nVertexIndex = F[nFace].ptIndex[nPoint];
			ScreenPoint4[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[nVertexIndex]);//��ά͸��ͶӰ
			N4[nPoint] = CVector3(V[nVertexIndex]);
			T4[nPoint] = T[nVertexIndex];
		}
		// ÿ��ƽ��ƬΪһ���ı��Σ����ǰ����ֳ����������ν�����ʾ��
		// ��������������
		CColorP3 TLP[3] = { ScreenPoint4[0] ,ScreenPoint4[2] ,ScreenPoint4[3] };
		CVector3 TLN[3] = { N4[0], N4[2], N4[3] };
		CTextureCoordinate TLT[3] = { T4[0], T4[2], T4[3] };
		pZBuffer->SetPoint(TLP, TLN, TLT);//���������εĶ���P����ʸ��N�������ַT
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
		// ��������������
		CColorP3 DRP[3] = { ScreenPoint4[0] ,ScreenPoint4[1] ,ScreenPoint4[2] };
		CVector3 DRN[3] = { N4[0], N4[1], N4[2] };
		CTextureCoordinate DRT[3] = { T4[0], T4[1], T4[2] };//���������εĶ���P����ʸ��N�������ַT
		pZBuffer->SetPoint(DRP, DRN, DRT);
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
	}
}

void CBumpTextureBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])//��˾���M*P
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];			
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CBumpTextureBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//��˾���M*W
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];		
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CBumpTextureBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])//�ҳ˾���P*M
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)	
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CBumpTextureBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//�ҳ˾���W*M
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CBumpTextureBiquatricBezierPatch::TransposeMatrix(double M[3][3])//ת�þ���
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[j][i] = M[i][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			M[i][j] = T[i][j];
}