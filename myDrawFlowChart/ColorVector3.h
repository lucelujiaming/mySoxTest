#if !defined(AFX_COLOR_VECTOR3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_VECTOR3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"

class CColorVector3
{
public:
	CColorVector3(void);
	virtual ~CColorVector3(void);
	CColorVector3(double x, double y, double z);//����ʸ��
	CColorVector3(const CColorP3 &p);
	CColorVector3(const CColorP3 &p0, const CColorP3 &p1);//���ʸ��
	double Magnitude(void);//����ʸ����ģ
	CColorVector3 Normalize(void);//��һ��ʸ��
	friend CColorVector3 operator + (const CColorVector3 &v0, const CColorVector3 &v1);//���������
	friend CColorVector3 operator - (const CColorVector3 &v0, const CColorVector3 &v1);
	friend CColorVector3 operator * (const CColorVector3 &v, double scalar);
	friend CColorVector3 operator * (double scalar, const CColorVector3 &v);
	friend CColorVector3 operator / (const CColorVector3 &v, double scalar);
	friend double DotProduct(const CColorVector3 &v0, const CColorVector3 &v1);//����ʸ���ĵ��
	friend CColorVector3 CrossProduct(const CColorVector3 &v0, const CColorVector3 &v1);//����ʸ���Ĳ��
public:
	double x,y,z;
};

#endif // !defined(AFX_COLOR_VECTOR3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
