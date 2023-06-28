#if !defined(AFX_PERSPECTIVE_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_PERSPECTIVE_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P3.h"

class CPerspectiveProjection
{
public:
	CPerspectiveProjection(void);
	virtual ~CPerspectiveProjection(void);
	void SetEye(CP3 Eye);//设置视点
	CP3 GetEye(void);//获得视点
	CP2 MakePerspectiveProjection(CP3 WorldPoint);//透视投影
private:
	CP3 Eye;//视点
	double R, d;//视径和视距
};

#endif // !defined(AFX_PERSPECTIVE_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
