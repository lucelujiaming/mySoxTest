#include "stdafx.h"
#include "TextureCoordinate.h"
#include "math.h"

CTextureCoordinate::CTextureCoordinate(void)
{
	u = 0;
	v = 0;
}

CTextureCoordinate::CTextureCoordinate(double u, double v)
{
	this->u = u;
	this->v = v;
}

CTextureCoordinate::~CTextureCoordinate(void)
{

}

CTextureCoordinate operator + (const CTextureCoordinate &t0, const CTextureCoordinate &t1)//��
{
	CTextureCoordinate result;
	result.u = t0.u + t1.u;
	result.v = t0.v + t1.v;
	return result;
}

CTextureCoordinate operator - (const CTextureCoordinate &t0, const CTextureCoordinate &t1)//��
{
	CTextureCoordinate result;
	result.u = t0.u - t1.u;
	result.v = t0.v - t1.v;
	return result;
}

CTextureCoordinate operator * (const CTextureCoordinate &t, double scalar)//��ͳ����Ļ�
{
	return CTextureCoordinate(t.u * scalar, t.v * scalar);
}

CTextureCoordinate operator * (double scalar, const CTextureCoordinate &t)//�����͵�Ļ�
{
	return CTextureCoordinate(t.u * scalar, t.v * scalar);
}

CTextureCoordinate operator / (const CTextureCoordinate &t, double scalar)//����
{
	if (fabs(scalar) < 1e-4)
		scalar = 1.0;
	CTextureCoordinate result;
	result.u = t.u / scalar;
	result.v = t.v / scalar;
	return result;
}

CTextureCoordinate operator += (CTextureCoordinate &t0, CTextureCoordinate &t1)
{
	t0.u = t0.u + t1.u;
	t0.v = t0.v + t1.v;
	return t0;
}

CTextureCoordinate operator -= (CTextureCoordinate &t0, CTextureCoordinate &t1)
{
	t0.u = t0.u - t1.u;
	t0.v = t0.v - t1.v;
	return t0;
}

CTextureCoordinate operator *= (CTextureCoordinate &t, double scalar)
{
	t.u = t.u * scalar;
	t.v = t.v * scalar;
	return t;
}

CTextureCoordinate operator /= (CTextureCoordinate &t, double scalar)
{
	if (fabs(scalar) < 1e-4)
		scalar = 1.0;
	t.u = t.u / scalar;
	t.v = t.v / scalar;
	return t;
}