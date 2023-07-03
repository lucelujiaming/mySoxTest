#if !defined(AFX_COLOR_P3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_P3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP2.h"

class CColorP3:public CColorP2
{
public:
	CColorP3(void);
	CColorP3(double x, double y, double z);
	virtual ~CColorP3(void);
	
	friend CColorP3 operator + (const CColorP3 &p0, const CColorP3 &p1);//运算符重载
	friend CColorP3 operator - (const CColorP3 &p0, const CColorP3 &p1);
	friend CColorP3 operator * (const CColorP3 &p, double scalar);
	friend CColorP3 operator * (double scalar, const CColorP3 &p);
	friend CColorP3 operator / (const CColorP3 &p, double scalar);
	friend CColorP3 operator += (CColorP3 &p0, CColorP3 &p1);
	friend CColorP3 operator -= (CColorP3 &p0, CColorP3 &p1);
	friend CColorP3 operator *= (CColorP3 &p, double scalar);
	friend CColorP3 operator /= (CColorP3 &p, double scalar);
	double Magnitude(void);//长度
public:
	double z;
};

#endif // !defined(AFX_COLOR_P3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
