// P3.h: interface for the P3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
#define AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CP3
{
public:
	CP3(void);
	~CP3(void);
	CP3(double x,double y,double z);
public:
	double x;
	double y;
	double z;
    double w;
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
