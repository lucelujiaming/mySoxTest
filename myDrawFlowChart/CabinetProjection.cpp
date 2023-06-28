#include "stdafx.h"
#include "CabinetProjection.h"
#define PI 3.1415926

CCabinetProjection::CCabinetProjection(void)
{
	
}

CCabinetProjection::~CCabinetProjection(void)
{

}

CP2 CCabinetProjection::MakeCavalierProjection(CP3 WorldPoint)//斜等测投影
{
	CP2 ScreenPoint;
	double m = 0.707;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

CP2 CCabinetProjection::MakeCabinetProjection(CP3 WorldPoint)//斜二测投影
{
	CP2 ScreenPoint;
	double m = 0.3536;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}