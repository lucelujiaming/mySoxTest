#include "stdafx.h"
#include "ZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CZBuffer::CZBuffer(void)
{
}

CZBuffer::~CZBuffer(void)
{
	for (int i = 0; i < nWidth; i++)
	{
		delete[] zBuffer[i];
		zBuffer[i] = NULL;
	}
	if (zBuffer != NULL)
	{
		delete zBuffer;
		zBuffer = NULL;
	}
}

/************************************************************************/
/* ��ʼ����Ȼ���ռ䡣һ���п�� * ��ȸ�Ԫ�ء�                        */
/*     nWidth  - ��ʾ��ȡ�                                             */
/*     nHeight - ��ʾ�߶ȡ�                                             */
/*     nDepth  - �����ȡ�                                             */
/************************************************************************/
void CZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
{
	// ��¼��Ⱥ���ȡ�
	this->nWidth = nWidth, this->nHeight = nHeight;
	// ʹ�ÿ�Ⱥ���ȷ�����Ȼ���ռ䡣һ���п�� * ��ȸ�Ԫ�ء�
	// ÿһ��Ԫ�ض���һ��double��
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
	{
		zBuffer[i] = new double[nHeight];
	}
	// ��ʼ����Ȼ���ռ䡣��ÿһ��Ԫ�ض�Ԥ��Ϊ�����ȡ�
	for (i = 0; i < nWidth; i++) 
	{
		for (int j = 0; j < nHeight; j++)
		{
			zBuffer[i][j] = nDepth;
		}
	}
}

void CZBuffer::SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2)
{
	// �����������㡣
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	// ���ҽ�������������������
	point0.x = ROUND(P0.x);
	point0.y = ROUND(P0.y);
	point0.z = P0.z;
	point0.c = P0.c;
	point1.x = ROUND(P1.x);
	point1.y = ROUND(P1.y);
	point1.z = P1.z;
	point1.c = P1.c;
	point2.x = ROUND(P2.x);
	point2.y = ROUND(P2.y);
	point2.z = P2.z;
	point2.c = P2.c;
}

