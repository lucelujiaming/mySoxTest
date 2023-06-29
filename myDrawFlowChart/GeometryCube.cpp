#include "stdafx.h"
#include "GeometryCube.h"
#define ROUND(d) int(d + 0.5)//��������ĺ궨��

CGeometryCube::CGeometryCube(void)
{
    m_xScale = m_yScale = m_zScale = 1;
}

CGeometryCube::~CGeometryCube(void)
{

}

/************************************************************************/
/* ���������Ķ����������ϵ�е�ʾ��ͼ���£�                           */
/*             ��Y                                                      */
/*           3 ��                                                       */
/*           ����-------��2                                             */
/*         ��  ��     ����                                              */
/*     7 ��    ��  6��  ��                                              */
/*       |-----ʮ---    ��                                              */
/*       |   0 ʮ��ʮ�������� X                                         */
/*       |   ��    |  �� 1                                              */
/*     4 | ��      |��                                                  */
/*       ��-------- 5                                                   */
/*   Z �L                                                               */
/*                                                                      */
/* ���Կ�������X�����ң�Y�����ϣ�Z��ָ�����ǡ�                          */
/* �������֮ǰ�������ǳ���߶�Ϊ1�ĵ�λ�����塣�Ҽ����˳���ߡ�        */
/* ʹ���뿴������������һ�㡣����ȫ��1����ħ�����֡�����������⡣      */
/************************************************************************/
void CGeometryCube::ReadVertex(int xScale, int yScale, int zScale) // �����
{
	m_xScale = xScale;
	m_yScale = yScale;
	m_zScale = zScale;
	V[0].x = 0;      V[0].y = 0;      V[0].z = 0;
	V[1].x = xScale, V[1].y = 0;      V[1].z = 0;
	V[2].x = xScale, V[2].y = yScale; V[2].z = 0;
	V[3].x = 0;      V[3].y = yScale; V[3].z = 0;
	V[4].x = 0;      V[4].y = 0;      V[4].z = zScale;
	V[5].x = xScale; V[5].y = 0;      V[5].z = zScale;
	V[6].x = xScale; V[6].y = yScale; V[6].z = zScale;
	V[7].x = 0;      V[7].y = yScale; V[7].z = zScale;
}

// ��������ķ�����֪��X�����ң�Y�����ϣ�Z��ָ�����ǡ���˿�֪��
void CGeometryCube::ReadFace(void) // ���
{
	// �������ǵ������������4,5,6,7�ĸ�����ɡ�
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7; // ǰ��
	// �������ǵ������������0,3,2,1�ĸ�����ɡ�
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1; // ����
	// ���������������0,4,7,3�ĸ�����ɡ�
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3; // ����
	// ��������Ҳ������1,2,6,5�ĸ�����ɡ�
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5; // ����
	// ������Ķ��˱�����2,3,7,6�ĸ�����ɡ�
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6; // ����
	// ������ĵ׶˱�����0,1,5,4�ĸ�����ɡ�
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4; // ����
}

// �����������������Ļ��������ǾͿ��Ի����������ˡ�
void CGeometryCube::Draw(CDC* pDC, BOOL isDrawMainDiagonal)
{
	CP2 ScreenPoint[4]; // ��άͶӰ��
	// ��Ȼ�������ڻ��Ƶ�����������򵥵���ά���塣
	// ��������Ҳ��Ҫ���ջ��Ƹ�����ά����Ĳ�����л��ơ�
    // Ҳ����һ����һ������ơ�
	// ��ѭ����Ҳ����һ����һ������ơ�
	for (int nFace = 0; nFace < 6; nFace++) 
	{
		// �õ���������ж�������ꡣ
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // ����ѭ��
		{
			ScreenPoint[nPoint].x = V[F[nFace].ptIndex[nPoint]].x;
			ScreenPoint[nPoint].y = V[F[nFace].ptIndex[nPoint]].y;
		}
		// ��ν�Ļ���һ���档��ʵ����ʹ��MoveTo��LineTo����һ������Ρ�
		// ���ƶ����
		pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
		pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
		pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
		pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
		pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
	}
	if(isDrawMainDiagonal)
	{
		//�������Խ���
		CPen NewPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&NewPen);
		pDC->MoveTo(ROUND(V[4].x), ROUND(V[4].y));
		pDC->LineTo(ROUND(V[2].x), ROUND(V[2].y));
		pDC->SelectObject(pOldPen);
	}
}