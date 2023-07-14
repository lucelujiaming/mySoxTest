#include "stdafx.h"
#include "RationalBiquatricBezierPatch.h"
#define ROUND(d) int(d+0.5)

CRationalBiquatricBezierPatch::CRationalBiquatricBezierPatch(void)
{
}

CRationalBiquatricBezierPatch::~CRationalBiquatricBezierPatch(void)
{
}

// ����˫����Bezier�������������Bezier���߽�֯���ɣ�
// ����������9�����Ƶ��9��Ȩ��������ɡ�
void CRationalBiquatricBezierPatch::ReadControlPoint(CP3 objBezierControlPoint[3][3])
{
   for(int i = 0; i < 3; i++)
   {
	   for(int j = 0;j < 3; j++)
	   {
   			this->m_objBezierControlPoint[i][j] = objBezierControlPoint[i][j];
	   }
   }
}

// ˫����Bezier�������������Bezier���߽�֯���ɣ�
// ����������9�����Ƶ��9��Ȩ��������ɡ�
void CRationalBiquatricBezierPatch::ReadWeight(double objWeightFactors[3][3])
{
   for(int i = 0; i < 3; i++)
   {
	   for(int j = 0;j < 3; j++)
	   {
   			this->m_objWeightFactors[i][j] = objWeightFactors[i][j];
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
void CRationalBiquatricBezierPatch::DrawCurvedPatch(CDC* pDC)
 {
	double M[3][3]; // ϵ������Mbe
	// �����������ǰ���ᵽ�Ķ���Bernstein��������ϵ����
	// �����������B0,2(v)��B1,2(v)��B2,2(v)�����У�
	// B0,2(v) = (1 - v)^2 = v^2 - 2v + 1
	M[0][0] = 1, M[0][1] =-2, M[0][2] = 1;
	// B1,2(v) = 2v(1 - v) = -2v^2 + 2v
	M[1][0] =-2, M[1][1] = 2, M[1][2] = 0;
	// B2,2(v) = v^2       = v^2
	M[2][0] = 1, M[2][1] = 0, M[2][2] = 0;
	CP3 Pw[3][3]; // ���߼����ÿ��Ƶ�����
	// ���� | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |
	//      | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 |
	//      | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |
	//  ���������Pw�С�
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0;j < 3;j++)
		{
			Pw[i][j] = m_objBezierControlPoint[i][j] * m_objWeightFactors[i][j];
		}
	}
	//  ϵ�������ҳ���ά�����
	//  ���� | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |
	//       | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |
	//       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |
	//  ���������Pw�С�
	RightMultiplyMatrix(Pw, M);
	// ϵ�������ҳ�Ȩ���Ӿ���
	//  ����  | w0,0 w0,1 w0,2 |   | B0,2(v) |
	//        | w1,0 w1,1 w1,2 | * | B1,2(v) |
	//        | w2,0 w2,1 w2,2 |   | B2,2(v) |
	//  ���������W�С�
	RightMultiplyMatrix(m_objWeightFactors, M);
	//  ����ת�þ��󡣵õ���
	//     [B0,2(u)��B1,2(u)��B2,2(u)]
	TransposeMatrix(M);//����ת�þ���
	// ϵ�����������ά�����
	//  ���� [B0,2(u) B1,2(u) B2,2(u)] *
	//       | w0,0 * P0,0 w0,1 * P0,1 w0,2 * P0,2 |   | B0,2(v) |
	//       | w1,0 * P1,0 w1,1 * P1,1 w1,2 * P1,2 | * | B1,2(v) |
	//       | w2,0 * P2,0 w2,1 * P2,1 w2,2 * P2,2 |   | B2,2(v) |
	//  ���������Pw�С�
	LeftMultiplyMatrix(M, Pw);
	// ϵ���������Ȩ���Ӿ���
	//  ���� [B0,2(u) B1,2(u) B2,2(u)] *
	//        | w0,0 w0,1 w0,2 |   | B0,2(v) |
	//        | w1,0 w1,1 w1,2 | * | B1,2(v) |
	//        | w2,0 w2,1 w2,2 |   | B2,2(v) |
	//  ���������W�С�
	LeftMultiplyMatrix(M, m_objWeightFactors);
	double tStep = 0.1;//t�Ĳ���
	// ��������ʱ�������ڱ���u��v��������
	double u0, u1, u2, v0, v1, v2;
	// ����ִ�е��������Ѿ����浽Pw��W���ˡ�
	// ����ʹ��Pw��W������ѭ�������ƽ�������Ƭ��
	for(double u = 0; u <= 1; u += tStep)
	{
		for(double v = 0; v <= 1; v += tStep)
		{
			// ����u�Ķ��η���һ�η�����η���
			u2 = u * u, u1 = u, u0 = 1;
			// ����v�Ķ��η���һ�η�����η���
			v2 = v * v, v1 = v, v0 = 1;

			// ʹ��ǰ�����õ�Pw�����p(u, v)�ķ��ӡ�������numerator�С�
			CP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
			               +(u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
			               +(u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			// ʹ��ǰ�����õ�W�����p(u, v)�ĵķ�ĸ��������denominator�С�
			double denominator = (u2 * m_objWeightFactors[0][0] + u1 * m_objWeightFactors[1][0] + u0 * m_objWeightFactors[2][0]) * v2
			                   + (u2 * m_objWeightFactors[0][1] + u1 * m_objWeightFactors[1][1] + u0 * m_objWeightFactors[2][1]) * v1
			                   + (u2 * m_objWeightFactors[0][2] + u1 * m_objWeightFactors[1][2] + u0 * m_objWeightFactors[2][2]) * v0;
			// ʹ��ǰ�����õķ��Ӻͷ�ĸ������p(u, v)��ֵ��
			CP3 pt = numerator / denominator;
			// ͶӰ����ά��������ʾ��
			CP2 Point2 = projection.PerspectiveProjection(pt); // ͶӰ
			// ��������
			if(v == 0)
            {
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
            }
			else
            {
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
            }
		}
	}
	// �����������ѭ��������u��v˳�򣬻������ҷ�������ߡ�
	// ����Ӽ���ĽǶ���˵����ʵ���ظ�������������Ϊ������Ҫ���ƽ����������������棬
	// ����ϣ����ǲ��ò��ظ����㲢�������ߡ�
	for(double v = 0; v <= 1;v += tStep)
	{
		for(double u = 0;u <= 1;u += tStep)
		{
			// ����u�Ķ��η���һ�η�����η���
			u2 = u * u, u1 = u, u0 = 1;
			// ����v�Ķ��η���һ�η�����η���
			v2 = v * v, v1 = v, v0 = 1;
			// ʹ��ǰ�����õķ��Ӻͷ�ĸ������p(u, v)��ֵ��
			CP3 numerator = (u2 * Pw[0][0] + u1 * Pw[1][0] + u0 * Pw[2][0]) * v2
			              + (u2 * Pw[0][1] + u1 * Pw[1][1] + u0 * Pw[2][1]) * v1
			              + (u2 * Pw[0][2] + u1 * Pw[1][2] + u0 * Pw[2][2]) * v0;
			// ʹ��ǰ�����õ�W�����p(u, v)�ĵķ�ĸ��������denominator�С�
			double denominator = (u2 * m_objWeightFactors[0][0] + u1 * m_objWeightFactors[1][0] + u0 * m_objWeightFactors[2][0]) * v2
			                    +(u2 * m_objWeightFactors[0][1] + u1 * m_objWeightFactors[1][1] + u0 * m_objWeightFactors[2][1]) * v1
			                    +(u2 * m_objWeightFactors[0][2] + u1 * m_objWeightFactors[1][2] + u0 * m_objWeightFactors[2][2]) * v0;
			// ʹ��ǰ�����õķ��Ӻͷ�ĸ������p(u, v)��ֵ��
			CP3 pt = numerator / denominator;
			// ͶӰ����ά��������ʾ��
			CP2 Point2 = projection.PerspectiveProjection(pt);//ͶӰ
			// ��������
			if(0 == u)
            {
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
            }
			else
            {
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
            }
		}
	}
}

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], CP3 P[3][3])//��˾���M*P
{
	CP3 T[3][3];//��ʱ����
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

void CRationalBiquatricBezierPatch::LeftMultiplyMatrix(double M[3][3], double W[3][3])//��˾���M*W
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

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(CP3 P[3][3], double M[3][3])//�ҳ˾���P*M
{
	CP3 T[3][3];//��ʱ����
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

void CRationalBiquatricBezierPatch::RightMultiplyMatrix(double W[3][3], double M[3][3])//�ҳ˾���W*M
{
	double T[3][3]; // ��ʱ����
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

void CRationalBiquatricBezierPatch::TransposeMatrix(double M[3][3])//ת�þ���
{
	double T[3][3]; // ��ʱ����
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

void CRationalBiquatricBezierPatch::DrawControlGrid(CDC* pDC) // ���ƿ�������
{
	CP2 P2[3][3];//��ά���Ƶ�
	for(int i = 0;i < 3;i++)
    {
		for(int j = 0;j < 3;j++)
        {
			P2[i][j] = projection.PerspectiveProjection(m_objBezierControlPoint[i][j]);
        }
    }
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0, 0, 255));
	pOldPen=pDC->SelectObject(&NewPen);
	for(i = 0;i < 3;i++)
	{
		pDC->MoveTo(ROUND(P2[i][0].x), ROUND(P2[i][0].y));
		for(int j = 1;j < 3;j++)
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	for(int j = 0;j < 3;j++)
	{
		pDC->MoveTo(ROUND(P2[0][j].x), ROUND(P2[0][j].y));
		for(int i = 1;i < 3;i++)
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}
