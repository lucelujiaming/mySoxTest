#include "stdafx.h"
#include "OrthogonalProjection.h"
#define PI 3.1415926

COrthogonalProjection::COrthogonalProjection(void)
{
	
}

COrthogonalProjection::~COrthogonalProjection(void)
{

}

CP2 COrthogonalProjection::MakeOrthogonalProjection(CP3 WorldPoint)//正交投影
{
	CP2 ScreenPoint;//屏幕坐标系二维点
	ScreenPoint.x = WorldPoint.x;
	ScreenPoint.y = WorldPoint.y;
	return ScreenPoint;
}