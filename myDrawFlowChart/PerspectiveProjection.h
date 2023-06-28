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
	void SetEye(CP3 Eye);//�����ӵ�
	CP3 GetEye(void);//����ӵ�
	CP2 MakePerspectiveProjection(CP3 WorldPoint);//͸��ͶӰ
private:
	CP3 Eye;//�ӵ�
	double R, d;//�Ӿ����Ӿ�
};

#endif // !defined(AFX_PERSPECTIVE_PROJECTION_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
