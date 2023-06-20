#if !defined(AFX_COLORPOINT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLORPOINT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"RGB.h"

class CColorPoint
{
public:
	CColorPoint(void);
	CColorPoint(int x, int y);
	CColorPoint(int x, int y, CRGB c);
	virtual ~CColorPoint(void);	
public:
	int x;
	int y;
	CRGB c;
};

#endif // !defined(AFX_COLORPOINT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
