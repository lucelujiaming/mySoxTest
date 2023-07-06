#include "stdafx.h"
#include "BackfaceCullCube.h"
#define ROUND(d) int(d + 0.5)//��������ĺ궨��

CBackfaceCullCube::CBackfaceCullCube(void)
{

}

CBackfaceCullCube::~CBackfaceCullCube(void)
{

}

/************************************************************************/
/* ��ά��ģ����ϵΪ����ϵ����Xˮƽ����Ϊ����Y�ᴹֱ����Ϊ����           */
/* Z��ָ��۲��ߡ�ʾ��ͼ���£�                                          */
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
/* һ���ǵ�λ��ԭ�㡣������ı���������ƽ�С�                           */
/************************************************************************/
void CBackfaceCullCube::ReadVertex(void)//���
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

/************************************************************************/
/* ��������ķ�����֪��X�����ң�Y�����ϣ�Z��ָ�����ǡ�                  */
/* �����ӵ�λ�����������ǰ����                                         */
/* ����ձ����ⷨ�����ķ����������ζ��������š���������������档   */
/* ˳����������������򡣼�4����ָ�ض���˳��                          */
/* ���Ĵָ����������ⷨ�����ķ���һ�¡�                               */
/* ��˿�֪��                                                           */
/************************************************************************/
void CBackfaceCullCube::ReadFace(void) // ���
{
	// �������ǵ������������4,5,6,7�ĸ�����ɡ�
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7; // ǰ��
	F[1].ptNumber = 4;
	// �������ǵ������������0,3,2,1�ĸ�����ɡ�
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1; // ����
	F[2].ptNumber = 4;
	// ���������������0,4,7,3�ĸ�����ɡ�
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
void CBackfaceCullCube::Draw(CDC* pDC)
{
	CP2 ScreenPoint[4]; // ��άͶӰ��
	// ���͸��ͶӰ���ӵ㡣
	CP3 Eye = projection.GetEye();
	// ��Ȼ�������ڻ��Ƶ�����������򵥵���ά���塣
	// ��������Ҳ��Ҫ���ջ��Ƹ�����ά����Ĳ�����л��ơ�
    // Ҳ����һ����һ������ơ�
	// ��ѭ����Ҳ����һ����һ������ơ�
	for (int nFace = 0; nFace < 6; nFace++)//��ѭ��
	{
		// ����ÿһ����ĵ�һ��������ӵ�����ʸ��
		CVector3 ViewVector(V[F[nFace].ptIndex[0]], Eye); // �����ʸ��
		// ��һ��ǰ������������ʸ��
		ViewVector = ViewVector.Normalize(); 
		// ����������һ����ʸ��
		// ����ĵ�һ�����㵽�ڶ������������ߵ����ʸ��
		CVector3 Vector01(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[1]]); 
		// ������������һ����ʸ����
		// ����ĵ�һ�����㵽����������ĶԽ��ߵ����ʸ��
		CVector3 Vector02(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[2]]); 
		// ����ߺ�һ���ߵĲ�ˡ��õ���ķ�ʸ��
		CVector3 FaceNormal = CrossProduct(Vector01, Vector02);
		// ��һ��ǰ���������ķ�ʸ��
		FaceNormal = FaceNormal.Normalize();
		// ����ʸ������ʸ�� >= 0 ��Ϊ���ƿɼ�����߽�Ļ���������
		// ������������˵����ʸ������ʸ�� >= 0�޳��˱����ӵ���������ɼ����档
		if (DotProduct(ViewVector, FaceNormal) >= 0) // �����޳�
		{
		    // �õ���������ж�������ꡣ
			for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // ����ѭ��
			{
			    // ��������ʹ��͸��ͶӰ����ͶӰ�㡣
				ScreenPoint[nPoint] = projection.PerspectiveProjection(
									V[F[nFace].ptIndex[nPoint]]); // ͸��ͶӰ
		    }
			// ��ν�Ļ���һ���档��ʵ����ʹ��MoveTo��LineTo����һ������Ρ�
			// ���ƶ����
			pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
			pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
			pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
			pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
			pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));			
		}
	}
}