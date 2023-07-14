#include "stdafx.h"
#include "TextureZBuffer.h"
#include "math.h"
#define ROUND(d) int(d + 0.5)
#define LEFT 1
#define RIGHT 0

CTextureZBuffer::CTextureZBuffer(void)
{

}

CTextureZBuffer::~CTextureZBuffer(void)
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

// ʹ�ö���P����ʸ��N�������ַT���������Ρ�
void CTextureZBuffer::SetPoint(CColorP3* objPoint, 
		CVector3* objNormal, CTextureCoordinate* objTexture)
{
	for (int nPoint = 0; nPoint < 3; nPoint++)
	{
		this->P[nPoint] = objPoint[nPoint];
		point[nPoint].x = ROUND(objPoint[nPoint].x);
		point[nPoint].y = ROUND(objPoint[nPoint].y);
		point[nPoint].z = objPoint[nPoint].z;
		point[nPoint].c = objPoint[nPoint].c;
		point[nPoint].n = objNormal[nPoint];
		point[nPoint].t = objTexture[nPoint];
	}
}

// ��ʼ����Ȼ���
void CTextureZBuffer::InitialDepthBuffer(int nWidth, int nHeight, double nDepth)
{
	this->nWidth = nWidth, this->nHeight = nHeight;
	zBuffer = new double* [nWidth];
	for (int i = 0; i < nWidth; i++)
	{
		zBuffer[i] = new double[nHeight];
	}
	for (i = 0; i < nWidth; i++) // ��ʼ����Ȼ���
	{
		for (int j = 0; j < nHeight; j++)
		{
			zBuffer[i][j] = nDepth;  // ����Ȼ�������ÿһ��Ԫ������ΪnDepth��
		}
	}
}

// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
void CTextureZBuffer::FillTriangle(CDC* pDC, CColorP3 Eye, 
								   CLightingScene* pScene, CTexture* pTexture)
{
	//���㰴��y������С��������
	SortPoint();
	//���������θ��ǵ�ɨ��������
	int nTotalScanLine = point[2].y - point[0].y + 1;
	//����span��������յ�����
	SpanLeft = new CColorPoint2[nTotalScanLine];
	SpanRight = new CColorPoint2[nTotalScanLine];
	//�ж�P1��λ��P0P2�ߵ���໹���Ҳ�
	int nDeltz = (point[2].x - point[0].x) * (point[1].y - point[0].y) 
	           - (point[2].y - point[0].y) * (point[1].x - point[0].x); // ��ʸ�������z����
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
	else // �������Ρ�˵����
	{
		// ��0��ʼ���㻭�ߵ���ʼ�㡣
		nIndex = 0;
		// P[0]��P[2]Ϊ���ߵ���ʼ�㡣
		EdgeFlag(point[0], point[2], LEFT);
		// ��0��ʼ���㻭�ߵĽ����㡣
		nIndex = 0;
		// P[0]��P[1]Ϊ���ߵĽ����㡣
		EdgeFlag(point[0], point[1], RIGHT);
		// P[1]��P[2]Ϊ���ߵĽ����㡣
		EdgeFlag(point[1], point[2], RIGHT);
	}
	// 1. ���ݶ���α��涥��������Լ����������������
	CVector3 Vector01(P[0], P[1]), Vector02(P[0], P[2]);
	// 2. ����ColorVector3.cpp�е�CrossProduct������
	//    ���������������Ĳ�ˡ��õ����������μ���ʽ(7-2)��
	CVector3 fNormal = CrossProduct(Vector01, Vector02);
	// ƽ�淽��Ax+By+Cz��D=0(7-1)��ϵ��ABC���Ƿ�������ϵ����
	double A = fNormal.x, B = fNormal.y, C = fNormal.z;
	// ��ϵ��Dʹ������Ĺ�ʽ���㡣�μ���ʽ(7-3)��
	double D = -A * P[0].x - B * P[0].y - C * P[0].z;
	// ���C�����㣬˵������α���ķ�������Z�ᴹֱ��
	// ͶӰ���һ��ֱ�ߡ����㷨�п��Բ����ǡ�
	if (fabs(C) < 1e-4)
    {
		C = 1.0;
    }
	// ɨ������Ȳ�����Ҳ����xÿ�ƶ�һ���������ӵ����ֵ���μ���ʽ(7-5)��
	double DepthStep = -A / C;//ɨ������Ȳ���
	// ���濪ʼ��������Ρ�
	// ���㷨��ʵ�ʾ��Ƕ���һ�����������ϵ�(x, y)�����Ҿ����ӵ������z(x, y)ֵ��
	// �±��Ͽ�����ʼɨ��y���귶Χ�ڵ�ÿһ�С�
	for (int y = point[0].y; y < point[2].y; y++)
	{
		// �õ�y������Span�ж�Ӧ��������
		int n = y - point[0].y;
		// ����ҿ����ӿ�ȵ����־���鵽�ұ�־����ɨ��ÿһ���е�ÿһ���㡣
		for (int x = SpanLeft[n].x; x < SpanRight[n].x; x++) 
		{
			// ���㵱ǰ���ص�(x, y)�������ֵ��
			// z = -(Ax + By + D) / C���μ���ʽ(7-4)��
			double CurrentDepth = -(A * x + B * y + D) / C;
			// ��ʸ�����Բ�ֵ��
			CVector3 ptNormal = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].n, SpanRight[n].n);
			// �����ַ���Բ�ֵ��
			CTextureCoordinate T = Interp(x, SpanLeft[n].x, SpanRight[n].x, SpanLeft[n].t, SpanRight[n].t);
			// ʹ��ǰ��õ��������ַ����������ж�Ӧλ�õ���ɫ��
			CRGB Texel = GetTexture(ROUND(T.u), ROUND(T.v), pTexture);
			// ��������ɫ����Ϊ�������ʡ�
			pScene->pMaterial->SetDiffuse(Texel);
			// ��������ɫ����Ϊ���������ʡ�
			pScene->pMaterial->SetAmbient(Texel);
			// �������Բ�ֵ�����ȡ�
			// double CurrentDepth = Interp(x, SpanLeft[n].x, SpanRight[n].x, 
			//  	SpanLeft[n].z, SpanRight[n].z);
			// ���㵱ǰ���ص�Ĺ��ա�
			CRGB I = pScene->Illuminate(Eye, CColorP3(x, y, CurrentDepth), 
				ptNormal, pScene->pMaterial);
			// ���濪ʼ����
			// �����ǰ����������С��֡��������ԭ����������
			if (CurrentDepth <= zBuffer[x + nWidth / 2][y + nHeight / 2])
			{
				// ʹ�õ�ǰ���������ȸ�����Ȼ�������
				zBuffer[x + nWidth / 2][y + nHeight / 2] = CurrentDepth;
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

void CTextureZBuffer::EdgeFlag(CColorPoint2 PStart, CColorPoint2 PEnd, BOOL bFeature)//DDA�㷨��ɢ��
{
	int dx = PEnd.x - PStart.x;
	int dy = PEnd.y - PStart.y;
	double m = double(dx) / dy;
	double x = PStart.x;
	for (int y = PStart.y; y < PEnd.y; y++)
	{
		CVector3 ptNormal = Interp(y, PStart.y, PEnd.y, PStart.n, PEnd.n);
		CTextureCoordinate T = Interp(y, PStart.y, PEnd.y, PStart.t, PEnd.t);
		if (bFeature)
        {
			SpanLeft[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal, T);
        }
		else
        {
			SpanRight[nIndex++] = CColorPoint2(ROUND(x), y, ptNormal, T);
        }
		x += m;
	}
}

void CTextureZBuffer::SortPoint(void)//����
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

// ��ʸ�����Բ�ֵ
CVector3 CTextureZBuffer::Interp(double m, 
						double m0, double m1, CVector3 N0, CVector3 N1)
{
	CVector3 vector;
	vector = (m1 - m) / (m1 - m0) * N0 + (m - m0) / (m1 - m0) * N1;
	return vector;
}

// �����ַ���Բ�ֵ
CTextureCoordinate CTextureZBuffer::Interp(double m, 
		double m0, double m1, CTextureCoordinate T0, CTextureCoordinate T1)
{
	CTextureCoordinate texture;
	texture = (m1 - m) / (m1 - m0) * T0 + (m - m0) / (m1 - m0) * T1;
	return texture;
}

// ������Բ�ֵ
double CTextureZBuffer::Interp(double m, double m0, double m1, double z0, double z1)
{
	double z = (m1 - m) / (m1 - m0) * z0 + (m - m0) / (m1 - m0) * z1;
	return z;
}

// ʹ��uv��ȡ�����ж�Ӧλ�õ���ɫ��
CRGB CTextureZBuffer::GetTexture(int u, int v, CTexture* pTexture) // ��ȡ����
{
	// ����v���ʵ�˳��Ĭ�ϵķ���˳������½ǿ�ʼ����仰����Ϊ�����Ͻǿ�ʼ��
	v = pTexture->bmp.bmHeight - 1 - v;
	/*���ͼƬ�ı߽磬��ֹԽ��*/
	if (u < 0) 
    {
		u = 0; 
    }
	if (v < 0) 
    {
		v = 0;
    }
	if (u > pTexture->bmp.bmWidth - 1) 	
    {
		u = pTexture->bmp.bmWidth - 1;
    }
	if (v > pTexture->bmp.bmHeight - 1)	
    {
		v = pTexture->bmp.bmHeight - 1;
    }
	/*���Ҷ�Ӧ����ռ����ɫֵ*/
	// ѭ��ÿһ�У�ÿ�ж��ĸ��ֽڡ�
	// ����RGB��͸����
	int position = v * pTexture->bmp.bmWidthBytes + 4 * u;
	return  CRGB(pTexture->image[position + 2] / 255.0, 
		         pTexture->image[position + 1] / 255.0, 
				 pTexture->image[position] / 255.0);
}
