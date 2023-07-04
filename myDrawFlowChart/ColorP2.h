#if !defined(AFX_COLOR_P2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_P2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RGB.h"

class CColorP2
{
public:
	CColorP2(void);
	virtual ~CColorP2(void);
	CColorP2(double x, double y);
	CColorP2(double x, double y, CRGB c);
	friend CColorP2 operator + (const CColorP2 &p0, const CColorP2 &p1);//运算符重载
	friend CColorP2 operator - (const CColorP2 &p0, const CColorP2 &p1);
	friend CColorP2 operator * (const CColorP2 &p, double scalar);
	friend CColorP2 operator * (double scalar, const CColorP2 &p);
	friend CColorP2 operator / (const CColorP2 &p, double scalar);
	friend CColorP2 operator += (CColorP2 &p0, CColorP2 &p1);
	friend CColorP2 operator -= (CColorP2 &p0, CColorP2 &p1);
	friend CColorP2 operator *= (CColorP2 &p, double scalar);
	friend CColorP2 operator /= (CColorP2 &p, double scalar);
public:
	double x, y;//坐标
	double w;//齐次坐标
	CRGB  c;//颜色
};

#endif // !defined(AFX_COLOR_P2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
