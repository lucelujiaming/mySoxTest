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
public:
	double z;
};

#endif // !defined(AFX_COLOR_P3_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
