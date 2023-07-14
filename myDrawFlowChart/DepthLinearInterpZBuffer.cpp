#include "stdafx.h"

#include "DepthLinearInterpZBuffer.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CDepthLinearInterpZBuffer::CDepthLinearInterpZBuffer(void)
{

}

CDepthLinearInterpZBuffer::~CDepthLinearInterpZBuffer(void)
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
void CDepthLinearInterpZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
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
			zBuffer[i][j] = nDepth;   // ����Ȼ�������ÿһ��Ԫ������ΪnDepth��
		}
	}
}

void CDepthLinearInterpZBuffer::SetPoint(CColorP3* P, CVector3* N)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
        // ���붥�㡣
		this->P[nPoint] = P[nPoint];
        // ���ҽ�������������
		point[nPoint].x = ROUND(P[nPoint].x);
		point[nPoint].y = ROUND(P[nPoint].y);
		point[nPoint].z = P[nPoint].z;
		point[nPoint].c = P[nPoint].c;
		point[nPoint].n = N[nPoint];
	}	
}

// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
void CDepthLinearInterpZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene)
{
	// ���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = point[2].y - point[0].y + 1;
	//����span��������յ�����
	SpanLeft = new CColorPoint3[nTotalScanLine];
	SpanRight = new CColorPoint3[nTotalScanLine];
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y) 
	           - (point[2].y - point[0].y) * (point[1].x - point[0].x);//��ʸ�������z����
	if (nDeltz > 0) // �������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		nIndex = 0;
		// P[0]��P[1]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point[0], point[1], LEFT);
		// P[1]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point[1], point[2], LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		nIndex = 0;
		// P[0]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(point[0], point[2], RIGHT);
	}
	else//��������
	{
		nIndex = 0;
		EdgeFlag(point[0], point[2], LEFT);
		nIndex = 0;
		EdgeFlag(point[0], point[1], RIGHT);
		EdgeFlag(point[1], point[2], RIGHT);
	}
	// ������ZBuffer�㷨�ĺ������ݡ�
	for (int y = point[0].y; y < point[2].y; y++)//�±��Ͽ�
	{
		// �õ�y������Span�ж�Ӧ��������
		int n = y - point[0].y;
		// ����ҿ����ӿ�ȵ����־���鵽�ұ�־����ɨ��ÿһ���е�ÿһ���㡣
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++)
		{
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			// ������ȣ����ſ������������Բ�ֵ��
			double depth = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].z, SpanRight[n].z); 
			// ���㵱ǰ���ص�Ĺ��ա�
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, depth), ptNormal, pScene->pMaterial);
			// �����ǰ����������С��֡��������ԭ����������
			if (depth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// ʹ�õ�ǰ���������ȸ�����Ȼ�����
				zBuffer[x + nWidth / 2][y + nHeight / 2] = depth;
				pDC->SetPixelV(m_ptDrawPosition.x + x, m_ptDrawPosition.y + y, 
					RGB(I.red * 255, I.green * 255, I.blue * 255));
			}
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

void CDepthLinearInterpZBuffer::EdgeFlag(CColorPoint3 PStart, CColorPoint3 PEnd, BOOL bFeature)//DDA�㷨��ɢ��
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		// ����˫���Բ�ֵ�㷨����������һ�����ص����ɫ���Բ�ֵ��
		CRGB crColor = Interp(y, PStart.y, PEnd.y, PStart.c, PEnd.c);
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		// ����˫���Բ�ֵ�㷨����������һ�����ص����ȡ�
		double zDepth = Interp(y, PStart.y, PEnd.y, PStart.z, PEnd.z);
		if (bFeature)
        {
			SpanLeft[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
        }
		else
        {
			SpanRight[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
        }
		x += m;
	}
}

void CDepthLinearInterpZBuffer::SortPoint(void)//����
{
	CColorPoint3 pt;//Ҫ�������P[0].y<P[1].y<P[2].y
	if (point[0].y > point[1].y)
	{
		pt = point[0];
		point[0] = point[1];
		point[1] = pt;
	}
	if (point[0].y > point[2].y)
	{
		pt = point[0];
		point[0] = point[2];
		point[2] = pt;
	}
	if (point[1].y > point[2].y)
	{
		pt = point[1];
		point[1] = point[2];
		point[2] = pt;
	}
}

CRGB CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, CRGB c0, CRGB c1)//��ɫ���Բ�ֵ
{
	CRGB color;
	color = (m1 - m) / (m1 - m0) * c0 + (m - m0) / (m1 - m0) * c1;
	return color;
}

CVector3 CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, CVector3 N0, CVector3 N1)//��ʸ�����Բ�ֵ
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

double CDepthLinearInterpZBuffer::Interp(double m, double m0, double m1, double z0, double z1)//������Բ�ֵ
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}
