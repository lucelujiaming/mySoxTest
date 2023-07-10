#include "stdafx.h"
#include "GouraudBicubicBezierPatch.h"
#define ROUND(d) int(d + 0.5)

CGouraudBicubicBezierPatch::CGouraudBicubicBezierPatch(void)
{

}

CGouraudBicubicBezierPatch::~CGouraudBicubicBezierPatch(void)
{

}

// ˫����Bezier��������������Bezier���߽�֯���ɣ�
// ����������16�����Ƶ���ɡ�
void CGouraudBicubicBezierPatch::ReadControlPoint(CColorP3 P[4][4])
{
   for(int i = 0;i < 4;i++)
   {
	   for(int j = 0;j < 4;j++)
	   {
   			this->P[i][j]  = P[i][j];
	   }
   }
}

void CGouraudBicubicBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

/************************************************************************/
/* ����˫��������Ƭ�Ķ��壬չ��ʽ���£�                                 */
/*   p(u, v) = [B0,3(u) B1,3(u) B2,3(u) B3,3(u)]                        */
/*       | P0,0 P0,1 P0,2 P0,3 |   | B0,3(v) |                          */
/*     * | P1,0 P1,1 P1,2 P1,3 | * | B1,3(v) |    (u, v) = [0, 1]       */
/*       | P2,0 P2,1 P2,2 P2,3 |   | B2,3(v) |           * [0, 1]       */
/*       | P3,0 P3,1 P3,2 P3,3 |   | B3,3(v) |                          */
/* ����Px,x�ǿ��Ƶ㡣��B0,3(u)��B1,3(u)��B2,3(u)��B3,3(u)               */
/* ��B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)������Bernstein��������          */
/************************************************************************/
/* ��������Bernstein��������Ҳ����˵��                                  */
/*     B0,3(u) = (1 - u)^3                                              */
/*             = -u^3 + 3u^2 - 3u + 1                                   */
/*     B1,3(u) = 3u(1 - u)^2                                            */
/*             = 3u^3 - 6u^2 + 3u                                       */
/*     B2,3(u) = 3u^2 * (1 - u)                                         */
/*             = -3u^3 + 3u^2                                           */
/*     B3,3(u) = u^3                                                    */
/* ͬ���У�                                                             */
/*     B0,3(v) = (1 - v)^3                                              */
/*             = -v^3 + 3v^2 - 3v + 1                                   */
/*     B1,3(v) = 3v(1 - v)^2                                            */
/*             = 3v^3 - 6v^2 + 3v                                       */
/*     B2,3(v) = 3v^2 * (1 - v)                                         */
/*             = -3v^3 + 3v^2                                           */
/*     B3,3(v) = v^3                                                    */
/************************************************************************/
/* ��Ϊ���ǻ��Ƶ�ʱ����ʹ��u��v��0��1���л��ơ�����ϣ�               */
/* ������Ҫ��u��v����������������ǰ������չ��ʽд�����µ���ʽ��       */
/*   p(u, v) = [u^3 u^2 u^1 u^0] *                                      */
/*        | -1  3 -3  1 |   | P0,0 P0,1 P0,2 P0,3 |                     */
/*        |  3 -6  3  0 | * | P1,0 P1,1 P1,2 P1,3 | *                   */
/*        | -3  3  0  0 |   | P2,0 P2,1 P2,2 P2,3 |                     */
/*        |  1  0  0  0 |   | P3,0 P3,1 P3,2 P3,3 |                     */
/*        | -1  3 -3  1 |   | v^3 |                                     */
/*        |  3 -6  3  0 | * | v^2 |   (u, v) = [0, 1]                   */
/*        | -3  3  0  0 |   | v^1 |          * [0, 1]                   */
/*        |  1  0  0  0 |   | v^0 |                                     */
/* ע�����������ϵ������ֻ�ǿ�����һ������ʵ��Ϊת�ù�ϵ��             */
/* ����Ĵ���������������ʽ�ļ�����̡�                               */
/* ע����ǣ������У�����˷������Ǵ�������˵ġ�Ҳ���������Ҳ�ĳ˷��� */
/************************************************************************/
void CGouraudBicubicBezierPatch::SaveFacetData(void)
{
	double M[4][4]; // ϵ������M
	// ������������������������Bernstein��������ϵ����
	// ��B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)Ϊ�������У�
	// B0,3(v) = -v^3 + 3v^2 - 3v + 1 
	M[0][0] = -1, M[0][1] = 3, M[0][2] = -3, M[0][3] = 1;
	// B1,3(v) = 3v^3 - 6v^2 + 3v 
	M[1][0] = 3,  M[1][1] = -6, M[1][2] = 3,  M[1][3] = 0;
	// B2,3(v) = -3v^3 + 3v^2 
	M[2][0] = -3, M[2][1] = 3,  M[2][2] = 0,  M[2][3] = 0;
	// B3,3(v) = v^3 
	M[3][0] = 1,  M[3][1] = 0,  M[3][2] = 0,  M[3][3] = 0;
	CColorP3 P3[4][4];//���߼����ÿ��Ƶ�����
	// ����һ�ݿ��Ƶ�����
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			P3[i][j] = P[i][j];
		}
	}
	//  ϵ�������ҳ���ά�����
	//  ���� | P0,0 P0,1 P0,2 P0,3 |   | -1  3 -3  1 |
	//       | P1,0 P1,1 P1,2 P1,3 | * |  3 -6  3  0 |
	//       | P2,0 P2,1 P2,2 P2,3 |   | -3  3  0  0 |
	//       | P3,0 P3,1 P3,2 P3,3 |   |  1  0  0  0 |
	//  ���������P3�С�
	RightMultiplyMatrix(P3, M);
	//  ����ת�þ��󡣵õ���
	//     | -1  3 -3  1 |
	//     |  3 -6  3  0 |
	//     | -3  3  0  0 |
	//     |  1  0  0  0 |
	TransposeMatrix(M);
	//  ϵ�����������ά�����
	//  ���㣺
	//    | -1  3 -3  1 |   | P0,0 P0,1 P0,2 P0,3 |
	//    |  3 -6  3  0 | * | P1,0 P1,1 P1,2 P1,3 |
	//    | -3  3  0  0 |   | P2,0 P2,1 P2,2 P2,3 |
	//    |  1  0  0  0 |   | P3,0 P3,1 P3,2 P3,3 |
	//  ���������P3�С�
	LeftMultiplyMatrix(M, P3); 
	int nStep = 9; // ����ϸ�ֵĲ���
	// ��������ʱ�������ڱ���u��v��������
	double u0,u1,u2,u3,v0,v1,v2,v3; 
	// ����ִ�е��������Ѿ����浽P3���ˡ�
	// ����ʹ��P3������ѭ��������������˫���α��������߽�����ɵ�˫���α���������Ƭ��
	// �����������ѭ�������Ȼ������·�������ߡ�
	for (int u = 0; u <= nStep; u ++)
		for (int v = 0; v <= nStep; v ++)
		{
			// ����u�����η������η���һ�η�����η���
			u3 = (u / double(nStep)) * (u / double(nStep)) * (u / double(nStep));
			u2 = (u / double(nStep)) * (u / double(nStep));
			u1 = (u / double(nStep));
			u0 = 1;
			// ����v�����η������η���һ�η�����η���
			v3 = (v / double(nStep)) * (v / double(nStep)) * (v / double(nStep));
			v2 = (v / double(nStep)) * (v / double(nStep));
			v1 = (v / double(nStep));
			v0 = 1;
			// ʹ��ǰ�����õ�P3�����p(u, v)��������V�С�
			V[u * (nStep + 1) + v] = (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
				+ (u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
				+ (u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
				+ (u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;
		}
		// 9��9��ƽ��Ƭ�����Ϣ������ÿ��ƽ��Ƭ��������
		for (int nFacet = 0; nFacet < 81; nFacet++)
		{
			F[nFacet].SetPtNumber(4);
			F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;
			F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;
			F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;
			F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;
		}
}

void CGouraudBicubicBezierPatch::Draw(CDC* pDC, CZBuffer* pZBuffer)
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


void CGouraudBicubicBezierPatch::LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4])//��˾���M*P
{
	CColorP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j] + M[i][3] * P[3][j];
	for(i = 0;i < 4;i++)
		for(int j =0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}
void CGouraudBicubicBezierPatch::RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4])//�ҳ˾���P*M
{
	CColorP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for(i=0;i < 4;i++)
		for(int j=0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}

void CGouraudBicubicBezierPatch::TransposeMatrix(double M[4][4])//ת�þ���
{
	double PTemp[4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp[j][i] = M[i][j];
	for(i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			M[i][j] = PTemp[i][j];
}