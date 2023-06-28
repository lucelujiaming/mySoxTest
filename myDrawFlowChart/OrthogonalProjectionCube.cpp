#include "stdafx.h"
#include "OrthogonalProjectionCube.h"
#define ROUND(d) int(d + 0.5)//��������ĺ궨��

COrthogonalProjectionCube::COrthogonalProjectionCube(void)
{

}

COrthogonalProjectionCube::~COrthogonalProjectionCube(void)
{

}

void COrthogonalProjectionCube::ReadVertex(void)//���
{
	V[0].x = -1; V[0].y = -1; V[0].z = -1;
	V[1].x = +1; V[1].y = -1; V[1].z = -1;
	V[2].x = +1; V[2].y = +1; V[2].z = -1;
	V[3].x = -1; V[3].y = +1; V[3].z = -1;
	V[4].x = -1; V[4].y = -1; V[4].z = +1;
	V[5].x = +1; V[5].y = -1; V[5].z = +1;
	V[6].x = +1; V[6].y = +1; V[6].z = +1;
	V[7].x = -1; V[7].y = +1; V[7].z = +1;
}

void COrthogonalProjectionCube::ReadFace(void)//���
{
	F[0].ptNumber = 4; F[0].ptIndex[0] = 4; F[0].ptIndex[1] = 5; F[0].ptIndex[2] = 6; F[0].ptIndex[3] = 7;//ǰ��
	F[1].ptNumber = 4; F[1].ptIndex[0] = 0; F[1].ptIndex[1] = 3; F[1].ptIndex[2] = 2; F[1].ptIndex[3] = 1;//����
	F[2].ptNumber = 4; F[2].ptIndex[0] = 0; F[2].ptIndex[1] = 4; F[2].ptIndex[2] = 7; F[2].ptIndex[3] = 3;//����
	F[3].ptNumber = 4; F[3].ptIndex[0] = 1; F[3].ptIndex[1] = 2; F[3].ptIndex[2] = 6; F[3].ptIndex[3] = 5;//����
	F[4].ptNumber = 4; F[4].ptIndex[0] = 2; F[4].ptIndex[1] = 3; F[4].ptIndex[2] = 7; F[4].ptIndex[3] = 6;//����
	F[5].ptNumber = 4; F[5].ptIndex[0] = 0; F[5].ptIndex[1] = 1; F[5].ptIndex[2] = 5; F[5].ptIndex[3] = 4;//����
}

void COrthogonalProjectionCube::Draw(CDC* pDC)
{
	CP2 ScreenPoint[4];//��άͶӰ��
	for (int nFace = 0; nFace < 6; nFace++)//��ѭ��
	{
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++)//����ѭ��
			ScreenPoint[nPoint] = projection.MakeOrthogonalProjection(V[F[nFace].ptIndex[nPoint]]);//����ͶӰ
		pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));//���ƶ����
		pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
		pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
		pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
		pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
	}
}