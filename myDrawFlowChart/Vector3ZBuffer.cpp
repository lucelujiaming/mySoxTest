#include "stdafx.h"
#include "Vector3ZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CVector3ZBuffer::CVector3ZBuffer(void)
{
}

CVector3ZBuffer::~CVector3ZBuffer(void)
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

void CVector3ZBuffer::SetPoint(CColorP3 P0, CColorP3 P1, CColorP3 P2, CVector3 N0, CVector3 N1, CVector3 N2)
{
	this->P0 = P0;
	this->P1 = P1;
	this->P2 = P2;
	point0.x = ROUND(P0.x);
	point0.y = ROUND(P0.y);
	point0.z = P0.z;
	point0.c = P0.c;
	point0.n = N0;
	point1.x = ROUND(P1.x);
	point1.y = ROUND(P1.y);
	point1.z = P1.z;
	point1.c = P1.c;
	point1.n = N1;
	point2.x = ROUND(P2.x);
	point2.y = ROUND(P2.y);
	point2.z = P2.z;
	point2.c = P2.c;
	point2.n = N2;
}

void CVector3ZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)//��ʼ����Ȼ���
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
		zBuffer[i] = new double[nHeight];
	for (i = 0; i < nWidth; i++)//��ʼ����Ȼ���
		for (int j = 0; j < nHeight; j++)
			zBuffer[i][j] = nDepth;
}

// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
void CVector3ZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene)
{
	//���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = point2.y - point0.y + 1;
	//����span��������յ�����
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
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
	CVector3 Vector01(P0, P1), Vector02(P0, P2);
	// 2. ����ColorVector3.cpp�е�CrossProduct������
	//    ���������������Ĳ�ˡ��õ����������μ���ʽ(7-2)��
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
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
	// ���濪ʼ��������Ρ�
	// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
	// �±��Ͽ�����ʼɨ��y���귶Χ�ڵ�ÿһ�С�
	for (int y = point0.y; y < point2.y; y++)
	{
		// �õ�y������Span�ж�Ӧ��������
		int n = y - point0.y;
		// ����ҿ����ӿ�ȵ����־���鵽�ұ�־����ɨ��ÿһ���е�ÿһ���㡣
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
				// �������������ƹ��̵ĺ����߼���Ҳ����Phong��������
				// ���ǲ��ټ򵥵ز���˫���Բ�ֵ�㷨����������һ�����ص����ɫ���Բ�ֵ��
		        // 2. ����ʹ��˫���Բ�ֵ���������ڲ�����ķ�������
				CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
				// 3. ����ʹ�ö���������ϸ���ķ��������ü򵥹���ģ�ͼ�������õĹ�ǿ��
				//    ��Gouraud������������ֱ��ʹ�ö���ζ�����ü򵥹���ģ�ͼ�������õĹ�ǿ��
				//    Phong������������ͨ��˫���Բ�ֵʹ�õĵõ�����������ϸ���ķ�������֮�������
				//    ���������ü򵥹���ģ�ͼ�������õĹ�ǿ��
				//    ���Ժ��ߵĴ������ϸ�¡�
				CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, CurrentDepth), ptNormal, pScene->pMaterial);
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(I.red * 255, I.green * 255, I.blue * 255));
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
void CVector3ZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA�㷨��ɢ��
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;	// ����б�ʵĵ�����
	double x = PStart.x;		// �ҵ���㡣
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// �������������ƹ��̵ĺ����߼���Ҳ����Phong��������
		// ���ǲ��ټ򵥵ز���˫���Բ�ֵ�㷨����������һ�����ص����ɫ���Բ�ֵ��
		// 2. ����ʹ��˫���Բ�ֵ�������α��ϸ���ķ�������
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal);
		else
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y,ptNormal);
		x += m;
	}
}

void CVector3ZBuffer::SortPoint(void)//����
{
	CColorPoint3 pt;//Ҫ�������P[0].y<P[1].y<P[2].y
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

// ��ʸ�����Բ�ֵ
CVector3 CVector3ZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}