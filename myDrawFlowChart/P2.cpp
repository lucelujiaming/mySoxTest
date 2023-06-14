// CP2.cpp: implementation of the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "P2.h"
#include"math.h"

CP2::CP2(void)
{
}


CP2::~CP2(void)
{
}

CP2::CP2(double x,double y)
{
	this->x=x;
	this->y=y;
}

 CP2 operator+(const CP2&p0,const CP2&p1)//‘ÀÀ„∑˚÷ÿ‘ÿ
 {
	 CP2 result;
	 result.x=p0.x+p1.x;
	 result.y=p0.y+p1.y;
	 return result;
 }
 CP2 operator-(const CP2&p0,const CP2&p1)
 {
	 CP2 result;
	 result.x=p0.x-p1.x;
	 result.y=p0.y-p1.y;
	 return result;
 }
 CP2 operator-(double scalar,const CP2&p)
 {
	 CP2 result;
	 result.x=scalar-p.x;
	 result.y=scalar-p.y;
	 return result;
 }
 CP2 operator-(const CP2&p,double scalar)
 {
	 CP2 result;
	 result.x=p.x-scalar;
	 result.y=p.y-scalar;
	 return result;
 }
 CP2 operator*(const CP2&p,double scalar)
 {
	 return CP2(p.x*scalar,p.y*scalar);
 }
 CP2 operator*(double scalar,const CP2&p)
 {
	 return CP2(p.x*scalar,p.y*scalar);
 }
 CP2 operator/(const CP2&p0, CP2&p1)
 {
	 if(fabs(p1.x)<1e-6)
	 {
		 p1.x=1.0;
	 }
	 if(fabs(p1.y)<1e-6)
	 {
		 p1.y=1.0;
	 }
	 CP2 result;
	 result.x=p0.x/p1.x;
	 result.y=p0.y/p1.y;
	 return result;
 }
 CP2 operator/(const CP2&p,double scalar)
 {
	 if(fabs(scalar)<1e-6)
	 {
		 scalar=1.0;
	 }
	 if(fabs(scalar)<1e-6)
	 {
		 scalar=1.0;
	 }
	 CP2 result;
	 result.x=p.x/scalar;
	 result.y=p.y/scalar;
	 return result;
 }


