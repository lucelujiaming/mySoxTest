#include "stdafx.h"
#include "OrthogonalProjection.h"
#define PI 3.1415926

COrthogonalProjection::COrthogonalProjection(void)
{
	
}

COrthogonalProjection::~COrthogonalProjection(void)
{

}

CP2 COrthogonalProjection::MakeOrthogonalProjection(CP3 WorldPoint)//����ͶӰ
{
	CP2 ScreenPoint;//��Ļ����ϵ��ά��
	ScreenPoint.x = WorldPoint.x;
	ScreenPoint.y = WorldPoint.y;
	return ScreenPoint;
}