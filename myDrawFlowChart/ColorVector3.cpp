#include "stdafx.h"
#include "ColorVector3.h"
#include "math.h"

CColorVector3::CColorVector3(void)
{
	x = 0.0,y = 0.0, z = 1.0;//ָ��z������
}


CColorVector3::~CColorVector3(void)
{
}

CColorVector3::CColorVector3(double x, double y, double z)//����ʸ��
{
	this->x = x;
	this->y = y;
	this->z = z;	
}

CColorVector3::CColorVector3(const CColorP3 &p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

CColorVector3::CColorVector3(const CColorP3 &p0, const CColorP3 &p1)//���ʸ��
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}

double CColorVector3::Magnitude(void)//ʸ����ģ
{
	return sqrt(x * x + y * y + z * z);
}

CColorVector3 CColorVector3::Normalize(void)//��һ��Ϊ��λʸ��
{
	CColorVector3 vector;
	double magnitude = sqrt(x * x + y * y + z * z);
	if(fabs(magnitude) < 1e-4)
		magnitude  = 1.0;
	vector.x = x / magnitude;
	vector.y = y / magnitude;
	vector.z = z / magnitude;
	return vector;
}

CColorVector3 operator + (const CColorVector3 &v0, const CColorVector3 &v1)//ʸ���ĺ�
{
	CColorVector3 vector;
	vector.x = v0.x + v1.x;
	vector.y = v0.y + v1.y;
	vector.z = v0.z + v1.z;
	return vector;
}

CColorVector3 operator - (const CColorVector3 &v0, const CColorVector3 &v1)//ʸ���Ĳ�
{
	CColorVector3 vector;
	vector.x = v0.x - v1.x;
	vector.y = v0.y - v1.y;
	vector.z = v0.z - v1.z;
	return vector;
}

CColorVector3 operator * (const CColorVector3 &v, double scalar)//ʸ���볣���Ļ�
{
	CColorVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CColorVector3 operator * (double scalar, const CColorVector3 &v)//������ʸ���Ļ�
{
	CColorVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CColorVector3 operator / (const CColorVector3 &v, double scalar)//ʸ������
{
	if(fabs(scalar) < 1e-4)
		scalar = 1.0;
	CColorVector3 vector;
	vector.x = v.x / scalar;
	vector.y = v.y / scalar;
	vector.z = v.z / scalar;
	return vector;
}

double DotProduct(const CColorVector3 &v0, const CColorVector3 &v1)//ʸ���ĵ��
{
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

CColorVector3 CrossProduct(const CColorVector3 &v0, const CColorVector3 &v1)//ʸ���Ĳ��
{
	CColorVector3 vector;
	vector.x = v0.y * v1.z - v0.z * v1.y;
	vector.y = v0.z * v1.x - v0.x * v1.z;
	vector.z = v0.x * v1.y - v0.y * v1.x;
	return vector;
}