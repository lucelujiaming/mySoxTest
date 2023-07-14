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

// ��ν��ͶӰ����ʵ���ǰ�һ����ά������һ����ά���ꡣ������Ļ��ʾ��
// ������ͶӰ��ǳ��򵥡�����XY���䣬Z��������
CP2 CProjection::OrthogonalProjection(CP3 WorldPoint)//����ͶӰ
{
	CP2 ScreenPoint; // ��Ļ����ϵ��ά��
	ScreenPoint.x = WorldPoint.x;
	ScreenPoint.y = WorldPoint.y;
	return ScreenPoint;
}

// бͶӰ�ļ��㹫ʽ�μ���ʽ(5-30)��
CP2 CProjection::CavalierProjection(CP3 WorldPoint) // б�Ȳ�ͶӰ
{
	CP2 ScreenPoint;
	// ȡBeta=45�ȣ�Alpha=45�ȡ��õ���ͶӰΪб�Ȳ�ͶӰ��
	// Cos(Alpha)Sin(Beta) = sqrt(2)/2 = 0.707��
	double m = 0.707;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

// бͶӰ�ļ��㹫ʽ�μ���ʽ(5-30)��
CP2 CProjection::CabinetProjection(CP3 WorldPoint) // б����ͶӰ
{
	CP2 ScreenPoint;
	// ȡBeta=45�ȣ�Alpha=63.4�ȡ��õ���ͶӰΪб����ͶӰ��
	// Cos(Alpha)Sin(Beta) = sqrt(2)/4 = 0.3536��
	// б����ͶӰ������ʵ����Ϊ͸����Сϵ��Ϊ1/2��
	double m = 0.3536;
	ScreenPoint.x = WorldPoint.x - m * WorldPoint.z;
	ScreenPoint.y = WorldPoint.y - m * WorldPoint.z;
	return ScreenPoint;
}

// ����͸��ͶӰ���ӵ㡣
void CProjection::SetEye(CP3 Eye)
{
	this->Eye = Eye;
}

// ���͸��ͶӰ���ӵ㡣
CP3 CProjection::GetEye(void)//��ȡ�ӵ�
{
	//�ӵ�λ����ǰ��
	// �ӵ�����������ϵ�е�����Ϊ(0, 0, R)��
	Eye.x = 0, Eye.y = 0, Eye.z = R;
	return Eye;
}

// ͸��ͶӰ����һ���ӵ�����⡣�������Ҫ���úͻ�ȡ������
CP2 CProjection::PerspectiveProjection(CP3 WorldPoint)
{
	CP3 ViewPoint; // �۲�����ϵ��ά��
	// �μ���ʽ(5-38)
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	CP2 ScreenPoint;//��Ļ����ϵ��ά��
	// �μ���ʽ(5-45)
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	return ScreenPoint;
}


// ����͸��ͶӰ���ӵ㡣
void CProjection::SetColorEye(CColorP3 Eye)
{
	this->ColorEye = ColorEye;
}

// ���͸��ͶӰ���ӵ㡣
CColorP3 CProjection::GetColorEye(void)//��ȡ�ӵ�
{
	//�ӵ�λ����ǰ��
	// �ӵ�����������ϵ�е�����Ϊ(0, 0, R)��
	ColorEye.x = 0, ColorEye.y = 0, ColorEye.z = R;
	return ColorEye;
}

CColorP2 CProjection::TwoDimColorPerspectiveProjection(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint; // �۲�����ϵ��ά��
	// �μ���ʽ(5-38)
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

CColorP3 CProjection::ThreeDimColorPerspectiveProjection(CColorP3 WorldPoint)
{
	CColorP3 ViewPoint; // �۲�����ϵ��ά��
	// ����ά��������ϵ����ά�۲�����ϵ���μ���ʽ(5-38)
	ViewPoint.x = WorldPoint.x;
	ViewPoint.y = WorldPoint.y;
	ViewPoint.z = R - WorldPoint.z;
	ViewPoint.c = WorldPoint.c;
	CColorP3 ScreenPoint; // ��Ļ����ϵ��ά��
	// ����ά�۲�����ϵ����Ļ��ά����ϵ������α��ȡ�
	// ʹ��Bouknight��ʽ����x���ꡣ�μ���ʽ(5-50)��
	ScreenPoint.x = d * ViewPoint.x / ViewPoint.z;
	// ʹ��Bouknight��ʽ����y���ꡣ�μ���ʽ(5-50)��
	ScreenPoint.y = d * ViewPoint.y / ViewPoint.z;
	// ʹ��Bouknight��ʽ����z���ꡣ�μ���ʽ(5-50)��
	ScreenPoint.z = (ViewPoint.z - d) * d / ViewPoint.z;
	ScreenPoint.c = ViewPoint.c;
	return ScreenPoint;
}
