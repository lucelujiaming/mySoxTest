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
void CGouraudBicubicBezierPatch::ReadControlPoint(CColorP3 objBezierControlPoint[4][4])
{
   for(int i = 0;i < 4;i++)
   {
	   for(int j = 0;j < 4;j++)
	   {
   			this->m_objBezierControlPoint[i][j]  = objBezierControlPoint[i][j];
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
	// �����������ǰ���ᵽ������Bernstein��������ϵ����
	// ��B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)Ϊ�������У�
	// B0,3(v) = -v^3 + 3v^2 - 3v + 1 
	M[0][0] = -1, M[0][1] = 3, M[0][2] = -3, M[0][3] = 1;
	// B1,3(v) = 3v^3 - 6v^2 + 3v 
	M[1][0] = 3,  M[1][1] = -6, M[1][2] = 3,  M[1][3] = 0;
	// B2,3(v) = -3v^3 + 3v^2 
	M[2][0] = -3, M[2][1] = 3,  M[2][2] = 0,  M[2][3] = 0;
	// B3,3(v) = v^3 
	M[3][0] = 1,  M[3][1] = 0,  M[3][2] = 0,  M[3][3] = 0;
	CColorP3 objCopyPoint[4][4]; // ���߼����ÿ��Ƶ�����
	// ����һ�ݿ��Ƶ�����
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			objCopyPoint[i][j] = m_objBezierControlPoint[i][j];
		}
	}
	//  ϵ�������ҳ���ά�����
	//  ���� | P0,0 P0,1 P0,2 P0,3 |   | -1  3 -3  1 |
	//       | P1,0 P1,1 P1,2 P1,3 | * |  3 -6  3  0 |
	//       | P2,0 P2,1 P2,2 P2,3 |   | -3  3  0  0 |
	//       | P3,0 P3,1 P3,2 P3,3 |   |  1  0  0  0 |
	//  ���������P3�С�
	RightMultiplyMatrix(objCopyPoint, M);
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
	LeftMultiplyMatrix(M, objCopyPoint);
	int nStep = 9; // ����ϸ�ֵĲ���
	// ��������ʱ�������ڱ���u��v��������
	double u0,u1,u2,u3,v0,v1,v2,v3; 
	// ����ִ�е��������Ѿ����浽P3���ˡ�
	// ����ʹ��P3������ѭ��������������˫���α��������߽�����ɵ�˫���α���������Ƭ��
	// �����������ѭ�������Ȼ������·�������ߡ�
	for (int u = 0; u <= nStep; u ++)
	{
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
			m_objBezierPatchPoint[u * (nStep + 1) + v] = 
				  (u3 * objCopyPoint[0][0] + u2 * objCopyPoint[1][0] + u1 * objCopyPoint[2][0] + u0 * objCopyPoint[3][0]) * v3
				+ (u3 * objCopyPoint[0][1] + u2 * objCopyPoint[1][1] + u1 * objCopyPoint[2][1] + u0 * objCopyPoint[3][1]) * v2
				+ (u3 * objCopyPoint[0][2] + u2 * objCopyPoint[1][2] + u1 * objCopyPoint[2][2] + u0 * objCopyPoint[3][2]) * v1
				+ (u3 * objCopyPoint[0][3] + u2 * objCopyPoint[1][3] + u1 * objCopyPoint[2][3] + u0 * objCopyPoint[3][3]) * v0;
		}
	}
	/************************************************************************/
	/* 9��9��ƽ��Ƭ�����Ϣ������ÿ��ƽ��Ƭ�������š�                        */
	/* ����Ĵ�������ĸ�����ÿһ��С���Ӧ���ĸ���������������             */
	/* ֻҪ�ֹ��г���Щ��������Ϳ��Կ����״����ˡ�100��������������£�    */
	/*      0  1  2  3  4  5  6  7  8  9                                    */
	/*     10 11 12 13 14 15 16 17 18 19                                    */
	/*     20 21 22 23 24 25 26 27 28 29                                    */
	/*     30 31 32 33 34 35 36 37 38 39                                    */
	/*     40 41 42 43 44 45 46 47 48 49                                    */
	/*     50 51 52 53 54 55 56 57 58 59                                    */
	/*     60 61 62 63 64 65 66 67 68 69                                    */
	/*     70 71 72 73 74 75 76 77 78 79                                    */
	/*     80 81 82 83 84 85 86 87 88 89                                    */
	/*     90 91 92 93 94 95 96 97 98 99                                    */
	/* ��m_objBezierPatchFace[0]Ϊ������Ӧ���ĸ�����ǡ��                    */
	/* ��(0, 1, 11, 10)��Ҳ������������Ͻǵ��ĸ��㡣                       */
	/* ��m_objBezierPatchFace[80]Ϊ������Ӧ���ĸ�����ǡ��                   */
	/* ��(88, 89, 99, 98)��Ҳ������������Ͻǵ��ĸ��㡣                     */
	/************************************************************************/
	for (int nFacet = 0; nFacet < 81; nFacet++)
	{
		m_objBezierPatchFace[nFacet].SetPtNumber(4);
		m_objBezierPatchFace[nFacet].ptIndex[0] = nFacet / nStep + nFacet;
		m_objBezierPatchFace[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;
		m_objBezierPatchFace[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;
		m_objBezierPatchFace[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;
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
			Point[nPoint] = m_objBezierPatchPoint[
								m_objBezierPatchFace[nFace].ptIndex[nPoint]];
			// ���������ʹ����ά͸��ͶӰͶӰ����ά��������ʾ��
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(
									m_objBezierPatchPoint[
										m_objBezierPatchFace[nFace].ptIndex[nPoint]]);
			// ���ù��պ�����������������ɫ��
			ScreenPoint[nPoint].c = pScene->Illuminate(colorEye, 
						Point[nPoint], CVector3(Point[nPoint]), pScene->pMaterial);
		}
		// ÿ��ƽ��ƬΪһ���ı��Σ����ǰ����ֳ����������ν�����ʾ��
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); // ��������
		pZBuffer->FillTriangle(pDC);
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]); // ��������
		pZBuffer->FillTriangle(pDC);
	}
}
// ��˾���M*P
void CGouraudBicubicBezierPatch::LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4])
{
	CColorP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
	{
		for(int j = 0;j < 4;j++)
		{
			PTemp [i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + 
		                   M[i][2] * P[2][j] + M[i][3] * P[3][j];
		}
	}
	for(i = 0;i < 4;i++)
	{
		for(int j =0;j < 4;j++)
		{
			P[i][j] = PTemp[i][j];
		}
	}
}
// �ҳ˾���P*M
void CGouraudBicubicBezierPatch::RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4])
{
	CColorP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + 
		                   P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for(i=0;i < 4;i++)
		for(int j=0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}
// ת�þ���
void CGouraudBicubicBezierPatch::TransposeMatrix(double M[4][4])
{
	double PTemp[4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp[j][i] = M[i][j];
	for(i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			M[i][j] = PTemp[i][j];
}