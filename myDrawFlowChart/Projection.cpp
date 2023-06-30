#include "stdafx.h"
#include "Projection.h"
#define PI 3.1415926

CProjection::CProjection(void)
{
	
}

CProjection::~CProjection(void)
{

}

CP2  CProjection::MakeCustomProjection(CP3 pt)
{
	if(projectionMode == ORTHOGONAL_PROJECTION)
		return MakeOrthogonalProjection(pt);
	else if(projectionMode == CABINET_PROJECTION)
		return MakeCabinetProjection(pt);
	else if(projectionMode == CAVALIER_PROJECTION)
		return MakeCavalierProjection(pt);
	else if(projectionMode == PERSPECTIVE_PROJECTION)
		return MakePerspectiveProjection(pt);
	else 
		return CP2();
}

CP2 CProjection::MakeOrthogonalProjection(CP3 WorldPoint)//����ͶӰ
{
	CP2 ScreenPoint;//��Ļ����ϵ��ά��
	ScreenPoint.x = WorldPoint.x;
	ScreenPoint.y = WorldPoint.y;
	return ScreenPoint;
}

CP2 CProjection::MakeCavalierProjection(CP3 WorldPoint)//б�Ȳ�ͶӰ
{
	CP2 ScreenPoint;
	double m = 0.707;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

CP2 CProjection::MakeCabinetProjection(CP3 WorldPoint)//б����ͶӰ
{
	CP2 ScreenPoint;
	double m = 0.3536;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

void CProjection::SetEye(CP3 Eye)
{
	this->Eye = Eye;
}

CP3 CProjection::GetEye(void)//��ȡ�ӵ�
{
	Eye.x = 0, Eye.y = 0, Eye.z = R;//�ӵ�λ����ǰ��
	return Eye;
}

CP2 CProjection::MakePerspectiveProjection(CP3 WorldPoint)
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

CColorP2 CProjection::PerspectiveProjection2(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint;//�۲�����ϵ��ά��
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	ViewPoint.c = WorldPoint.c;
	CColorP2 ScreenPoint;//��Ļ����ϵ��ά��
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}

CColorP3 CProjection::PerspectiveProjection3(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint;//�۲�����ϵ��ά��
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	ViewPoint.c = WorldPoint.c;
	CColorP3 ScreenPoint;//��Ļ����ϵ��ά��
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	ScreenPoint.z = (ViewPoint.z - d) * d / ViewPoint.z;//Bouknight��ʽ
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}