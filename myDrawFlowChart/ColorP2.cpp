#include "stdafx.h"
#include "ColorP2.h"
#include<math.h>

CColorP2::CColorP2(void)
{
	x = 0.0, y = 0.0, w = 1.0;
	c = CRGB(0.5, 0.4, 0.0);
}

CColorP2::CColorP2(double x, double y, CRGB c)
{
	this->x = x;
	this->y = y;
	this->w = 1.0;
	this->c = c;
}

CColorP2::CColorP2(double x, double y)
{
	this->x = x;
	this->y = y;
	this->w = 1.0;
	this->c = CRGB(1.0, 1.0, 1.0);
}

CColorP2::~CColorP2(void)
{

}

CColorP2 operator + (const CColorP2 &p0, const CColorP2 &p1)//和
{
	CColorP2 result;
	result.x = p0.x + p1.x;
	result.y = p0.y + p1.y;
	return result;
}

CColorP2 operator - (const CColorP2 &p0, const CColorP2 &p1)//差
{
	CColorP2 result;
	result.x = p0.x - p1.x;
	result.y = p0.y - p1.y;
	return result;
}

CColorP2 operator * (const CColorP2 &p, double scalar)//点和常量的积
{
	return CColorP2(p.x * scalar, p.y * scalar);
}

CColorP2 operator * (double scalar, const CColorP2 &p)//常量和点的积
{
	return CColorP2(p.x * scalar, p.y * scalar);
}

CColorP2 operator / (const CColorP2 &p, double scalar)//数除
{
	if (fabs(scalar) < 1e-4)
		scalar = 1.0;
	CColorP2 result;
	result.x = p.x / scalar;
	result.y = p.y / scalar;
	return result;
}

CColorP2 operator += (CColorP2 &p0, CColorP2 &p1)
{
	p0.x += p1.x;
	p0.y += p1.y;
	return p0;
}

CColorP2 operator-=(CColorP2 &p0, CColorP2 &p1)
{
	p0.x -= p1.x;
	p0.y -= p1.y;
	return p0;
}

CColorP2 operator *= (CColorP2 &p, double scalar)
{
	p.x *= scalar;
	p.y *= scalar;
	return p;
}

CColorP2 operator /= (CColorP2 &p, double scalar)
{
	if (fabs(scalar) < 1e-4)
		scalar = 1.0;
	p.x /= scalar;
	p.y /= scalar;
	return p;
}