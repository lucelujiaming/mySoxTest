#if !defined(AFX_CABINET_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_CABINET_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P3.h"

class CCabinetProjection
{
public:
	CCabinetProjection(void);
	virtual ~CCabinetProjection(void);
	CP2 MakeCavalierProjection(CP3 WorldPoint); // 斜等测投影
	CP2 MakeCabinetProjection(CP3 WorldPoint);  // 斜二测投影
};

#endif // !defined(AFX_CABINET_PROJECTION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
