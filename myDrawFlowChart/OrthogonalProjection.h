#if !defined(AFX_ORTHOGONAL_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ORTHOGONAL_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P3.h"

class COrthogonalProjection
{
public:
	COrthogonalProjection(void);
	virtual ~COrthogonalProjection(void);
	CP2 MakeOrthogonalProjection(CP3 WorldPoint);//正交投影
};

#endif // !defined(AFX_ORTHOGONAL_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
