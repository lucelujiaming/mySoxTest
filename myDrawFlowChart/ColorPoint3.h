#if !defined(AFX_COLOR_POINT3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_POINT3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint2.h"

class CColorPoint3 : public CColorPoint2
{
public:
	CColorPoint3(void);
	CColorPoint3(int x, int y, double z);
	CColorPoint3(int x, int y, double z, CVector3 n);
	virtual ~CColorPoint3(void);
public:
	double z;
};

#endif // !defined(AFX_COLOR_POINT3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
