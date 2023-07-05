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

void CDepthLinearInterpZBuffer::SetPoint(CColorP3* P, CVector3* N)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
		this->P[nPoint] = P[nPoint];
		point[nPoint].x = ROUND(P[nPoint].x);
		point[nPoint].y = ROUND(P[nPoint].y);
		point[nPoint].z = P[nPoint].z;
		point[nPoint].c = P[nPoint].c;
		point[nPoint].n = N[nPoint];
	}	
}

void CDepthLinearInterpZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)//��ʼ����Ȼ���
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
		zBuffer[i] = new double[nHeight];
	for (i = 0; i < nWidth; i++)//��ʼ����Ȼ���
		for (int j = 0; j < nHeight; j++)
			zBuffer[i][j] = nDepth;
}

void CDepthLinearInterpZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, CLightingScene* pScene)
{
	//���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = point[2].y - point[0].y + 1;
	//����span��������յ�����
	SpanLeft = new CColorPoint3[nTotalScanLine];
	SpanRight = new CColorPoint3[nTotalScanLine];
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y) - (point[2].y - point[0].y) * (point[1].x - point[0].x);//��ʸ�������z����
	if (nDeltz > 0)//��������
	{
		nIndex = 0;
		EdgeFlag(point[0], point[1], LEFT);
		EdgeFlag(point[1], point[2], LEFT);
		nIndex = 0;
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
	for (int y = point[0].y; y < point[2].y; y++)//�±��Ͽ�
	{
		int n = y - point[0].y;
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++)//����ҿ�
		{
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			double depth = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].z, SpanRight[n].z); 
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, depth), ptNormal, pScene->pMaterial);
			if (depth <= zBuffer[x + nWidth / 2][y + nHeight / 2])//�����ǰ����������С��֡��������ԭ����������
			{
				zBuffer[x + nWidth / 2][y + nHeight / 2] = depth;//ʹ�õ�ǰ���������ȸ�����Ȼ�����
				pDC->SetPixelV(x, y, RGB(I.red * 255, I.green * 255, I.blue * 255));
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
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		double zDepth = Interp(y, PStart.y, PEnd.y, PStart.z, PEnd.z);
		if (bFeature)
			SpanLeft[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
		else
			SpanRight[nIndex++] = CColorPoint3(ROUND(x), y, zDepth, ptNormal);
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