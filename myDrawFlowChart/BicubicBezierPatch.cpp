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
void CBicubicBezierPatch::ReadControlPoint(CP3 P[4][4])
{
   for(int i = 0;i < 4;i++)
   {
	   for(int j = 0;j < 4;j++)
	   {
   			this->P[i][j]  = P[i][j];
	   }
   }
}

/************************************************************************/
/* ˫��������Ƭ�Ķ������£�                                             */
/*   p(u, v) = [B0,3(u) B1,3(u) B2,3(u) B3,3(u)]                        */
/*       | P0,0 P0,1 P0,2 P0,3 |   | B0,3(v) |                          */
/*     * | P1,0 P1,1 P1,2 P1,3 | * | B1,3(v) |                          */
/*       | P2,0 P2,1 P2,2 P2,3 |   | B2,3(v) |                          */
/*       | P3,0 P3,1 P3,2 P3,3 |   | B3,3(v) |                          */
/* ����B0,3(u)��B1,3(u)��B2,3(u)��B3,3(u)                               */
/* ��B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)������Bernstein��������          */
/************************************************************************/
/* Ҳ����˵��                                                           */
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
/* ����Ĵ������ʹ��B0,3(u)��B1,3(u)��B2,3(u)��B3,3(u)                 */
/* ��B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)����p(u, v)�Ĺ��̡�              */
/* ע����ǣ������У�����˷������Ǵ�������˵ġ�Ҳ���������Ҳ�ĳ˷��� */
/************************************************************************/
void CBicubicBezierPatch::DrawCurvedPatch(CDC* pDC)
{
	double M[4][4]; // ϵ������M
	// �����������B0,3(v)��B1,3(v)��B2,3(v)��B3,3(v)�����У�
	// B0,3(v) = -v^3 + 3v^2 - 3v + 1 
	M[0][0]=-1,M[0][1]=3, M[0][2]=-3,M[0][3]=1;
	// B1,3(v) = 3v^3 - 6v^2 + 3v 
	M[1][0]= 3,M[1][1]=-6,M[1][2]= 3,M[1][3]=0;
	// B2,3(v) = -3v^3 + 3v^2 
	M[2][0]=-3,M[2][1]=3, M[2][2]= 0,M[2][3]=0;
	// B3,3(v) = v^3 
	M[3][0]=1, M[3][1]=0, M[3][2]= 0,M[3][3]=0;
	CP3 P3[4][4]; // ���߼����ÿ��Ƶ�����
	// ����һ�ݿ��Ƶ�����
	for(int i=0;i < 4;i++)
	{
		for(int j = 0;j < 4;j++)
		{
			P3[i][j] = P[i][j];
		}
	}
	//  ϵ�������ҳ���ά�����
	//  ���� | P0,0 P0,1 P0,2 P0,3 |   | B0,3(v) |
	//       | P1,0 P1,1 P1,2 P1,3 | * | B1,3(v) |
	//       | P2,0 P2,1 P2,2 P2,3 |   | B2,3(v) |
	//       | P3,0 P3,1 P3,2 P3,3 |   | B3,3(v) |
	//  ���������P3�С�
	RightMultiplyMatrix(P3, M);
	//  ����ת�þ��󡣵õ���
	//     [B0,3(u) B1,3(u) B2,3(u) B3,3(u)]
	TransposeMatrix(M);
	//  ϵ�����������ά�����
	//  ���� [B0,3(u) B1,3(u) B2,3(u) B3,3(u)]     
	//       | P0,0 P0,1 P0,2 P0,3 | 
	//     * | P1,0 P1,1 P1,2 P1,3 | 
	//       | P2,0 P2,1 P2,2 P2,3 | 
	//       | P3,0 P3,1 P3,2 P3,3 | 
	//  ���������P3�С�
	LeftMultiplyMatrix(M, P3); 
	double tStep = 0.1;//����
	double u0,u1,u2,u3,v0,v1,v2,v3; // u��v��������
	// ����ִ�е��������Ѿ����浽P3���ˡ�
	// ����ʹ��P3������ѭ�������ƽ�������Ƭ��
	for(double u = 0; u <= 1; u += tStep)
	{
		for(double v = 0;v <= 1;v += tStep)
		{
			u3 = u * u * u, u2 = u * u, u1 = u, u0 = 1;
			v3 = v * v * v, v2 = v * v, v1 = v, v0 = 1;
			// ʹ��P3�����ά���ꡣ
			CP3 pt =  (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
			         +(u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
			         +(u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
			         +(u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;
			// ͶӰ����ά��������ʾ��
			CP2 Point2 = projection.CustomProjection(pt); // бͶӰ
			// ��������
			if(0 == v)
				pDC->MoveTo(ROUND(Point2.x), ROUND(Point2.y));
			else
				pDC->LineTo(ROUND(Point2.x), ROUND(Point2.y));
		}
	}
	for(double v = 0; v <= 1; v += tStep)
	{
		for(double u = 0;u <= 1;u += tStep)
		{
			u3 = u * u * u; u2 = u * u; u1 = u; u0 = 1; 
			v3 = v * v * v; v2 = v * v; v1 = v; v0 = 1;
			// ʹ��P3�����ά���ꡣ
			CP3 pt = (u3 * P3[0][0] + u2 * P3[1][0] + u1 * P3[2][0] + u0 * P3[3][0]) * v3
			        +(u3 * P3[0][1] + u2 * P3[1][1] + u1 * P3[2][1] + u0 * P3[3][1]) * v2
			        +(u3 * P3[0][2] + u2 * P3[1][2] + u1 * P3[2][2] + u0 * P3[3][2]) * v1
			        +(u3 * P3[0][3] + u2 * P3[1][3] + u1 * P3[2][3] + u0 * P3[3][3]) * v0;	
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
	CP2 P2[4][4];//��ά���Ƶ�
	for(int i = 0;i < 4;i++)
		for(int j = 0;j < 4;j++)
			P2[i][j] = projection.CustomProjection(P[i][j]);
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	pOldPen=pDC->SelectObject(&NewPen);
	for(i = 0; i < 4; i++)
	{
		pDC->MoveTo(ROUND(P2[i][0].x), ROUND(P2[i][0].y));
		for(int j = 1;j < 4;j++)
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
	}
	for(int j = 0;j < 4;j++)
	{
		pDC->MoveTo(ROUND(P2[0][j].x), ROUND(P2[0][j].y));
		for(int i = 1;i < 4;i++)
			pDC->LineTo(ROUND(P2[i][j].x), ROUND(P2[i][j].y));
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}