/************************************************************************/
/* �����������ZBuffer�㷨���⣬��ʵ���������λ�����ɫ��˫���Բ�ֵ��    */
/* �����λ�����ɫ��˫���Բ�ֵ���߼����£�                               */
/*   1. �����ڵ����������֮ǰ������ʹ�ü򵥹���ģ�ͻ�����              */
/*      Gouraud���������㷨��Ҳ����˫���Թ�ǿǿ�Ȳ�ֵģ�͡�             */
/*      �������������ÿһ���������ɫ��                                */
/*   2. ���������ζ���Ĺ�ǿ������ɨ����˳��ʹ��˫���Բ�ֵ����������    */
/*      ����ÿһ��Ĺ�ǿ��Ȼ�󽫹�ǿ�ֽ�ΪRGB��ԭɫ����ɫֵ��           */
/*      ����CZBuffer::EdgeFlag�е���Interp������                        */
/*   3. ��ɨ�������������ཻ����ڣ�ʹ��˫���Բ�ֵ����ÿһ��Ĺ�ǿ��    */
/*      Ȼ�󽫹�ǿ�ֽ�ΪRGB��ԭɫ����ɫֵ��                             */
/*      ����CZBuffer::FillTriangle�е���Interp������                    */
/*   �������е���ɫ��ֵ��������Gouraud���������㷨���ᵽ��˫���Բ�ֵ��  */
/************************************************************************/
void CZBuffer::FillTriangle(CDC* pDC)
{
	// ���㰴��y������С��������
	SortPoint();
	// ���������θ��ǵ�ɨ��������
	int nTotalScanLine = point2.y - point0.y + 1;
	// ����Span��������յ�����
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	// �ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (point2.x - point0.x) * (point1.y - point0.y)
		       - (point2.y - point0.y) * (point1.x - point0.x); // ��ʸ�������z����
	if (nDeltz > 0) // �������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		nIndex = 0;
		// P[0]��P[1]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point0, point1, LEFT);
		// P[1]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point1, point2, LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		nIndex = 0;
		// P[0]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(point0, point2, RIGHT);
	}
	else // �������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		nIndex = 0;
		// P[0]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point0, point2, LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		nIndex = 0;
		// P[0]��P[1]Ϊ���ߵĽ����㡣
		EdgeFlag(point0, point1, RIGHT);
		// P[1]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(point1, point2, RIGHT);
	}
	// 1. ���ݶ���α��涥��������Լ����������������
	CColorVector3 Vector01(P0, P1), Vector02(P0, P2);
	// 2. ����ColorVector3.cpp�е�CrossProduct������
	//    ���������������Ĳ�ˡ��õ����������μ���ʽ(7-2)��
	CColorVector3 fNormal = CrossProduct(Vector01, Vector02);
	// ƽ�淽��Ax+By+Cz��D=0(7-1)��ϵ��ABC���Ƿ�������ϵ����
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;
	// ��ϵ��Dʹ������Ĺ�ʽ���㡣�μ���ʽ(7-3)��
	double D = -A * P0.x - B * P0.y - C * P0.z;
	// ���C�����㣬˵������α���ķ�������Z�ᴹֱ��
	// ͶӰ���һ��ֱ�ߡ����㷨�п��Բ����ǡ�
	if (fabs(C) < 1e-4)
		C = 1.0;
	// ɨ������Ȳ�����Ҳ����xÿ�ƶ�һ���������ӵ����ֵ���μ���ʽ(7-5)��
	double DepthStep = -A / C;
	// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
	// �±��Ͽ�����ʼɨ��y���귶Χ�ڵ�ÿһ�С�
	for (int y = point0.y; y < point2.y; y++)
	{
		// �õ�y������Span�ж�Ӧ��������
		int n = y - point0.y;
		// ����ҿ�����ʼɨ��ÿһ���е�ÿһ���㡣
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++) 
		{
			// ���㵱ǰ���ص�(x, y)�������ֵ��
			// z = -(Ax + By + D) / C���μ���ʽ(7-4)��
			double CurrentDepth = -(A * x + B * y + D) / C;
			// �Ѽ�������ĵ�ǰ���ص�(x, y)�������ֵ��
			// ���Ѿ��洢����Ȼ������е�ԭ�ɼ����ص����ֵ��Աȡ�
			// �����ǰ����������С��֡��������ԭ���������ȣ�
			// ������ǰ���ظ������۲��� ���ڵ���ԭ���ء�
			// ����ϣ�ZBuffer�㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)��
			// ���Ҿ����ӵ������z(x, y)ֵ��
			if (CurrentDepth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// ʹ�õ�ǰ���������ȸ�����Ȼ�������
				zBuffer[x + nWidth / 2][y + nHeight / 2] = CurrentDepth;
				// ����˫���Բ�ֵ�㷨����������һ�����ص����ɫ���Բ�ֵ��
				CRGB clr = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].c, SpanRight[n].c);
				// �����SetPixelV����ǰ���ص���ɫֵд��֡��������
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(clr.red * 255, clr.green * 255, clr.blue * 255));
			}
			// CurrentDepth += DepthStep;
		}
	}
	if(SpanLeft)
	{
		delete []SpanLeft;
		SpanLeft = NULL;
	}
	if(SpanRight)
	{
		delete []SpanRight;
		SpanRight = NULL;
	}
}

// DDA�㷨��ɢ�������Բ�ֵ�����Span�������յ����顣
void CZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;	// ����б�ʵĵ�����
	double x = PStart.x;		// �ҵ���㡣
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// ����˫���Բ�ֵ�㷨����������һ�����ص����ɫ���Բ�ֵ��
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		// // ����˫���Բ�ֵ�㷨����������һ�����ص����ȡ�
		// double zDepth = Interp(y, PStart.y, PEnd.y, PStart.z, PEnd.z);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
		else
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y, crColor);
		x += m;
	}
}

void CZBuffer::SortPoint(void)//����
{
	// Ҫ�������P[0].y<P[1].y<P[2].y
	CColorPoint3 pt;
	if (point0.y > point1.y)
	{
		pt = point0;
		point0 = point1;
		point1 = pt;
	}
	if (point0.y > point2.y)
	{
		pt = point0;
		point0 = point2;
		point2 = pt;
	}
	if (point1.y > point2.y)
	{
		pt = point1;
		point1 = point2;
		point2 = pt;
	}
}

CRGB CZBuffer::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//��ɫ���Բ�ֵ
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}
// ��ʸ�����Բ�ֵ
CVector3 CZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

double CZBuffer::Interp(double m, double m0, double m1, double z0, double z1)//������Բ�ֵ
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}