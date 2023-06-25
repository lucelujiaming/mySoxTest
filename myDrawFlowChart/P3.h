// P3.h: interface for the P3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
#define AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P2.h"

class CP3:public CP2
{
public:
	CP3(void);
	CP3(double x, double y, double z);
	virtual ~CP3(void);
	friend CP3 operator + (const CP3 &p0, const CP3 &p1);//���������
	friend CP3 operator - (const CP3 &p0, const CP3 &p1);
	friend CP3 operator * (const CP3 &p, double scalar);
	friend CP3 operator * (double scalar, const CP3 &p);
	friend CP3 operator / (const CP3 &p, double scalar);
	friend CP3 operator += (CP3 &p0, CP3 &p1);
	friend CP3 operator -= (CP3 &p0, CP3 &p1);
	friend CP3 operator *= (CP3 &p, double scalar);
	friend CP3 operator /= (CP3 &p, double scalar);
	double Magnitude(void);//����
public:
	double z;
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
