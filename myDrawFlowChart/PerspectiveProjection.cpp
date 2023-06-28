#include "stdafx.h"
#include "PerspectiveProjection.h"
#define PI 3.1415926

CPerspectiveProjection::CPerspectiveProjection(void)
{
	R = 1200, d = 800;
}

CPerspectiveProjection::~CPerspectiveProjection(void)
{

}

void CPerspectiveProjection::SetEye(CP3 Eye)
{
	this->Eye = Eye;
}

CP3 CPerspectiveProjection::GetEye(void)//��ȡ�ӵ�
{
	Eye.x = 0, Eye.y = 0, Eye.z = R;//�ӵ�λ����ǰ��
	return Eye;
}

CP2 CPerspectiveProjection::MakePerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint;//�۲�����ϵ��ά��
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	CP2 ScreenPoint;//��Ļ����ϵ��ά��
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	return ScreenPoint;
}