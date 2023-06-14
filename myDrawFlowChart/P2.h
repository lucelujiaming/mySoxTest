// CP2.h: interface for the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CP2_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
#define AFX_CP2_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma once
//为了避免按照x和y方向进行重复运算，重载运算对象
class CP2
{
public:
	CP2(void);
	~CP2(void);
	CP2(double x,double y);
	friend CP2 operator+(const CP2&p0,const CP2&p1);//运算符重载
	friend CP2 operator-(const CP2&p0,const CP2&p1);
	friend CP2 operator-(double scalar,const CP2&p);
	friend CP2 operator-(const CP2&p,double scalar);
	friend CP2 operator*(const CP2&p,double scalar);
	friend CP2 operator*(double scalar,const CP2&p);
	friend CP2 operator/(const CP2&p0,const CP2&p1);
	friend CP2 operator/(const CP2&p,double scalar);

public:
	double x;
	double y;
};


#endif // !defined(AFX_CP2_H__780ECDA0_093A_460B_95A0_4172D5F78CB3__INCLUDED_)
