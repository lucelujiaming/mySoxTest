#include "stdafx.h"
#include "PhongBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CPhongBiquatricBezierPatch::CPhongBiquatricBezierPatch(void)
{

}


CPhongBiquatricBezierPatch::~CPhongBiquatricBezierPatch(void)
{

}

void CPhongBiquatricBezierPatch::ReadControlPoint(CColorP3 P[3][3])
{
    for(int i = 0; i < 3; i++)
    {
	   for(int j = 0;j < 3; j++)
	   {
   			this->P[i][j] = P[i][j];
	   }
   }
}

void CPhongBiquatricBezierPatch::ReadWeight(double W[3][3])
{
    for(int i = 0; i < 3; i++)
	{
	   for(int j = 0;j < 3; j++)
	   {
   			this->W[i][j] = W[i][j];
	   }
    }
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
/* ����Px,x�ǿ��Ƶ㡣wij�ǿ��ƶ����Ȩ���ӡ���B0,2(u)��B1,2(u)��B2,2(u) */
/* ��B0,2(v)��B1,2(v)��B2,2(v)�Ƕ���Bernstein��������                   */
/************************************************************************/
/* ���ݶ���Bernstein��������Ҳ����˵��                                  */
/*     B0,2(u) = (1 - u)^2 = u^2 - 2u + 1                               */
/*     B1,2(u) = 2u(1 - u) = -2u^2 + 2u                                 */
/*     B2,2(u) = u^2       = u^2                                        */
/* ͬ���У�                                                             */
/*     B0,2(v) = (1 - v)^2 = v^2 - 2v + 1                               */
/*     B1,2(v) = 2v(1 - v) = -2v^2 + 2v                                 */
/*     B2,2(v) = v^2       = v^2                                        */
/************************************************************************/
/* ��Ϊ���ǻ��Ƶ�ʱ����ʹ��u��v��0��1���л��ơ�����ϣ�               */
/* ������Ҫ��u��v����������������ǰ������չ��ʽд�����µ���ʽ��       */
/*                             |  1 -2  1 |                             */
/*   p(u, v) = [u^2 u^1 u^0] * | -2  2  0 | *                           */
/*                             |  1  0  0 |                             */
/*   | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   |  1 -2  1 |   | v^2 |   */
/*   | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | -2  2  0 | * | v   | / */
/*   | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   |  1  0  0 |   | 1   |   */
/*                   |  1 -2  1 |                                       */
/*   [u^2 u^1 u^0] * | -2  2  0 | *                                     */
/*                   |  1  0  0 |                                       */
/*   | w0,0 w0,1 w0,2 |     |  1 -2  1 |   | v^2 |                      */
/*   | w1,0 w1,1 w1,2 |  *  | -2  2  0 | * | v   |                      */
/*   | w2,0 w2,1 w2,2 |     |  1  0  0 |   | 1   |                      */
/* ע�����������ϵ������ֻ�ǿ�����һ������ʵ��Ϊת�ù�ϵ��             */
/* ����Ĵ���������������ʽ�ļ�����̡�                               */
/************************************************************************/
void CPhongBiquatricBezierPatch::SaveFacetData(void)
 {
	double M[3][3];//ϵ������Mbe
	// �����������ǰ���ᵽ�Ķ���Bernstein��������ϵ����
	// �����������B0,2(v)��B1,2(v)��B2,2(v)�����У�
	// B0,2(v) = (1 - v)^2 = v^2 - 2v + 1
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	// B1,2(v) = 2v(1 - v) = -2v^2 + 2v
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	// B2,2(v) = v^2       = v^2
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CColorP3 Pw[3][3];//���߼����ÿ��Ƶ�����
	// ���� | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |
	//      | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 |
	//      | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |
	//  ���������Pw�С�
	for(int i = 0; i < 3; i++)
    {
		for(int j = 0;j < 3;j++)
        {
			Pw[i][j] = P[i][j] * W[i][j];
        }
    }
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
		F[nFacet].SetPtNumber(4);
		F[nFacet].ptIndex[0] = nFacet / nStep + nFacet;//0
		F[nFacet].ptIndex[1] = nFacet / nStep + nFacet + 1;//1
		F[nFacet].ptIndex[2] = nFacet / nStep + nFacet + nStep + 2;//11
		F[nFacet].ptIndex[3] = nFacet / nStep + nFacet + nStep + 1;//10
	}
}

void CPhongBiquatricBezierPatch::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CPhongBiquatricBezierPatch::Draw(CDC* pDC, CVector3ZBuffer* pZBuffer)
{
	SaveFacetData();//��ȡƽ��Ƭ�ĵ�������
	CColorP3 colorEye = projection.GetColorEye();
	// ��ǰ����ͶӰ��
	CColorP3 Point[4], ScreenPoint[4];
	CVector3 N[4]; // �㷨ʸ��
	// ����Ƭ����Ϊ81��ƽ��Ƭ
	for (int nFace = 0; nFace < 81; nFace++)
	{
		// ÿ��ƽ��ƬΪ�ı���
		for (int nPoint = 0; nPoint < 4; nPoint++)
		{
			Point[nPoint] = V[F[nFace].ptIndex[nPoint]];
			// ��ά͸��ͶӰ
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// �������������ƹ��̵ĺ����߼���Ҳ����Phong��������
			// ���ǲ��ټ򵥵ص��ù��պ�����������������ɫ��
	        // 1. �������������ÿ������ķ�������
            //    ����������˵��������һ��ķ�ʸ��������������һ�㵽���ĵ�ʸ�����档
			N[nPoint] = CVector3(Point[nPoint]);
		}
		// ÿ��ƽ��ƬΪһ���ı��Σ����ǰ����ֳ����������ν�����ʾ��
		// �Ѹղż��������ÿ������ķ�������ÿ������һ���롣
		// ������������
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3], N[0], N[2], N[3]);
		// �����λ��ƺ���Ҳ�޸�Ϊ�����ӵ�͹��ն�����������ù��ռ����ǿ�������ɫ��
		pZBuffer->FillTriangle(pDC, colorEye, pScene);
		// ��������
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2], N[0], N[1], N[2]);
		// �����λ��ƺ���Ҳ�޸�Ϊ�����ӵ�͹��ն�����������ù��ռ����ǿ�������ɫ��
		pZBuffer->FillTriangle(pDC, colorEye, pScene);
	}
}

// ��˾���M*P
void CPhongBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CColorP3 P[3][3])
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			P[i][j] = T[i][j];
		}
	}
}

// ��˾���M*W
void CPhongBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = M[i][0] * W[0][j] + M[i][1] * W[1][j] + M[i][2] * W[2][j];
		}
	}
	for(i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			W[i][j] = T[i][j];
		}
	}
}

// �ҳ˾���P*M
void CPhongBiquatricBezierPatch::RightMultiplyMatrix(CColorP3 P[3][3], double M[3][3])
{
	CColorP3 T[3][3];//��ʱ����
	for(int i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j];
		}
	}
	for(i = 0;i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			P[i][j] = T[i][j];
		}
	}
}

// �ҳ˾���W*M
void CPhongBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[i][j] = W[i][0] * M[0][j] + W[i][1] * M[1][j] + W[i][2] * M[2][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			W[i][j] = T[i][j];
		}
	}
}

// ת�þ���
void CPhongBiquatricBezierPatch::TransposeMatrix(double M[3][3])
{
	double T[3][3];//��ʱ����
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			T[j][i] = M[i][j];
		}
	}
	for(i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			M[i][j] = T[i][j];
		}
	}
}
