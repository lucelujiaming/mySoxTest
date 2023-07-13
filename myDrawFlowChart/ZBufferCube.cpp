#include "stdafx.h"
#include "ZBufferCube.h"
#define ROUND(d) int(d + 0.5)//��������ĺ궨��

CZBufferCube::CZBufferCube(void)
{

}

CZBufferCube::~CZBufferCube(void)
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
/* ��������������ǳ���߶�Ϊ1�ĵ�λ�����塣                            */
/************************************************************************/
void CZBufferCube::ReadVertex(void)//���
{
	V[0].x = 0, V[0].y = 0, V[0].z = 0;
	V[1].x = 1, V[1].y = 0, V[1].z = 0;
	V[2].x = 1, V[2].y = 1, V[2].z = 0;
	V[3].x = 0, V[3].y = 1, V[3].z = 0;
	V[4].x = 0, V[4].y = 0, V[4].z = 1;
	V[5].x = 1, V[5].y = 0, V[5].z = 1;
	V[6].x = 1, V[6].y = 1, V[6].z = 1;
	V[7].x = 0, V[7].y = 1, V[7].z = 1;
}

// ��������ķ�����֪��X�����ң�Y�����ϣ�Z��ָ�����ǡ�
void CZBufferCube::ReadFace(void) // ���
{
    // ��������ķ�����֪��
	// �������ǵ������������4,5,6,7�ĸ�����ɡ�
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7;  // ǰ��
	F[0].c = CRGB(1.0, 0.0, 0.0);              // ǰ�����ɫ����
	// �������ǵ������������0,3,2,1�ĸ�����ɡ�
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1;  // ����
	F[1].c = CRGB(0.0, 1.0, 0.0);              // �������ɫ����
	// ���������������0,4,7,3�ĸ�����ɡ�
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3;  // ����
	F[2].c = CRGB(0.0, 0.0, 1.0);              // �������ɫ����
	// ��������Ҳ������1,2,6,5�ĸ�����ɡ�
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5;  // ����
	F[3].c = CRGB(1.0, 1.0, 0.0);              // �������ɫ����
	// ������Ķ��˱�����2,3,7,6�ĸ�����ɡ�
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6;   // ����
	F[4].c = CRGB(1.0, 0.0, 1.0);               // �������ɫ��Ʒ��
	// ������ĵ׶˱�����0,1,5,4�ĸ�����ɡ�
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4;   // ����
	F[5].c = CRGB(0.0, 1.0, 1.0);               // �������ɫ����
}

/************************************************************************/
/* �����������������Ļ��������ǾͿ��Ի����������ˡ�                     */
/* CZBufferCubeʹ�ô����CZBuffer���л��ơ�                             */
/* ��������������һ�������߼�������                                   */
// ��������������                                                       */
// pDC      - ��ͼ�����                                                */
// pZBuffer - ��Ȼ�����������ʹ��ͬһ����Ȼ��������ƶ�����塣        */
//            ��Ϊ��Ȼ�������������֪���Լ�Ҫ���Ƶ���ʲô��            */
//            ���ͻ���������Ϣ��������Ρ�                            */
// nSign    - �������š����ڷ��䲻ͬ����ɫ��                          */
/************************************************************************/
void CZBufferCube::Draw(CDC* pDC, CZBuffer* pZBuffer, int nSign)
{
	CColorP3 ScreenPoint[4];//��άͶӰ��
	// ��Ȼ�������ڻ��Ƶ�����������򵥵���ά���塣
	// ��������Ҳ��Ҫ���ջ��Ƹ�����ά����Ĳ�����л��ơ�
    // Ҳ����һ����һ������ơ�
	// ��ѭ����Ҳ����һ����һ������ơ�
	for (int nFace = 0; nFace < 6; nFace++)
	{
		// �õ���������ж�������ꡣ
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // ����ѭ��
		{
			// ʹ����ά͸��ͶӰ�õ�͸�����ꡣ
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// �����������ŷ��䲻ͬ����ɫ�� 
//			if (nSign == 0)
//				ScreenPoint[nPoint].c = CRGB(0.5, 0.0, 0.0);   // ��
//			else if (nSign == 1)
//				ScreenPoint[nPoint].c = CRGB(0.0, 0.5, 0.0);   // ��
//			else if (nSign == 2)
//				ScreenPoint[nPoint].c = CRGB(0.0, 0.0, 0.5);   // ��
//			else
				ScreenPoint[nPoint].c = F[nFace].c;
		}
		// �������ÿһ���涼��һ�����Σ����ǰ����ֳ����������ν�����ʾ��
        // ��������
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); 
		pZBuffer->FillTriangle(pDC);
        // ��������
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);
		pZBuffer->FillTriangle(pDC);
	}
}