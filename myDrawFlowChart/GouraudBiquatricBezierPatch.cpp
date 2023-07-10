#include "stdafx.h"
#include "GouraudBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CGouraudBiquatricBezierPatch::CGouraudBiquatricBezierPatch(void)
{

}


CGouraudBiquatricBezierPatch::~CGouraudBiquatricBezierPatch(void)
{

}

void CGouraudBiquatricBezierPatch::ReadControlPoint(CColorP3 P[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->P[i][j] = P[i][j];
}

void CGouraudBiquatricBezierPatch::ReadWeight(double W[3][3])
{
   for(int i = 0; i < 3; i++)
	   for(int j = 0;j < 3; j++)
   			this->W[i][j] = W[i][j];
}

/************************************************************************/
/* ����˫����Bezier����Ƭ�Ķ������£�                                   */
/*   p(u, v) = [B0,2(u) B1,2(u) B2,2(u)]                                */
/*       | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |          */
/*     * | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |  /       */
/*       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |          */
/*                                | w0,0 w0,1 w0,2 |   | B0,2(v) |      */
/*    [B0,2(u) B1,2(u) B2,2(u)] * | w1,0 w1,1 w1,2 | * | B1,2(v) |      */
/*                                | w2,0 w2,1 w2,2 |   | B2,2(v) |      */
/* ����B0,2(u)��B1,2(u)��B2,2(u)                                        */
/* ��B0,2(v)��B1,2(v)��B2,2(v)�Ƕ���Bernstein��������                   */
/************************************************************************/
/* Ҳ����˵��                                                           */
/*     B0,2(u) = (1 - u)^2 = u^2 - 2u + 1                               */
/*     B1,2(u) = 2u(1 - u) = -2u^2 + 2u                                 */
/*     B2,2(u) = u^2       = u^2                                        */
/* ͬ���У�                                                             */
/*     B0,2(v) = (1 - v)^2 = v^2 - 2v + 1                               */
/*     B1,2(v) = 2v(1 - v) = -2v^2 + 2v                                 */
/*     B2,2(v) = v^2       = v^2                                        */
/* ����Ĵ������ʹ��B0,2(u)��B1,2(u)��B2,2(u)                          */
/* ��B0,2(v)��B1,2(v)��B2,2(v)����p(u, v)�Ĺ��̡�                       */
/************************************************************************/
void CGouraudBiquatricBezierPatch::SaveFacetData(void)
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
			// ����u�Ķ��η���һ�η�����η���
			u2 = (u / double(nStep)) * (u / double(nStep)), u1 = u / double(nStep), u0 = 1;

			// ����v�Ķ��η���һ�η�����η���
			v2 = (v / double(nStep)) * (v / double(nStep)), v1 = v / double(nStep), v0 = 1;

			// ʹ��ǰ�����õ�Pw�����p(u, v)�ķ��ӡ�������numerator�С�
			CColorP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
				               + (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
				               + (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			// ʹ��ǰ�����õ�W�����p(u, v)�ĵķ�ĸ��������denominator�С�
			double denominator = (u2 * W[0][0] + u1 * W[1][0] + u0 * W[2][0]) * v2
			                   + (u2 * W[0][1] + u1 * W[1][1] + u0 * W[2][1]) * v1
				               + (u2 * W[0][2] + u1 * W[1][2] + u0 * W[2][2]) * v0;
			// ʹ��ǰ�����õķ��Ӻͷ�ĸ������p(u, v)��ֵ��
			V[u * (nStep + 1) + v] = numerator / denominator;
		}
	}
	// 9��9��ƽ��Ƭ�����Ϣ������ÿ��ƽ��Ƭ��������
	for (int nFacet = 0; nFacet < 81; nFacet++)
	{
		F[nFacet].SetPtNumber(4);		
		F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;//0
		F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;//1
		F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;//11
		F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;//10		
	}
}

void CGouraudBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CGouraudBiquatricBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
{
	SaveFacetData();//��ȡƽ��Ƭ�ĵ�������
	CColorP3 colorEye = projection.GetColorEye();
	CColorP3 Point[4], ScreenPoint[4];//��ǰ����ͶӰ��
	// ����Ƭ����Ϊ81��ƽ��Ƭ
	for (int nFace = 0; nFace < 81; nFace++)
	{
		// ÿ��ƽ��ƬΪ�ı���
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{
			// ȡ��ƽ��Ƭ�ı��ε�һ����������ꡣ
			Point[nPoint] = V[F[nFace].ptIndex[nPoint]];
			// ���������ʹ����ά͸��ͶӰͶӰ����ά��������ʾ��
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// ���ù��պ�����������������ɫ��
			ScreenPoint[nPoint].c = pScene->Illuminate(colorEye, Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);
		}
		// ÿ��ƽ��ƬΪһ���ı��Σ����ǰ����ֳ����������ν�����ʾ��
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); // ��������
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]); // ��������
		pZBuffer->FillTriangle(pDC);
	}
}

void CGouraudBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])//��˾���M*P
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];			
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CGouraudBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//��˾���M*W
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];		
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CGouraudBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])//�ҳ˾���P*M
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)	
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
	for(i = 0;i < 3; i++)
		for(int j = 0;j < 3;j++)
			P[i][j] = T[i][j];
}

void CGouraudBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//�ҳ˾���W*M
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			W[i][j] = T[i][j];
}

void CGouraudBiquatricBezierPatch::TransposeMatrix(double M[3][3])//ת�þ���
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			T[j][i] = M[i][j];
	for(i = 0;i < 3;i++)
		for(int j = 0;j < 3;j++)
			M[i][j] = T[i][j];
}