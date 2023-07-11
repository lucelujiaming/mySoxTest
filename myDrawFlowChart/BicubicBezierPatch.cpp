#include "stdafx.h"
#include "BicubicBezierPatch.h"
#define ROUND(d) int(d + 0.5)

CBicubicBezierPatch::CBicubicBezierPatch(void)
{
}

CBicubicBezierPatch::~CBicubicBezierPatch(void)
{

}

// ˫����Bezier��������������Bezier���߽�֯���ɣ�
// ����������16�����Ƶ���ɡ�
void CBicubicBezierPatch::ReadControlPoint(CP3 objBezierControlPoint[4][4])
{
   for(int i = 0;i < 4;i++)
   {
	   for(int j = 0;j < 4;j++)
	   {
   			this->m_objBezierControlPoint[i][j]  = objBezierControlPoint[i][j];
	   }
   }
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
void CBicubicBezierPatch::DrawCurvedPatch(CDC* pDC)
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
	CP3 objCopyPoint[4][4]; // ���߼����ÿ��Ƶ�����
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
	double tStep = 0.1;//����
	// ��������ʱ�������ڱ���u��v��������
	double u0,u1,u2,u3,v0,v1,v2,v3;
	// ����ִ�е��������Ѿ����浽P3���ˡ�
	// ����ʹ��P3������ѭ��������������˫���α��������߽�����ɵ�˫���α���������Ƭ��
	// �����������ѭ�������Ȼ������·�������ߡ�
	for(double u = 0; u <= 1; u += tStep)
	{
		for(double v = 0;v <= 1;v += tStep)
		{
			// ����u�����η������η���һ�η�����η���
			u3 = u * u * u, u2 = u * u, u1 = u, u0 = 1;
			// ����v�����η������η���һ�η�����η���
			v3 = v * v * v, v2 = v * v, v1 = v, v0 = 1;
			// ʹ��ǰ�����õ�P3�����p(u, v)��������pt�С�
			CP3 pt =  (u3 * objCopyPoint[0][0] + u2 * objCopyPoint[1][0] + u1 * objCopyPoint[2][0] + u0 * objCopyPoint[3][0]) * v3
			         +(u3 * objCopyPoint[0][1] + u2 * objCopyPoint[1][1] + u1 * objCopyPoint[2][1] + u0 * objCopyPoint[3][1]) * v2
			         +(u3 * objCopyPoint[0][2] + u2 * objCopyPoint[1][2] + u1 * objCopyPoint[2][2] + u0 * objCopyPoint[3][2]) * v1
			         +(u3 * objCopyPoint[0][3] + u2 * objCopyPoint[1][3] + u1 * objCopyPoint[2][3] + u0 * objCopyPoint[3][3]) * v0;
			// ͶӰ����ά��������ʾ��
			CP2 Point2 = projection.CustomProjection(pt); // бͶӰ
			// ��������
			if(0 == v)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}
	}
	// �����������ѭ��������u��v˳�򣬻������ҷ�������ߡ�
	// ����Ӽ���ĽǶ���˵����ʵ���ظ�������������Ϊ������Ҫ���ƽ����������������棬
	// ����ϣ����ǲ��ò��ظ����㲢�������ߡ�
	for(double v = 0; v <= 1; v += tStep)
	{
		for(double u = 0;u <= 1;u += tStep)
		{
			// ����u�����η������η���һ�η�����η���
			u3 = u * u * u; u2 = u * u; u1 = u; u0 = 1;
			// ����v�����η������η���һ�η�����η���
			v3 = v * v * v; v2 = v * v; v1 = v; v0 = 1;
			// ʹ��ǰ�����õ�P3�����p(u, v)��������pt�С�
			CP3 pt = (u3 * objCopyPoint[0][0] + u2 * objCopyPoint[1][0] + u1 * objCopyPoint[2][0] + u0 * objCopyPoint[3][0]) * v3
			        +(u3 * objCopyPoint[0][1] + u2 * objCopyPoint[1][1] + u1 * objCopyPoint[2][1] + u0 * objCopyPoint[3][1]) * v2
			        +(u3 * objCopyPoint[0][2] + u2 * objCopyPoint[1][2] + u1 * objCopyPoint[2][2] + u0 * objCopyPoint[3][2]) * v1
			        +(u3 * objCopyPoint[0][3] + u2 * objCopyPoint[1][3] + u1 * objCopyPoint[2][3] + u0 * objCopyPoint[3][3]) * v0;
			// ͶӰ����ά��������ʾ��
			CP2 Point2 = projection.CustomProjection(pt); // бͶӰ
			if(0 == u)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x),ROUND(Point2.y));
		}
	}
}

void CBicubicBezierPatch::LeftMultiplyMatrix(double M[4][4],CP3 P[4][4])//��˾���M*P
{
	CP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = M[i][0] * P[0][j] + M[i][1] * P[1][j] + M[i][2] * P[2][j] + M[i][3] * P[3][j];
	for(i = 0;i < 4;i++)
		for(int j =0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}

void CBicubicBezierPatch::RightMultiplyMatrix(CP3 P[4][4],double M[4][4])//�ҳ˾���P*M
{
	CP3 PTemp [4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp [i][j] = P[i][0] * M[0][j] + P[i][1] * M[1][j] + P[i][2] * M[2][j] + P[i][3] * M[3][j];
	for(i=0;i < 4;i++)
		for(int j=0;j < 4;j++)
			P[i][j] = PTemp [i][j];
}

void CBicubicBezierPatch::TransposeMatrix(double M[][4])//ת�þ���
{
	double PTemp[4][4];//��ʱ����
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			PTemp[j][i] = M[i][j];
	for(i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			M[i][j] = PTemp[i][j];
}

void CBicubicBezierPatch::DrawControlGrid(CDC* pDC)//���ƿ�������
{
	CP2 P2[4][4]; // ��ά���Ƶ�
	// ��ÿһ�����Ƶ�ͶӰ����ά��������ʾ��
	for(int i = 0;i < 4;i++)
	{
		for(int j = 0;j < 4;j++)
		{
			P2[i][j] = projection.CustomProjection(m_objBezierControlPoint[i][j]);
		}
	}
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	pOldPen=pDC->SelectObject(&NewPen);
	// �����������ѭ�����������·���Ŀ��Ƶ����ߡ�
	for(i = 0; i < 4; i++)
	{
		pDC->MoveTo(ROUND(P2[i][0].x), ROUND(P2[i][0].y));
		for(int j = 1;j < 4;j++)
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	// �����������ѭ��������i��j˳�򣬻������ҷ���Ŀ��Ƶ����ߡ�
	for(int j = 0;j < 4;j++)
	{
		pDC->MoveTo(ROUND(P2[0][j].x), ROUND(P2[0][j].y));
		for(int i = 1;i < 4;i++)
		{
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
		}
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}