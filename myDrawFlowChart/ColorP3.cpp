#include "stdafx.h"
#include "ColorP3.h"
#include "math.h"


CColorP3::CColorP3(void)
{
	z = 0.0;
}

CColorP3::CColorP3(double x, double y, double z):CColorP2(x, y)
{
	this->z = z;
}

CColorP3::~CColorP3(void)
{

}

CColorP3 operator + (const CColorP3 &p0, const CColorP3 &p1)//和
{
	CColorP3 p;
	p.x = p0.x + p1.x;
	p.y = p0.y + p1.y;
	p.z = p0.z + p1.z;
	return p;
}

CColorP3 operator - (const CColorP3 &p0, const CColorP3 &p1)//差
{
	CColorP3 p;
	p.x = p0.x - p1.x;
	p.y = p0.y - p1.y;
	p.z = p0.z - p1.z;
	return p;
}

CColorP3 operator * (const CColorP3 &p, double scalar)//点和常量的积
{
	return CColorP3(p.x * scalar, p.y * scalar, p.z * scalar);
}

CColorP3 operator * (double scalar, const CColorP3 &p)//点和常量的积
{
	return CColorP3(p.x * scalar, p.y * scalar, p.z * scalar);
}

CColorP3 operator / (const CColorP3 &p0, double scalar)//数除
{
	if (fabs(scalar) < 1e-4)
    {
		scalar = 1.0;
    }
	CColorP3 p;
	p.x = p0.x / scalar;
	p.y = p0.y / scalar;
	p.z = p0.z / scalar;
	return p;
}

CColorP3 operator += (CColorP3 &p0, CColorP3 &p1)
{
	p0.x = p0.x + p1.x;
	p0.y = p0.y + p1.y;
	p0.z = p0.z + p1.z;
	return p0;
}

CColorP3 operator -= (CColorP3 &p0, CColorP3 &p1)
{
	p0.x = p0.x - p1.x;
	p0.y = p0.y - p1.y;
	p0.z = p0.z - p1.z;
	return p0;
}

CColorP3 operator *= (CColorP3 &p, double scalar)
{
	p.x = p.x * scalar;
	p.y = p.y * scalar;
	p.z = p.z * scalar;
	return p;
}

CColorP3 operator /= (CColorP3 &p, double scalar)
{
	if (fabs(scalar) < 1e-4)
    {
		scalar = 1.0;
    }
	p.x = p.x / scalar;
	p.y = p.y / scalar;
	p.z = p.z / scalar;
	return p;
}

double CColorP3::Magnitude(void)//长度
{
	return sqrt(x * x + y * y + z * z);
}
