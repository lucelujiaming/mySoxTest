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

CP3 CPerspectiveProjection::GetEye(void)//读取视点
{
	Eye.x = 0, Eye.y = 0, Eye.z = R;//视点位于正前方
	return Eye;
}

CP2 CPerspectiveProjection::MakePerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint;//观察坐标系三维点
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	CP2 ScreenPoint;//屏幕坐标系二维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	return ScreenPoint;
}