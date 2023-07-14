#include "stdafx.h"
#include "Projection.h"
#define PI 3.1415926

CProjection::CProjection(void)
{
	R = 1200, d = 800;
}

CProjection::~CProjection(void)
{

}

CP2  CProjection::CustomProjection(CP3 pt)
{
	if(projectionMode == ORTHOGONAL_PROJECTION)
    {
		return OrthogonalProjection(pt);
    }
	else if(projectionMode == CABINET_PROJECTION)
    {
		return CabinetProjection(pt);
    }
	else if(projectionMode == CAVALIER_PROJECTION)
    {
		return CavalierProjection(pt);
    }
	else if(projectionMode == PERSPECTIVE_PROJECTION)
    {
		return PerspectiveProjection(pt);
    }
	else 
    {
		return CP2();
    }
}

// 所谓的投影，其实就是把一个三维坐标变成一个二维坐标。用于屏幕显示。
// 而正交投影则非常简单。就是XY不变，Z被舍弃。
CP2 CProjection::OrthogonalProjection(CP3 WorldPoint)//正交投影
{
	CP2 ScreenPoint; // 屏幕坐标系二维点
	ScreenPoint.x = WorldPoint.x;
	ScreenPoint.y = WorldPoint.y;
	return ScreenPoint;
}

// 斜投影的计算公式参见公式(5-30)。
CP2 CProjection::CavalierProjection(CP3 WorldPoint) // 斜等测投影
{
	CP2 ScreenPoint;
	// 取Beta=45度，Alpha=45度。得到的投影为斜等测投影。
	// Cos(Alpha)Sin(Beta) = sqrt(2)/2 = 0.707。
	double m = 0.707;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

// 斜投影的计算公式参见公式(5-30)。
CP2 CProjection::CabinetProjection(CP3 WorldPoint) // 斜二测投影
{
	CP2 ScreenPoint;
	// 取Beta=45度，Alpha=63.4度。得到的投影为斜二测投影。
	// Cos(Alpha)Sin(Beta) = sqrt(2)/4 = 0.3536。
	// 斜二测投影更加真实，因为透视缩小系数为1/2。
	double m = 0.3536;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

// 设置透视投影的视点。
void CProjection::SetEye(CP3 Eye)
{
	this->Eye = Eye;
}

// 获得透视投影的视点。
CP3 CProjection::GetEye(void)//读取视点
{
	//视点位于正前方
	// 视点在世界坐标系中的坐标为(0, 0, R)。
	Eye.x = 0, Eye.y = 0, Eye.z = R;
	return Eye;
}

// 透视投影存在一个视点的问题。因此上需要设置和获取函数。
CP2 CProjection::PerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint; // 观察坐标系三维点
	// 参见公式(5-38)
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	CP2 ScreenPoint;//屏幕坐标系二维点
	// 参见公式(5-45)
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	return ScreenPoint;
}


// 设置透视投影的视点。
void CProjection::SetColorEye(CColorP3 Eye)
{
	this->ColorEye = ColorEye;
}

// 获得透视投影的视点。
CColorP3 CProjection::GetColorEye(void)//读取视点
{
	//视点位于正前方
	// 视点在世界坐标系中的坐标为(0, 0, R)。
	ColorEye.x = 0, ColorEye.y = 0, ColorEye.z = R;
	return ColorEye;
}

CColorP2 CProjection::TwoDimColorPerspectiveProjection(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint; // 观察坐标系三维点
	// 参见公式(5-38)
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	ViewPoint.c = WorldPoint.c;
	CColorP2 ScreenPoint;//屏幕坐标系二维点
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

CColorP3 CProjection::ThreeDimColorPerspectiveProjection(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint; // 观察坐标系三维点
	// 从三维世界坐标系到三维观察坐标系。参见公式(5-38)
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	ViewPoint.c = WorldPoint.c;
	CColorP3 ScreenPoint; // 屏幕坐标系三维点
	// 从三维观察坐标系到屏幕三维坐标系。给出伪深度。
	// 使用Bouknight公式计算x坐标。参见公式(5-50)。
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	// 使用Bouknight公式计算y坐标。参见公式(5-50)。
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	// 使用Bouknight公式计算z坐标。参见公式(5-50)。
	ScreenPoint.z = (ViewPoint.z - d) * d / ViewPoint.z;
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}
